package com.rusefi.functional_tests;

import com.devexperts.logging.Logging;
import com.rusefi.IoUtil;
import com.rusefi.Timeouts;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.CommandQueue;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.IoUtil.getEnableCommand;
import static com.rusefi.IoUtil.sleepSeconds;

public class BaseTest {
    private static final Logging log = getLogging(BaseTest.class);

    public static final int COMPLEX_COMMAND_RETRY = 10000;
    public static int currentEngineType;
    protected final CommandQueue commandQueue;

    public BaseTest(CommandQueue commandQueue) {
        this.commandQueue = commandQueue;
    }

    protected void sendCommand(String command) {
        sendCommand(command, CommandQueue.DEFAULT_TIMEOUT, Timeouts.CMD_TIMEOUT);
    }

    protected void sendCommand(String command, int retryTimeoutMs, int timeoutMs) {
        TestHelper.INSTANCE.assertNotFatal();
        IoUtil.sendCommand(command, retryTimeoutMs, timeoutMs, commandQueue);
    }

    /**
     * this seem to adjust engine sniffer behaviour
     */
    protected void enableFunctionalMode() {
        sendCommand(getEnableCommand(Fields.CMD_FUNCTIONAL_TEST_MODE));
    }

    protected void changeRpm(final int rpm) {
        IoUtil.changeRpm(commandQueue, rpm);
    }

    protected void setEngineType(int type) {
        log.info("AUTOTEST setEngineType " + type);
        currentEngineType = type;
//        sendCommand(CMD_PINS);
        sendCommand("set " + Fields.CMD_ENGINE_TYPE + " " + type, COMPLEX_COMMAND_RETRY, Timeouts.SET_ENGINE_TIMEOUT);
        // TODO: document the reason for this sleep?!
        sleepSeconds(1);
        sendCommand(getEnableCommand(Fields.CMD_SELF_STIMULATION));
    }
}
