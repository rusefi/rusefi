package com.rusefi.mcp;

import com.rusefi.config.generated.Integration;
import com.rusefi.core.EngineState;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.TimeUnit;

/** Observe one future Console chart without taking ownership of the UI's string action. */
@SuppressWarnings("unchecked")
final class ConsoleSnifferCapture {
    static JSONObject capture(EngineState state, int timeoutMs, int maxEvents, Runnable check) throws Exception {
        ArrayBlockingQueue<Chart> charts = new ArrayBlockingQueue<>(1);
        long deadline = System.nanoTime() + TimeUnit.MILLISECONDS.toNanos(timeoutMs);
        check.run();
        try (AutoCloseable observer = state.addStringValueObserver(Integration.PROTOCOL_ENGINE_SNIFFER, raw -> {
            if (!raw.isEmpty()) { charts.offer(new Chart(raw.length() > 35000 ? null : raw)); }
        })) {
            while (System.nanoTime() - deadline < 0) {
                check.run();
                Chart chart = charts.poll(Math.max(1, Math.min(TimeUnit.MILLISECONDS.toNanos(50), deadline - System.nanoTime())), TimeUnit.NANOSECONDS);
                if (chart != null) {
                    check.run();
                    if (chart.raw == null) { return error("Sniffer chart exceeds 35000 characters."); }
                    try {
                        JSONObject result = EngineSnifferCapture.decode(chart.raw, chart.receivedAt);
                        JSONArray events = (JSONArray) result.get("events");
                        for (Object item : events) {
                            check.run();
                            JSONObject event = (JSONObject) item;
                            if (((String) event.get("channel")).length() > 64 || ((String) event.get("signal")).length() > 64) {
                                return error("Sniffer identifiers exceed the result limits.");
                            }
                        }
                        JSONArray channels = (JSONArray) result.get("channels");
                        boolean truncated = events.size() > maxEvents || channels.size() > 32;
                        while (events.size() > maxEvents) { events.remove(events.size() - 1); }
                        while (channels.size() > 32) { channels.remove(channels.size() - 1); }
                        result.remove("raw");
                        result.put("truncated", truncated);
                        result.put("note", "One received chart using existing sniffer settings. Event count and duration describe the full chart; returned events/channels may be partial. Event times are chart-relative, receivedAt is host time. No enable, reset or actuator command was sent.");
                        while (result.toJSONString().length() > 48000 && !events.isEmpty()) {
                            check.run();
                            events.remove(events.size() - 1);
                            result.put("truncated", true);
                        }
                        check.run();
                        return result;
                    } catch (IllegalArgumentException malformed) {
                        return error("Malformed engine-sniffer chart.");
                    }
                }
            }
            check.run();
            return error("No new engine-sniffer chart arrived. Check engine activity and current Console sniffer settings; the assistant does not enable or reset acquisition.");
        }
    }

    private static JSONObject error(String message) {
        JSONObject result = new JSONObject();
        result.put("success", false);
        result.put("error", message);
        return result;
    }
    private static final class Chart {
        final String raw;
        final long receivedAt = System.currentTimeMillis();
        Chart(String raw) { this.raw = raw; }
    }
}
