package com.rusefi.f4discovery;

import com.devexperts.logging.Logging;
import com.rusefi.IoUtil;
import com.rusefi.RusefiTestBase;
import com.rusefi.Timeouts;
import com.rusefi.common.PwmHardwareTestLogic;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.enums.engine_type_e;
import org.junit.Test;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.IoUtil.getDisableCommand;
import static com.rusefi.IoUtil.getEnableCommand;
import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;
import static com.rusefi.config.generated.Integration.CMD_BURNCONFIG;
import static com.rusefi.config.generated.Integration.CMD_EXTERNAL_STIMULATION;
import static org.junit.Assert.assertEquals;

/**
 * This test relies on jumpers connecting physical pins on Discovery:
 * PD1<>PC6
 * PD2<>PA5
 */

public class DiscoveryPwmHardwareTest extends RusefiTestBase {
    private static final Logging log = getLogging(DiscoveryPwmHardwareTest.class);

    @Override
    protected boolean needsHardwareTriggerInput() {
        // This test uses hardware trigger input!
        return true;
    }

    @Test
    public void scheduleBurnDoesNotAffectTriggerIssue2839() {
        ecu.setEngineType(engine_type_e.MINIMAL_PINS);
        ecu.sendCommand(IoUtil.setTriggerType(com.rusefi.enums.trigger_type_e.TT_TOOTHED_WHEEL_60_2));
        ecu.sendCommand(getDisableCommand(Integration.CMD_SELF_STIMULATION));
        ecu.sendCommand(getEnableCommand(CMD_EXTERNAL_STIMULATION));
        ecu.changeRpm(1200);
        nextChart();
        nextChart();
        int triggerErrors = (int) SensorCentral.getInstance().getValueSource(Sensor.totalTriggerErrorCounter).getValue();
        log.info("triggerErrors " + triggerErrors);
        for (int i = 0; i < 10; i++) {
            ecu.sendCommand(CMD_BURNCONFIG);
            sleep(5 * Timeouts.SECOND);
        }
        int totalTriggerErrorsNow = (int) SensorCentral.getInstance().getValueSource(Sensor.totalTriggerErrorCounter).getValue();
        log.info("totalTriggerErrorsNow " + totalTriggerErrorsNow);

        assertEquals("totalTriggerErrorCounter", triggerErrors, totalTriggerErrorsNow);
    }

    @Test
    public void testIdlePin() {
        PwmHardwareTestLogic.runIdlePwmTest(ecu, "PD2", "PA5");
    }
}
