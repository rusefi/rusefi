package com.rusefi.ui.widgets;

/**
 * A live gauge presentation. Values may arrive off the EDT; labels are set on the EDT.
 */
public interface GaugeDisplay {
    void setValue(double value);

    void setTitle(String title);

    void setUnitString(String units);
}
