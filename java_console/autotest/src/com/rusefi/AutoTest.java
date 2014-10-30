package com.rusefi;


import com.irnems.FileLog;
import com.rusefi.waves.WaveChart;

import static com.rusefi.IoUtil.nextChart;
import static com.rusefi.IoUtil.sendCommand;
import static com.rusefi.TestingUtils.assertNull;
import static com.rusefi.TestingUtils.assertTrue;
import static com.rusefi.TestingUtils.assertWave;

/**
 * rusEfi firmware simulator functional test suite
 * <p/>
 * java -cp rusefi_console.jar com.rusefi.AutoTest
 *
 * @author Andrey Belomutskiy
 *         3/5/14
 */
public class AutoTest {
    private static void mainTestBody() {
        testFordAspire();

        testDodgeNeon();

        sendCommand("set_engine_type 7");
        testFord6();

        sendCommand("set_engine_type 4");
        testFordFiesta();
    }

    private static void testDodgeNeon() {
        sendCommand("set_engine_type 2");
        WaveChart chart;
        IoUtil.changeRpm(2000);
        sendCommand("set_ignition_mode 1");
        chart = nextChart();

        String msg = "Neon";
        float x = 110;
        assertWave(msg, chart, WaveChart.INJECTOR_4, 0.1, x + 540);
        assertWave(msg, chart, WaveChart.INJECTOR_2, 0.1, x);
        assertWave(msg, chart, WaveChart.INJECTOR_1, 0.1, x + 180);
        assertWave(msg, chart, WaveChart.INJECTOR_3, 0.1, x + 360);

        x = 122;
        assertWave(msg, chart, WaveChart.SPARK_4, 0.13333, x + 540);
        assertWave(msg, chart, WaveChart.SPARK_2, 0.13333, x);
        assertWave(msg, chart, WaveChart.SPARK_1, 0.13333, x + 180);
        assertWave(msg, chart, WaveChart.SPARK_3, 0.13333, x + 360);

        // switching to Speed Density
        sendCommand("set_mock_map_voltage 1");
        sendCommand("set_algorithm 3");
        chart = nextChart();
        x = 110;
        assertWave(msg, chart, WaveChart.INJECTOR_4, 0.42966, x + 540);
    }

    private static void testFordFiesta() {
        WaveChart chart;
        IoUtil.changeRpm(2000);
        chart = nextChart();

        String msg = "Fiesta";
        double x = 312;
        assertWave(msg, chart, WaveChart.SPARK_1, 0.1333333, x, x + 360);
        assertWave(msg, chart, WaveChart.SPARK_3, 0.1333333, x + 180, x + 540);
    }

    private static void testFord6() {
        WaveChart chart;
        IoUtil.changeRpm(2000);
        chart = nextChart();

        String msg = "ford 6";

        int x = 7;
        assertWave(msg, chart, WaveChart.SPARK_1, 0.01666, x, x + 120, x + 240, x + 360, x + 480, x + 600);

        assertNull(msg, chart.get(WaveChart.TRIGGER_2));
        sendCommand("set_trigger_type 1"); // TT_FORD_ASPIRE
        chart = nextChart();
        assertTrue(msg, chart.get(WaveChart.TRIGGER_2) != null);

    }

    private static void testFordAspire() {
        String msg = "Aspire";
        WaveChart chart;
        // todo: interesting changeRpm(100);
        sendCommand("set_cranking_rpm 500");
        IoUtil.changeRpm(200);

        double x;
        chart = nextChart();
        x = 55;
        assertWave("aspire default cranking ", chart, WaveChart.SPARK_1, 0.1944, x, x + 180, x + 360, x + 540);


        IoUtil.changeRpm(600);
        chart = nextChart();
        x = 76;
        assertWave("aspire default running ", chart, WaveChart.SPARK_1, 0.04, x, x + 180, x + 360, x + 540);

        IoUtil.changeRpm(200);

        sendCommand("set_cranking_charge_angle 65");
        sendCommand("set_cranking_timing_angle 31");

        chart = nextChart();
        x = 55;
        assertWave("aspire", chart, WaveChart.SPARK_1, 0.18, x, x + 180, x + 360, x + 540);

        sendCommand("set_cranking_timing_angle 40");
        chart = nextChart();
        x = 64;
        assertWave("aspire", chart, WaveChart.SPARK_1, 0.18, x, x + 180, x + 360, x + 540);
        sendCommand("set_cranking_timing_angle -149");

        sendCommand("set_cranking_charge_angle 40");
        chart = nextChart();
        x = 80;
        assertWave("aspire", chart, WaveChart.SPARK_1, 40.0 / 360, x, x + 180, x + 360, x + 540);
        sendCommand("set_cranking_charge_angle 65");

        IoUtil.changeRpm(600);
        sendCommand("set_cranking_rpm 700");
        chart = nextChart();
        x = 55;
        assertWave("cranking@600", chart, WaveChart.SPARK_1, 0.18, x, x + 180, x + 360, x + 540);

        IoUtil.changeRpm(2000);
        sendCommand("set_whole_fuel_map 1.57");

        chart = nextChart();

        assertWave(chart, WaveChart.INJECTOR_1, 0.051, 238.75);
        assertWave(chart, WaveChart.INJECTOR_2, 0.051, 53.04);
        assertWave(chart, WaveChart.INJECTOR_3, 0.051, 417.04);
        assertWave(chart, WaveChart.INJECTOR_4, 0.051, 594.04);

        x = 22;
        assertWave(chart, WaveChart.SPARK_1, 0.133, x, x + 180, x + 360, x + 540);

        sendCommand("set_fuel_map 2200 4 15.66");
        sendCommand("set_fuel_map 2000 4 15.66");
        sendCommand("set_fuel_map 2200 4.2 15.66");
        sendCommand("set_fuel_map 2000 4.2 15.66");
        // mock 2 means 4 on the gauge because of the divider. should we simplify this?
        sendCommand("set_mock_maf_voltage 2");
        sendCommand("set_global_trigger_offset_angle 175");
        chart = nextChart();

        assertWave(chart, WaveChart.INJECTOR_1, 0.522, 238.75);
        assertWave(chart, WaveChart.INJECTOR_2, 0.522, 53.04);
        assertWave(chart, WaveChart.INJECTOR_3, 0.522, 417.04);
        assertWave(chart, WaveChart.INJECTOR_4, 0.522, 594.04);

        x = 41;
        assertWave(chart, WaveChart.SPARK_1, 0.133, x, x + 180, x + 360, x + 540);
        assertNull("chart for " + WaveChart.SPARK_2, chart.get(WaveChart.SPARK_2));

        sendCommand("set_global_trigger_offset_angle 130");
        chart = nextChart();
        x = 580;
        assertWave(chart, WaveChart.SPARK_1, 0.133, x, x + 180, x + 360, x + 540);

        // let's enable more channels dynamically
        sendCommand("set_ignition_mode 1");
        chart = nextChart();
        assertWave(chart, WaveChart.SPARK_2, 0.133, x);
        assertWave(chart, WaveChart.SPARK_3, 0.133, x + 360);

        sendCommand("set_whole_timing_map 520");
        chart = nextChart();
        x = 59;
        assertWave(chart, WaveChart.SPARK_2, 0.133, x);


        // switching to Speed Density
        sendCommand("set_mock_map_voltage 1");
        sendCommand("set_algorithm 3");
        chart = nextChart();
        x = 8.88;
        assertWave(msg, chart, WaveChart.INJECTOR_1, 0.296666, x + 180);
        assertWave(msg, chart, WaveChart.INJECTOR_2, 0.296666, x);
        assertWave(msg, chart, WaveChart.INJECTOR_4, 0.296666, x + 540);

        // above hard limit
        IoUtil.changeRpm(10000);
        chart = nextChart();
        assertNull("hard limit check", chart.get(WaveChart.INJECTOR_1));
    }

    public static void main(String[] args) throws InterruptedException {
        FileLog.SIMULATOR_CONSOLE.start();
        FileLog.MAIN.start();

        boolean failed = false;
        try {
            IoUtil.launchSimulator();
            mainTestBody();
        } catch (Throwable e) {
            e.printStackTrace();
            failed = true;
        } finally {
            ExecHelper.destroy();
        }
        if (failed)
            System.exit(-1);
        FileLog.MAIN.logLine("*******************************************************************************");
        FileLog.MAIN.logLine("************************************  Looks good! *****************************");
        FileLog.MAIN.logLine("*******************************************************************************");
        System.exit(0); // this is a safer method eliminating the issue of non-daemon threads
    }
}
