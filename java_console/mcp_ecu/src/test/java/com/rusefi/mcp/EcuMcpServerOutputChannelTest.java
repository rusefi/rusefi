package com.rusefi.mcp;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniMemberNotFound;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.Listener;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolLocalCache;
import com.rusefi.binaryprotocol.BinaryProtocolState;
import com.rusefi.binaryprotocol.IniFileProvider;
import com.rusefi.config.FieldType;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.WellKnownGauges;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.LinkConnector;
import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.ServerSocketReference;
import com.rusefi.proxy.MockIniFileProvider;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.TreeMap;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.TimeUnit;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.when;

/**
 * End-to-end coverage of the {@code read_output_channel} MCP tool against a fake ECU:
 * {@link BinaryProtocolServer} in "detached" mode serves the TS protocol over TCP from an in-memory
 * {@link BinaryProtocolState}, and {@link EcuMcpServer} connects to it exactly like it would to real hardware
 * (the port string is a TCP address, everything else - LinkManager, BinaryProtocol, output-channel polling,
 * SensorCentral - is production code).
 *
 * <p>Regression test for the subscription-based output polling of #10171: the console now fetches only the
 * output-channel byte ranges somebody subscribed to. A headless MCP process has exactly two implicit
 * subscriptions - {@code seconds} (link health, {@link ConnectionStatusLogic}) and {@code RPMValue}
 * ({@code BinaryProtocolLogger}) - so without an explicit full-frame demand every other channel is never read
 * from the ECU and {@code read_output_channel} answered {@code found: false} forever.
 *
 * <p>The fix is a {@link SensorCentral.FullOutputLease} held for the lifetime of the ECU connection, i.e. the
 * pre-#10171 "always poll everything" behaviour: the demand must be full from the very first poll until the
 * server shuts down, and not full before or after.
 */
class EcuMcpServerOutputChannelTest {
    private static final int OCH_BLOCK_SIZE = 512;
    private static final int SECONDS_OFFSET = 0;
    /** Adjacent to 'seconds' so the two implicit subscriptions merge into one small range at the block start. */
    private static final int RPM_OFFSET = 4;
    /** Far from the implicit subscriptions: fetched only by a full-frame poll. */
    private static final int CLT_OFFSET = 400;
    private static final int RPM = 1234;
    private static final short CLT_RAW = 8765; // x0.01 -> 87.65

    @Test
    void readOutputChannelReturnsChannelsNobodySubscribedTo() throws Throwable {
        IniFileModel ini = createIni();
        IniFileProvider previousProvider = BinaryProtocol.iniFileProvider;
        BinaryProtocol.iniFileProvider = signature -> ini;
        BinaryProtocolLocalCache.DISABLE_LOCAL_CONFIGURATION_CACHE = true;
        SensorCentral sensorCentral = SensorCentral.getInstance();
        sensorCentral.reset();

        assertFalse(sensorCentral.getOutputChannelDemand().isFull(), "another test leaked a full-output lease");

        // Passive: snapshot listeners do not contribute output-channel demand, so they do not mask the issue.
        CountDownLatch polls = new CountDownLatch(3);
        List<Boolean> pollWasFull = new CopyOnWriteArrayList<>();
        SensorCentral.SnapshotListenerToken snapshotToken = sensorCentral.addSnapshotListener(snapshot -> {
            pollWasFull.add(snapshot.isFull());
            polls.countDown();
        });
        ServerSocketReference fakeEcu = startFakeEcu(ini);
        try (McpStdioHarness mcp = new McpStdioHarness()) {
            JSONObject connected = mcp.call("connect", "{\"port\":\"localhost:" + fakeEcu.getLocalPort() + "\"}", 60_000);
            assertEquals(Boolean.TRUE, connected.get("connected"), connected.toJSONString());
            assertTrue(polls.await(20, TimeUnit.SECONDS), "output channels were never polled");

            // auto-subscribed to everything, like before #10171: the lease is taken before connecting, so even
            // the pull thread's first poll is a full frame - no partial snapshot ever reaches SensorCentral
            assertTrue(sensorCentral.getOutputChannelDemand().isFull(), "MCP server must demand the full output frame while connected");
            List<Boolean> observed = new ArrayList<>(pollWasFull);
            assertFalse(observed.contains(Boolean.FALSE), "every poll must be a full frame, got " + observed);

            // implicitly subscribed by BinaryProtocolLogger: worked before and after #10171
            JSONObject rpm = mcp.call("read_output_channel", "{\"name\":\"RPMValue\"}", 20_000);
            assertEquals(Boolean.TRUE, rpm.get("found"), rpm.toJSONString());
            assertEquals(RPM, ((Number) rpm.get("value")).doubleValue(), 0.001);

            // nobody in a headless MCP process subscribes to coolant temperature
            JSONObject clt = mcp.call("read_output_channel", "{\"name\":\"CLTValue\"}", 20_000);
            assertEquals(Boolean.TRUE, clt.get("found"), "channel outside the implicit subscriptions: " + clt.toJSONString());
            assertEquals(CLT_RAW * 0.01, ((Number) clt.get("value")).doubleValue(), 0.001);
        } finally {
            snapshotToken.remove();
            fakeEcu.close();
            BinaryProtocol.iniFileProvider = previousProvider;
        }
        // stdin EOF -> shutdown released the lease: the demand is back to the implicit subscriptions only
        assertFalse(sensorCentral.getOutputChannelDemand().isFull(), "full-output lease leaked after MCP shutdown");
    }

    /**
     * Fake ECU: the proxy server in detached mode serves reads/CRC from a configuration image and
     * TS_OUTPUT_COMMAND from a fixed output-channel frame. Port 0 = ephemeral.
     */
    private static ServerSocketReference startFakeEcu(IniFileModel ini) throws IOException {
        BinaryProtocolState state = new BinaryProtocolState();
        state.setConfigurationImage(new ConfigurationImage(ini.getMetaInfo().getPageSize(0)));
        byte[] outputs = new byte[OCH_BLOCK_SIZE];
        ByteBuffer frame = ByteBuffer.wrap(outputs).order(ByteOrder.LITTLE_ENDIAN);
        frame.putInt(SECONDS_OFFSET, 7);
        frame.putShort(RPM_OFFSET, (short) RPM);
        frame.putShort(CLT_OFFSET, CLT_RAW);
        state.setCurrentOutputs(outputs);

        LinkManager ecuSide = new LinkManager();
        ecuSide.setConnector(new LinkConnector() {
            @Override
            public void connectAndReadConfiguration(BinaryProtocol.Arguments arguments, ConnectionStatusLogic.Listener listener) {
            }

            @Override
            public void send(String command, boolean fireEvent) {
            }

            @Override
            public BinaryProtocol getBinaryProtocol() {
                // null = "not a proxy of a live ECU": the server answers from getBinaryProtocolState() directly
                return null;
            }

            @Override
            public BinaryProtocolState getBinaryProtocolState() {
                return state;
            }
        });
        return new BinaryProtocolServer().start(ecuSide, 0, Listener.empty(), new BinaryProtocolServer.Context());
    }

    /**
     * The generic connect-capable mock .ini plus three output channels: the two the console subscribes to
     * implicitly and one it does not.
     */
    private static IniFileModel createIni() throws Throwable {
        IniFileModel ini = MockIniFileProvider.create().provide(null);
        when(ini.getMetaInfo().getOchBlockSize()).thenReturn(OCH_BLOCK_SIZE);

        Map<String, IniField> channels = new LinkedHashMap<>();
        channels.put(WellKnownGauges.SECONDS.getOutputChannelName(),
                new ScalarIniField(WellKnownGauges.SECONDS.getOutputChannelName(), SECONDS_OFFSET, "s", FieldType.INT, 1, "0", 0));
        channels.put(WellKnownGauges.RPMGauge.getOutputChannelName(),
                new ScalarIniField(WellKnownGauges.RPMGauge.getOutputChannelName(), RPM_OFFSET, "RPM", FieldType.UINT16, 1, "0", 0));
        channels.put("CLTValue", new ScalarIniField("CLTValue", CLT_OFFSET, "deg C", FieldType.INT16, 0.01, "2", 0));
        Map<String, IniField> byName = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        byName.putAll(channels);

        when(ini.getAllOutputChannels()).thenReturn(channels);
        when(ini.getOutputChannel(anyString())).thenAnswer(invocation -> {
            IniField field = byName.get(invocation.<String>getArgument(0));
            if (field == null) {
                throw new IniMemberNotFound("no output channel " + invocation.getArgument(0));
            }
            return field;
        });
        when(ini.getExpressionOutputChannels()).thenReturn(Collections.emptyMap());
        when(ini.getGauges()).thenReturn(Collections.emptyMap());
        when(ini.findIniField(anyString())).thenReturn(Optional.empty());
        return ini;
    }
}
