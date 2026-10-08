package com.rusefi.mcp;

import com.opensr5.ini.DatalogEntry;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TsStringFunction;
import com.opensr5.ini.field.IniField;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.TreeMap;

/** Descriptive INI metadata only: never evaluates expressions or reads the tune. */
@SuppressWarnings("unchecked")
final class ConsoleChannelCatalog {
    private static final int MAX_RESULT_CHARS = 48000;

    static JSONObject list(IniFileModel ini, String filter, Runnable cancellation) {
        Map<String, IniField> fields = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        fields.putAll(ini.getAllOutputChannels());
        Map<String, List<GaugeModel>> gauges = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        // Stable title selection when several gauges refer to one channel.
        for (GaugeModel gauge : new TreeMap<>(ini.getGauges()).values()) {
            cancellation.run();
            gauges.computeIfAbsent(gauge.getChannel(), ignored -> new ArrayList<>()).add(gauge);
        }
        JSONArray channels = new JSONArray();
        JSONObject result = object("channels", channels);
        int matches = 0;
        boolean full = false;
        String needle = filter.toLowerCase(Locale.ROOT);
        for (DatalogEntry entry : ini.getDatalogEntries()) {
            cancellation.run();
            JSONObject metadata = metadata(entry, fields.get(entry.getChannel()), gauges.get(entry.getChannel()));
            if (!(entry.getChannel() + " " + entry.getLabel() + " " + metadata.get("description")
                    + " " + (metadata.get("units") == null ? "" : metadata.get("units")))
                    .toLowerCase(Locale.ROOT).contains(needle)) {
                continue;
            }
            matches++;
            if (!full && channels.size() < 100) {
                channels.add(metadata);
                if (result.toJSONString().length() > MAX_RESULT_CHARS) {
                    channels.remove(channels.size() - 1);
                    full = true;
                }
            }
        }
        result.put("matches", matches);
        result.put("truncated", matches > channels.size());
        return result;
    }

    private static JSONObject metadata(DatalogEntry entry, IniField field, List<GaugeModel> gauges) {
        String description = entry.getLabel();
        String descriptionSource = "datalog";
        if (gauges != null) {
            for (GaugeModel gauge : gauges) {
                String title = gauge.getTitle();
                if (title != null && !title.trim().isEmpty() && !dynamic(title)) {
                    description = title;
                    descriptionSource = "gauge";
                    break;
                }
            }
        }
        JSONObject result = object("name", entry.getChannel(), "label", bounded(entry.getLabel(), 256),
                "description", bounded(description, 512), "descriptionSource", descriptionSource);
        boolean clipped = length(entry.getLabel()) > 256 || length(description) > 512;
        String units = field == null ? null : field.getUnits();
        String source = units == null ? "none" : "output_channel";
        String status = units == null ? "unknown" : dynamic(units) ? "dynamic" : "known";
        if (units == null && gauges != null) {
            boolean unresolved = false;
            boolean conflicting = false;
            for (GaugeModel gauge : gauges) {
                String candidate = gauge.getUnits();
                if (candidate == null) { continue; }
                source = "gauge";
                if (dynamic(candidate)) {
                    unresolved = true;
                } else if (units == null) {
                    units = candidate;
                } else if (!units.equals(candidate)) {
                    conflicting = true;
                }
            }
            status = conflicting ? "conflicting" : unresolved ? "dynamic" : units == null ? "unknown" : "known";
        }
        if (length(units) > 128) {
            clipped = true;
            status = "unknown";
        }
        result.put("units", "known".equals(status) ? units : null);
        result.put("unitsStatus", status);
        result.put("unitsSource", source);
        result.put("metadataTruncated", clipped);
        return result;
    }

    private static boolean dynamic(String text) {
        // Literal units such as m/s and titles such as RPM - engine speed are not arithmetic.
        return text.contains("{") || text.contains("}") || text.contains("?")
                || TsStringFunction.containsStringFunction(text);
    }

    private static int length(String text) { return text == null ? 0 : text.length(); }
    private static String bounded(String text, int limit) {
        return text == null ? null : text.substring(0, Math.min(limit, text.length()));
    }

    private static JSONObject object(Object... pairs) {
        JSONObject result = new JSONObject();
        for (int i = 0; i < pairs.length; i += 2) { result.put(pairs[i], pairs[i + 1]); }
        return result;
    }
}
