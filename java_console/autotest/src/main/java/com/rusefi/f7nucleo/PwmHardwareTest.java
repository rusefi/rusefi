package com.rusefi.f7nucleo;

import com.devexperts.logging.Logging;
import com.rusefi.IoUtil;
import com.rusefi.RusefiTestBase;
import com.rusefi.Timeouts;
import com.rusefi.config.generated.Fields;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.enums.engine_type_e;
import com.rusefi.functional_tests.EcuTestHelper;
import org.junit.Test;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.IoUtil.getDisableCommand;
import static com.rusefi.IoUtil.getEnableCommand;
import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;
import static com.rusefi.config.generated.Fields.*;
import static org.junit.Assert.assertEquals;

/**
 * This test relies on jumpers connecting physical pins on Discovery:
 * PD1<>PC6
 * PD2<>PA6
 */

public class PwmHardwareTest extends RusefiTestBase {
    private static final Logging log = getLogging(PwmHardwareTest.class);

    @Override
    protected boolean needsHardwareTriggerInput() {
        // This test uses hardware trigger input!
        return true;
    }

    private static final int FREQUENCY = 160;

    @Test
    public void scheduleBurnDoesNotAffectTriggerIssue2839() {
        ecu.setEngineType(engine_type_e.FORD_ASPIRE_1996);
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
        ecu.setEngineType(engine_type_e.FRANKENSO_MIATA_NA6_MAP);
        ecu.changeRpm(1000);

        ecu.sendCommand(CMD_TRIGGER_SIMULATOR_PIN + " 0 none");
        ecu.sendCommand(CMD_TRIGGER_SIMULATOR_PIN + " 1 none");
        ecu.sendCommand(CMD_IDLE_PIN + " PD2");

        /* wasted two wire, so we really need 1..3 to be none */
        ecu.sendCommand(CMD_IGNITION_PIN + " 1 none");
        ecu.sendCommand(CMD_IGNITION_PIN + " 2 none");
        ecu.sendCommand(CMD_IGNITION_PIN + " 3 none");

        ecu.sendCommand(CMD_TRIGGER_PIN + " 1 PB1");

        /* DBG_LOGIC_ANALYZER */
        ecu.sendCommand("set debug_mode " +  com.rusefi.enums.debug_mode_e.DBG_LOGIC_ANALYZER.ordinal());

        /* 160 Hz */
        ecu.sendCommand("set idle_solenoid_freq " + FREQUENCY);

        /* save these for last to ensure logic is started */
        ecu.sendCommand(CMD_LOGIC_PIN + " 0 PA6");
        ecu.sendCommand(CMD_WRITECONFIG);
        sleep(2 * Timeouts.SECOND);
        ecu.sendCommand(getEnableCommand(Integration.CMD_SELF_STIMULATION));

        sleep(2 * Timeouts.SECOND);

        /* +-2% is still acceptable */
        EcuTestHelper.assertSomewhatClose("Idle PWM freq", FREQUENCY, SensorCentral.getInstance().getValue(Sensor.debugIntField1), 0.02);
    }
}
