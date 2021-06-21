package com.rusefi;

public enum PinType {
    OUTPUTS("output_pin_e_enum", "brain_pin_e", "GPIO_UNASSIGNED"),
    ANALOG_INPUTS("adc_channel_e_enum", "adc_channel_e", "EFI_ADC_NONE"),
    EVENT_INPUTS("brain_input_pin_e_enum", "brain_pin_e", "GPIO_UNASSIGNED"),
    SWITCH_INPUTS("switch_input_pin_e_enum", "brain_pin_e", "GPIO_UNASSIGNED");


    private final String outputEnumName;
    private final String pinType;
    private final String nothingName;

    PinType(String outputEnumName, String pinType, String nothingName) {
        this.outputEnumName = outputEnumName;
        this.pinType = pinType;
        this.nothingName = nothingName;
    }

    public String getOutputEnumName() {
        return outputEnumName;
    }

    public String getPinType() {
        return pinType;
    }

    public String getNothingName() {
        return nothingName;
    }

    public static PinType find(String key) {
        for (PinType pinType : values()) {
            if (pinType.name().equalsIgnoreCase(key)) {
                return pinType;
            }
        }
        throw new IllegalArgumentException(key + " not expected, possible keys are " + values());
    }
}
