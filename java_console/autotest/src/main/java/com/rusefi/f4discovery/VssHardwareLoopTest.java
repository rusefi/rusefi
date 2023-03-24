package com.rusefi.f4discovery;

import com.rusefi.autotest.ControllerConnectorState;
import com.rusefi.RusefiTestBase;
import com.rusefi.Timeouts;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.enums.engine_type_e;
import com.rusefi.functional_tests.EcuTestHelper;
import org.junit.Test;

import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;
import static com.rusefi.config.generated.Fields.*;

/**
 * This test relies on jumpers connecting physical pins on Discovery:
 * PD1<>PC6
 * PD2<>PA5
 */
public class VssHardwareLoopTest extends RusefiTestBase {
    @Override
    protected boolean needsHardwareTriggerInput() {
        // This test uses hardware trigger input!
        return true;
    }

    @Test
    public void test() {
        ecu.setEngineType(engine_type_e.FRANKENSO_MIATA_NA6_MAP);
        ecu.changeRpm(1000);

        // making output pins available
        ecu.sendCommand(CMD_TRIGGER_SIMULATOR_PIN + " 0 none");
        ecu.sendCommand(CMD_TRIGGER_SIMULATOR_PIN + " 1 none");
        ecu.sendCommand(CMD_TRIGGER_PIN + " 1 none");

        // Hook up 1khz idle on formerly-trigger-stim pin
        ecu.sendCommand(CMD_IDLE_PIN + " PD2");
        ecu.sendCommand("set idle_solenoid_freq 100");

        EcuTestHelper.assertSomewhatClose("VSS no input", 0, SensorCentral.getInstance().getValue(Sensor.vehicleSpeedKph));

        // attaching VSS to idle output since there is a jumper on test discovery
        ecu.sendCommand("set " + CMD_VSS_PIN + " " + "pa5");

        sleep(2 * Timeouts.SECOND);

        EcuTestHelper.assertSomewhatClose("VSS with input", 145, SensorCentral.getInstance().getValue(Sensor.vehicleSpeedKph));

        // not related to VSS test, just need to validate this somewhere, so this random test is as good as any
        if (ControllerConnectorState.firmwareVersion == null)
            throw new IllegalStateException("firmwareVersion has not arrived");
    }
}
