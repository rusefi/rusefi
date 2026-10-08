package com.rusefi.mcp;

import com.rusefi.config.generated.Integration;
import com.rusefi.core.EngineState;
import com.rusefi.io.LinkDecoder;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;

import java.util.concurrent.CancellationException;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class ConsoleSnifferCaptureTest {
    private final EngineState state = new EngineState(new EngineState.EngineStateListenerImpl());

    @Test void observesAlongsideConsoleOwnerAndNeverReplaysAnEarlierChart() throws Exception {
        AtomicReference<String> console = new AtomicReference<>();
        state.registerStringValueAction(Integration.PROTOCOL_ENGINE_SNIFFER, console::set);
        deliver("c1!u!999!");
        AtomicInteger checks = new AtomicInteger();
        JSONObject result = ConsoleSnifferCapture.capture(state, 1000, 100, () -> {
            if (checks.incrementAndGet() == 2) { deliver("c1!u_1!0!c1!d_2!20!"); }
        });
        assertEquals(Boolean.TRUE, result.get("success"));
        assertEquals(2, result.get("eventCount"));
        assertEquals(200L, result.get("durationUs"));
        assertEquals("c1!u_1!0!c1!d_2!20!", console.get());
        assertFalse(result.containsKey("raw"));
        JSONObject expired = ConsoleSnifferCapture.capture(state, 1, 100, () -> {});
        assertEquals(Boolean.FALSE, expired.get("success"));
        deliver("c1!u!100!");
        assertEquals("c1!u!100!", console.get());
    }

    @Test void boundsEventsChannelsAndSerializedResultButKeepsFullChartCounts() throws Exception {
        StringBuilder chart = new StringBuilder();
        for (int i = 0; i < 180; i++) { chart.append("c").append(i % 40).append("!u!").append(i).append("!"); }
        AtomicInteger checks = new AtomicInteger();
        JSONObject result = ConsoleSnifferCapture.capture(state, 1000, 10, () -> {
            if (checks.incrementAndGet() == 2) { deliver(chart.toString()); }
        });
        assertEquals(180, result.get("eventCount"));
        assertEquals(10, ((JSONArray) result.get("events")).size());
        assertEquals(32, ((JSONArray) result.get("channels")).size());
        assertEquals(Boolean.TRUE, result.get("truncated"));
        assertTrue(result.toJSONString().length() < 49000);
    }

    @Test void rejectsMalformedAndOversizedChartsWithoutThrowingIntoTheConsole() throws Exception {
        for (String raw : new String[]{"c1!u!bad!", "x".repeat(35001), "x".repeat(65) + "!u!1!"}) {
            AtomicInteger checks = new AtomicInteger();
            JSONObject result = ConsoleSnifferCapture.capture(state, 1000, 100, () -> {
                if (checks.incrementAndGet() == 2) { deliver(raw); }
            });
            assertEquals(Boolean.FALSE, result.get("success"));
        }
    }

    @Test void cancellationAndTimeoutCloseOnlyTheObserver() throws Exception {
        EngineState mocked = mock(EngineState.class);
        AutoCloseable token = mock(AutoCloseable.class);
        when(mocked.addStringValueObserver(eq(Integration.PROTOCOL_ENGINE_SNIFFER), any())).thenReturn(token);
        AtomicInteger checks = new AtomicInteger();
        assertThrows(CancellationException.class, () -> ConsoleSnifferCapture.capture(mocked, 1000, 100, () -> {
            if (checks.incrementAndGet() == 2) { throw new CancellationException(); }
        }));
        verify(token).close();
        verify(mocked, never()).removeAction(anyString());
        assertEquals(Boolean.FALSE, ConsoleSnifferCapture.capture(mocked, 1, 100, () -> {}).get("success"));
        verify(token, times(2)).close();
    }

    private void deliver(String raw) {
        state.processNewData(Integration.PROTOCOL_ENGINE_SNIFFER + Integration.LOG_DELIMITER + raw
                + Integration.LOG_DELIMITER + "\r\n", LinkDecoder.VOID);
    }
}
