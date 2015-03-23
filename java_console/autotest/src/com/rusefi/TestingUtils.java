package com.rusefi;

import com.rusefi.core.EngineState;
import com.rusefi.io.LinkManager;
import com.rusefi.waves.RevolutionLog;
import com.rusefi.waves.WaveChart;
import com.rusefi.waves.WaveChartParser;
import com.rusefi.waves.WaveReport;

import java.util.Arrays;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicReference;

import static com.rusefi.waves.WaveReport.isCloseEnough;

/**
 * @author Andrey Belomutskiy
 *         3/19/14.
 */
public class TestingUtils {
    static boolean isRealHardware;

    static void assertTrue(String msg, boolean b) {
        if (!b)
            fail("Not true: " + msg);
    }

    static void assertCloseEnough(String msg, double current, double ratio, double... expectations) {
        for (double expected : expectations) {
            if (isCloseEnough(fixAngle(expected), current, ratio))
                return;
        }
        if (expectations.length == 1)
            fail(msg + ": Got " + current + " while expecting " + Arrays.toString(expectations) + " ratio=" + Math.abs(1 - expectations[0] / current) + printRange(current, ratio));
        fail(msg + ": Got " + current + " while expecting " + Arrays.toString(expectations) + printRange(current, ratio));
    }

    private static String printRange(double current, double ratio) {
        return " expected range from " + current * (1 - ratio) + " to " + current * (1 + ratio) + " with ratio " + ratio;
    }

    private static double fixAngle(double angle) {
        while (angle >= 720)
            angle -= 720;
        return angle;
    }

    private static void fail(String message) {
        FileLog.MAIN.logLine("FAILURE: " + message);
        IllegalStateException exception = new IllegalStateException(message);
        FileLog.MAIN.log(exception);
        throw exception;
    }

    static void assertTrue(boolean b) {
        if (!b)
            fail("Not true");
    }

    static void assertWave(WaveChart chart, String key, double width, double... expectedAngles) {
        assertWave("", chart, key, width, expectedAngles);
    }

    static void assertWave(String msg, WaveChart chart, String key, double width, double... expectedAngles) {
        assertWave(true, msg, chart, key, width, WaveReport.RATIO, WaveReport.RATIO, expectedAngles);
    }

    static void assertWaveFall(WaveChart chart, String key, double width, double... expectedAngles) {
        assertWaveFall("", chart, key, width, expectedAngles);
    }

    static void assertWaveFall(String msg, WaveChart chart, String key, double width, double... expectedAngles) {
        assertWave(false, msg, chart, key, width, WaveReport.RATIO, WaveReport.RATIO, expectedAngles);
    }

    static void assertWave(boolean rise, String msg, WaveChart chart, String key, double expectedWidth, double angleRatio, double widthRatio, double... expectedAngles) {
        if (isRealHardware)
            return;
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

            double actualWidth = ud.getDutyCycle(revolutionLog);
            if (!isCloseEnough(fixAngle(actualWidth), expectedWidth, widthRatio)) {
                System.out.println("f " + revolutionLog.getCrankAngleByTime(ud.downTime));
                System.out.println("t " + revolutionLog.getCrankAngleByTime(ud.upTime));
            }
            assertCloseEnough(msg + "width for " + key, actualWidth, widthRatio, expectedWidth);

        }
    }

    static void assertNull(String msg, Object value) {
        assertTrue(msg, value == null);
    }

    static WaveChart nextChart() {
        return WaveChartParser.unpackToMap(getNextWaveChart());
    }

    static String getNextWaveChart() {
        IoUtil.sendCommand("reset_wave_chart");
        String result = getWaveChart();
        FileLog.MAIN.logLine("current chart: " + result);
        return result;
    }

    /**
     * This method is blocking and waits for the next wave chart to arrive
     *
     * @return next wave chart in the I/O pipeline
     */
    private static String getWaveChart() {
        final CountDownLatch waveChartLatch = new CountDownLatch(1);

        final AtomicReference<String> result = new AtomicReference<>();

        FileLog.MAIN.logLine("waiting for next chart");
        LinkManager.engineState.replaceStringValueAction(WaveReport.WAVE_CHART, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String value) {
                waveChartLatch.countDown();
                result.set(value);
            }
        });
        int timeout = 60;
        long waitStartTime = System.currentTimeMillis();
        IoUtil.wait(waveChartLatch, timeout);
        FileLog.MAIN.logLine("got next chart in " + (System.currentTimeMillis() - waitStartTime) + "ms for engine_type " + AutoTest.currentEngineType);
        LinkManager.engineState.replaceStringValueAction(WaveReport.WAVE_CHART, (EngineState.ValueCallback<String>) EngineState.ValueCallback.VOID);
        if (result.get() == null)
            throw new IllegalStateException("Chart timeout: " + timeout);
        return result.get();
    }
}
