package com.rusefi.ui.widgets;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;

/**
 * Range, warning/danger limits and decimals a gauge is drawn with: the .ini gauge definition,
 * replaced setting by setting with the user's {@link GaugeScaleOverrides}. Every presentation
 * (round, text, bars) is built from one of these so they never disagree.
 */
public final class GaugeScale {
    public static final int MAX_DECIMALS = 6;

    private final double min;
    private final double max;
    private final double lowDanger;
    private final double lowWarning;
    private final double highWarning;
    private final double highDanger;
    private final int valueDecimals;
    private final int labelDecimals;
    /** The engine speed gauge, where 0 means a stopped engine rather than a fault. */
    private final boolean engineSpeed;

    private GaugeScale(double min, double max, double lowDanger, double lowWarning, double highWarning,
                       double highDanger, int valueDecimals, int labelDecimals, boolean engineSpeed) {
        if (!Double.isFinite(min)) {
            min = 0;
        }
        if (!Double.isFinite(max) || max <= min || !Double.isFinite(max - min)) {
            max = min + 10;
            if (!Double.isFinite(max) || max <= min) {
                min = 0;
                max = 10;
            }
        }
        this.min = min;
        this.max = max;
        this.lowDanger = finiteOrNaN(lowDanger);
        this.lowWarning = finiteOrNaN(lowWarning);
        this.highWarning = finiteOrNaN(highWarning);
        this.highDanger = finiteOrNaN(highDanger);
        this.valueDecimals = boundedDecimals(valueDecimals);
        this.labelDecimals = boundedDecimals(labelDecimals);
        this.engineSpeed = engineSpeed;
    }

    /** A scale from plain numbers; {@code highDanger} NaN means no danger value. */
    static GaugeScale of(double min, double max, double highDanger, int valueDecimals, int labelDecimals) {
        return of(min, max, Double.NaN, Double.NaN, Double.NaN, highDanger, valueDecimals, labelDecimals);
    }

    /** A scale with all four TS limits; NaN leaves that limit's zone out. */
    public static GaugeScale of(double min, double max, double lowDanger, double lowWarning, double highWarning,
                                double highDanger, int valueDecimals, int labelDecimals) {
        return new GaugeScale(min, max, lowDanger, lowWarning, highWarning, highDanger, valueDecimals, labelDecimals, false);
    }

    /** The output channel of engine speed in rusEFI's .ini. */
    static final String ENGINE_SPEED_CHANNEL = "RPMValue";

    public static GaugeScale fromIni(GaugeModel model, IniFileModel ini, ConfigurationImage configuration) {
        GaugeScale scale = of(CircularGauge.resolve(model.getLowValueValue(), ini, configuration, 0),
            CircularGauge.resolve(model.getHighValueValue(), ini, configuration, 0),
            CircularGauge.resolve(model.getLowDangerValueValue(), ini, configuration, Double.NaN),
            CircularGauge.resolve(model.getLowWarningValueValue(), ini, configuration, Double.NaN),
            CircularGauge.resolve(model.getHighWarningValueValue(), ini, configuration, Double.NaN),
            CircularGauge.resolve(model.getHighDangerValueValue(), ini, configuration, Double.NaN),
            (int) CircularGauge.resolve(model.getValueDecimalPlacesValue(), ini, configuration, 0),
            (int) CircularGauge.resolve(model.getLabelDecimalPlacesValue(), ini, configuration, 0));
        return ENGINE_SPEED_CHANNEL.equals(model.getChannel()) ? scale.asEngineSpeed() : scale;
    }

    /** This scale for the engine speed gauge: a stopped engine (0) is not flagged. */
    public GaugeScale asEngineSpeed() {
        return new GaugeScale(min, max, lowDanger, lowWarning, highWarning, highDanger, valueDecimals, labelDecimals, true);
    }

    /** Whether this is the engine speed gauge, where 0 means a stopped engine. */
    public boolean isEngineSpeed() {
        return engineSpeed;
    }

    public static GaugeScale resolve(GaugeModel model, IniFileModel ini, ConfigurationImage configuration,
                                     GaugeScaleOverrides overrides) {
        return fromIni(model, ini, configuration).with(overrides);
    }

    /**
     * This scale with every setting {@code overrides} defines replaced. The .ini low and warning
     * limits stay; the round gauge clamps them into an overridden range and below an overridden
     * danger value.
     */
    public GaugeScale with(GaugeScaleOverrides overrides) {
        if (overrides == null || overrides.isEmpty()) {
            return this;
        }
        Integer decimals = overrides.getDecimals();
        return new GaugeScale(
            overrides.getMin() != null ? overrides.getMin() : min,
            overrides.getMax() != null ? overrides.getMax() : max,
            lowDanger, lowWarning, highWarning,
            overrides.getDanger() != null ? overrides.getDanger() : highDanger,
            decimals != null ? decimals : valueDecimals,
            labelDecimals, engineSpeed);
    }

    public double getMin() {
        return min;
    }

    public double getMax() {
        return max;
    }

    /** NaN when there is no low danger value. */
    public double getLowDanger() {
        return lowDanger;
    }

    /** NaN when there is no low warning value. */
    public double getLowWarning() {
        return lowWarning;
    }

    /** NaN when there is no high warning value. */
    public double getHighWarning() {
        return highWarning;
    }

    /** NaN when there is no danger value. */
    public double getHighDanger() {
        return highDanger;
    }

    /** Decimals of the value readout; the round gauge also rounds its reading to them. */
    public int getValueDecimals() {
        return valueDecimals;
    }

    /** Decimals of the scale labels; the user's decimals setting does not change them. */
    public int getLabelDecimals() {
        return labelDecimals;
    }

    static int boundedDecimals(int decimals) {
        return Math.max(0, Math.min(MAX_DECIMALS, decimals));
    }

    private static double finiteOrNaN(double value) {
        return Double.isFinite(value) ? value : Double.NaN;
    }

    @Override
    public String toString() {
        return "GaugeScale{" + min + ".." + max + ", low=" + lowDanger + "/" + lowWarning
            + ", high=" + highWarning + "/" + highDanger + ", decimals=" + valueDecimals
            + ", labels=" + labelDecimals + '}';
    }
}
