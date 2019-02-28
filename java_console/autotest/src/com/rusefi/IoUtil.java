package com.rusefi;

import com.rusefi.core.EngineState;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.InvocationConfirmationListener;
import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.TcpConnector;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static com.rusefi.waves.EngineReport.isCloseEnough;

/**
 * @author Andrey Belomutskiy
 *         3/19/14.
 */
public class IoUtil {

    public static final String RESET_ENGINE_CHART = "reset_engine_chart";

    /**
     * Send a command and wait for the confirmation
     *
     * @throws IllegalStateException if command was not confirmed
     */
    static void sendCommand(String command) {
        sendCommand(command, CommandQueue.DEFAULT_TIMEOUT, Timeouts.CMD_TIMEOUT);
    }

    static void sendCommand(String command, int retryTimeoutMs, int totalTimeoutSeconds) {
        final CountDownLatch responseLatch = new CountDownLatch(1);
        long time = System.currentTimeMillis();
        if (LinkManager.hasError())
            throw new IllegalStateException("IO error");
        FileLog.MAIN.logLine("Sending command [" + command + "]");
        final long begin = System.currentTimeMillis();
        CommandQueue.getInstance().write(command, retryTimeoutMs, new InvocationConfirmationListener() {
            @Override
            public void onCommandConfirmation() {
                responseLatch.countDown();
                FileLog.MAIN.logLine("Got confirmation in " + (System.currentTimeMillis() - begin) + "ms");
            }
        });
        wait(responseLatch, totalTimeoutSeconds);
        if (responseLatch.getCount() > 0)
            FileLog.MAIN.logLine("No confirmation in " + retryTimeoutMs);
        if (LinkManager.hasError())
            throw new IllegalStateException("IO error");
        FileLog.MAIN.logLine("Command [" + command + "] executed in " + (System.currentTimeMillis() - time));
    }

    static void wait(CountDownLatch responseLatch, int seconds) {
        try {
            responseLatch.await(seconds, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
    }

    static void changeRpm(final int rpm) {
        sendCommand("rpm " + rpm);
        long time = System.currentTimeMillis();

        final CountDownLatch rpmLatch = new CountDownLatch(1);
        SensorCentral.SensorListener listener = new SensorCentral.SensorListener() {
            @Override
            public void onSensorUpdate(double value) {
                double actualRpm = SensorCentral.getInstance().getValue(Sensor.RPM);
                if (isCloseEnough(rpm, actualRpm))
                    rpmLatch.countDown();
            }
        };
        SensorCentral.getInstance().addListener(Sensor.RPM, listener);
        try {
            rpmLatch.await(40, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
        FileLog.MAIN.logLine("RPM change [" + rpm + "] executed in " + (System.currentTimeMillis() - time));
        SensorCentral.getInstance().removeListener(Sensor.RPM, listener);

        double actualRpm = SensorCentral.getInstance().getValue(Sensor.RPM);

        if (!isCloseEnough(rpm, actualRpm))
            throw new IllegalStateException("rpm change did not happen: " + rpm + ", actual " + actualRpm);
        sendCommand(RESET_ENGINE_CHART);
    }

    static void waitForFirstResponse() throws InterruptedException {
        FileLog.MAIN.logLine("Let's give it some time to start...");
        final CountDownLatch startup = new CountDownLatch(1);
        SensorCentral.SensorListener listener = new SensorCentral.SensorListener() {
            @Override
            public void onSensorUpdate(double value) {
                startup.countDown();
            }
        };
        long waitStart = System.currentTimeMillis();
        SensorCentral.getInstance().addListener(Sensor.RPM, listener);
        startup.await(5, TimeUnit.SECONDS);
        SensorCentral.getInstance().removeListener(Sensor.RPM, listener);
        FileLog.MAIN.logLine("Got first signal in " + (System.currentTimeMillis() - waitStart));
    }

    static void connectToSimulator(boolean startProcess) throws InterruptedException {
        if (startProcess) {
            if (!TcpConnector.getAvailablePorts().isEmpty())
                throw new IllegalStateException("Port already binded on startup?");
            ExecHelper.startSimulator();
        }


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
        LinkManager.engineState.registerStringValueAction(EngineState.RUS_EFI_VERSION_TAG, (EngineState.ValueCallback<String>) EngineState.ValueCallback.VOID);
        waitForFirstResponse();
    }

    @SuppressWarnings("UnusedDeclaration")
    static void sleep(int seconds) {
        FileLog.MAIN.logLine("Sleeping " + seconds + " seconds");
        try {
            Thread.sleep(seconds * 1000L);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
    }

    static void realHardwareConnect(String port) {
        LinkManager.engineState.registerStringValueAction(EngineState.RUS_EFI_VERSION_TAG, (EngineState.ValueCallback<String>) EngineState.ValueCallback.VOID);
        LinkManager.engineState.registerStringValueAction(EngineState.OUTPIN_TAG, (EngineState.ValueCallback<String>) EngineState.ValueCallback.VOID);
        LinkManager.engineState.registerStringValueAction(AverageAnglesUtil.KEY, (EngineState.ValueCallback<String>) EngineState.ValueCallback.VOID);

        final CountDownLatch connected = LinkManager.connect(port);
        if (connected.getCount() > 0)
            throw new IllegalStateException("Not connected in time");
    }

}
