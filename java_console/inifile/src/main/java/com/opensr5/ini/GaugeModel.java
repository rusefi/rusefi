package com.opensr5.ini;

/**
 * Represents a gauge template configuration from INI file [GaugeConfigurations] section.
 */
public class GaugeModel {
    private final String name;
    private final String channel;
    private final String title;
    private final String units;
    private final double lowValue;
    private final double highValue;
    private final double lowDangerValue;
    private final double lowWarningValue;
    private final double highWarningValue;
    private final double highDangerValue;
    private final int valueDecimalPlaces;
    private final int labelDecimalPlaces;

    public GaugeModel(String name, String channel, String title, String units,
                      double lowValue, double highValue,
                      double lowDangerValue, double lowWarningValue, double highWarningValue, double highDangerValue,
                      int valueDecimalPlaces, int labelDecimalPlaces) {
        this.name = name;
        this.channel = channel;
        this.title = title;
        this.units = units;
        this.lowValue = lowValue;
        this.highValue = highValue;
        this.lowDangerValue = lowDangerValue;
        this.lowWarningValue = lowWarningValue;
        this.highWarningValue = highWarningValue;
        this.highDangerValue = highDangerValue;
        this.valueDecimalPlaces = valueDecimalPlaces;
        this.labelDecimalPlaces = labelDecimalPlaces;
    }

    public String getName() {
        return name;
    }

    public String getChannel() {
        return channel;
    }

    public String getTitle() {
        return title;
    }

    public String getUnits() {
        return units;
    }

    public double getLowValue() {
        return lowValue;
    }

    public double getHighValue() {
        return highValue;
    }

    public double getLowDangerValue() {
        return lowDangerValue;
    }

    public double getLowWarningValue() {
        return lowWarningValue;
    }

    public double getHighWarningValue() {
        return highWarningValue;
    }

    public double getHighDangerValue() {
        return highDangerValue;
    }

    public int getValueDecimalPlaces() {
        return valueDecimalPlaces;
    }

    public int getLabelDecimalPlaces() {
        return labelDecimalPlaces;
    }

}
