package com.rusefi.mcp;

import com.rusefi.config.generated.Integration;
import com.rusefi.core.EngineState;
import com.rusefi.io.LinkDecoder;
import com.rusefi.io.LinkManager;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;

import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class EngineSnifferCaptureTest {
    private final EngineState state = new EngineState(new EngineState.EngineStateListenerImpl());
    private final LinkManager link = mock(LinkManager.class);

    private void deliver(String raw) {
        state.processNewData(Integration.PROTOCOL_ENGINE_SNIFFER + Integration.LOG_DELIMITER + raw
                + Integration.LOG_DELIMITER + "\r\n", LinkDecoder.VOID);
    }

    private void assertListenerReleased() {
        AtomicReference<String> next = new AtomicReference<>();
        state.registerStringValueAction(Integration.PROTOCOL_ENGINE_SNIFFER, next::set);
        deliver("c1!u!9!");
        assertEquals("c1!u!9!", next.get());
        state.removeAction(Integration.PROTOCOL_ENGINE_SNIFFER);
    }

    @Test
    void capturesFirstChartThroughEngineStateAndDoesNotReplayIt() throws Exception {
        when(link.getEngineState()).thenReturn(state);
        when(link.isActive()).thenAnswer(call -> {
            deliver(""); // Empty status is not a waveform.
            deliver("c1!u_12!0!inj1!u!3!c1!d_13!11!r!1200!12!inj1!d!20!");
            deliver("c1!u!999!"); // A later chart must not replace the first.
            return true;
        });
        JSONObject result = EngineSnifferCapture.capture(link, 1000);
        assertEquals(true, result.get("success"));
        assertEquals("wave_chart", result.get("source"));
        assertEquals(5, result.get("eventCount"));
        assertEquals(200L, result.get("durationUs"));
        assertEquals(10, result.get("resolutionUs"));
        assertTrue((long) result.get("receivedAt") > 0);
        JSONArray events = (JSONArray) result.get("events");
        JSONObject crank = (JSONObject) events.get(2);
        assertEquals("c1", crank.get("channel"));
        assertEquals("d_13", crank.get("signal"));
        assertEquals("falling", crank.get("edge"));
        assertEquals(13, crank.get("triggerIndex"));
        assertEquals(110L, crank.get("timeUs"));
        assertEquals(1200, ((JSONObject) events.get(3)).get("rpm"));
        JSONArray channels = (JSONArray) result.get("channels");
        assertEquals(3, channels.size());
        JSONObject injector = (JSONObject) channels.get(1);
        assertEquals("inj1", injector.get("channel"));
        assertEquals(2L, injector.get("eventCount"));
        assertEquals(1L, injector.get("risingEdges"));
        assertEquals(1L, injector.get("fallingEdges"));
        assertEquals(30L, injector.get("firstTimeUs"));
        assertEquals(200L, injector.get("lastTimeUs"));
        assertListenerReleased();

        when(link.isActive()).thenReturn(true);
        result = EngineSnifferCapture.capture(link, 10);
        assertEquals(false, result.get("success"));
        assertTrue(result.get("error").toString().contains("Timed out"));
        assertListenerReleased();
        verify(link, never()).getCommandQueue();
    }

    @Test
    void disconnectAndMalformedChartsReleaseListener() throws Exception {
        when(link.getEngineState()).thenReturn(state);
        when(link.isActive()).thenReturn(false);
        JSONObject result = EngineSnifferCapture.capture(link, 1000);
        assertEquals(false, result.get("success"));
        assertTrue(result.get("error").toString().contains("disconnected"));
        assertListenerReleased();

        when(link.isActive()).thenAnswer(call -> {
            deliver("c1!u!bad-time!");
            return true;
        });
        assertThrows(IllegalArgumentException.class, () -> EngineSnifferCapture.capture(link, 1000));
        assertListenerReleased();
    }

    @Test
    void interruptionReleasesListener() {
        when(link.getEngineState()).thenReturn(state);
        when(link.isActive()).thenAnswer(call -> {
            Thread.currentThread().interrupt();
            return true;
        });
        try {
            assertThrows(InterruptedException.class, () -> EngineSnifferCapture.capture(link, 1000));
        } finally {
            Thread.interrupted();
        }
        assertListenerReleased();
    }

    @Test
    void doesNotReplaceOrRemoveExistingConsumer() {
        AtomicReference<String> existing = new AtomicReference<>();
        state.registerStringValueAction(Integration.PROTOCOL_ENGINE_SNIFFER, existing::set);
        when(link.getEngineState()).thenReturn(state);
        assertThrows(IllegalStateException.class, () -> EngineSnifferCapture.capture(link, 1000));
        deliver("c1!u!7!");
        assertEquals("c1!u!7!", existing.get());
    }

    @Test
    void validatesWireRecordsAndKeepsUnknownSignals() {
        for (String bad : new String[]{"", "c1!u!", "c1!u!1!cam!", "!u!0!", "c1!!0!",
                "c1!u!-1!", "c1!u!4294967296!", "c1!u!NaN!", "x".repeat(35001)}) {
            assertThrows(IllegalArgumentException.class, () -> EngineSnifferCapture.decode(bad, 1), bad);
        }
        JSONObject result = EngineSnifferCapture.decode("custom!marker!4294967295!c1!u!1!", 42);
        JSONArray events = (JSONArray) result.get("events");
        JSONObject unknown = (JSONObject) events.get(0);
        assertEquals("marker", unknown.get("signal"));
        assertFalse(unknown.containsKey("edge"));
        assertEquals(42949672950L, unknown.get("timeUs"));
        // Preserve producer order even if timestamps arrive out of order.
        assertEquals(42949672940L, result.get("durationUs"));
        assertEquals(42L, result.get("receivedAt"));
    }
}
