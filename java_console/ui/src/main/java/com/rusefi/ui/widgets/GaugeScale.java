package com.rusefi.ui.widgets;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;

/**
 * Range, high danger value and decimals a gauge is drawn with: the .ini gauge definition,
 * replaced setting by setting with the user's {@link GaugeScaleOverrides}. Every presentation
 * (round, text, bars) is built from one of these so they never disagree.
 */
public final class GaugeScale {
    public static final int MAX_DECIMALS = 6;

    private final double min;
    private final double max;
    private final double highDanger;
    private final int valueDecimals;
    private final int labelDecimals;
    private final boolean customDecimals;

    private GaugeScale(double min, double max, double highDanger, int valueDecimals, int labelDecimals,
                       boolean customDecimals) {
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
        this.highDanger = Double.isFinite(highDanger) ? highDanger : Double.NaN;
        this.valueDecimals = boundedDecimals(valueDecimals);
        this.labelDecimals = boundedDecimals(labelDecimals);
        this.customDecimals = customDecimals;
    }

    /** A scale from plain numbers; {@code highDanger} NaN means no danger value. */
    static GaugeScale of(double min, double max, double highDanger, int valueDecimals, int labelDecimals) {
        return new GaugeScale(min, max, highDanger, valueDecimals, labelDecimals, false);
    }

    public static GaugeScale fromIni(GaugeModel model, IniFileModel ini, ConfigurationImage configuration) {
        return of(CircularGauge.resolve(model.getLowValueValue(), ini, configuration, 0),
            CircularGauge.resolve(model.getHighValueValue(), ini, configuration, 0),
            CircularGauge.resolve(model.getHighDangerValueValue(), ini, configuration, Double.NaN),
            (int) CircularGauge.resolve(model.getValueDecimalPlacesValue(), ini, configuration, 0),
            (int) CircularGauge.resolve(model.getLabelDecimalPlacesValue(), ini, configuration, 0));
    }

    public static GaugeScale resolve(GaugeModel model, IniFileModel ini, ConfigurationImage configuration,
                                     GaugeScaleOverrides overrides) {
        return fromIni(model, ini, configuration).with(overrides);
    }

    /** This scale with every setting {@code overrides} defines replaced. */
    public GaugeScale with(GaugeScaleOverrides overrides) {
        if (overrides == null || overrides.isEmpty()) {
            return this;
        }
        Integer decimals = overrides.getDecimals();
        return new GaugeScale(
            overrides.getMin() != null ? overrides.getMin() : min,
            overrides.getMax() != null ? overrides.getMax() : max,
            overrides.getDanger() != null ? overrides.getDanger() : highDanger,
            decimals != null ? decimals : valueDecimals,
            labelDecimals,
            decimals != null || customDecimals);
    }

    public double getMin() {
        return min;
    }

    public double getMax() {
        return max;
    }

    /** NaN when there is no danger value. */
    public double getHighDanger() {
        return highDanger;
    }

    /** Decimals of the value readout. */
    public int getValueDecimals() {
        return valueDecimals;
    }

    /** Decimals of the scale labels; the user's decimals setting does not change them. */
    public int getLabelDecimals() {
        return labelDecimals;
    }

    /**
     * True when the user chose the value decimals. The round gauge then rounds its reading to
     * those decimals instead of to its range-based step, which could hide the chosen digits.
     */
    public boolean hasCustomDecimals() {
        return customDecimals;
    }

    static int boundedDecimals(int decimals) {
        return Math.max(0, Math.min(MAX_DECIMALS, decimals));
    }

    @Override
    public String toString() {
        return "GaugeScale{" + min + ".." + max + ", danger=" + highDanger + ", decimals=" + valueDecimals
            + (customDecimals ? " (custom)" : "") + ", labels=" + labelDecimals + '}';
    }
}
