package com.rusefi.mcp;

import com.opensr5.ini.DatalogEntry;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.FieldType;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.OutputChannelSnapshot;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.LinkManager;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;

import javax.swing.SwingUtilities;
import java.io.IOException;
import java.util.BitSet;
import java.util.Collections;
import java.util.concurrent.CancellationException;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

@SuppressWarnings("unchecked")
class ConsoleEcuSessionTest {
    private final LinkManager link = mock(LinkManager.class);
    private final BinaryProtocol protocol = mock(BinaryProtocol.class);
    private final SensorCentral sensors = SensorCentral.getInstance();

    private ConsoleEcuSession open() throws IOException {
        protocol.signature = "rusEFI-test-board";
        when(link.getBinaryProtocol()).thenReturn(protocol);
        return new ConsoleEcuSession(link);
    }

    @Test void borrowsConsoleConnectionAndReleasesOnlyItsOwnLease() throws Exception {
        assertFalse(sensors.getOutputChannelDemand().isFull());
        try (SensorCentral.FullOutputLease consoleLease = sensors.acquireFullOutput()) {
            ConsoleEcuSession session = open();
            assertEquals(4, session.definitions().size());
            assertEquals("rusEFI-test-board", session.execute("ecu_info", object(), () -> {}).get("signature"));
            session.close();
            session.close();
            assertTrue(sensors.getOutputChannelDemand().isFull(), "The Console's own lease remains");
            assertThrows(IOException.class, session::checkConnected);
        }
        assertFalse(sensors.getOutputChannelDemand().isFull());
        verify(link, never()).close();
        verify(protocol, never()).close();
        // All link interactions are reads of the existing protocol: no serial scan, connection or submit.
        verify(link, atLeastOnce()).getBinaryProtocol();
        verifyNoMoreInteractions(link);
    }

    @Test void refusesDisconnectedOrReplacedConnectionsWithoutAutodetect() throws Exception {
        assertThrows(IOException.class, () -> new ConsoleEcuSession(link));
        try (ConsoleEcuSession session = open()) {
            when(link.getBinaryProtocol()).thenReturn(mock(BinaryProtocol.class));
            assertThrows(IOException.class, () -> session.execute("ecu_info", object(), () -> {}));
            assertFalse(session.isCurrent());
        }
        when(link.getBinaryProtocol()).thenReturn(protocol);
        when(protocol.isClosed()).thenReturn(true);
        assertThrows(IOException.class, () -> new ConsoleEcuSession(link));
        assertFalse(sensors.getOutputChannelDemand().isFull());
        verify(link, never()).close();
    }

    @Test void validatesArgumentsAndExposesNoWriteOrArbitraryCommandPath() throws Exception {
        try (ConsoleEcuSession session = open()) {
            for (String name : new String[]{"send_command", "command", "connect", "write_tune", "set_lua", "update_firmware", "reboot"}) {
                assertEquals(Boolean.FALSE, session.execute(name, object(), () -> {}).get("success"));
                assertFalse(session.definitions().toJSONString().contains("\"name\":\"" + name + "\""));
            }
            assertError(session, "ecu_info", object("unexpected", true));
            assertError(session, "read_output_channel", object());
            assertError(session, "read_output_channel", object("name", 123));
            assertError(session, "read_messages", object("maxLines", 1.5));
            assertError(session, "read_messages", object("maxLines", 0));
            assertError(session, "read_messages", object("maxLines", Long.MAX_VALUE));
            assertError(session, "read_messages", object("sinceSeq", -2L));
            assertError(session, "read_messages", object("sourceFilter", null));
        }
    }

    @Test void listsChannelsAndReadsOnlyValuesFromFreshFullSamples() throws Exception {
        sensors.reset();
        IniFileModel ini = mock(IniFileModel.class);
        when(protocol.getIniFile()).thenReturn(ini);
        ScalarIniField rpm = new ScalarIniField("RPMValue", 0, "RPM", FieldType.UINT16, 1, "0", 0);
        when(ini.getAllOutputChannels()).thenReturn(Collections.singletonMap("RPMValue", rpm));
        when(ini.getOutputChannel("RPMValue")).thenReturn(rpm);
        when(ini.getDatalogEntries()).thenReturn(Collections.singletonList(new DatalogEntry("RPMValue", "Engine speed")));
        try (ConsoleEcuSession session = open()) {
            JSONObject listing = session.execute("list_output_channels", object("filter", "speed"), () -> {});
            assertEquals("RPMValue", ((JSONObject) ((JSONArray) listing.get("channels")).get(0)).get("name"));
            sensors.setValue(9999, "staleValue"); // Old cache values cannot be presented as current observations.
            BitSet valid = new BitSet();
            valid.set(0, 2);
            long generation = sensors.getOutputChannelDemand().getGeneration();
            sensors.grabSensorValues(new OutputChannelSnapshot(new byte[]{0, (byte) 0xd2, 4}, valid,
                    Collections.emptySet(), generation, true), ini, null);
            JSONObject reading = session.execute("read_output_channel", object("name", "rpmvalue"), () -> {});
            assertEquals(1234.0, reading.get("value"));
            assertEquals(Boolean.TRUE, reading.get("found"));
            assertTrue(((Number) reading.get("sampleTimestampMs")).longValue() > 0);
            assertTrue(((Number) reading.get("sampleAgeMs")).longValue() < 2000);
            assertEquals(Boolean.FALSE, session.execute("read_output_channel", object("name", "staleValue"), () -> {}).get("found"));
        } finally {
            sensors.reset();
        }
        assertFalse(sensors.getOutputChannelDemand().isFull());
    }

    @Test void waitingForFirstSampleCanBeCancelled() throws Exception {
        try (ConsoleEcuSession session = open()) {
            int[] checks = {0};
            assertThrows(CancellationException.class, () -> session.execute("read_output_channel", object("name", "RPMValue"), () -> {
                if (++checks[0] > 1) { throw new CancellationException(); }
            }));
        }
    }

    @Test void capturesMessagesOnlyFromTheConversationAndCanPage() throws Exception {
        MessagesCentral messages = MessagesCentral.getInstance();
        messages.postMessage(ConsoleEcuSessionTest.class, "before conversation");
        SwingUtilities.invokeAndWait(() -> {});
        try (ConsoleEcuSession session = open()) {
            messages.postMessage(ConsoleEcuSessionTest.class, "test diagnostic");
            SwingUtilities.invokeAndWait(() -> {});
            JSONObject result = session.execute("read_messages", object("sourceFilter", "ConsoleEcuSessionTest", "maxLines", 1L), () -> {});
            JSONArray lines = (JSONArray) result.get("messages");
            assertEquals(1, lines.size());
            assertEquals("test diagnostic", ((JSONObject) lines.get(0)).get("message"));
            JSONObject next = session.execute("read_messages", object("sinceSeq", result.get("latestSeq")), () -> {});
            assertTrue(((JSONArray) next.get("messages")).isEmpty());
        }
        try (ConsoleEcuSession next = open()) {
            assertTrue(((JSONArray) next.execute("read_messages", object(), () -> {}).get("messages")).isEmpty());
        }
    }

    private static void assertError(ConsoleEcuSession session, String name, JSONObject args) throws Exception {
        assertEquals(Boolean.FALSE, session.execute(name, args, () -> {}).get("success"));
    }
    private static JSONObject object(Object... pairs) {
        JSONObject result = new JSONObject();
        for (int i = 0; i < pairs.length; i += 2) { result.put(pairs[i], pairs[i + 1]); }
        return result;
    }
}
