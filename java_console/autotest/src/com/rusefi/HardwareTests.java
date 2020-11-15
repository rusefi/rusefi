package com.rusefi;

import com.rusefi.config.generated.Fields;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.functional_tests.BaseTest;
import com.rusefi.io.CommandQueue;

import static com.rusefi.IoUtil.getDisableCommand;
import static com.rusefi.IoUtil.getEnableCommand;
import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;
import static com.rusefi.config.generated.Fields.*;

public class HardwareTests extends BaseTest {

    public HardwareTests(CommandQueue commandQueue) {
        super(commandQueue);
    }

    /**
     * This test relies on jumpers connecting physical pins on Discovery:
     * PD1<>PC6
     * PD2<>PA5
     */
    public void runRealHardwareTests() {
        sendCommand(getEnableCommand(Fields.CMD_TRIGGER_HW_INPUT));
        enableFunctionalMode();


        setEngineType(ET_FRANKENSO_MIATA_NA6);
        sendCommand(getDisableCommand(Fields.CMD_SELF_STIMULATION));
        changeRpm(1400);

        // moving second trigger to another pin
        sendCommand(CMD_TRIGGER_PIN + " 1 PA8");

        assertEquals("VSS no input", 0, SensorCentral.getInstance().getValue(Sensor.VSS));

        // attaching VSS to trigger simulator since there is a jumper on test discovery
        sendCommand("set " + CMD_VSS_PIN + " pa5");

        sleep(2 * Timeouts.SECOND);

        assertEquals("VSS with input", 3, SensorCentral.getInstance().getValue(Sensor.VSS));
    }
}
