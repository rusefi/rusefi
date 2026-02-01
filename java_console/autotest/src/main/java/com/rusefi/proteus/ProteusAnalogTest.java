package com.rusefi.proteus;

import com.rusefi.RusefiTestBase;
import com.rusefi.binaryprotocol.BinaryProtocolLogger;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.enums.engine_type_e;
import org.junit.Test;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

import static com.rusefi.IoUtil.*;
import static com.rusefi.config.generated.Integration.CMD_SELF_STIMULATION;
import static org.junit.Assert.*;

/*
 This test requires a particular hardware setup connected to a Proteus board.
    - External power supplied to the 12v_IGN pin, nominal 12 volts (but 11-13 will work, see testVbatt).
        Go buy some cheapie $8 12v power supply, cut the barrel jack off, and crimp pins on.
    - A jumper wire with a 1k resistor in series from Ignition output 10 to Analog volt 2 (TPS, see testTpsAnalogInput) and 100uF capacitor from AV2 to GND.
 */
public class ProteusAnalogTest extends RusefiTestBase {
    @Test
    public void testVbatt() {
        double vbatt = SensorCentral.getInstance().getValue(Sensor.VBatt);
        String expectedVbattString = System.getenv("HARDWARE_CI_VBATT");
        double expectedVbatt = expectedVbattString == null ? 12 : Double.parseDouble(expectedVbattString);

        // allow some tolerance for the supply voltage...
        assertTrue("VBatt " + vbatt + " out of range", vbatt > 0.9 * expectedVbatt && vbatt < 1.1 * expectedVbatt);
    }

    // not really 'analog' test. Not the best placement since we are unable to rebuild discovery HW CI :(
    @Test
    public void testTextPull() throws InterruptedException {
        requestText();
        BinaryProtocolLogger logger = new BinaryProtocolLogger(ecu.getLinkManager());
        ecu.getLinkManager().submit(new Runnable() {
            @Override
            public void run() {
                logger.compositeLogic(ecu.getLinkManager().getBinaryProtocol());
            }
        });
        requestText();
    }

    private void requestText() throws InterruptedException {
        CountDownLatch latch = new CountDownLatch(1);
        AtomicReference<String> textReference = new AtomicReference<>();
        ecu.getLinkManager().submit(() -> {
            String pendingTextMessages = ecu.getLinkManager().getBinaryProtocol().requestPendingTextMessages();
            textReference.set(pendingTextMessages);
            latch.countDown();
        });
        latch.await(60, TimeUnit.SECONDS);
        assertNotNull("Not null text protocol response expected", textReference.get());
    }

    private void setIdlePositionAndAssertTps(int idle, int expectedTps) {
        ecu.sendCommand("set idle_position " + idle);

        // wait a sec for sensors to update
        sleepSeconds(5);

        double actualTps = SensorCentral.getInstance().getValue(Sensor.TPSVALUE);

        // Accept up to 2.5% error - there is an analog filter installed, it should be at least be close
        assertEquals("With idle of " + idle, expectedTps, actualTps, 2.5);
    }

    @Test
    public void testTpsAnalogInput() {
        ecu.setEngineType(engine_type_e.PROTEUS_ANALOG_PWM_TEST); // proteus analog input PWM tester
        ecu.changeRpm(1000);
        // engine has to be spinning for idle valve to work
        ecu.sendCommand(getEnableCommand(CMD_SELF_STIMULATION));

        // 0% duty -> failed TPS (voltage too low)
        setIdlePositionAndAssertTps(2, 0);

        // These should all be valid points
        setIdlePositionAndAssertTps(20, 0);
        setIdlePositionAndAssertTps(35, 25);
        setIdlePositionAndAssertTps(50, 50);
        setIdlePositionAndAssertTps(65, 75);
        setIdlePositionAndAssertTps(80, 100);

        // 100% duty -> failed TPS (voltage too high)
        setIdlePositionAndAssertTps(98, 0);
    }

    void assertSensorValue(Sensor sensor, double expected) {
        double actual = SensorCentral.getInstance().getValue(sensor);
        assertEquals(sensor.toString(), expected, actual, 0.5);
    }

    @Test
    public void testUnconnectedInputs() {
        // CLT/IAT inputs should float at ~5 volts
        assertSensorValue(Sensor.rawClt, 5.0);
        assertSensorValue(Sensor.rawIat, 5.0);
    }
}
