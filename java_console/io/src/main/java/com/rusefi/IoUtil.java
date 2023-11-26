package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.ISensorCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.enums.trigger_type_e;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.TcpConnector;
import org.jetbrains.annotations.NotNull;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.config.generated.Fields.CMD_RPM;
import static com.rusefi.waves.EngineReport.isCloseEnough;

/**
 * @author Andrey Belomutskiy
 *         3/19/14.
 */
public class IoUtil {
    private static final Logging log = getLogging(IoUtil.class);

    /**
     * Send a command and wait for the confirmation
     *
     * @throws IllegalStateException if command was not confirmed
     */
    static void sendBlockingCommand(String command, CommandQueue commandQueue) {
        sendBlockingCommand(command, CommandQueue.DEFAULT_TIMEOUT, commandQueue);
    }

    public static String getSetCommand(String settingName) {
        return Fields.CMD_SET + " " + settingName;
    }

    public static String getEnableCommand(String settingName) {
        return Fields.CMD_ENABLE + " " + settingName;
    }

    public static String getDisableCommand(String settingName) {
        return Fields.CMD_DISABLE + " " + settingName;
    }

    /**
     * blocking method which would for confirmation from rusEFI
     */
    public static void sendBlockingCommand(String command, int timeoutMs, CommandQueue commandQueue) {
        final CountDownLatch responseLatch = new CountDownLatch(1);
        long time = System.currentTimeMillis();
        log.info("Sending command [" + command + "]");
        final long begin = System.currentTimeMillis();
        commandQueue.write(command, timeoutMs, () -> {
            responseLatch.countDown();
            log.info("Got confirmation in " + (System.currentTimeMillis() - begin) + "ms");
        });
        wait(responseLatch, timeoutMs);
        if (responseLatch.getCount() > 0)
            log.info("No confirmation in " + timeoutMs);
        log.info("Command [" + command + "] executed in " + (System.currentTimeMillis() - time));
    }

    static void wait(CountDownLatch responseLatch, int milliseconds) {
        try {
            responseLatch.await(milliseconds, TimeUnit.MILLISECONDS);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
    }

    public static void changeRpm(CommandQueue commandQueue, final int rpm) {
        log.info("AUTOTEST rpm EN " + rpm);
        sendBlockingCommand(CMD_RPM + " " + rpm, commandQueue);
        long time = System.currentTimeMillis();

        final CountDownLatch rpmLatch = new CountDownLatch(1);

        SensorCentral.ListenerToken listenerToken = SensorCentral.getInstance().addListener(Sensor.RPMValue, actualRpm -> {
            if (isCloseEnough(rpm, actualRpm))
                rpmLatch.countDown();
        });

        // Wait for RPM to change
        try {
            rpmLatch.await(40, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }

        // We don't need to listen to RPM anymore
        listenerToken.remove();

        double actualRpm = SensorCentral.getInstance().getValue(Sensor.RPMValue);

        if (!isCloseEnough(rpm, actualRpm))
            throw new IllegalStateException("rpm change did not happen: " + rpm + ", actual " + actualRpm);
//        sendCommand(Fields.CMD_RESET_ENGINE_SNIFFER);
        log.info("AUTOTEST RPM change [" + rpm + "] executed in " + (System.currentTimeMillis() - time));
    }

    static void waitForFirstResponse() throws InterruptedException {
        log.info("Let's give it some time to start...");
        final CountDownLatch startup = new CountDownLatch(1);
        long waitStart = System.currentTimeMillis();

        ISensorCentral.ListenerToken listener = SensorCentral.getInstance().addListener(Sensor.RPMValue, value -> startup.countDown());
        startup.await(5, TimeUnit.SECONDS);
        listener.remove();
        FileLog.MAIN.logLine("Got first signal in " + (System.currentTimeMillis() - waitStart));
    }

    static void connectToSimulator(LinkManager linkManager, boolean startProcess) throws InterruptedException {
        if (startProcess) {
            if (!TcpConnector.getAvailablePorts().isEmpty())
                throw new IllegalStateException("Port already binded on startup?");
            SimulatorExecHelper.startSimulator();
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

        /**
         * TCP connector is blocking
         */
        linkManager.startAndConnect("" + TcpConnector.DEFAULT_PORT, ConnectionStateListener.VOID);
        linkManager.getEngineState().registerStringValueAction(Fields.PROTOCOL_VERSION_TAG, (s) -> { });
        waitForFirstResponse();
    }

    @SuppressWarnings("UnusedDeclaration")
    public static void sleepSeconds(int seconds) {
        FileLog.MAIN.logLine("Sleeping " + seconds + " seconds");
        try {
            Thread.sleep(seconds * 1000L);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
    }

    public static void realHardwareConnect(LinkManager linkManager, String port) {
        linkManager.getEngineState().registerStringValueAction(Fields.PROTOCOL_OUTPIN, (s) -> { });
        linkManager.getEngineState().registerStringValueAction(AverageAnglesUtil.KEY, (s) -> { });

        try {
            linkManager.connect(port).await(60, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new IllegalStateException("Not connected in time");
        }
    }

    @NotNull
    public static String setTriggerType(trigger_type_e triggerType) {
        return "set " + "trigger_type" + " " + triggerType.ordinal();
    }
}
