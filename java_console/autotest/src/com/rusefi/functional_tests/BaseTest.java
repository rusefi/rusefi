package com.rusefi.functional_tests;

import com.rusefi.IoUtil;
import com.rusefi.Timeouts;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.CommandQueue;

import static com.rusefi.IoUtil.getEnableCommand;

public class BaseTest {
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
}
