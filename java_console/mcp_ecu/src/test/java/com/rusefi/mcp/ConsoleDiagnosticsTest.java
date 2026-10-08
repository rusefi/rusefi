package com.rusefi.mcp;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.FieldType;
import com.rusefi.core.OutputChannelSnapshot;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.LinkManager;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.BitSet;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.concurrent.CancellationException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

@SuppressWarnings("unchecked")
class ConsoleDiagnosticsTest {
    private final SensorCentral sensors = SensorCentral.getInstance();
    private final LinkManager link = mock(LinkManager.class);
    private final BinaryProtocol protocol = mock(BinaryProtocol.class);
    private final IniFileModel ini = mock(IniFileModel.class);
    private final AtomicLong nanos = new AtomicLong();
    private ConsoleEcuSession session;

    @BeforeEach void open() throws Exception {
        sensors.reset();
        protocol.signature = "diagnostics-test-ecu";
        when(link.getBinaryProtocol()).thenReturn(protocol);
        when(protocol.getIniFile()).thenReturn(ini);
        Map<String, IniField> fields = new LinkedHashMap<>();
        fields.put("RPMValue", new ScalarIniField("RPMValue", 0, "RPM", FieldType.UINT16, 1, "0", 0));
        fields.put("VBatt", new ScalarIniField("VBatt", 2, "V", FieldType.UINT16, .001, "3", 0));
        fields.put("checkEngine", new ScalarIniField("checkEngine", 4, "", FieldType.UINT16, 1, "0", 0));
        fields.put("lastErrorCode", new ScalarIniField("lastErrorCode", 6, "error", FieldType.UINT16, 1, "0", 0));
        fields.put("notFinite", new ScalarIniField("notFinite", 8, "", FieldType.FLOAT, 1, "0", 0));
        when(ini.getAllOutputChannels()).thenReturn(fields);
        for (Map.Entry<String, IniField> field : fields.entrySet()) {
            when(ini.getOutputChannel(field.getKey())).thenReturn(field.getValue());
        }
        session = new ConsoleEcuSession(link, nanos::get, () -> 123456L);
    }

    @AfterEach void close() {
        session.close();
        sensors.reset();
        verify(link, never()).close();
        verify(protocol, never()).close();
    }

    @Test void pinsValuesAndFaultsToOnePollEvenWhenNextPollArrivesDuringTheRead() throws Exception {
        publish(250, 11000, 1, 42);
        AtomicInteger checks = new AtomicInteger();
        JSONObject result = session.execute("diagnostic_snapshot", object("names", array("rpmvalue", "VBatt")), () -> {
            // The third check occurs after the sample is pinned, while assembling its first value.
            if (checks.incrementAndGet() == 3) { publish(900, 14000, 0, 99); }
        });
        assertEquals(250.0, channel(result, "values", "rpmvalue").get("value"));
        assertEquals(11.0, channel(result, "values", "VBatt").get("value"));
        assertEquals(1.0, channel(result, "faults", "checkEngine").get("value"));
        assertEquals(42.0, channel(result, "faults", "lastErrorCode").get("value"));
        assertEquals(1L, result.get("sampleId"));
        assertEquals(123456L, result.get("sampleTimestampMs"));
        assertEquals(0L, result.get("sampleAgeMs"));
        assertEquals("diagnostics-test-ecu", result.get("signature"));
        JSONObject next = session.execute("read_live_values", object("names", array("RPMValue", "VBatt")), () -> {});
        assertEquals(2L, next.get("sampleId"));
        assertEquals(900.0, channel(next, "values", "RPMValue").get("value"));
        assertEquals(14.0, channel(next, "values", "VBatt").get("value"));
    }

    @Test void defaultSnapshotIncludesFaultHistoryAndMarksUnavailableChannels() throws Exception {
        publish(0, 12000, 0, 42);
        JSONObject result = session.execute("diagnostic_snapshot", object(), () -> {});
        assertEquals(7, ((JSONArray) result.get("values")).size());
        assertEquals(14, ((JSONArray) result.get("faults")).size());
        assertEquals(0.0, channel(result, "values", "RPMValue").get("value"));
        JSONObject missing = channel(result, "faults", "recentErrorCode1");
        assertEquals(Boolean.FALSE, missing.get("found"));
        assertFalse(missing.containsKey("value"));
        assertTrue(((String) result.get("faultNote")).contains("past events"));
        assertEquals(42.0, channel(result, "faults", "lastErrorCode").get("value"));
    }

    @Test void rejectsMalformedDuplicateAndOversizedChannelListsBeforeWaiting() throws Exception {
        JSONArray tooMany = new JSONArray();
        for (int i = 0; i < 33; i++) { tooMany.add("channel" + i); }
        for (String tool : new String[]{"read_live_values", "diagnostic_snapshot"}) {
            for (Object names : new Object[]{null, "RPMValue", array(), array(1), array((Object) null),
                    array(" "), array(String.join("", Collections.nCopies(257, "x"))),
                    array("RPMValue", "rpmvalue"), tooMany}) {
                assertEquals(Boolean.FALSE, session.execute(tool, object("names", names), () -> {}).get("success"));
            }
            assertEquals(Boolean.FALSE, session.execute(tool, object("extra", true), () -> {}).get("success"));
        }
        assertEquals(Boolean.FALSE, session.execute("read_live_values", object(), () -> {}).get("success"));
    }

    @Test void boundsResultsAndDoesNotSerializeMissingOrNonFiniteValuesAsNumbers() throws Exception {
        publish(250, 11000, 1, 42);
        JSONArray names = array("notFinite", "RPMValue");
        for (int i = 2; i < 32; i++) { names.add("missing" + i); }
        JSONObject result = session.execute("read_live_values", object("names", names), () -> {});
        assertEquals(32, ((JSONArray) result.get("values")).size());
        assertEquals(Boolean.FALSE, channel(result, "values", "notFinite").get("found"));
        assertFalse(channel(result, "values", "notFinite").containsKey("value"));
        assertEquals(Boolean.FALSE, channel(result, "values", "missing2").get("found"));
        assertEquals(250.0, channel(result, "values", "RPMValue").get("value"));
    }

    @Test void waitsForFreshSampleInsteadOfReturningAnExpiredOne() throws Exception {
        publish(250, 11000, 1, 42);
        nanos.set(TimeUnit.SECONDS.toNanos(3));
        AtomicInteger checks = new AtomicInteger();
        JSONObject result = session.execute("read_live_values", object("names", array("RPMValue")), () -> {
            if (checks.incrementAndGet() == 3) { publish(350, 10000, 0, 0); }
        });
        assertEquals(2L, result.get("sampleId"));
        assertEquals(350.0, channel(result, "values", "RPMValue").get("value"));
    }

    @Test void partialAndPreLeaseSamplesCannotSatisfyAReadAndWaitsTimeOut() throws Exception {
        long generation = sensors.getOutputChannelDemand().getGeneration();
        publish(250, 11000, 1, 42, false, generation);
        publish(350, 11000, 1, 42, true, generation - 1);
        JSONObject result = session.execute("diagnostic_snapshot", object(), () -> nanos.addAndGet(TimeUnit.SECONDS.toNanos(1)));
        assertEquals(Boolean.FALSE, result.get("success"));
        assertTrue(((String) result.get("error")).contains("No fresh full"));
        assertFalse(result.containsKey("values"));
    }

    @Test void expiredSamplesTimeOutAndCancellationOrReplacementDiscardsReads() throws Exception {
        publish(250, 11000, 1, 42);
        nanos.set(TimeUnit.SECONDS.toNanos(3));
        JSONObject expired = session.execute("read_live_values", object("names", array("RPMValue")),
                () -> nanos.addAndGet(TimeUnit.SECONDS.toNanos(1)));
        assertEquals(Boolean.FALSE, expired.get("success"));
        AtomicInteger checks = new AtomicInteger();
        assertThrows(CancellationException.class, () -> session.execute("diagnostic_snapshot", object(), () -> {
            if (checks.incrementAndGet() == 2) { throw new CancellationException(); }
        }));
        publish(900, 14000, 0, 0);
        checks.set(0);
        assertThrows(IOException.class, () -> session.execute("diagnostic_snapshot", object(), () -> {
            if (checks.incrementAndGet() == 3) { when(link.getBinaryProtocol()).thenReturn(mock(BinaryProtocol.class)); }
        }));
    }

    private void publish(int rpm, int millivolts, int checkEngine, int error) {
        publish(rpm, millivolts, checkEngine, error, true, sensors.getOutputChannelDemand().getGeneration());
    }

    @Test void liveLogCollectsOnlyFuturePollsWithBoundedSamplesAndCoherentValues() throws Exception {
        publish(9999, 11000, 0, 0);
        AtomicInteger checks = new AtomicInteger();
        JSONObject result = session.execute("capture_live_log", object("names", array("RPMValue", "VBatt"),
                "duration_ms", 1000, "max_samples", 2), () -> {
            nanos.addAndGet(TimeUnit.MILLISECONDS.toNanos(100));
            int index = checks.incrementAndGet();
            publish(index, index * 1000, 0, 0);
        });
        assertEquals(Boolean.TRUE, result.get("success"));
        assertEquals(2, result.get("sampleCount"));
        assertEquals(Boolean.TRUE, result.get("truncated"));
        long previous = 1;
        for (Object item : (JSONArray) result.get("samples")) {
            JSONObject sample = (JSONObject) item;
            assertTrue(((Number) sample.get("sampleId")).longValue() > previous);
            previous = ((Number) sample.get("sampleId")).longValue();
            assertEquals(channel(sample, "values", "RPMValue").get("value"), channel(sample, "values", "VBatt").get("value"));
            assertNotEquals(9999.0, channel(sample, "values", "RPMValue").get("value"));
        }
    }

    @Test void liveLogDoesNotReplayAvailablePollsAndHonorsCancellationAndReplacement() throws Exception {
        publish(250, 11000, 0, 0);
        JSONObject result = session.execute("capture_live_log", object("names", array("RPMValue"), "duration_ms", 100),
                () -> nanos.addAndGet(TimeUnit.MILLISECONDS.toNanos(100)));
        assertEquals(Boolean.FALSE, result.get("success"));
        assertEquals(0, result.get("sampleCount"));
        AtomicInteger checks = new AtomicInteger();
        assertThrows(CancellationException.class, () -> session.execute("capture_live_log", object("names", array("RPMValue")), () -> {
            if (checks.incrementAndGet() == 2) { throw new CancellationException(); }
        }));
        checks.set(0);
        assertThrows(CancellationException.class, () -> session.execute("capture_live_log", object("names", array("RPMValue")), () -> {
            if (checks.incrementAndGet() == 2) { when(link.getBinaryProtocol()).thenReturn(mock(BinaryProtocol.class)); }
        }));
    }

    @Test void evidenceToolsRejectUnboundedArgumentsBeforeWaitingOrReading() throws Exception {
        JSONArray tooMany = new JSONArray();
        for (int i = 0; i < 17; i++) { tooMany.add("channel" + i); }
        assertEquals(Boolean.FALSE, session.execute("capture_live_log", object("names", tooMany), () -> {}).get("success"));
        for (Object duration : new Object[]{0, 10001, 100.5, null}) {
            assertEquals(Boolean.FALSE, session.execute("capture_live_log", object("names", array("RPMValue"), "duration_ms", duration), () -> {}).get("success"));
        }
        assertEquals(Boolean.FALSE, session.execute("capture_live_log", object("names", array("RPMValue"), "max_samples", 21), () -> {}).get("success"));
        assertEquals(Boolean.FALSE, session.execute("capture_engine_sniffer", object("timeoutMs", 10001), () -> {}).get("success"));
        assertEquals(Boolean.FALSE, session.execute("capture_engine_sniffer", object("max_events", 129), () -> {}).get("success"));
        assertEquals(Boolean.FALSE, session.execute("get_lua", object("max_lines", 121), () -> {}).get("success"));
        assertEquals(Boolean.FALSE, session.execute("get_lua", object("start_line", 0), () -> {}).get("success"));
        verify(protocol, never()).readFromPage(anyInt(), anyInt(), anyInt());
    }

    private void publish(int rpm, int millivolts, int checkEngine, int error, boolean full, long generation) {
        byte[] response = ByteBuffer.allocate(13).order(ByteOrder.LITTLE_ENDIAN).put((byte) 0)
                .putShort((short) rpm).putShort((short) millivolts).putShort((short) checkEngine)
                .putShort((short) error).putFloat(Float.POSITIVE_INFINITY).array();
        BitSet valid = new BitSet();
        valid.set(0, 12);
        sensors.grabSensorValues(new OutputChannelSnapshot(response, valid, Collections.emptySet(), generation, full), ini, null);
    }

    private static JSONObject channel(JSONObject result, String group, String name) {
        for (Object entry : (JSONArray) result.get(group)) {
            JSONObject value = (JSONObject) entry;
            if (name.equals(value.get("name"))) { return value; }
        }
        throw new AssertionError("Missing channel " + name);
    }

    private static JSONArray array(Object... values) {
        JSONArray result = new JSONArray();
        Collections.addAll(result, values);
        return result;
    }

    private static JSONObject object(Object... pairs) {
        JSONObject result = new JSONObject();
        for (int i = 0; i < pairs.length; i += 2) { result.put(pairs[i], pairs[i + 1]); }
        return result;
    }
}
