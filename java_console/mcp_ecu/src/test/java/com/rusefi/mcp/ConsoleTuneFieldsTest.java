package com.rusefi.mcp;

import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.*;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.FieldType;
import com.rusefi.io.LinkManager;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

@SuppressWarnings("unchecked")
class ConsoleTuneFieldsTest {
    private final LinkManager link = mock(LinkManager.class);
    private final BinaryProtocol protocol = mock(BinaryProtocol.class);
    private final IniFileModel ini = mock(IniFileModel.class);
    private final Map<String, IniField> fields = new LinkedHashMap<>();
    private final Map<String, IniField> secondary = new LinkedHashMap<>();
    private final byte[] main = new byte[1024];
    private final byte[] extra = new byte[1024];
    private final ExecutorService wire = Executors.newSingleThreadExecutor(r -> new Thread(r, "test-tune-link"));
    private ConsoleEcuSession session;

    @BeforeEach void open() throws Exception {
        protocol.signature = "test-tune-ecu";
        when(link.getBinaryProtocol()).thenReturn(protocol);
        when(protocol.getIniFile()).thenReturn(ini);
        when(ini.getAllIniFields()).thenReturn(fields);
        when(ini.getSecondaryIniFields()).thenReturn(secondary);
        when(ini.getBlockingFactor()).thenReturn(4);
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(ini.getMetaInfo()).thenReturn(meta);
        when(meta.getnPages()).thenReturn(2);
        when(meta.getPageIdentifier(0)).thenReturn(0);
        when(meta.getPageIdentifier(1)).thenReturn(0x400);
        when(meta.getPageSize(anyInt())).thenReturn(1024);
        doAnswer(call -> wire.submit((Runnable) call.getArgument(0))).when(link).submit(any(Runnable.class));
        when(protocol.readFromPage(anyInt(), anyInt(), anyInt())).thenAnswer(call -> {
            assertEquals("test-tune-link", Thread.currentThread().getName());
            int page = call.getArgument(0), offset = call.getArgument(1), size = call.getArgument(2);
            return Arrays.copyOfRange(page == 0 ? main : extra, offset, offset + size);
        });
        session = new ConsoleEcuSession(link);
    }

    @AfterEach void close() throws Exception {
        session.close();
        wire.shutdown();
        assertTrue(wire.awaitTermination(2, TimeUnit.SECONDS));
        verify(link, never()).close();
        verify(protocol, never()).close();
        verify(protocol, never()).getControllerConfiguration();
    }

    @Test void readsOnlyRequestedRangesAndDecodesScaledScalarsEnumsAndSecondaryArrays() throws Exception {
        fields.put("scaled", new ScalarIniField("scaled", 10, "deg C", FieldType.INT16, .5, "1", 30));
        fields.put("mode", new EnumIniField("mode", 30, FieldType.UINT16,
                new EnumIniField.EnumKeyValueMap(Collections.singletonMap(10, "Selected")), 4, 3));
        ArrayIniField table = new ArrayIniField("smallTable", 8, FieldType.UINT16, 3, 2, "ms", .5, null, null, "1");
        table.setPageIndex(0x400);
        secondary.put("smallTable", table);
        ByteBuffer.wrap(main).order(ByteOrder.LITTLE_ENDIAN).putShort(10, (short) -40).putShort(30, (short) 0xa0);
        ByteBuffer bytes = ByteBuffer.wrap(extra).order(ByteOrder.LITTLE_ENDIAN);
        for (int i = 0; i < 6; i++) { bytes.putShort(8 + i * 2, (short) (i + 1)); }
        JSONObject result = read("SCALED", "mode", "smalltable");
        assertEquals(Boolean.TRUE, result.get("complete"));
        assertEquals("ecu", result.get("source"));
        assertEquals("test-tune-ecu", result.get("signature"));
        assertEquals(10.0, item(result, 0).get("value"));
        assertEquals("scaled", item(result, 0).get("name"));
        assertEquals("deg C", item(result, 0).get("units"));
        assertEquals(10L, item(result, 1).get("value"));
        assertEquals("Selected", item(result, 1).get("label"));
        assertEquals(array(array(.5, 1.0, 1.5), array(2.0, 2.5, 3.0)), item(result, 2).get("values"));
        assertEquals(0x400, item(result, 2).get("page"));
        assertTrue(((Number) item(result, 0).get("readCompletedTimestampMs")).longValue() >=
                ((Number) result.get("readStartedTimestampMs")).longValue());
        verify(protocol).readFromPage(0, 10, 2);
        verify(protocol).readFromPage(0, 30, 2);
        verify(protocol).readFromPage(0x400, 8, 4);
        verify(protocol).readFromPage(0x400, 12, 4);
        verify(protocol).readFromPage(0x400, 16, 4);
        verify(protocol, times(5)).readFromPage(anyInt(), anyInt(), anyInt());
    }

    @Test void unavailableUnsupportedAndOutOfBoundsFieldsNeverReadBytes() throws Exception {
        fields.put("lua", new StringIniField("lua", 0, 100));
        fields.put("large", new ArrayIniField("large", 0, FieldType.FLOAT, 16, 16, "", 1, null, null, "0"));
        scalar("outside", 1023, FieldType.UINT16);
        scalar("negative", -1, FieldType.UINT8);
        scalar("pageMissing", 0, FieldType.UINT8).setPageIndex(99);
        fields.put("badBits", new EnumIniField("badBits", 0, FieldType.UINT8,
                new EnumIniField.EnumKeyValueMap(Collections.emptyMap()), 7, 1));
        JSONObject result = read("missing", "lua", "large", "outside", "negative", "pageMissing", "badBits");
        assertEquals(Boolean.FALSE, result.get("complete"));
        for (Object field : (JSONArray) result.get("fields")) {
            assertEquals(Boolean.FALSE, ((JSONObject) field).get("success"));
            assertFalse(((JSONObject) field).containsKey("value"));
        }
        verify(protocol, never()).readFromPage(anyInt(), anyInt(), anyInt());
    }

    @Test void marksFailedAndNonFiniteReadsWithoutHidingOtherSelectedValues() throws Exception {
        scalar("failed", 0, FieldType.UINT8);
        scalar("nan", 4, FieldType.FLOAT);
        scalar("good", 8, FieldType.UINT8);
        ByteBuffer.wrap(main).order(ByteOrder.LITTLE_ENDIAN).putFloat(4, Float.NaN);
        main[8] = 42;
        doReturn(null).when(protocol).readFromPage(0, 0, 1);
        JSONObject result = read("failed", "nan", "good");
        assertEquals(Boolean.FALSE, result.get("complete"));
        assertEquals(Boolean.FALSE, item(result, 0).get("success"));
        assertEquals(Boolean.FALSE, item(result, 1).get("success"));
        assertFalse(item(result, 1).containsKey("value"));
        assertEquals(42.0, item(result, 2).get("value"));
    }

    @Test void honorsAggregateValueBudgetAndDoesNotResolveDynamicUnits() throws Exception {
        when(ini.getBlockingFactor()).thenReturn(256);
        JSONArray names = new JSONArray();
        for (int i = 0; i < 9; i++) {
            String name = "array" + i;
            names.add(name);
            fields.put(name, new ArrayIniField(name, i * 64, FieldType.UINT8, 8, 8,
                    "{units}", 1, null, null, "0"));
        }
        JSONObject result = session.execute("read_tune_fields", object("names", names), () -> {});
        assertEquals(Boolean.FALSE, result.get("complete"));
        assertEquals(Boolean.TRUE, item(result, 7).get("success"));
        assertEquals(Boolean.FALSE, item(result, 8).get("success"));
        assertEquals("dynamic", item(result, 0).get("unitsStatus"));
        assertNull(item(result, 0).get("units"));
        verify(protocol, times(8)).readFromPage(anyInt(), anyInt(), eq(64));
    }

    @Test void extractsFullWidthUnsignedEnumWithoutInventingAnUnknownLabel() throws Exception {
        fields.put("bits", new EnumIniField("bits", 0, FieldType.INT,
                new EnumIniField.EnumKeyValueMap(Collections.emptyMap()), 0, 31));
        Arrays.fill(main, 0, 4, (byte) 0xff);
        JSONObject field = item(read("bits"), 0);
        assertEquals(4294967295L, field.get("value"));
        assertNull(field.get("label"));
    }

    @Test void rejectsMalformedNamesBeforeSubmittingWork() throws Exception {
        JSONArray tooMany = new JSONArray();
        for (int i = 0; i < 33; i++) { tooMany.add("field" + i); }
        for (Object names : new Object[]{null, "a", array(), array(123), array("a", "A"), array(" "), tooMany}) {
            assertEquals(Boolean.FALSE, session.execute("read_tune_fields", object("names", names), () -> {}).get("success"));
        }
        verify(link, never()).submit(any(Runnable.class));
    }

    @Test void cancelledOrTimedOutQueuedReadsNeverStartLater() throws Exception {
        AtomicReference<Runnable> queued = new AtomicReference<>();
        doAnswer(call -> { queued.set(call.getArgument(0)); return null; }).when(link).submit(any(Runnable.class));
        AtomicInteger checks = new AtomicInteger();
        assertThrows(CancellationException.class, () -> session.execute("read_tune_fields", object("names", array("a")), () -> {
            if (checks.incrementAndGet() == 3) { throw new CancellationException(); }
        }));
        queued.get().run();
        assertThrows(CancellationException.class, () -> ConsoleTuneFields.read(link, protocol, array("a"), () -> {}, 20));
        queued.get().run();
        verify(protocol, never()).getIniFile();
        verify(protocol, never()).readFromPage(anyInt(), anyInt(), anyInt());
    }

    @Test void replacingConnectionAfterOneChunkDiscardsTurnAndStopsRemainingChunks() throws Exception {
        fields.put("table", new ArrayIniField("table", 0, FieldType.UINT8, 8, 1, "", 1, null, null, "0"));
        doAnswer(call -> {
            when(link.getBinaryProtocol()).thenReturn(mock(BinaryProtocol.class));
            return new byte[4];
        }).when(protocol).readFromPage(0, 0, 4);
        assertThrows(CancellationException.class, () -> read("table"));
        verify(protocol, times(1)).readFromPage(anyInt(), anyInt(), anyInt());
    }

    @Test void cancellationReturnsPromptlyAndDoesNotInterruptAnInFlightWireRead() throws Exception {
        fields.put("table", new ArrayIniField("table", 0, FieldType.UINT8, 8, 1, "", 1, null, null, "0"));
        CountDownLatch entered = new CountDownLatch(1), release = new CountDownLatch(1);
        AtomicBoolean cancelled = new AtomicBoolean();
        doAnswer(call -> {
            entered.countDown();
            assertTrue(release.await(2, TimeUnit.SECONDS));
            assertFalse(Thread.currentThread().isInterrupted());
            return new byte[4];
        }).when(protocol).readFromPage(0, 0, 4);
        ExecutorService caller = Executors.newSingleThreadExecutor();
        try {
            Future<JSONObject> result = caller.submit(() -> session.execute("read_tune_fields", object("names", array("table")), () -> {
                if (cancelled.get()) { throw new CancellationException(); }
            }));
            assertTrue(entered.await(1, TimeUnit.SECONDS));
            cancelled.set(true);
            ExecutionException failure = assertThrows(ExecutionException.class, () -> result.get(1, TimeUnit.SECONDS));
            assertInstanceOf(CancellationException.class, failure.getCause());
        } finally {
            release.countDown();
            caller.shutdownNow();
        }
        wire.submit(() -> {}).get(1, TimeUnit.SECONDS);
        verify(protocol, times(1)).readFromPage(anyInt(), anyInt(), anyInt());
    }

    private ScalarIniField scalar(String name, int offset, FieldType type) {
        ScalarIniField field = new ScalarIniField(name, offset, "", type, 1, "0", 0);
        fields.put(name, field);
        return field;
    }
    private JSONObject read(String... names) throws Exception {
        return session.execute("read_tune_fields", object("names", array((Object[]) names)), () -> {});
    }
    private static JSONObject item(JSONObject result, int index) { return (JSONObject) ((JSONArray) result.get("fields")).get(index); }
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
