package com.rusefi.ui.logview;

/**
 * What the Colours popup ({@link HistogramColorsPanel}) needs to know about a view coloured by Z: whether it shows Z
 * values (rather than occupancy), the unit they are shown in, and the values on screen at the palette's ends and
 * centre, to start a manual range or centre from. The histogram table and the scatter plot both are one.
 */
interface ColourScale {
    /** Z values are shown (Mean / Min / Max, or Z colouring), so a manual range and centre apply. */
    boolean showsValues();

    HistogramTable.Mixture mixture();

    /** Z's unit as shown: λ or AFR for a mixture, else the log's unit. */
    String zUnit();

    double scaleLow();

    double scaleHigh();

    /** The diverging palette's centre on screen, or NaN. */
    double scaleCentre();
}
