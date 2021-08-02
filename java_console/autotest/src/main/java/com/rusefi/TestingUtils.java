package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.EngineState;
import com.rusefi.functional_tests.EcuTestHelper;
import com.rusefi.io.CommandQueue;
import com.rusefi.waves.EngineChart;
import com.rusefi.waves.EngineReport;
import com.rusefi.waves.RevolutionLog;
import com.rusefi.waves.EngineChartParser;

import java.util.Arrays;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.Assert.fail;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.waves.EngineReport.isCloseEnough;

/**
 * @author Andrey Belomutskiy
 *         3/19/14.
 */
public class TestingUtils {
    private static final Logging log = getLogging(TestingUtils.class);

    public static boolean isRealHardware;

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
        return " expected range from " + current / (1 + ratio) + " to " + current / (1 - ratio) + " with ratio " + ratio;
    }

    private static double fixAngle(double angle) {
        while (angle >= 720)
            angle -= 720;
        return angle;
    }

    static void assertTrue(boolean b) {
        if (!b)
            fail("Not true");
    }

    static void assertWave(EngineChart chart, String key, double width, double... expectedAngles) {
        assertWave("", chart, key, width, expectedAngles);
    }

    static void assertWave(String msg, EngineChart chart, String key, double width, double... expectedAngles) {
        assertWave(true, msg, chart, key, width, EngineReport.RATIO, EngineReport.RATIO, expectedAngles);
    }

    static void assertWaveFall(EngineChart chart, String key, double width, double... expectedAngles) {
        assertWaveFall("", chart, key, width, expectedAngles);
    }

    static void assertWaveFall(String msg, EngineChart chart, String key, double width, double... expectedAngles) {
        assertWave(false, msg, chart, key, width, EngineReport.RATIO, EngineReport.RATIO, expectedAngles);
    }

    static void assertWave(boolean rise, String msg, EngineChart chart, String key, double expectedWidth, double angleRatio, double widthRatio, double... expectedAngles) {
        if (isRealHardware)
            return;
        RevolutionLog revolutionLog = chart.getRevolutionsLog();
        if (revolutionLog.getSize() < 2)
            throw new IllegalStateException(msg + " Not many revolutions in " + chart);

        StringBuilder events = chart.get(key);
        assertTrue(msg + " Events not null for " + key, events != null);
        List<EngineReport.UpDown> wr = EngineReport.parse(events.toString());
        assertTrue(msg + " waves for " + key, !wr.isEmpty());
        for (EngineReport.UpDown ud : wr) {
            int eventTime = rise ? ud.upTime : ud.downTime;
            double angleByTime = revolutionLog.getCrankAngleByTime(eventTime);
            assertCloseEnough(msg + " angle for " + key + "@" + eventTime, fixAngle(angleByTime), angleRatio, expectedAngles);

            double actualWidth = ud.getDutyCycle(revolutionLog);
            if (!isCloseEnough(fixAngle(actualWidth), expectedWidth, widthRatio)) {
                System.out.println("f " + revolutionLog.getCrankAngleByTime(ud.downTime));
                System.out.println("t " + revolutionLog.getCrankAngleByTime(ud.upTime));
            }
            assertCloseEnough(msg + " width for " + key, actualWidth, widthRatio, expectedWidth);
        }
    }

    public static void assertNull(String msg, Object value) {
        assertTrue(msg, value == null);
    }

    public static EngineChart nextChart(CommandQueue commandQueue) {
        long start = System.currentTimeMillis();
        EngineChart chart = EngineChartParser.unpackToMap(getNextWaveChart(commandQueue), FileLog.LOGGER);
        FileLog.MAIN.logLine("AUTOTEST nextChart() in " + (System.currentTimeMillis() - start));
        return chart;
    }

    static String getNextWaveChart(CommandQueue commandQueue) {
        IoUtil.sendCommand(Fields.CMD_RESET_ENGINE_SNIFFER, commandQueue);
        String result = getEngineChart(commandQueue);
        FileLog.MAIN.logLine("current chart: " + result);
        return result;
    }

    /**
     * This method is blocking and waits for the next wave chart to arrive
     *
     * @return next wave chart in the I/O pipeline
     * @param commandQueue
     */
    private static String getEngineChart(CommandQueue commandQueue) {
        final CountDownLatch engineChartLatch = new CountDownLatch(1);

        final AtomicReference<String> result = new AtomicReference<>();

        FileLog.MAIN.logLine("waiting for next chart");
        commandQueue.getLinkManager().getEngineState().replaceStringValueAction(EngineReport.ENGINE_CHART, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String value) {
                engineChartLatch.countDown();
                result.set(value);
            }
        });
        int timeoutMs = 60 * Timeouts.SECOND;
        long waitStartTime = System.currentTimeMillis();
        IoUtil.wait(engineChartLatch, timeoutMs);
        log.info("got next chart in " + (System.currentTimeMillis() - waitStartTime) + "ms for engine_type " + EcuTestHelper.currentEngineType);
        commandQueue.getLinkManager().getEngineState().replaceStringValueAction(EngineReport.ENGINE_CHART, (EngineState.ValueCallback<String>) EngineState.ValueCallback.VOID);
        if (result.get() == null)
            throw new IllegalStateException("Chart timeout: " + timeoutMs);
        return result.get();
    }
}
