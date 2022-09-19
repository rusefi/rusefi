package com.rusefi.waves;

/**
 * Date: 6/25/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public interface TimeAxisTranslator {
    int timeToScreen(double time, int width);

    double screenToTime(int screenX, int screenWidth);

    int getMaxTime();

    int getMinTime();
}
