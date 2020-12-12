package com.rusefi.functional_tests;

import com.devexperts.logging.Logging;
import com.rusefi.IoUtil;
import com.rusefi.Timeouts;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.CommandQueue;
import com.rusefi.waves.EngineReport;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.IoUtil.*;
import static com.rusefi.waves.EngineReport.isCloseEnough;

public class EcuTestHelper {
    private static final Logging log = getLogging(EcuTestHelper.class);

    public static final int COMPLEX_COMMAND_RETRY = 10000;
    public static int currentEngineType;
    public final CommandQueue commandQueue;

    public EcuTestHelper(CommandQueue commandQueue) {
        this.commandQueue = commandQueue;
    }

    public static void assertEquals(double expected, double actual) {
        EcuTestHelper.assertEquals("", expected, actual);
    }

    public static void assertEquals(String msg, double expected, double actual) {
        EcuTestHelper.assertEquals(msg, expected, actual, EngineReport.RATIO);
    }

    public static void assertEquals(String msg, double expected, double actual, double ratio) {
        if (!isCloseEnough(expected, actual, ratio))
            throw new IllegalStateException(msg + " Expected " + expected + " but got " + actual);
    }

    public void sendCommand(String command) {
        sendCommand(command, CommandQueue.DEFAULT_TIMEOUT, Timeouts.CMD_TIMEOUT);
    }

    public void sendCommand(String command, int retryTimeoutMs, int timeoutMs) {
        TestHelper.INSTANCE.assertNotFatal();
        IoUtil.sendCommand(command, retryTimeoutMs, timeoutMs, commandQueue);
    }

    /**
     * this seem to adjust engine sniffer behaviour
     */
    public void enableFunctionalMode() {
        sendCommand(getEnableCommand(Fields.CMD_FUNCTIONAL_TEST_MODE));
    }

    public void changeRpm(final int rpm) {
        IoUtil.changeRpm(commandQueue, rpm);
    }

    public void setEngineType(int type) {
        log.info("AUTOTEST setEngineType " + type);
        currentEngineType = type;
//        sendCommand(CMD_PINS);
        /*
         * we need to stop all activity - that means:
         * - stopping input event
         * - waiting for scheduled actuator actions to run out
         * - disabling PWM
         */
        sendCommand(getDisableCommand(Fields.CMD_SELF_STIMULATION));
        sendCommand(getDisableCommand(Fields.CMD_INJECTION));
        sendCommand(getDisableCommand(Fields.CMD_IGNITION));
        sendCommand(getDisableCommand(Fields.CMD_PWM));
        // changing engine type while engine is running does not work well - we rightfully
        // get invalid configuration critical errors
        sleepSeconds(2);
        sendCommand("set " + Fields.CMD_ENGINE_TYPE + " " + type, COMPLEX_COMMAND_RETRY, Timeouts.SET_ENGINE_TIMEOUT);
        // TODO: document the reason for this sleep?!
        sleepSeconds(3);
        sendCommand(getEnableCommand(Fields.CMD_PWM));
        sendCommand(getEnableCommand(Fields.CMD_SELF_STIMULATION));
    }
}
