package com.rusefi.logic;

/**
 * (c) Andrey Belomutskiy 2012-2015
 * 12/24/2015
 */
public class IgnitionMapBuilder {
    public enum ChamberStyle {
        OPEN_CHAMBER(33),
        CLOSED_CHAMBER(28),
        SWITL_TUMBLE(22);


        private final int advance;

        ChamberStyle(int advance) {
            this.advance = advance;
        }

        public int getAdvance() {
            return advance;
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

    public static double round10(double result) {
        return ((int)(result * 10)) / 10.0;
    }

    public static double getBoreCorrection(double bore) {
        return (bore - 4 * 25.4) / 25.4 * 6;
    }
}
