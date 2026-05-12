package com.opensr5.ini;

/**
 * Represents a gauge template configuration from INI file [GaugeConfigurations] section.
 * Supports both numeric values and runtime expressions (e.g., {rpmHardLimit + 2000}).
 */
public class GaugeModel {
    private final String name;
    private final String channel;
    private final IniValue title;
    private final IniValue units;
    private final IniValue lowValue;
    private final IniValue highValue;
    private final IniValue lowDangerValue;
    private final IniValue lowWarningValue;
    private final IniValue highWarningValue;
    private final IniValue highDangerValue;
    private final IniValue valueDecimalPlaces;
    private final IniValue labelDecimalPlaces;

    public GaugeModel(String name, String channel, IniValue title, IniValue units,
                      IniValue lowValue, IniValue highValue,
                      IniValue lowDangerValue, IniValue lowWarningValue,
                      IniValue highWarningValue, IniValue highDangerValue,
                      IniValue valueDecimalPlaces, IniValue labelDecimalPlaces) {
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

    /**
     * @return the title string
     */
    public String getTitle() {
        return title.getRawString();
    }

    /**
     * @return the title as an IniValue (for accessing type and raw value)
     */
    public IniValue getTitleValue() {
        return title;
    }

    /**
     * @return the units string
     */
    public String getUnits() {
        return units.getRawString();
    }

    /**
     * @return the units as an IniValue (for accessing type and raw value)
     */
    public IniValue getUnitsValue() {
        return units;
    }

    /**
     * @return the numeric low value, or 0.0 if this is an expression
     */
    public double getLowValue() {
        return lowValue.getNumericValue();
    }

    /**
     * @return the low value as an IniValue (for accessing expression vs numeric)
     */
    public IniValue getLowValueValue() {
        return lowValue;
    }

    /**
     * @return the numeric high value, or 0.0 if this is an expression
     */
    public double getHighValue() {
        return highValue.getNumericValue();
    }

    /**
     * @return the high value as an IniValue (for accessing expression vs numeric)
     */
    public IniValue getHighValueValue() {
        return highValue;
    }

    /**
     * @return the numeric low danger value, or 0.0 if this is an expression
     */
    public double getLowDangerValue() {
        return lowDangerValue.getNumericValue();
    }

    /**
     * @return the low danger value as an IniValue (for accessing expression vs numeric)
     */
    public IniValue getLowDangerValueValue() {
        return lowDangerValue;
    }

    /**
     * @return the numeric low warning value, or 0.0 if this is an expression
     */
    public double getLowWarningValue() {
        return lowWarningValue.getNumericValue();
    }

    /**
     * @return the low warning value as an IniValue (for accessing expression vs numeric)
     */
    public IniValue getLowWarningValueValue() {
        return lowWarningValue;
    }

    /**
     * @return the numeric high warning value, or 0.0 if this is an expression
     */
    public double getHighWarningValue() {
        return highWarningValue.getNumericValue();
    }

    /**
     * @return the high warning value as an IniValue (for accessing expression vs numeric)
     */
    public IniValue getHighWarningValueValue() {
        return highWarningValue;
    }

    /**
     * @return the numeric high danger value, or 0.0 if this is an expression
     */
    public double getHighDangerValue() {
        return highDangerValue.getNumericValue();
    }

    /**
     * @return the high danger value as an IniValue (for accessing expression vs numeric)
     */
    public IniValue getHighDangerValueValue() {
        return highDangerValue;
    }

    /**
     * @return the numeric value decimal places, or 0 if this is an expression
     */
    public int getValueDecimalPlaces() {
        return (int) valueDecimalPlaces.getNumericValue();
    }

    /**
     * @return the value decimal places as an IniValue (for accessing expression vs numeric)
     */
    public IniValue getValueDecimalPlacesValue() {
        return valueDecimalPlaces;
    }

    /**
     * @return the numeric label decimal places, or 0 if this is an expression
     */
    public int getLabelDecimalPlaces() {
        return (int) labelDecimalPlaces.getNumericValue();
    }

    /**
     * @return the label decimal places as an IniValue (for accessing expression vs numeric)
     */
    public IniValue getLabelDecimalPlacesValue() {
        return labelDecimalPlaces;
    }

    /**
     * @return true if any field contains an expression that requires runtime evaluation
     */
    public boolean needsOutputChannelEvaluation() {
        return title.isExpression() || units.isExpression() ||
               lowValue.isExpression() || highValue.isExpression() ||
               lowDangerValue.isExpression() || lowWarningValue.isExpression() ||
               highWarningValue.isExpression() || highDangerValue.isExpression() ||
               valueDecimalPlaces.isExpression() || labelDecimalPlaces.isExpression();
    }

}
