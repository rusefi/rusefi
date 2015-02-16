package com.rusefi.waves;

/**
 * Date: 6/25/13
 * (c) Andrey Belomutskiy
 */
public interface TimeAxisTranslator {
    int timeToScreen(int time, int width, ZoomProvider zoomProvider);

    double screenToTime(int screenX, int screenWidth, ZoomProvider zoomProvider);

    int getMaxTime();

    int getMinTime();
}
