package com.opensr5.ini;

/**
 * Represents a single readout entry in a readoutPanel.
 * Three forms:
 *   readout = gaugeName                                                (isRef=true, gauge ref)
 *   readout = channelName                                              (isRef=true, channel ref – resolved at render time)
 *   readout = channel, title, units, min, max, loD, loW, hiW, hiD, valDig, lblDig
 */
public class ReadoutModel {
    private final String channelOrGaugeName;
    private final String title;
    private final String units;
    private final Double min;
    private final Double max;
    private final Double lowDanger;
    private final Double lowWarn;
    private final Double highWarn;
    private final Double highDang;
    private final int valDigits;
    private final int labelDigits;

    /** Single-name form – may be a gauge reference or a bare output-channel name. */
    public static ReadoutModel ofRef(String name) {
        return new ReadoutModel(name, null, null, null, null, null, null, null, null, 1, 0);
    }

    public ReadoutModel(String channelOrGaugeName, String title, String units,
                        Double min, Double max,
                        Double lowDanger, Double lowWarn, Double highWarn, Double highDang,
                        int valDigits, int labelDigits) {
        this.channelOrGaugeName = channelOrGaugeName;
        this.title = title;
        this.units = units;
        this.min = min;
        this.max = max;
        this.lowDanger = lowDanger;
        this.lowWarn = lowWarn;
        this.highWarn = highWarn;
        this.highDang = highDang;
        this.valDigits = valDigits;
        this.labelDigits = labelDigits;
    }

    public String getChannelOrGaugeName() { return channelOrGaugeName; }
    public String getTitle()              { return title; }
    public String getUnits()              { return units; }
    public Double getMin()                { return min; }
    public Double getMax()                { return max; }
    public Double getLowDanger()          { return lowDanger; }
    public Double getLowWarn()            { return lowWarn; }
    public Double getHighWarn()           { return highWarn; }
    public Double getHighDang()           { return highDang; }
    public int getValDigits()             { return valDigits; }
    public int getLabelDigits()           { return labelDigits; }

    @Override
    public String toString() {
        return "ReadoutModel{channel='" + channelOrGaugeName + '\'' +
                ", title='" + title + '\'' + '}';
    }
}
