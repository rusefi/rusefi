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


    public static double getTopAdvanceForBore(ChamberStyle style) {
        return style.advance;
    }
}
