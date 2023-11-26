package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.config.generated.Fields;
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

    public static EngineChart nextChart(CommandQueue commandQueue) {
        return EngineChartParser.unpackToMap(getNextWaveChart(commandQueue));
    }

    static String getNextWaveChart(CommandQueue commandQueue) {
        IoUtil.sendBlockingCommand(Fields.CMD_RESET_ENGINE_SNIFFER, commandQueue);
        return getEngineChart(commandQueue);
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

        // FileLog.MAIN.logLine("waiting for next chart");
        commandQueue.getLinkManager().getEngineState().replaceStringValueAction(EngineReport.ENGINE_CHART, (String value) -> {
            engineChartLatch.countDown();
            result.set(value);
        });
        int timeoutMs = 60 * Timeouts.SECOND;
        long waitStartTime = System.currentTimeMillis();
        IoUtil.wait(engineChartLatch, timeoutMs);
        log.info("got next chart in " + (System.currentTimeMillis() - waitStartTime) + "ms for engine_type " + EcuTestHelper.currentEngineType);
        commandQueue.getLinkManager().getEngineState().replaceStringValueAction(EngineReport.ENGINE_CHART, (s) -> { });
        if (result.get() == null)
            throw new IllegalStateException("Chart timeout: " + timeoutMs);
        return result.get();
    }
}
