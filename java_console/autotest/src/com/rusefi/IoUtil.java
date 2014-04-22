package com.rusefi;

import com.irnems.core.EngineState;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.InvocationConfirmationListener;
import com.rusefi.io.LinkManager;
import com.rusefi.waves.WaveReport;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

/**
 * @author Andrey Belomutskiy
 *         3/19/14.
 */
public class IoUtil {
    static void sendCommand(String command) throws InterruptedException {
        final CountDownLatch responseLatch = new CountDownLatch(1);
        CommandQueue.getInstance().write(command, CommandQueue.DEFAULT_TIMEOUT, new InvocationConfirmationListener() {
            @Override
            public void onCommandConfirmation() {
                responseLatch.countDown();
            }
        });
        responseLatch.await(20, TimeUnit.SECONDS);
    }

    static String getNextWaveChart() throws InterruptedException {
        getWaveChart();
        // we want to wait for the 2nd chart to see same same RPM across the whole chart
        return getWaveChart();
    }

    private static String getWaveChart() throws InterruptedException {
        final CountDownLatch waveChartLatch = new CountDownLatch(1);

        final AtomicReference<String> result = new AtomicReference<String>();

        LinkManager.engineState.registerStringValueAction(WaveReport.WAVE_CHART, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String value) {
                waveChartLatch.countDown();
                result.set(value);
            }
        });
        waveChartLatch.await(5, TimeUnit.SECONDS);
        LinkManager.engineState.removeAction(WaveReport.WAVE_CHART);
        return result.get();
    }
}
