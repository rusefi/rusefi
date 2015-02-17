package com.rusefi;

import com.rusefi.waves.RevolutionLog;
import com.rusefi.waves.WaveChart;
import com.rusefi.waves.WaveReport;

import java.util.Arrays;
import java.util.List;

import static com.rusefi.waves.WaveReport.isCloseEnough;

/**
 * @author Andrey Belomutskiy
 *         3/19/14.
 */
public class TestingUtils {
    static void assertTrue(String msg, boolean b) {
        if (!b)
            fail("Not true: " + msg);
    }

    static void assertCloseEnough(String msg, double current, double ratio, double... expectations) {
        for (double expected : expectations) {
            if (isCloseEnough(fixAngle(expected), current, ratio))
                return;
        }
        fail(msg + ": Got " + current + " while expecting " + Arrays.toString(expectations));
    }

    private static double fixAngle(double angle) {
        while (angle >= 720)
            angle -= 720;
        return angle;
    }

    private static void fail(String message) {
        FileLog.MAIN.logLine("FAILURE: " + message);
        throw new IllegalStateException(message);
    }

    static void assertTrue(boolean b) {
        if (!b)
            fail("Not true");
    }

    static void assertWave(WaveChart chart, String key, double width, double... expectedAngles) {
        assertWave("", chart, key, width, expectedAngles);
    }

    static void assertWave(String msg, WaveChart chart, String key, double width, double... expectedAngles) {
        assertWave(true, msg, chart, key, width, WaveReport.RATIO, expectedAngles);
    }

    static void assertWaveFall(WaveChart chart, String key, double width, double... expectedAngles) {
        assertWaveFall("", chart, key, width, expectedAngles);
    }

    static void assertWaveFall(String msg, WaveChart chart, String key, double width, double... expectedAngles) {
        assertWave(false, msg, chart, key, width, WaveReport.RATIO, expectedAngles);
    }

    static void assertWave(boolean rise, String msg, WaveChart chart, String key, double width, double angleRatio, double... expectedAngles) {
        RevolutionLog revolutionLog = chart.getRevolutionsLog();
        if (revolutionLog.keySet().isEmpty())
            throw new IllegalStateException(msg + " Empty revolutions in " + chart);

        StringBuilder events = chart.get(key);
        assertTrue(msg + " Events not null for " + key, events != null);
        List<WaveReport.UpDown> wr = WaveReport.parse(events.toString());
        assertTrue(msg + " waves for " + key, !wr.isEmpty());
        for (WaveReport.UpDown ud : wr) {
            int eventTime = rise ? ud.upTime : ud.downTime;
            double angleByTime = revolutionLog.getCrankAngleByTime(eventTime);
            assertCloseEnough(msg + " angle for " + key + "@" + eventTime, fixAngle(angleByTime), angleRatio, expectedAngles);

            assertCloseEnough(msg + "width for " + key, ud.getDutyCycle(revolutionLog), WaveReport.RATIO, width);
        }
    }

    static void assertNull(String msg, Object value) {
        assertTrue(msg, value == null);
    }
}
