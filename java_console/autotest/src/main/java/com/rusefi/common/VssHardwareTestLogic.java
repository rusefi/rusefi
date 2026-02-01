package com.rusefi.common;

import com.rusefi.Timeouts;
import com.rusefi.autotest.ControllerConnectorState;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.enums.engine_type_e;
import com.rusefi.functional_tests.EcuTestHelper;

import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;
import static com.rusefi.config.generated.Integration.*;

public class VssHardwareTestLogic {
    private static final int FREQUENCY = 160;

    public static void runIdleVssTest(EcuTestHelper ecu, String idlePin, String vssPin) {
         ecu.setEngineType(engine_type_e.FRANKENSO_MIATA_NA6_MAP);
        ecu.changeRpm(1000);

        // making output pins available
        ecu.sendCommand(CMD_TRIGGER_SIMULATOR_PIN + " 0 none");
        ecu.sendCommand(CMD_TRIGGER_SIMULATOR_PIN + " 1 none");
        ecu.sendCommand(CMD_TRIGGER_PIN + " 1 none");

        // Hook up 1khz idle on formerly-trigger-stim pin
        ecu.sendCommand(CMD_IDLE_PIN + " " + idlePin);
        ecu.sendCommand("set idle_solenoid_freq 100");

        EcuTestHelper.assertSomewhatClose("VSS no input", 0, SensorCentral.getInstance().getValue(Sensor.vehicleSpeedKph));

        // attaching VSS to idle output since there is a jumper on test discovery
        ecu.sendCommand("set " + CMD_VSS_PIN + " " + vssPin);

        sleep(2 * Timeouts.SECOND);

        // todo: this command does not seem to work for whatever reasons :( cAsE? else?
        ecu.sendCommand("set " + "driveWheelRevPerKm" + " " + "500");
        EcuTestHelper.assertSomewhatClose("VSS with input", 145.58, SensorCentral.getInstance().getValue(Sensor.vehicleSpeedKph));

        // not related to VSS test, just need to validate this somewhere, so this random test is as good as any
        if (ControllerConnectorState.firmwareVersion == null)
            throw new IllegalStateException("firmwareVersion has not arrived");
    }
}
