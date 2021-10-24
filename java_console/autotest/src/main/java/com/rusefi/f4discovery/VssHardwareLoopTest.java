package com.rusefi.f4discovery;

import com.rusefi.autotest.ControllerConnectorState;
import com.rusefi.RusefiTestBase;
import com.rusefi.Timeouts;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.enums.engine_type_e;
import com.rusefi.functional_tests.EcuTestHelper;
import org.junit.Test;

import static com.rusefi.IoUtil.getEnableCommand;
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
        ecu.sendCommand(getEnableCommand(Fields.CMD_EXTERNAL_STIMULATION));
        ecu.changeRpm(1400);

        // moving second trigger to another pin
        ecu.sendCommand(CMD_TRIGGER_PIN + " 1 PA8");

        EcuTestHelper.assertSomewhatClose("VSS no input", 0, SensorCentral.getInstance().getValue(Sensor.VSS));

        // attaching VSS to trigger simulator since there is a jumper on test discovery
        ecu.sendCommand("set " + CMD_VSS_PIN + " pa5");

        sleep(2 * Timeouts.SECOND);

        EcuTestHelper.assertSomewhatClose("VSS with input", 3, SensorCentral.getInstance().getValue(Sensor.VSS));

        // not related to VSS test, just need to validate this somewhere, so this random test is as good as any
        if (ControllerConnectorState.firmwareVersion == null)
            throw new IllegalStateException("firmwareVersion has not arrived");
    }

}
