package com.rusefi.common;

import com.rusefi.Timeouts;
import com.rusefi.autotest.ControllerConnectorState;
import com.rusefi.core.SensorNames;
import com.rusefi.enums.engine_type_e;
import com.rusefi.functional_tests.EcuTestHelper;

import static com.rusefi.config.generated.Integration.*;

public class VssHardwareTestLogic {
    private static final int FREQUENCY = 100;
    private static final int WHEEL_REV_PER_KM = 500;
    // Miata preset: 4.3 final drive, 6/23 speedometer gears, four sensor teeth.
    private static final double EXPECTED_SPEED_KPH = FREQUENCY * 3600.0 / (WHEEL_REV_PER_KM * (4.3 * 6 / 23) * 4);

    public static void runIdleVssTest(EcuTestHelper ecu, String idlePin, String vssPin) {
        ecu.setEngineType(engine_type_e.FRANKENSO_MIATA_NA6_MAP);
        ecu.changeRpm(1000);
        ecu.sendCommand("set driveWheelRevPerKm " + WHEEL_REV_PER_KM);

        // making output pins available
        ecu.sendCommand(CMD_TRIGGER_SIMULATOR_PIN + " 0 none");
        ecu.sendCommand(CMD_TRIGGER_SIMULATOR_PIN + " 1 none");
        ecu.sendCommand(CMD_TRIGGER_PIN + " 1 none");

        // Hook up 100 Hz idle on formerly-trigger-stim pin.
        ecu.sendCommand(CMD_IDLE_PIN + " " + idlePin);
        ecu.sendCommand("set idle_solenoid_freq " + FREQUENCY);

        EcuTestHelper.assertSensorEventually("VSS no input", SensorNames.VEHICLESPEEDKPH, 0, Timeouts.CMD_TIMEOUT);

        // attaching VSS to idle output since there is a jumper on test discovery
        ecu.sendCommand("set " + CMD_VSS_PIN + " " + vssPin);

        // Engine-type changes can still be burning flash when the input is attached.
        EcuTestHelper.assertSensorEventually("VSS with input", SensorNames.VEHICLESPEEDKPH, EXPECTED_SPEED_KPH, Timeouts.CMD_TIMEOUT);

        // not related to VSS test, just need to validate this somewhere, so this random test is as good as any
        if (ControllerConnectorState.firmwareVersion == null)
            throw new IllegalStateException("firmwareVersion has not arrived");
    }
}
