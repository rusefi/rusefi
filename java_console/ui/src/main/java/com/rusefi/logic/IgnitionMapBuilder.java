package com.rusefi.logic;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 12/24/2015
 */
public class IgnitionMapBuilder {
    public enum ChamberStyle {
        OPEN_CHAMBER(33),
        CLOSED_CHAMBER(28),
        SWIRL_TUMBLE(22);


        private final int advance;

        ChamberStyle(int advance) {
            this.advance = advance;
        }
    }


    public static double getTopAdvanceForBore(ChamberStyle style, int octane, double compression, double bore) {
        int octaneCorrection;
        if ( octane <= 90) {
            octaneCorrection = -2;
        } else if (octane < 94) {
            octaneCorrection = -1;
        } else {
            octaneCorrection = 0;
        }

        int compressionCorrection;
        if (compression <= 9) {
            compressionCorrection = 2;
        } else if (compression <= 10) {
            compressionCorrection = 1;
        } else if (compression <= 11) {
            compressionCorrection = 0;
        } else {
            // compression ratio above 11
            compressionCorrection = -2;
        }

        double result = style.advance + octaneCorrection + compressionCorrection + getBoreCorrection(bore);
        return round10(result);
    }

    public static double interpolate(double x1, double y1, double x2, double y2, double x) {
        double a = ((y1 - y2) / (x1 - x2));
        double b = y1 - a * x1;
        return a * x + b;
    }

    public static double getAdvanceForRpm(int rpm, double advanceMax) {
        if (rpm >= 3000)
            return advanceMax;
        if (rpm < 600)
            return 10;
        return interpolate(600, 10, 3000, advanceMax, rpm);
    }

    public static double getInitialAdvance(int rpm, double map, double advanceMax) {
        double advance = getAdvanceForRpm(rpm, advanceMax);

        if (rpm > 3000)
            return round10(advance + 0.1 * (100 - map));

        return round10(advance + 0.1 * (100 - map) * rpm / 3000);
    }

    public static double round10(double result) {
        return ((int)(result * 10)) / 10.0;
    }

    public static double getBoreCorrection(double bore) {
        return (bore - 4 * 25.4) / 25.4 * 6;
    }
}
