package com.rusefi.f4discovery;

import com.devexperts.logging.Logging;
import com.rusefi.RusefiTestBase;
import com.rusefi.Timeouts;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
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
 * PD2<>PA5
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
        ecu.setEngineType(ET_FORD_ASPIRE);
        ecu.sendCommand("set " + "trigger_type" + " " + TT_TT_TOOTHED_WHEEL_60_2);
        ecu.sendCommand(getDisableCommand(Fields.CMD_SELF_STIMULATION));
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
        ecu.setEngineType(ET_FRANKENSO_MIATA_NA6);
        ecu.changeRpm(1000);

        ecu.sendCommand(CMD_TRIGGER_SIMULATOR_PIN + " 0 none");
        ecu.sendCommand(CMD_TRIGGER_SIMULATOR_PIN + " 1 none");
        ecu.sendCommand(CMD_IDLE_PIN + " PD2");

        /* wasted two wire, so we really need 1..3 to be none */
        ecu.sendCommand(CMD_IGNITION_PIN + " 1 none");
        ecu.sendCommand(CMD_IGNITION_PIN + " 2 none");
        ecu.sendCommand(CMD_IGNITION_PIN + " 3 none");

        ecu.sendCommand(CMD_TRIGGER_PIN + " 1 PA8");

        /* DBG_LOGIC_ANALYZER */
        ecu.sendCommand("set debug_mode " +  com.rusefi.enums.debug_mode_e.DBG_LOGIC_ANALYZER.ordinal());

        /* 160 Hz */
        ecu.sendCommand("set idle_solenoid_freq " + FREQUENCY);

        /* save these for last to ensure logic is started */
        ecu.sendCommand(CMD_LOGIC_PIN + " 0 PA5");
        ecu.sendCommand(CMD_WRITECONFIG);
        sleep(2 * Timeouts.SECOND);
        ecu.sendCommand(getEnableCommand(Fields.CMD_SELF_STIMULATION));

        sleep(2 * Timeouts.SECOND);

        /* +-1% is still acceptable */
        EcuTestHelper.assertSomewhatClose("Idle PWM freq", FREQUENCY, SensorCentral.getInstance().getValue(Sensor.debugIntField1), 0.01);
    }
}