package com.rusefi.waves;

/**
 * Date: 6/25/13
 * (c) Andrey Belomutskiy
 */
public interface TimeAxisTranslator {
    int timeToScreen(int time, int width);

    double screenToTime(int screenX, int screenWidth);

    int getMaxTime();

    int getMinTime();
}
