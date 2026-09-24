package com.rusefi.mcp;

import com.rusefi.config.generated.Integration;
import com.rusefi.core.EngineState;
import com.rusefi.io.LinkManager;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.TimeUnit;

/** One Console Digital Sniffer chart, received through the normal text polling path. */
final class EngineSnifferCapture {
    private EngineSnifferCapture() {
    }

    static JSONObject capture(LinkManager link, long timeoutMs) throws InterruptedException {
        EngineState state = link.getEngineState();
        ArrayBlockingQueue<Chart> charts = new ArrayBlockingQueue<>(1);
        long deadline = System.nanoTime() + TimeUnit.MILLISECONDS.toNanos(timeoutMs);
        // EngineState allows one owner per key. Fail rather than replace an existing consumer.
        state.registerStringValueAction(Integration.PROTOCOL_ENGINE_SNIFFER, value -> {
            if (!value.isEmpty()) {
                charts.offer(new Chart(value, System.currentTimeMillis()));
            }
        });
        try {
            while (System.nanoTime() < deadline) {
                if (!link.isActive()) {
                    return failure("ECU disconnected while waiting for an engine-sniffer chart");
                }
                long remaining = deadline - System.nanoTime();
                Chart chart = charts.poll(Math.max(1, Math.min(remaining,
                        TimeUnit.MILLISECONDS.toNanos(100))), TimeUnit.NANOSECONDS);
                if (chart != null) {
                    return decode(chart.raw, chart.receivedAt);
                }
            }
            return failure("Timed out waiting for wave_chart. Check engine activity, engineChartSize, " +
                    "engineSnifferRpmThreshold, and firmware EFI_ENGINE_SNIFFER/EFI_TEXT_LOGGING support. " +
                    "Capture uses the current settings and does not enable or reset the sniffer.");
        } finally {
            state.removeAction(Integration.PROTOCOL_ENGINE_SNIFFER);
        }
    }

    @SuppressWarnings("unchecked")
    static JSONObject decode(String raw, long receivedAt) {
        // Production charts use a 5000-byte buffer (35000 in non-production builds).
        if (raw.length() > 35000) {
            throw new IllegalArgumentException("Engine-sniffer chart exceeds 35000 characters");
        }
        String[] tokens = raw.split("!", -1);
        int count = tokens.length;
        if (tokens[count - 1].isEmpty()) {
            count--;
        }
        if (count == 0 || count % 3 != 0) {
            throw new IllegalArgumentException("Incomplete engine-sniffer channel/event/time triplet");
        }
        JSONArray events = new JSONArray();
        Map<String, JSONObject> channels = new LinkedHashMap<>();
        long first = Long.MAX_VALUE;
        long last = 0;
        for (int i = 0; i < count; i += 3) {
            String channel = tokens[i];
            String signal = tokens[i + 1];
            if (channel.isEmpty() || signal.isEmpty()) {
                throw new IllegalArgumentException("Empty engine-sniffer channel or event");
            }
            long ticks = Long.parseLong(tokens[i + 2]);
            if (ticks < 0 || ticks > 0xffffffffL) {
                throw new IllegalArgumentException("Invalid engine-sniffer timestamp: " + tokens[i + 2]);
            }
            long timeUs = ticks * Integration.ENGINE_SNIFFER_UNIT_US;
            JSONObject event = new JSONObject();
            event.put("channel", channel);
            event.put("signal", signal);
            event.put("timeUs", timeUs);
            String edge = null;
            if (signal.equals("u") || signal.startsWith("u_")) {
                edge = "rising";
            } else if (signal.equals("d") || signal.startsWith("d_")) {
                edge = "falling";
            }
            if (edge != null) {
                event.put("edge", edge);
                if (signal.length() > 2) {
                    event.put("triggerIndex", Integer.parseInt(signal.substring(2)));
                }
            } else if (Integration.TOP_DEAD_CENTER_MESSAGE.equals(channel)) {
                event.put("rpm", Integer.parseInt(signal));
            }
            events.add(event);
            JSONObject summary = channels.get(channel);
            if (summary == null) {
                summary = new JSONObject();
                summary.put("channel", channel);
                summary.put("eventCount", 0L);
                summary.put("risingEdges", 0L);
                summary.put("fallingEdges", 0L);
                summary.put("firstTimeUs", timeUs);
                summary.put("lastTimeUs", timeUs);
                channels.put(channel, summary);
            }
            summary.put("eventCount", (long) summary.get("eventCount") + 1);
            if (edge != null) {
                String key = edge.equals("rising") ? "risingEdges" : "fallingEdges";
                summary.put(key, (long) summary.get(key) + 1);
            }
            summary.put("firstTimeUs", Math.min((long) summary.get("firstTimeUs"), timeUs));
            summary.put("lastTimeUs", Math.max((long) summary.get("lastTimeUs"), timeUs));
            first = Math.min(first, timeUs);
            last = Math.max(last, timeUs);
        }
        JSONObject result = new JSONObject();
        result.put("success", true);
        result.put("source", Integration.PROTOCOL_ENGINE_SNIFFER);
        result.put("receivedAt", receivedAt);
        result.put("resolutionUs", Integration.ENGINE_SNIFFER_UNIT_US);
        result.put("eventCount", events.size());
        result.put("durationUs", last - first);
        result.put("events", events);
        JSONArray summaries = new JSONArray();
        summaries.addAll(channels.values());
        result.put("channels", summaries);
        result.put("raw", raw);
        return result;
    }

    @SuppressWarnings("unchecked")
    private static JSONObject failure(String message) {
        JSONObject result = new JSONObject();
        result.put("success", false);
        result.put("error", message);
        return result;
    }

    private static final class Chart {
        final String raw;
        final long receivedAt;

        Chart(String raw, long receivedAt) {
            this.raw = raw;
            this.receivedAt = receivedAt;
        }
    }
}
