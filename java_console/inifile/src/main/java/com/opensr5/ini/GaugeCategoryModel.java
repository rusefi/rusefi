package com.opensr5.ini;

import java.util.*;

/**
 * Represents a category of gauges from INI file [GaugeConfigurations] section.
 */
public class GaugeCategoryModel {
    private final String name;
    private final List<GaugeModel> gauges;

    public GaugeCategoryModel(String name, List<GaugeModel> gauges) {
        this.name = name;
        this.gauges = Collections.unmodifiableList(new ArrayList<>(gauges));
    }

    public String getName() {
        return name;
    }

    public List<GaugeModel> getGauges() {
        return gauges;
    }
}
