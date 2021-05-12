package com.rusefi;

/**
 * 1/18/2015
 */
public enum OutputChannel {
    WaveChartCurrentSize(11),
    RunningTriggerError(12),
    RunningOrderingTriggerError(13),
    ;
    private final int protocolId;

    OutputChannel(int protocolId) {
        this.protocolId = protocolId;
    }

    public int getProtocolId() {
        return protocolId;
    }
}
