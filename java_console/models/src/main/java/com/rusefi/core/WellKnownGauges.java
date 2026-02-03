package com.rusefi.core;

public enum WellKnownGauges {
    RPMGauge("RPMValue"),
    SECONDS("seconds"),
    ;

    private final String outputChannelName;

    WellKnownGauges(String outputChannelName) {
        this.outputChannelName = outputChannelName;
    }

    public String getOutputChannelName() {
        return outputChannelName;
    }
}
