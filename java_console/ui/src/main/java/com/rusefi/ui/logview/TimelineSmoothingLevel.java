package com.rusefi.ui.logview;

/** The global display-smoothing choices offered by the timeline toolbar. */
enum TimelineSmoothingLevel {
    RAW(0, "Raw"),
    MS_50(0.05, "50 ms"),
    MS_100(0.10, "100 ms"),
    MS_250(0.25, "250 ms"),
    MS_500(0.50, "500 ms"),
    SECOND_1(1, "1 s");

    final double seconds;
    final String label;

    TimelineSmoothingLevel(double seconds, String label) {
        this.seconds = seconds;
        this.label = label;
    }

    static TimelineSmoothingLevel of(double seconds) {
        for (TimelineSmoothingLevel level : values()) {
            if (level.seconds == seconds) {
                return level;
            }
        }
        return null;
    }

    static String label(double seconds) {
        TimelineSmoothingLevel level = of(seconds);
        if (level != null) {
            return level.label;
        }
        return Math.round(seconds * 1000) + " ms";
    }
}
