package com.rusefi;

import com.rusefi.config.generated.Fields;
import com.rusefi.functional_tests.BaseTest;
import com.rusefi.io.CommandQueue;

import static com.rusefi.IoUtil.getEnableCommand;

public class HardwareTests extends BaseTest {

    public HardwareTests(CommandQueue commandQueue) {
        super(commandQueue);
    }

    public void runRealHardwareTests() {
        sendCommand(getEnableCommand(Fields.CMD_TRIGGER_HW_INPUT));
        enableFunctionalMode();

    }
}
