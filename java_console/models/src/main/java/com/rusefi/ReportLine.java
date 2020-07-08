package com.rusefi;

import com.rusefi.models.MafValue;
import com.rusefi.models.RpmValue;

/**
 * @author Andrey Belomutskiy
 *         1/29/13
 */
public class ReportLine {
    private final int time;
    private final MafValue maf;
    private final RpmValue rpm;
    private final int wave;

    public ReportLine(int time, MafValue maf, RpmValue rpm, int wave) {
        this.time = time;
        this.maf = maf;
        this.rpm = rpm;
        this.wave = wave;
    }

    public int getTime() {
        return time;
    }

    public MafValue getMaf() {
        return maf;
    }

    public RpmValue getRpm() {
        return rpm;
    }

    public int getWave() {
        return wave;
    }

    @Override
    public String toString() {
        return "ReportLine{" +
                "time=" + time +
                ", maf=" + maf +
                ", rpm=" + rpm +
                ", wave=" + wave +
                '}';
    }
}
