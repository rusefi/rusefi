package com.rusefi.f4discovery;

import com.rusefi.RusefiTestBase;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.enums.SensorType;
import com.rusefi.enums.engine_type_e;
import com.rusefi.functional_tests.EcuTestHelper;
import com.rusefi.waves.EngineChart;
import org.junit.Test;

import java.util.Arrays;

import static com.rusefi.IoUtil.getEnableCommand;
import static com.rusefi.TestingUtils.assertNull;
import static com.rusefi.config.generated.Integration.CMD_SET_SENSOR_MOCK;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

/**
 * rusEFI firmware simulator functional test suite
 * <p/>
 * java -cp rusefi_console.jar com.rusefi.AutoTest
 *
 * @author Andrey Belomutskiy
 * 3/5/14
 */
public class CommonFunctionalTest extends RusefiTestBase {
    @Test
    public void testChangingIgnitionMode() {
        ecu.setEngineType(engine_type_e.MINIMAL_PINS);
        ecu.changeRpm(2000);

        // First is wasted spark
        ecu.sendCommand("set ignition_mode 2");

        {
            // Check that we're in wasted spark mode
            EngineChart chart = nextChart();

            // Wasted spark should fire cylinders 1/3...
            assertWaveNotNull(chart,
                    EngineChart.SPARK_1,
                    EngineChart.SPARK_3
            );

            // ...but not cylinders 2/4
            assertWaveNull(chart,
                    EngineChart.SPARK_2,
                    EngineChart.SPARK_4
            );
        }

        // Now switch to sequential mode
        ecu.sendCommand("set ignition_mode 1");

        {
            // Check that we're in sequential spark mode
            EngineChart chart = nextChart();

            // All 4 cylinders should be firing
            assertWaveNotNull(chart,
                    EngineChart.SPARK_1,
                    EngineChart.SPARK_2,
                    EngineChart.SPARK_3,
                    EngineChart.SPARK_4
            );
        }

        // Now switch to "one coil" mode
        ecu.sendCommand("set ignition_mode 0");

        {
            // Check that we're in sequential spark mode
            EngineChart chart = nextChart();

            // Only coil 1 should be active
            assertWaveNotNull(chart, EngineChart.SPARK_1);

            // And no others
            assertWaveNull(chart,
                    EngineChart.SPARK_2,
                    EngineChart.SPARK_3,
                    EngineChart.SPARK_4
            );
        }

        // Now switch BACK to wasted mode
        ecu.sendCommand("set ignition_mode 2");

        {
            // Check that we're in wasted spark mode
            EngineChart chart = nextChart();

            // Wasted spark should fire cylinders 1/3...
            assertWaveNotNull(chart,
                    EngineChart.SPARK_1,
                    EngineChart.SPARK_3
            );

            // ...but not cylinders 2/4
            assertWaveNull(chart,
                    EngineChart.SPARK_2,
                    EngineChart.SPARK_4
            );
        }
    }

    @Test
    public void testRevLimiter() {
        ecu.setEngineType(engine_type_e.MINIMAL_PINS);
        ecu.changeRpm(2000);

        // Alpha-N mode so that we actually inject some fuel (without mocking tons of sensors)
        ecu.sendCommand("set algorithm 2");
        // Set tps to 25% - make alpha-n happy
        ecu.sendCommand(CMD_SET_SENSOR_MOCK + " " + SensorType.Tps1.name() + " 25");
        ecu.sendCommand("set rpm_hard_limit 2500");

        {
            // Check that neither ignition nor injection is cut
            EngineChart chart = nextChart();

            assertWaveNotNull(chart,
                    EngineChart.SPARK_1,
                    EngineChart.INJECTOR_1
            );
        }

        // Now go above the hard limiter
        ecu.changeRpm(3000);

        {
            // Check that both ignition and injection are cut
            EngineChart chart = nextChart();

            // These channels are allowed to have falling edges - aka closing injectors and firing coils
            // but not allowed to have rising edges - aka opening injectors and charging coils
            assertWaveNoRises(chart,
                    EngineChart.SPARK_1,
                    EngineChart.INJECTOR_1
            );
        }

        // Check that it recovers when we go back under the limit
        ecu.changeRpm(2000);

        {
            // Check that neither ignition nor injection is cut
            EngineChart chart = nextChart();

            assertWaveNotNull(chart,
                    EngineChart.SPARK_1,
                    EngineChart.INJECTOR_1
            );
        }
    }

    @Test
    public void testCustomEngine() {
        ecu.setEngineType(engine_type_e.DEFAULT_FRANKENSO);
        ecu.sendCommand("set_toothed_wheel 4 0");
//        sendCommand("enable trigger_only_front");
//        changeRpm(100);
//        changeRpm(1500);
//        sendCommand("disable trigger_only_front");
//        changeRpm(100);
//        changeRpm(1500);
    }

    @Test
    public void testTwoStrokeSachs() {
        ecu.setEngineType(engine_type_e.SACHS);
        ecu.changeRpm(1200);
    }

    @Test
    public void test2003DodgeNeon() {
        ecu.setEngineType(engine_type_e.DODGE_NEON_2003_CRANK);
        ecu.sendCommand("set wwaeTau 0");
        ecu.sendCommand("set wwaeBeta 0");
        ecu.sendCommand(CMD_SET_SENSOR_MOCK + " " + SensorType.Map.name() + " 69.12");
        ecu.sendCommand(CMD_SET_SENSOR_MOCK + " " + SensorType.BatteryVoltage.name() + " 12");
        ecu.sendCommand("disable cylinder_cleanup");
        EngineChart chart;
        String msg = "2003 Neon cranking ";
        ecu.changeRpm(200);
        ecu.changeRpm(250); // another approach to artificial delay
        ecu.changeRpm(200);
        EcuTestHelper.assertSomewhatClose("VBatt", 12, SensorCentral.getInstance().getValue(Sensor.VBATT));

        chart = nextChart();
        assertWaveNotNull(msg, chart, EngineChart.SPARK_1);
        assertWaveNull(msg, chart, EngineChart.SPARK_2);
        assertWaveNotNull(msg, chart, EngineChart.SPARK_3);
        assertWaveNull(msg, chart, EngineChart.SPARK_4);

        // todo: why is width precision so low here? is that because of loaded Windows with 1ms precision?
        double widthRatio = 0.25;
        // WAT? this was just 0.009733333333333387?
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_1);
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_2);
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_3);
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_4);

        msg = "2003 Neon running";
        ecu.changeRpm(2000);
        ecu.changeRpm(2700);
        ecu.changeRpm(2000);
        chart = nextChart();
        assertWaveNotNull(msg, chart, EngineChart.SPARK_1);
        assertWaveNull(msg, chart, EngineChart.SPARK_2);
        assertWaveNotNull(msg, chart, EngineChart.SPARK_3);
        assertWaveNull(msg, chart, EngineChart.SPARK_4);

        chart = nextChart();
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_1);
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_2);
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_3);
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_4);

        ecu.sendCommand(getEnableCommand("trigger_only_front"));
        chart = nextChart();
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_1);
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_2);
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_3);
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_4);

        ecu.sendCommand("set_whole_timing_map 520");
        chart = nextChart();
        assertWaveNotNull(msg, chart, EngineChart.SPARK_1);

        ecu.sendCommand("set_whole_timing_map 0");
        chart = nextChart();
        assertWaveNotNull(msg, chart, EngineChart.SPARK_1);
    }

    @Test
    public void testMazdaProtege() {
        ecu.setEngineType(engine_type_e.FORD_ESCORT_GT);
        EngineChart chart;
        ecu.sendCommand(CMD_SET_SENSOR_MOCK + " " + SensorType.BatteryVoltage.name() + " 12");

        // Alpha-N mode so that we actually inject some fuel (without mocking tons of sensors)
        ecu.sendCommand("set algorithm 2");
        // Set tps to 25% - make alpha-n happy
        ecu.sendCommand(CMD_SET_SENSOR_MOCK + " " + SensorType.Tps1.name() + " 25");

        ecu.changeRpm(200);
        ecu.changeRpm(260);
        ecu.changeRpm(200);
        String msg = "ProtegeLX cranking";
        chart = nextChart();
        EcuTestHelper.assertSomewhatClose("", 12, SensorCentral.getInstance().getValue(Sensor.VBATT), 0.1);
        assertWaveNotNull(msg, chart, EngineChart.SPARK_3);
        assertWaveNotNull(msg, chart, EngineChart.SPARK_1);
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_1);
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_2);

        msg = "ProtegeLX running";
        ecu.changeRpm(2000);
        chart = nextChart();
        assertWaveNotNull(msg, chart, EngineChart.SPARK_1);
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_1);
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_2);
    }

    @Test
    public void test1995DodgeNeon() {
        ecu.setEngineType(engine_type_e.DODGE_NEON_1995);
        EngineChart chart;
        sendComplexCommand(Integration.CMD_INDIVIDUAL_INJECTION);
        /**
         * note that command order matters - RPM change resets wave chart
         */
        ecu.changeRpm(2000);
        chart = nextChart();

        String msg = "1995 Neon";
        //assertWaveNotNull(msg, chart, EngineChart.INJECTOR_4);
        //assertWaveNotNull(msg, chart, EngineChart.INJECTOR_2);
        //assertWaveNotNull(msg, chart, EngineChart.INJECTOR_1);
        //assertWaveNotNull(msg, chart, EngineChart.INJECTOR_3);

        assertWaveNotNull(msg, chart, EngineChart.SPARK_4);
        assertWaveNotNull(msg, chart, EngineChart.SPARK_2);
        assertWaveNotNull(msg, chart, EngineChart.SPARK_1);
        assertWaveNotNull(msg, chart, EngineChart.SPARK_3);

        // switching to Speed Density
        ecu.sendCommand(CMD_SET_SENSOR_MOCK + " " + SensorType.Map.name() + " 69.12");
        sendComplexCommand("set algorithm 0");
        ecu.changeRpm(2600);
        ecu.changeRpm(2000);
        chart = nextChart();
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_4);
    }

    @Test
    public void testFord6() {
        ecu.setEngineType(engine_type_e.FORD_INLINE_6_1995);
        EngineChart chart;
        ecu.changeRpm(2000);
        chart = nextChart();

        String msg = "ford 6";

        assertWaveNotNull(msg, chart, EngineChart.SPARK_1);

        assertWaveNull(msg, chart, EngineChart.TRIGGER_2);
        sendComplexCommand("set " + "trigger_type" + " " + com.rusefi.enums.trigger_type_e.TT_FORD_ASPIRE.ordinal());
        chart = nextChart();
        assertTrue(chart.get(EngineChart.TRIGGER_2) != null);
    }

    @Test
    public void testFordAspire() {
        ecu.setEngineType(engine_type_e.FORD_ASPIRE_1996);
        ecu.sendCommand("disable cylinder_cleanup");
        ecu.sendCommand(CMD_SET_SENSOR_MOCK + " " + SensorType.Map.name() + " 69.12");
        ecu.sendCommand(CMD_SET_SENSOR_MOCK + " " + SensorType.BatteryVoltage.name() + " 12");
        String msg;
        EngineChart chart;
        // todo: interesting changeRpm(100);
        sendComplexCommand("set cranking_rpm 500");
        ecu.changeRpm(200);

        chart = nextChart();
        EcuTestHelper.assertSomewhatClose(12, SensorCentral.getInstance().getValue(Sensor.VBATT));
        assertWaveNotNull("aspire default cranking ", chart, EngineChart.SPARK_1);

        ecu.changeRpm(600);
        chart = nextChart();
        assertWaveNotNull("aspire default running ", chart, EngineChart.SPARK_1);

        ecu.changeRpm(200);

        ecu.sendCommand("set cranking_charge_angle 65");
        ecu.sendCommand("set cranking_timing_angle -31");

        chart = nextChart();
        assertWaveNotNull("aspire cranking", chart, EngineChart.SPARK_1);

        ecu.sendCommand("set cranking_timing_angle -40");
        chart = nextChart();
        assertWaveNotNull("aspire", chart, EngineChart.SPARK_1);
        ecu.sendCommand("set cranking_timing_angle 149");

        ecu.sendCommand("set cranking_charge_angle 40");
        chart = nextChart();

        assertWaveNotNull("aspire", chart, EngineChart.SPARK_1);
        ecu.sendCommand("set cranking_charge_angle 65");

        ecu.changeRpm(600);
        sendComplexCommand("set cranking_rpm 700");
        chart = nextChart();
        assertWaveNotNull("cranking@600", chart, EngineChart.SPARK_1);

        ecu.changeRpm(2000);

        ecu.changeRpm(2600);
        ecu.changeRpm(2000);
        chart = nextChart();

        msg = "aspire running";

        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_1);
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_2);
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_3);
        assertWaveNotNull(msg, chart, EngineChart.INJECTOR_4);

        assertWaveNotNull(chart, EngineChart.SPARK_1);

        ecu.sendCommand("set_fuel_map 2200 4 15.66");
        ecu.sendCommand("set_fuel_map 2000 4 15.66");
        ecu.sendCommand("set_fuel_map 2200 4.2 15.66");
        ecu.sendCommand("set_fuel_map 2000 4.2 15.66");
        sendComplexCommand("set global_trigger_offset_angle 175");
        chart = nextChart();
        assertWaveNotNull(msg + " fuel", chart, EngineChart.INJECTOR_1);
        assertWaveNotNull(msg + " fuel", chart, EngineChart.INJECTOR_2);
        assertWaveNotNull(msg + " fuel", chart, EngineChart.INJECTOR_3);
        assertWaveNotNull(msg + " fuel", chart, EngineChart.INJECTOR_4);

        assertWaveNotNull(chart, EngineChart.SPARK_1);
        assertWaveNull(chart, EngineChart.SPARK_2);

        sendComplexCommand("set global_trigger_offset_angle 130");
        sendComplexCommand("set injection_offset 369");
        chart = nextChart();
        assertWaveNotNull(chart, EngineChart.SPARK_1);

        // let's enable more channels dynamically
        sendComplexCommand(Integration.CMD_INDIVIDUAL_INJECTION);
        chart = nextChart();
        assertWaveNotNull("Switching Aspire into INDIVIDUAL_COILS mode", chart, EngineChart.SPARK_2);
        assertWaveNotNull(chart, EngineChart.SPARK_3);

        ecu.sendCommand("set_whole_timing_map 520");
        chart = nextChart();
        assertWaveNotNull(chart, EngineChart.SPARK_2);

        // switching to Speed Density
        sendComplexCommand("set algorithm 0");
        ecu.changeRpm(2400);
        ecu.changeRpm(2000);
        chart = nextChart();
        EcuTestHelper.assertSomewhatClose("MAP", 69.12, SensorCentral.getInstance().getValue(Sensor.MAPVALUE));
        //assertEquals(1, SensorCentral.getInstance().getValue(Sensor.));

        assertWaveNotNull(msg + " fuel SD #1", chart, EngineChart.INJECTOR_1);
        assertWaveNotNull(msg + " fuel SD #2", chart, EngineChart.INJECTOR_2);
        assertWaveNotNull(msg + " fuel SD #3", chart, EngineChart.INJECTOR_3);
        assertWaveNotNull(msg + " fuel SD #4", chart, EngineChart.INJECTOR_4);

        // above hard limit
        ecu.changeRpm(10000);
        chart = nextChart();
        assertWaveNull("hard limit check", chart, EngineChart.INJECTOR_1);
    }

    @Test
    public void testStackOverflow() {
        // This ECU command has a built in check to make sure no threads are near overflow
        ecu.sendCommand("threadsinfo");
    }

    /**
     * This method waits for longer then usual.
     * todo: inline this method? complex and less complex seem to have same timeout these days?
     */
    private void sendComplexCommand(String command) {
        ecu.sendCommand(command);
    }

    private static void assertWaveNull(EngineChart chart, String key) {
        assertWaveNull("", chart, key);
    }

    private static void assertWaveNull(EngineChart chart, String... keys) {
        Arrays.stream(keys).peek(k -> assertWaveNull(chart, k));
    }

    private static void assertWaveNull(String msg, EngineChart chart, String key) {
        assertNull(msg + "chart for " + key, chart.get(key));
    }

    private static void assertWaveNotNull(EngineChart chart, String key) {
        assertWaveNotNull("", chart, key);
    }

    private static void assertWaveNotNull(String msg, EngineChart chart, String key) {
        assertTrue(msg, chart.get(key) != null);
    }

    private static void assertWaveNotNull(EngineChart chart, String... keys) {
        Arrays.stream(keys).peek(k -> assertWaveNotNull(chart, k));
    }

    private static void assertWaveNoRises(EngineChart chart, String key) {
        StringBuilder events = chart.get(key);

        // if no events of this type at all, return since this passes the test
        if (events == null) {
            return;
        }

        // Assert that there are no up (rise) events in the channel's sequence
        assertFalse(events.toString().contains("u"));
    }

    private static void assertWaveNoRises(EngineChart chart, String... keys) {
        Arrays.stream(keys).peek(k -> assertWaveNoRises(chart, k));
    }

}
