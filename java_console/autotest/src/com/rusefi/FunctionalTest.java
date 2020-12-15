package com.rusefi;


import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.functional_tests.EcuTestHelper;
import com.rusefi.waves.EngineChart;
import com.rusefi.waves.EngineReport;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;

import static com.rusefi.IoUtil.getEnableCommand;
import static com.rusefi.TestingUtils.*;
import static com.rusefi.config.generated.Fields.*;

/**
 * rusEfi firmware simulator functional test suite
 * <p/>
 * java -cp rusefi_console.jar com.rusefi.AutoTest
 *
 * @author Andrey Belomutskiy
 * 3/5/14
 */
public class FunctionalTest {
    private EcuTestHelper ecu;

    @Before
    public void startUp() {
        ecu = EcuTestHelper.createInstance();
    }

    @Ignore
    @Test
    public void testCustomEngine() {
        ecu.setEngineType(ET_DEFAULT_FRANKENSO);
        ecu.sendCommand("set_toothed_wheel 4 0");
//        sendCommand("enable trigger_only_front");
//        changeRpm(100);
//        changeRpm(1500);
//        sendCommand("disable trigger_only_front");
//        changeRpm(100);
//        changeRpm(1500);
    }

    @Ignore
    @Test
    public void testMazdaMiata2003() {
        ecu.setEngineType(ET_FRANKENSO_MIATA_NB2);
        ecu.sendCommand("get cranking_dwell"); // just test coverage
//        sendCommand("get nosuchgettersdfsdfsdfsdf"); // just test coverage
    }

    @Ignore
    @Test
    public void testCamaro() {
        ecu.setEngineType(ET_CAMARO);
    }

    @Test
    @Ignore
    public void testSachs() {
        ecu.setEngineType(ET_SACHS);
//        String msg = "BMW";
        ecu.changeRpm(1200);
        // todo: add more content
    }

    @Test
    @Ignore
    public void testBmwE34() {
        ecu.setEngineType(ET_BMW_E34);
        ecu.sendCommand("chart 1");
        String msg = "BMW";
        EngineChart chart;
        ecu.changeRpm(200);
        chart = nextChart();
        double x = 173.988;
        // something is wrong here - it's a 6 cylinder here, why 4 cylinder cycle?
        assertWave(msg, chart, EngineChart.SPARK_1, 0.0199666, x, x + 180, x + 360, x + 540);

        ecu.changeRpm(1200);
        chart = nextChart();

        x = 688.464;
        // something is wrong here - it's a 6 cylinder here, why 4 cylinder cycle?
        assertWave(msg, chart, EngineChart.SPARK_1, 0.0597999999, x, x + 180, x + 360, x + 540);

        x = 101;
        // 6 cylinder
        assertWave(msg, chart, EngineChart.MAP_AVERAGING, 0.139, x, x + 120, x + 240, x + 360, x + 480, x + 600);
    }

    @Test
    @Ignore
    public void testCitroenBerlingo() {
        ecu.setEngineType(ET_CITROEN_TU3JP);
//        String msg = "Citroen";
        ecu.changeRpm(1200);
        // todo: add more content
    }

    @Test
    @Ignore
    public void test2003DodgeNeon() {
        ecu.setEngineType(ET_DODGE_NEON_2003_CRANK);
        ecu.sendCommand("set wwaeTau 0");
        ecu.sendCommand("set wwaeBeta 0");
        ecu.sendCommand("set mock_map_voltage 1");
        ecu.sendCommand("set mock_vbatt_voltage 1.20");
        ecu.sendCommand("disable cylinder_cleanup");
        EngineChart chart;
        String msg = "2003 Neon cranking ";
        ecu.changeRpm(200);
        ecu.changeRpm(250); // another approach to artificial delay
        ecu.changeRpm(200);
        EcuTestHelper.assertEquals("VBatt", 12, SensorCentral.getInstance().getValue(Sensor.VBATT));

        chart = nextChart();
        double x = 100;
        assertWave(true, msg, chart, EngineChart.SPARK_1, 0.194433, 0.01, EngineReport.RATIO, x + 180, x + 540);
        assertWaveNull(msg, chart, EngineChart.SPARK_2);
        assertWave(true, msg, chart, EngineChart.SPARK_3, 0.194433, 0.01, EngineReport.RATIO, x, x + 360);
        assertWaveNull(msg, chart, EngineChart.SPARK_4);

        x = 176.856;
        // todo: why is width precision so low here? is that because of loaded Windows with 1ms precision?
        double widthRatio = 0.25;
        // WAT? this was just 0.009733333333333387?
        assertWave(true, msg, chart, EngineChart.INJECTOR_1, 0.01056666666666691, 0.02, widthRatio, x, x + 180, x + 360, x + 540);
        assertWave(true, msg, chart, EngineChart.INJECTOR_2, 0.01056666666666691, 0.02, widthRatio, x, x + 180, x + 360, x + 540);
        assertWave(true, msg, chart, EngineChart.INJECTOR_3, 0.01056666666666691, 0.02, widthRatio, x, x + 180, x + 360, x + 540);
        assertWave(true, msg, chart, EngineChart.INJECTOR_4, 0.01056666666666691, 0.02, widthRatio, x, x + 180, x + 360, x + 540);

        msg = "2003 Neon running";
        ecu.changeRpm(2000);
        ecu.changeRpm(2700);
        ecu.changeRpm(2000);
        chart = nextChart();
        x = 104.0;
        assertWave(true, msg, chart, EngineChart.SPARK_1, 0.13299999999999998, EngineReport.RATIO, EngineReport.RATIO, x + 180, x + 540);
        assertWaveNull(msg, chart, EngineChart.SPARK_2);
        assertWave(true, msg, chart, EngineChart.SPARK_3, 0.13299999999999998, EngineReport.RATIO, EngineReport.RATIO, x, x + 360);
        assertWaveNull(msg, chart, EngineChart.SPARK_4);

        chart = nextChart();
        x = 74;
        assertWave(true, msg, chart, EngineChart.INJECTOR_1, 0.29233, EngineReport.RATIO, 0.2, x + 360);
        assertWave(true, msg, chart, EngineChart.INJECTOR_2, 0.29233, 0.15, 0.2, x + 180);
        assertWave(true, msg, chart, EngineChart.INJECTOR_3, 0.29233, 0.15, EngineReport.RATIO, x + 540);
        assertWave(true, msg, chart, EngineChart.INJECTOR_4, 0.29233, 0.15, 0.2, x);

        ecu.sendCommand(getEnableCommand("trigger_only_front"));
        chart = nextChart();
        assertWave(true, msg, chart, EngineChart.INJECTOR_1, 0.29233, 0.1, 0.2, x + 360);
        assertWave(true, msg, chart, EngineChart.INJECTOR_2, 0.29233, EngineReport.RATIO, 0.2, x + 180);
        assertWave(true, msg, chart, EngineChart.INJECTOR_3, 0.29233, 0.1, 0.2, x + 540);
        assertWave(true, msg, chart, EngineChart.INJECTOR_4, 0.29233, 0.1, 0.2, x);

        ecu.sendCommand("set_whole_timing_map 520");
        chart = nextChart();
        x = 328;
        assertWave(true, msg, chart, EngineChart.SPARK_1, 0.13299999999999998, EngineReport.RATIO, EngineReport.RATIO, x + 180, x + 540);

        ecu.sendCommand("set_whole_timing_map 0");
        chart = nextChart();
        x = 128;
        assertWave(true, msg, chart, EngineChart.SPARK_1, 0.13299999999999998, EngineReport.RATIO, EngineReport.RATIO, x + 180, x + 540);
    }

    @Test
    @Ignore
    public void testMazdaProtege() {
        ecu.setEngineType(ET_FORD_ESCORT_GT);
        EngineChart chart;
        ecu.sendCommand("set mock_vbatt_voltage 1.395");
        ecu.changeRpm(200);
        ecu.changeRpm(260);
        ecu.changeRpm(200);
        String msg = "ProtegeLX cranking";
        chart = nextChart();
        EcuTestHelper.assertEquals("", 12, SensorCentral.getInstance().getValue(Sensor.VBATT), 0.1);
        double x = 107;
        assertWave(msg, chart, EngineChart.SPARK_3, 0.194433, x);
        assertWave(msg, chart, EngineChart.SPARK_1, 0.194433, x + 540);
        x = 0;
        assertWaveFall(msg, chart, EngineChart.INJECTOR_1, 0.008566666666, x, x + 180, x + 360, x + 540);
        assertWaveFall(msg, chart, EngineChart.INJECTOR_2, 0.008566666666, x, x + 180, x + 360, x + 540);

        msg = "ProtegeLX running";
        ecu.changeRpm(2000);
        chart = nextChart();
        x = 112;
        assertWave(msg, chart, EngineChart.SPARK_1, 0.13333333333333333, x, x + 180, x + 360, x + 540);
        x = 0;
        assertWaveFall(msg, chart, EngineChart.INJECTOR_1, 0.21433333333333345, x + 180, x + 540);
        assertWaveFall(msg, chart, EngineChart.INJECTOR_2, 0.21433333333333345, x, x + 360);
    }

    @Test
    @Ignore
    public void test1995DodgeNeon() {
        ecu.setEngineType(ET_DODGE_NEON_1995);
        EngineChart chart;
        sendComplexCommand("set_whole_fuel_map 3");
        sendComplexCommand("set_individual_coils_ignition");
        /**
         * note that command order matters - RPM change resets wave chart
         */
        ecu.changeRpm(2000);
        chart = nextChart();

        String msg = "1995 Neon";
        double x = -70;
        assertWaveFall(msg, chart, EngineChart.INJECTOR_4, 0.133, x + 540);
        assertWaveFall(msg, chart, EngineChart.INJECTOR_2, 0.133, x + 720);
        assertWaveFall(msg, chart, EngineChart.INJECTOR_1, 0.133, x + 180);
        assertWaveFall(msg, chart, EngineChart.INJECTOR_3, 0.133, x + 360);

        x = 112.92;
        assertWave(msg, chart, EngineChart.SPARK_4, 0.13333, x + 540);
        assertWave(msg, chart, EngineChart.SPARK_2, 0.13333, x);
        assertWave(msg, chart, EngineChart.SPARK_1, 0.13333, x + 180);
        assertWave(msg, chart, EngineChart.SPARK_3, 0.13333, x + 360);

        // switching to Speed Density
        ecu.sendCommand("set mock_map_voltage 1");
        sendComplexCommand("set algorithm 3");
        ecu.changeRpm(2600);
        ecu.changeRpm(2000);
        chart = nextChart();
        x = -70;
        assertWaveFall(msg, chart, EngineChart.INJECTOR_4, 0.493, x + 540);
    }

    @Test
    @Ignore
    public void testRoverV8() {
        ecu.setEngineType(ET_ROVER_V8);
    }

    @Test
    public void testFordFiesta() {
        ecu.setEngineType(ET_FORD_FIESTA);
        EngineChart chart;
        ecu.changeRpm(2000);
        chart = nextChart();

        String msg = "Fiesta";
        double x = 312;
        assertWave("wasted spark #1 with Fiesta", chart, EngineChart.SPARK_1, 0.1333333, x, x + 360);
        assertWaveNull(chart, EngineChart.SPARK_2);
        assertWave("wasted spark #3 with Fiesta", chart, EngineChart.SPARK_3, 0.1333333, x + 180, x + 540);
        assertWaveNull(msg, chart, EngineChart.SPARK_4);
    }

    @Test
    public void testFord6() {
        ecu.setEngineType(ET_FORD_INLINE_6);
        EngineChart chart;
        ecu.changeRpm(2000);
        chart = nextChart();

        String msg = "ford 6";

        double x = 7;
        assertWave(msg, chart, EngineChart.SPARK_1, 0.01666, x, x + 120, x + 240, x + 360, x + 480, x + 600);

        assertWaveNull(msg, chart, EngineChart.TRIGGER_2);
        sendComplexCommand("set " + "trigger_type" + " 1"); // TT_FORD_ASPIRE
        chart = nextChart();
        assertTrue(msg + " trigger2", chart.get(EngineChart.TRIGGER_2) != null);
    }

    @Test
    public void testFordAspire() {
        ecu.setEngineType(ET_FORD_ASPIRE);
        ecu.sendCommand("disable cylinder_cleanup");
        ecu.sendCommand("set mock_map_voltage 1");
        ecu.sendCommand("set mock_vbatt_voltage 2.2");
        String msg;
        EngineChart chart;
        // todo: interesting changeRpm(100);
        sendComplexCommand("set cranking_rpm 500");
        ecu.changeRpm(200);

        double x;
        chart = nextChart();
        EcuTestHelper.assertEquals(12, SensorCentral.getInstance().getValue(Sensor.VBATT));
        x = 55;
        assertWave("aspire default cranking ", chart, EngineChart.SPARK_1, 0.1944, x, x + 180, x + 360, x + 540);


        ecu.changeRpm(600);
        chart = nextChart();
        x = 78;
        assertWave(true, "aspire default running ", chart, EngineChart.SPARK_1, 0.04, 0.1, 0.1, x, x + 180, x + 360, x + 540);

        ecu.changeRpm(200);

        ecu.sendCommand("set cranking_charge_angle 65");
        ecu.sendCommand("set cranking_timing_angle -31");

        chart = nextChart();
        x = 55;
        assertWave("aspire cranking", chart, EngineChart.SPARK_1, 0.18, x, x + 180, x + 360, x + 540);

        ecu.sendCommand("set cranking_timing_angle -40");
        chart = nextChart();
        x = 64;
        assertWave("aspire", chart, EngineChart.SPARK_1, 0.18, x, x + 180, x + 360, x + 540);
        ecu.sendCommand("set cranking_timing_angle 149");

        ecu.sendCommand("set cranking_charge_angle 40");
        chart = nextChart();
        x = 80;
        assertWave("aspire", chart, EngineChart.SPARK_1, 40.0 / 360, x, x + 180, x + 360, x + 540);
        ecu.sendCommand("set cranking_charge_angle 65");

        ecu.changeRpm(600);
        sendComplexCommand("set cranking_rpm 700");
        chart = nextChart();
        x = 55;
        assertWave("cranking@600", chart, EngineChart.SPARK_1, 0.18, x, x + 180, x + 360, x + 540);

        ecu.changeRpm(2000);
        ecu.sendCommand("set_whole_fuel_map 1.57");

        ecu.changeRpm(2600);
        ecu.changeRpm(2000);
        chart = nextChart();

        msg = "aspire running";

        assertWaveFall(msg, chart, EngineChart.INJECTOR_1, 0.109, 238.75);
        assertWaveFall(msg, chart, EngineChart.INJECTOR_2, 0.109, 53.04);
        assertWaveFall(msg, chart, EngineChart.INJECTOR_3, 0.109, 417.04);
        assertWaveFall(msg, chart, EngineChart.INJECTOR_4, 0.109, 594.04);

        x = 7;
        assertWave(chart, EngineChart.SPARK_1, 0.133, x, x + 180, x + 360, x + 540);

        ecu.sendCommand("set_fuel_map 2200 4 15.66");
        ecu.sendCommand("set_fuel_map 2000 4 15.66");
        ecu.sendCommand("set_fuel_map 2200 4.2 15.66");
        ecu.sendCommand("set_fuel_map 2000 4.2 15.66");
        // mock 2 means 4 on the gauge because of the divider. should we simplify this?
        ecu.sendCommand("set " + MOCK_MAF_COMMAND + " 2");
        sendComplexCommand("set global_trigger_offset_angle 175");
        chart = nextChart();

        assertWaveFall(msg + " fuel", chart, EngineChart.INJECTOR_1, 0.763, 238.75);
        assertWaveFall(msg + " fuel", chart, EngineChart.INJECTOR_2, 0.763, 53.04);
        assertWaveFall(msg + " fuel", chart, EngineChart.INJECTOR_3, 0.763, 417.04);
        assertWaveFall(msg + " fuel", chart, EngineChart.INJECTOR_4, 0.763, 594.04);

        x = 33.0;
        assertWave(chart, EngineChart.SPARK_1, 0.133, x, x + 180, x + 360, x + 540);
        assertWaveNull(chart, EngineChart.SPARK_2);

        sendComplexCommand("set global_trigger_offset_angle 130");
        sendComplexCommand("set injection_offset 369");
        chart = nextChart();
        x = 33;
        assertWave(chart, EngineChart.SPARK_1, 0.133, x, x + 180, x + 360, x + 540);

        // let's enable more channels dynamically
        sendComplexCommand("set_individual_coils_ignition");
        chart = nextChart();
        assertWave("Switching Aspire into INDIVIDUAL_COILS mode", chart, EngineChart.SPARK_2, 0.133, x + 540);
        assertWave(chart, EngineChart.SPARK_3, 0.133, x + 180);

        ecu.sendCommand("set_whole_timing_map 520");
        chart = nextChart();
        x = 58.92;
        assertWave(chart, EngineChart.SPARK_2, 0.133, x);

        // switching to Speed Density
        ecu.sendCommand("set mock_maf_voltage 2");
        sendComplexCommand("set algorithm 3");
        ecu.changeRpm(2400);
        ecu.changeRpm(2000);
        chart = nextChart();
        EcuTestHelper.assertEquals("MAP", 69.12, SensorCentral.getInstance().getValue(Sensor.MAP));
        //assertEquals(1, SensorCentral.getInstance().getValue(Sensor.));
        x = 8.88;
        assertWave(false, msg + " fuel SD #1", chart, EngineChart.INJECTOR_1, 0.577, 0.1, 0.1, x + 180);
        assertWave(false, msg + " fuel SD #2", chart, EngineChart.INJECTOR_2, 0.577, 0.1, 0.1, x);
        assertWave(false, msg + " fuel SD #3", chart, EngineChart.INJECTOR_3, 0.577, 0.1, 0.1, x + 360);
        assertWave(false, msg + " fuel SD #4", chart, EngineChart.INJECTOR_4, 0.577, 0.1, 0.1, x + 540);

        // above hard limit
        ecu.changeRpm(10000);
        chart = nextChart();
        assertWaveNull("hard limit check", chart, EngineChart.INJECTOR_1);
    }

    /**
     * This method waits for longer then usual.
     */
    private void sendComplexCommand(String command) {
        ecu.sendCommand(command, EcuTestHelper.COMPLEX_COMMAND_RETRY, Timeouts.CMD_TIMEOUT);
    }

    private static void assertWaveNull(EngineChart chart, String key) {
        assertWaveNull("", chart, key);
    }

    private static void assertWaveNull(String msg, EngineChart chart, String key) {
        assertNull(msg + "chart for " + key, chart.get(key));
    }

    private EngineChart nextChart() {
        return TestingUtils.nextChart(ecu.commandQueue);
    }
}
