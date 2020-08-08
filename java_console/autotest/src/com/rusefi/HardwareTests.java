package com.rusefi;

import com.rusefi.config.generated.Fields;
import com.rusefi.functional_tests.BaseTest;
import com.rusefi.io.CommandQueue;

import static com.rusefi.IoUtil.getDisableCommand;
import static com.rusefi.IoUtil.getEnableCommand;
import static com.rusefi.config.generated.Fields.*;

public class HardwareTests extends BaseTest {

    public HardwareTests(CommandQueue commandQueue) {
        super(commandQueue);
    }

    public void runRealHardwareTests() {
        sendCommand(getEnableCommand(Fields.CMD_TRIGGER_HW_INPUT));
        enableFunctionalMode();


        setEngineType(ET_FRANKENSO_MIATA_NA6);
        sendCommand(getDisableCommand(Fields.CMD_SELF_STIMULATION));
        changeRpm(1400);

    }
}
