package com.rusefi;

import java.util.Arrays;

public enum PinType {
    OUTPUTS("output_pin_e", "Gpio", "Unassigned"),
    ANALOG_INPUTS("adc_channel_e", "adc_channel_e", "EFI_ADC_NONE"),
    EVENT_INPUTS("brain_input_pin_e", "Gpio", "Unassigned"),
    SENT_INPUTS("sent_input_pin_e", "Gpio", "Unassigned"),
    SWITCH_INPUTS("switch_input_pin_e", "Gpio", "Unassigned");


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
        throw new IllegalArgumentException(key + " not expected, possible keys are " + Arrays.toString(values()));
    }

    public static PinType findByOutputEnum(String key) {
        for (PinType pinType : values()) {
            if (pinType.getOutputEnumName().equalsIgnoreCase(key)) {
                return pinType;
            }
        }
        return null;
    }
}
