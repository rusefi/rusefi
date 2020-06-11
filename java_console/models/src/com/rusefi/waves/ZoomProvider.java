package com.rusefi.waves;

/**
 * 7/7/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public interface ZoomProvider {
    ZoomProvider DEFAULT = new ZoomProvider() {
        @Override
        public double getZoomValue() {
            return 1;
        }

        public String toString() {
            return "default";
        }
    };

    double getZoomValue();
}
