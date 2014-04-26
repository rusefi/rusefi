package com.rusefi;

import com.irnems.FileLog;
import com.irnems.core.EngineState;
import com.irnems.core.Sensor;
import com.irnems.core.SensorCentral;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.InvocationConfirmationListener;
import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.waves.WaveChart;
import com.rusefi.waves.WaveChartParser;
import com.rusefi.waves.WaveReport;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

import static com.rusefi.waves.WaveReport.isCloseEnough;

/**
 * @author Andrey Belomutskiy
 *         3/19/14.
 */
public class IoUtil {
    static void sendCommand(String command) {
        final CountDownLatch responseLatch = new CountDownLatch(1);
        long time = System.currentTimeMillis();
        if (LinkManager.hasError())
            throw new IllegalStateException("IO error");
        FileLog.MAIN.logLine("Sending command [" + command + "]");
        CommandQueue.getInstance().write(command, CommandQueue.DEFAULT_TIMEOUT, new InvocationConfirmationListener() {
            @Override
            public void onCommandConfirmation() {
                responseLatch.countDown();
            }
        });
        wait(responseLatch, 20);
        if (LinkManager.hasError())
            throw new IllegalStateException("IO error");
        FileLog.MAIN.logLine("Command [" + command + "] executed in " + (System.currentTimeMillis() - time));
    }

    private static void wait(CountDownLatch responseLatch, int seconds) {
        try {
            responseLatch.await(seconds, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
    }

    static String getNextWaveChart() {
        // we need to skip TWO because spark could have been scheduled a while ago and happen now
        // todo: improve this logic, compare times
        getWaveChart();
        getWaveChart();
        // we want to wait for the 2nd chart to see same same RPM across the whole chart
        String result = getWaveChart();
        FileLog.MAIN.logLine("current chart: " + result);
        return result;
    }

    private static String getWaveChart() {
        final CountDownLatch waveChartLatch = new CountDownLatch(1);

        final AtomicReference<String> result = new AtomicReference<String>();

        LinkManager.engineState.registerStringValueAction(WaveReport.WAVE_CHART, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String value) {
                waveChartLatch.countDown();
                result.set(value);
            }
        });
        int timeout = 15;
        wait(waveChartLatch, timeout);
        LinkManager.engineState.removeAction(WaveReport.WAVE_CHART);
        if (result.get() == null)
            throw new IllegalStateException("Did not receive chart in " + timeout);
        return result.get();
    }

    static WaveChart nextChart() {
        return WaveChartParser.unpackToMap(getNextWaveChart());
    }

    static void changeRpm(final int rpm) {
        sendCommand("rpm " + rpm);
        long time = System.currentTimeMillis();

        final CountDownLatch rpmLatch = new CountDownLatch(1);
        SensorCentral.AdcListener listener = new SensorCentral.AdcListener() {
            @Override
            public void onAdcUpdate(SensorCentral model, double value) {
                double actualRpm = SensorCentral.getInstance().getValue(Sensor.RPM);
                if (isCloseEnough(rpm, actualRpm))
                    rpmLatch.countDown();
            }
        };
        SensorCentral.getInstance().addListener(Sensor.RPM, listener);
        try {
            rpmLatch.await(10, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
        FileLog.MAIN.logLine("RPM change [" + rpm + "] executed in " + (System.currentTimeMillis() - time));
        SensorCentral.getInstance().removeListener(Sensor.RPM, listener);

        double actualRpm = SensorCentral.getInstance().getValue(Sensor.RPM);

        if (!isCloseEnough(rpm, actualRpm))
            throw new IllegalStateException("rpm change did not happen: " + rpm + ", actual " + actualRpm);
    }

    static void waitForFirstResponse() throws InterruptedException {
        FileLog.MAIN.logLine("Let's give it some time to start...");
        final CountDownLatch startup = new CountDownLatch(1);
        SensorCentral.AdcListener listener = new SensorCentral.AdcListener() {
            @Override
            public void onAdcUpdate(SensorCentral model, double value) {
                startup.countDown();
            }
        };
        long waitStart = System.currentTimeMillis();
        SensorCentral.getInstance().addListener(Sensor.RPM, listener);
        startup.await(5, TimeUnit.SECONDS);
        SensorCentral.getInstance().removeListener(Sensor.RPM, listener);
        FileLog.MAIN.logLine("Got first signal in " + (System.currentTimeMillis() - waitStart));
    }

    static void launchSimulator() throws InterruptedException {
        if (!TcpConnector.getAvailablePorts().isEmpty())
            throw new IllegalStateException("Port already binded on startup?");

        ExecHelper.startSimulator();


//        FileLog.rlog("Waiting for TCP port...");
//        for (int i = 0; i < 180; i++) {
//            if (!TcpConnector.getAvailablePorts().isEmpty())
//                break;
//            Thread.sleep(1000);
//        }
//        if (TcpConnector.getAvailablePorts().isEmpty())
//            throw new IllegalStateException("Did we start it?");
//        /**
//         * If we open a connection just to validate that the process has started, we are getting
//         * weird issues with the second - actual connection
//         */
//        FileLog.rlog("Time for simulator to close the port...");
//        Thread.sleep(3000);
//
//        FileLog.rlog("Got a TCP port! Connecting...");
        LinkManager.start("" + TcpConnector.DEFAULT_PORT);
        /**
         * TCP connector is blocking
         */
        LinkManager.open();

        waitForFirstResponse();
    }
}
