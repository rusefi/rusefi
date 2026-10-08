package com.rusefi.mcp;

import com.opensr5.ini.DatalogEntry;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CancellationException;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class ConsoleChannelCatalogTest {
    private final IniFileModel ini = mock(IniFileModel.class);
    private final Map<String, IniField> fields = new LinkedHashMap<>();
    private final Map<String, GaugeModel> gauges = new LinkedHashMap<>();
    private final List<DatalogEntry> entries = new ArrayList<>();

    private JSONObject list(String filter) {
        when(ini.getAllOutputChannels()).thenReturn(fields);
        when(ini.getGauges()).thenReturn(gauges);
        when(ini.getDatalogEntries()).thenReturn(entries);
        return ConsoleChannelCatalog.list(ini, filter, () -> {});
    }

    @Test void usesOutputUnitsAndStableGaugeDescriptionsWithCaseInsensitiveMatching() {
        entries.add(new DatalogEntry("coolant", "CLT"));
        field("Coolant", "deg C");
        gauge("zGauge", "COOLANT", "Other temperature", "deg F");
        gauge("aGauge", "Coolant", "Coolant - temperature", "deg F");
        JSONObject channel = first(list("TEMPERATURE"));
        assertEquals("coolant", channel.get("name"));
        assertEquals("CLT", channel.get("label"));
        assertEquals("Coolant - temperature", channel.get("description"));
        assertEquals("gauge", channel.get("descriptionSource"));
        assertEquals("deg C", channel.get("units"));
        assertEquals("output_channel", channel.get("unitsSource"));
        assertEquals("known", channel.get("unitsStatus"));
        assertEquals(1, list("DEG C").get("matches"));
        assertEquals(0, list("deg F").get("matches"));
    }

    @Test void distinguishesMissingUnitsFromExplicitlyEmptyUnitsAndPreservesRatioUnits() {
        entries.add(new DatalogEntry("unknown", "Unspecified quantity"));
        assertEquals("unknown", first(list("")).get("unitsStatus"));
        assertNull(first(list("")).get("units"));
        assertEquals("Unspecified quantity", first(list("")).get("description"));
        assertEquals("datalog", first(list("")).get("descriptionSource"));
        field("unknown", "");
        gauge("gauge", "unknown", "", "V");
        assertEquals("known", first(list("")).get("unitsStatus"));
        assertEquals("", first(list("")).get("units"));
        field("unknown", "m/s");
        assertEquals("m/s", first(list("")).get("units"));
    }

    @Test void expressionChannelsUseOnlyUnambiguousLiteralGaugeUnits() {
        entries.add(new DatalogEntry("derived", "Derived speed"));
        gauge("aGauge", "derived", "Speed", "km/h");
        gauge("bGauge", "derived", "Speed", "km/h");
        assertEquals("km/h", first(list("")).get("units"));
        assertEquals("gauge", first(list("")).get("unitsSource"));
        gauge("bGauge", "derived", "Speed", "mph");
        assertEquals("conflicting", first(list("")).get("unitsStatus"));
        assertNull(first(list("")).get("units"));
        gauge("bGauge", "derived", "{ stringValue(note) }", "{ useMetric ? \"km/h\" : \"mph\" }");
        assertEquals("dynamic", first(list("")).get("unitsStatus"));
        assertNull(first(list("")).get("units"));
    }

    @Test void doesNotEvaluateDynamicTitlesOrReplaceDynamicOutputUnitsWithGaugeUnits() {
        entries.add(new DatalogEntry("debug", "Debug channel"));
        field("debug", "{ bitStringValue(units, mode) }");
        gauge("gauge", "debug", "stringValue(debugName)", "V");
        JSONObject channel = first(list(""));
        assertEquals("Debug channel", channel.get("description"));
        assertEquals("dynamic", channel.get("unitsStatus"));
        assertNull(channel.get("units"));
        assertEquals("output_channel", channel.get("unitsSource"));
    }

    @Test void limitsResultCountAndSupportsNarrowingByDescriptionOrUnits() {
        for (int i = 0; i < 101; i++) {
            entries.add(new DatalogEntry("channel" + i, "Label" + i));
        }
        gauge("gauge", "channel100", "Manifold pressure", "kPa");
        JSONObject all = list("");
        assertEquals(101, all.get("matches"));
        assertEquals(100, ((JSONArray) all.get("channels")).size());
        assertEquals(Boolean.TRUE, all.get("truncated"));
        assertEquals("channel100", first(list("MANIFOLD")).get("name"));
        assertEquals("channel100", first(list("kpa")).get("name"));
        assertEquals(Boolean.FALSE, list("kpa").get("truncated"));
    }

    @Test void boundsLongMetadataAndEscapedJsonWithoutInventingTruncatedUnits() {
        String longText = String.join("", Collections.nCopies(800, "\u0001"));
        for (int i = 0; i < 100; i++) {
            entries.add(new DatalogEntry("channel" + i, longText));
        }
        field("channel0", String.join("", Collections.nCopies(129, "u")));
        JSONObject all = list("");
        JSONObject first = first(all);
        assertEquals(256, ((String) first.get("label")).length());
        assertEquals(512, ((String) first.get("description")).length());
        assertEquals(Boolean.TRUE, first.get("metadataTruncated"));
        assertNull(first.get("units"));
        assertEquals("unknown", first.get("unitsStatus"));
        assertEquals(100, all.get("matches"));
        assertEquals(Boolean.TRUE, all.get("truncated"));
        assertTrue(all.toJSONString().length() < 49000);
    }

    @Test void scanningCanBeCancelled() {
        entries.add(new DatalogEntry("a", "First"));
        entries.add(new DatalogEntry("b", "Second"));
        list("");
        AtomicInteger checks = new AtomicInteger();
        assertThrows(CancellationException.class, () -> ConsoleChannelCatalog.list(ini, "", () -> {
            if (checks.incrementAndGet() == 2) { throw new CancellationException(); }
        }));
    }

    private void field(String name, String units) {
        fields.put(name, new ScalarIniField(name, 0, units, FieldType.UINT16, 1, "0", 0));
    }

    private void gauge(String name, String channel, String title, String units) {
        GaugeModel gauge = mock(GaugeModel.class);
        when(gauge.getChannel()).thenReturn(channel);
        when(gauge.getTitle()).thenReturn(title);
        when(gauge.getUnits()).thenReturn(units);
        gauges.put(name, gauge);
    }

    private static JSONObject first(JSONObject result) {
        return (JSONObject) ((JSONArray) result.get("channels")).get(0);
    }
}
