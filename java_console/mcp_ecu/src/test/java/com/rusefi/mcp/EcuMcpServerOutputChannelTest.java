package com.rusefi.mcp;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.DatalogEntry;
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
import org.junit.jupiter.api.io.TempDir;
import java.nio.file.Path;
import java.nio.file.Files;
import java.nio.charset.StandardCharsets;
import java.time.LocalDate;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.Constant;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Arrays;
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
    @TempDir
    Path tempDir;

    private static final int OCH_BLOCK_SIZE = 512;
    private static final int SECONDS_OFFSET = 0;
    /** Adjacent to 'seconds' so the two implicit subscriptions merge into one small range at the block start. */
    private static final int RPM_OFFSET = 4;
    /** Far from the implicit subscriptions: fetched only by a full-frame poll. */
    private static final int CLT_OFFSET = 400;
    private static final int RPM = 1234;
    private static final short CLT_RAW = 8765; // x0.01 -> 87.65

    @Test
    @SuppressWarnings("unchecked")
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
        BinaryProtocolState ecuState = new BinaryProtocolState();
        ecuState.setConfigurationImage(new ConfigurationImage(ini.getMetaInfo().getPageSize(0)));
        ServerSocketReference fakeEcu = startFakeEcu(ini, ecuState);
        Path shutdownFile = tempDir.resolve("shutdown.mlg");
        try (McpStdioHarness mcp = new McpStdioHarness()) {
            assertEquals(Boolean.FALSE, mcp.call("data_logging_status", "{}", 5_000).get("logging"));
            assertEquals(Boolean.TRUE, mcp.call("stop_data_logging", "{}", 5_000).get("success"));
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

            Path recording = tempDir.resolve("operating-data.mlg");
            JSONObject started = mcp.call("start_data_logging", pathArgument(recording), 5_000);
            assertEquals(Boolean.TRUE, started.get("logging"), started.toJSONString());
            assertEquals(3L, ((Number) started.get("channelCount")).longValue());
            assertEquals(recording.toString(), started.get("path"));
            Path initialTune = tempDir.resolve(LocalDate.now() + ".msq");
            assertEquals(initialTune.toString(), started.get("tunePath"));
            assertEquals("0", Msq.readTune(initialTune.toString()).getConstantsAsMap()
                    .get("mockScalarIniField").getValue());
            assertEquals(Boolean.FALSE, mcp.call("start_data_logging", pathArgument(shutdownFile), 5_000).get("success"));
            assertFalse(Files.exists(shutdownFile), "duplicate start must not create a file");
            awaitSamples(mcp);
            JSONObject stopped = mcp.call("stop_data_logging", "{}", 5_000);
            assertEquals(Boolean.TRUE, stopped.get("success"), stopped.toJSONString());
            assertEquals(Boolean.FALSE, stopped.get("logging"));
            assertEquals(stopped, mcp.call("stop_data_logging", "{}", 5_000));
            verifyRecording(recording, ((Number) stopped.get("sampleCount")).intValue());
            byte[] before = Files.readAllBytes(recording);
            assertEquals(Boolean.FALSE, mcp.call("start_data_logging", pathArgument(recording), 5_000).get("success"));
            org.junit.jupiter.api.Assertions.assertArrayEquals(before, Files.readAllBytes(recording));

            // Full tune access is symmetric: edit an MSQ exported by read_tune, write it back,
            // and confirm both the physical fake ECU image and a subsequent export changed.
            Path tuneToWrite = tempDir.resolve("round-trip.msq");
            JSONObject readTune = mcp.call("read_tune", pathArgument(tuneToWrite), 20_000);
            assertEquals(Boolean.TRUE, readTune.get("success"), readTune.toJSONString());
            Msq edited = Msq.readTune(tuneToWrite.toString());
            Constant scalar = edited.getConstantsAsMap().get("mockScalarIniField");
            scalar.setValue("42");
            edited.writeXmlFile(tuneToWrite.toString());

            JSONObject writeTune = mcp.call("write_tune", pathArgument(tuneToWrite), 60_000);
            assertEquals(Boolean.TRUE, writeTune.get("success"), writeTune.toJSONString());
            assertEquals(Boolean.TRUE, writeTune.get("changed"));
            assertEquals(Boolean.TRUE, writeTune.get("verified"));
            assertEquals(42, ByteBuffer.wrap(ecuState.getConfigurationImage().getRange(13, 2))
                    .order(ByteOrder.LITTLE_ENDIAN).getShort());

            JSONObject unchangedTune = mcp.call("write_tune", pathArgument(tuneToWrite), 60_000);
            assertEquals(Boolean.TRUE, unchangedTune.get("success"), unchangedTune.toJSONString());
            assertEquals(Boolean.FALSE, unchangedTune.get("changed"));
            assertEquals(Boolean.TRUE, unchangedTune.get("verified"));

            Path writtenTune = tempDir.resolve("written.msq");
            mcp.call("read_tune", pathArgument(writtenTune), 20_000);
            assertEquals("42", Msq.readTune(writtenTune.toString()).getConstantsAsMap()
                    .get("mockScalarIniField").getValue());

            // Change the live ECU image after connection: a stale connection cache would miss this.
            ecuState.setRange(new byte[]{1}, 0, 13, 1);
            JSONObject changed = mcp.call("start_data_logging", pathArgument(tempDir.resolve("changed.mlg")), 5_000);
            Path changedTune = tempDir.resolve(LocalDate.now() + "_1.msq");
            assertEquals(changedTune.toString(), changed.get("tunePath"), changed.toJSONString());
            assertEquals("1", Msq.readTune(changedTune.toString()).getConstantsAsMap()
                    .get("mockScalarIniField").getValue());
            mcp.call("stop_data_logging", "{}", 5_000);

            Path noTuneFolder = Files.createDirectory(tempDir.resolve("disabled"));
            JSONObject disabledArgs = new JSONObject();
            disabledArgs.put("path", noTuneFolder.resolve("only-data.mlg").toString());
            disabledArgs.put("saveTune", false);
            JSONObject disabled = mcp.call("start_data_logging", disabledArgs.toJSONString(), 5_000);
            assertEquals(Boolean.TRUE, disabled.get("logging"), disabled.toJSONString());
            org.junit.jupiter.api.Assertions.assertNull(disabled.get("tunePath"));
            mcp.call("stop_data_logging", "{}", 5_000);
            try (java.util.stream.Stream<Path> files = Files.list(noTuneFolder)) {
                assertEquals(1, files.count(), "saveTune=false creates only the data log");
            }

            // Leave a recording active: EOF must close it and release both output leases.
            JSONObject restarted = mcp.call("start_data_logging", pathArgument(shutdownFile), 5_000);
            assertEquals(Boolean.TRUE, restarted.get("logging"));
            assertEquals(changedTune.toString(), restarted.get("tunePath"));
            awaitSamples(mcp);
        } finally {
            snapshotToken.remove();
            fakeEcu.close();
            BinaryProtocol.iniFileProvider = previousProvider;
        }
        // stdin EOF -> shutdown released the lease: the demand is back to the implicit subscriptions only
        assertFalse(sensorCentral.getOutputChannelDemand().isFull(), "full-output lease leaked after MCP shutdown");
        assertTrue(Files.size(shutdownFile) > 24);
        // This also proves the file handle was closed on Windows.
        Files.delete(shutdownFile);
    }

    @SuppressWarnings("unchecked")
    private static String pathArgument(Path path) {
        JSONObject args = new JSONObject();
        args.put("path", path.toString());
        return args.toJSONString();
    }

    private static void awaitSamples(McpStdioHarness mcp) throws Exception {
        long deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(10);
        while (System.nanoTime() < deadline) {
            JSONObject status = mcp.call("data_logging_status", "{}", 5_000);
            assertEquals(Boolean.TRUE, status.get("success"), status.toJSONString());
            if (((Number) status.get("sampleCount")).longValue() >= 2) {
                return;
            }
            Thread.sleep(10);
        }
        throw new AssertionError("No ECU samples recorded");
    }

    private static void verifyRecording(Path file, int samples) throws IOException {
        byte[] bytes = Files.readAllBytes(file);
        assertEquals("MLVLG\0", new String(bytes, 0, 6, StandardCharsets.US_ASCII));
        ByteBuffer data = ByteBuffer.wrap(bytes).order(ByteOrder.BIG_ENDIAN);
        assertEquals(3, data.getShort(22));
        assertEquals("Seconds", readFieldName(bytes, 0));
        assertEquals("RPM", readFieldName(bytes, 1));
        assertEquals("CLT", readFieldName(bytes, 2));
        int dataStart = data.getInt(16);
        int rowSize = data.getShort(20) + 5;
        assertEquals(dataStart + samples * rowSize, bytes.length);
        assertTrue(samples >= 2);
        for (int i = 0; i < samples; i++) {
            data.position(dataStart + i * rowSize + 4);
            assertEquals(7, data.getInt());
            assertEquals(RPM, data.getShort());
            assertEquals(CLT_RAW, data.getShort());
        }
    }

    private static String readFieldName(byte[] bytes, int index) {
        return new String(bytes, 24 + index * 89 + 1, 34, StandardCharsets.US_ASCII).trim();
    }

    /**
     * Fake ECU: the proxy server in detached mode serves reads/CRC from a configuration image and
     * TS_OUTPUT_COMMAND from a fixed output-channel frame. Port 0 = ephemeral.
     */
    private static ServerSocketReference startFakeEcu(IniFileModel ini, BinaryProtocolState state) throws IOException {
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
        when(ini.getMetaInfo().getnPages()).thenReturn(1);

        Map<String, IniField> channels = new LinkedHashMap<>();
        channels.put(WellKnownGauges.SECONDS.getOutputChannelName(),
                new ScalarIniField(WellKnownGauges.SECONDS.getOutputChannelName(), SECONDS_OFFSET, "s", FieldType.INT, 1, "0", 0));
        channels.put(WellKnownGauges.RPMGauge.getOutputChannelName(),
                new ScalarIniField(WellKnownGauges.RPMGauge.getOutputChannelName(), RPM_OFFSET, "RPM", FieldType.UINT16, 1, "0", 0));
        channels.put("CLTValue", new ScalarIniField("CLTValue", CLT_OFFSET, "deg C", FieldType.INT16, 0.01, "2", 0));
        Map<String, IniField> byName = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        byName.putAll(channels);

        when(ini.getAllOutputChannels()).thenReturn(channels);
        when(ini.getDatalogEntries()).thenReturn(Arrays.asList(
                new DatalogEntry("seconds", "Seconds"),
                new DatalogEntry("RPMValue", "RPM"),
                new DatalogEntry("CLTValue", "CLT")));
        when(ini.getOutputChannel(anyString())).thenAnswer(invocation -> {
            IniField field = byName.get(invocation.<String>getArgument(0));
            if (field == null) {
                throw new IniMemberNotFound("no output channel " + invocation.getArgument(0));
            }
            return field;
        });
        when(ini.getExpressionOutputChannels()).thenReturn(Collections.emptyMap());
        when(ini.getGauges()).thenReturn(Collections.emptyMap());
        when(ini.getSecondaryIniFields()).thenReturn(Collections.emptyMap());
        when(ini.findIniField(anyString())).thenAnswer(invocation ->
                Optional.ofNullable(ini.getAllIniFields().get(invocation.<String>getArgument(0))));
        return ini;
    }
}
