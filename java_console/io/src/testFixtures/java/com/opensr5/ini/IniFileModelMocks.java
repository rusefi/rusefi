package com.opensr5.ini;

import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Optional;

import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.RETURNS_DEFAULTS;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * Mockito-based factory replacing the hand-written {@code StubIniFileModel} stubs that
 * previously had to override every {@link IniFileModel} method with empty/null defaults.
 * Tests call {@link #empty()} for a baseline mock that is safe to drive through
 * {@code SensorCentral#grabSensorValues}, then layer additional Mockito stubs on top
 * for whatever the specific test cares about.
 */
public final class IniFileModelMocks {
    private IniFileModelMocks() {}

    /**
     * Returns a fresh {@link IniFileModel} mock with the minimum stubbing required so
     * that real production code (notably {@code ISensorHolder#grabSensorValues}) can
     * iterate over its collections without NPEs:
     * empty maps for {@code getAllOutputChannels}/{@code getGauges} and
     * {@link Optional#empty()} for {@code findIniField(...)}.
     */
    public static IniFileModel empty() {
        IniFileModel m = mock(IniFileModel.class, RETURNS_DEFAULTS);
        when(m.getAllOutputChannels()).thenReturn(Collections.emptyMap());
        when(m.getGauges()).thenReturn(Collections.emptyMap());
        when(m.findIniField(anyString())).thenReturn(Optional.empty());
        return m;
    }

    /**
     * Returns a mock pre-populated with the given gauges, exposed via both
     * {@code getGauges()} and {@code getGauge(name)}.
     */
    public static IniFileModel withGauges(GaugeModel... gauges) {
        IniFileModel m = empty();
        Map<String, GaugeModel> map = new LinkedHashMap<>();
        for (GaugeModel g : gauges) {
            map.put(g.getName(), g);
        }
        when(m.getGauges()).thenReturn(map);
        for (Map.Entry<String, GaugeModel> e : map.entrySet()) {
            when(m.getGauge(e.getKey())).thenReturn(e.getValue());
        }
        return m;
    }

    /**
     * Mutable-style builder: returns a mock plus a backing map; callers can
     * {@link GaugeRegistry#register(GaugeModel)} more gauges later (the mock holds
     * a live reference to the map).
     */
    public static GaugeRegistry mutableWithGauges() {
        IniFileModel m = empty();
        Map<String, GaugeModel> map = new HashMap<>();
        when(m.getGauges()).thenReturn(map);
        return new GaugeRegistry(m, map);
    }

    public static final class GaugeRegistry {
        public final IniFileModel model;
        private final Map<String, GaugeModel> gauges;

        GaugeRegistry(IniFileModel model, Map<String, GaugeModel> gauges) {
            this.model = model;
            this.gauges = gauges;
        }

        public void register(GaugeModel g) {
            gauges.put(g.getName(), g);
            when(model.getGauge(g.getName())).thenReturn(g);
        }
    }
}
