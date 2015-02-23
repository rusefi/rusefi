package com.rusefi;


import com.rusefi.waves.WaveChart;
import com.rusefi.waves.WaveReport;

import static com.rusefi.TestingUtils.nextChart;
import static com.rusefi.IoUtil.sendCommand;
import static com.rusefi.IoUtil.sleep;
import static com.rusefi.TestingUtils.*;

/**
 * rusEfi firmware simulator functional test suite
 * <p/>
 * java -cp rusefi_console.jar com.rusefi.AutoTest
 *
 * @author Andrey Belomutskiy
 *         3/5/14
 */
public class AutoTest {
    static int currentEngineType;

    static void mainTestBody() {
        testCitroenBerlingo();
        testMazda626();
        test2003DodgeNeon();
        testFordAspire();
        testMazdaProtege();
        test1995DodgeNeon();
        testFord6();
        testFordFiesta();
    }

    private static void testCitroenBerlingo() {
        setEngineType(15);
        String msg = "Citroen";
        // todo: add more content
    }

    private static void setEngineType(int type) {
        currentEngineType = type;
        sendCommand("set_engine_type " + type, 10000);
        sleep(5);
        sendCommand("enable self_stimulation");
    }

    private static void testMazda626() {
        setEngineType(28);
        String msg = "mazda 626 default cranking";
        IoUtil.changeRpm(200);
        WaveChart chart;
        chart = nextChart();

        double x = 275;
        assertWave(msg, chart, WaveChart.SPARK_1, 0.1944, x, x + 180, x + 360, x + 540);
    }

    private static void test2003DodgeNeon() {
        setEngineType(23);
        WaveChart chart;
        String msg = "2003 Neon cranking ";
        IoUtil.changeRpm(200);

        chart = nextChart();
        double x = 100;
        assertWave(true, msg, chart, WaveChart.SPARK_1, 0.194433, 0.01, WaveReport.RATIO, x + 180, x + 540);
        assertWaveNull(msg, chart, WaveChart.SPARK_2);
        assertWave(true, msg, chart, WaveChart.SPARK_3, 0.194433, 0.01, WaveReport.RATIO, x, x + 360);
        assertWaveNull(msg, chart, WaveChart.SPARK_4);

        x = 176.856;
        // todo: why is width precision so low here? is that because of loaded Windows with 1ms precision?
        double widthRatio = 0.25;
        assertWave(true, msg, chart, WaveChart.INJECTOR_1, 0.006266666666, 0.01, widthRatio, x, x + 180, x + 360, x + 540);
        assertWave(true, msg, chart, WaveChart.INJECTOR_2, 0.006266666666, 0.01, widthRatio, x, x + 180, x + 360, x + 540);
        assertWave(true, msg, chart, WaveChart.INJECTOR_3, 0.006266666666, 0.01, widthRatio, x, x + 180, x + 360, x + 540);
        assertWave(true, msg, chart, WaveChart.INJECTOR_4, 0.006266666666, 0.01, widthRatio, x, x + 180, x + 360, x + 540);

        msg = "2003 Neon running";
        IoUtil.changeRpm(2000);
        chart = nextChart();
        x = 120;
        assertWave(true, msg, chart, WaveChart.SPARK_1, 0.13299999999999998, 0.005, WaveReport.RATIO, x + 180, x + 540);
        assertWaveNull(msg, chart, WaveChart.SPARK_2);
        assertWave(true, msg, chart, WaveChart.SPARK_3, 0.13299999999999998, 0.005, WaveReport.RATIO, x, x + 360);
        assertWaveNull(msg, chart, WaveChart.SPARK_4);

        x = 168.12;
        assertWave(true, msg, chart, WaveChart.INJECTOR_1, 0.03333333333333302, 0.01, 0.1, x + 360);
        assertWave(true, msg, chart, WaveChart.INJECTOR_2, 0.03333333333333302, 0.01, 0.1, x + 180);
        assertWave(true, msg, chart, WaveChart.INJECTOR_3, 0.03333333333333302, 0.01, 0.1, x + 540);
        assertWave(true, msg, chart, WaveChart.INJECTOR_4, 0.03333333333333302, 0.01, 0.1, x);
    }

    private static void testMazdaProtege() {
        setEngineType(14);
        WaveChart chart;
        IoUtil.changeRpm(200);
        String msg = "ProtegeLX cranking";
        chart = nextChart();
        double x = 107;
        assertWave(msg, chart, WaveChart.SPARK_1, 0.194433, x, x + 180, x + 360, x + 540);
        x = 0;
        assertWaveFall(msg, chart, WaveChart.INJECTOR_1, 0.004666666666, x, x + 180, x + 360, x + 540);
        assertWaveFall(msg, chart, WaveChart.INJECTOR_2, 0.004666666666, x, x + 180, x + 360, x + 540);

        msg = "ProtegeLX running";
        IoUtil.changeRpm(2000);
        chart = nextChart();
        x = 121;
        assertWave(msg, chart, WaveChart.SPARK_1, 0.13333333333333333, x, x + 180, x + 360, x + 540);
        x = 0;
        assertWaveFall(msg, chart, WaveChart.INJECTOR_1, 0.09766666666666689, x + 180, x + 540);
        assertWaveFall(msg, chart, WaveChart.INJECTOR_2, 0.09766666666666689, x, x + 360);
    }

    private static void test1995DodgeNeon() {
        setEngineType(2);
        WaveChart chart;
        sendCommand("set_whole_fuel_map 3");
        sendCommand("set_ignition_mode 1");
        /**
         * note that command order matters - RPM change resets wave chart
         */
        IoUtil.changeRpm(2000);
        chart = nextChart();

        String msg = "1995 Neon";
        float x = -70;
        assertWaveFall(msg, chart, WaveChart.INJECTOR_4, 0.133, x + 540);
        assertWaveFall(msg, chart, WaveChart.INJECTOR_2, 0.133, x + 720);
        assertWaveFall(msg, chart, WaveChart.INJECTOR_1, 0.133, x + 180);
        assertWaveFall(msg, chart, WaveChart.INJECTOR_3, 0.133, x + 360);

        x = 106;
        assertWave(msg, chart, WaveChart.SPARK_4, 0.13333, x + 540);
        assertWave(msg, chart, WaveChart.SPARK_2, 0.13333, x);
        assertWave(msg, chart, WaveChart.SPARK_1, 0.13333, x + 180);
        assertWave(msg, chart, WaveChart.SPARK_3, 0.13333, x + 360);

        // switching to Speed Density
        sendCommand("set_mock_map_voltage 1");
        sendCommand("set_algorithm 3");
        chart = nextChart();
        x = -70;
        assertWaveFall(msg, chart, WaveChart.INJECTOR_4, 0.463, x + 540);
    }

    private static void testFordFiesta() {
        setEngineType(4);
        WaveChart chart;
        IoUtil.changeRpm(2000);
        chart = nextChart();

        String msg = "Fiesta";
        double x = 312;
        assertWave("wasted spark #1 with Fiesta", chart, WaveChart.SPARK_1, 0.1333333, x, x + 360);
        assertWaveNull(chart, WaveChart.SPARK_2);
        assertWave("wasted spark #3 with Fiesta", chart, WaveChart.SPARK_3, 0.1333333, x + 180, x + 540);
        assertWaveNull(msg, chart, WaveChart.SPARK_4);
    }

    private static void testFord6() {
        setEngineType(7);
        WaveChart chart;
        IoUtil.changeRpm(2000);
        chart = nextChart();

        String msg = "ford 6";

        int x = 7;
        assertWave(msg, chart, WaveChart.SPARK_1, 0.01666, x, x + 120, x + 240, x + 360, x + 480, x + 600);

        assertWaveNull(msg, chart, WaveChart.TRIGGER_2);
        sendCommand("set_trigger_type 1"); // TT_FORD_ASPIRE
        chart = nextChart();
        assertTrue(msg, chart.get(WaveChart.TRIGGER_2) != null);
    }

    private static void testFordAspire() {
        setEngineType(3);
        String msg;
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
        sendCommand("set_cranking_timing_angle -31");

        chart = nextChart();
        x = 55;
        assertWave("aspire cranking", chart, WaveChart.SPARK_1, 0.18, x, x + 180, x + 360, x + 540);

        sendCommand("set_cranking_timing_angle -40");
        chart = nextChart();
        x = 64;
        assertWave("aspire", chart, WaveChart.SPARK_1, 0.18, x, x + 180, x + 360, x + 540);
        sendCommand("set_cranking_timing_angle 149");

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

        msg = "aspire running";

        assertWaveFall(msg, chart, WaveChart.INJECTOR_1, 0.086, 238.75);
        assertWaveFall(msg, chart, WaveChart.INJECTOR_2, 0.086, 53.04);
        assertWaveFall(msg, chart, WaveChart.INJECTOR_3, 0.086, 417.04);
        assertWaveFall(msg, chart, WaveChart.INJECTOR_4, 0.086, 594.04);

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

        assertWaveFall(chart, WaveChart.INJECTOR_1, 0.555, 238.75);
        assertWaveFall(chart, WaveChart.INJECTOR_2, 0.555, 53.04);
        assertWaveFall(chart, WaveChart.INJECTOR_3, 0.555, 417.04);
        assertWaveFall(chart, WaveChart.INJECTOR_4, 0.555, 594.04);

        x = 41;
        assertWave(chart, WaveChart.SPARK_1, 0.133, x, x + 180, x + 360, x + 540);
        assertWaveNull(chart, WaveChart.SPARK_2);

        sendCommand("set_global_trigger_offset_angle 130");
        sendCommand("set_injection_offset 369");
        chart = nextChart();
        x = 580;
        assertWave(chart, WaveChart.SPARK_1, 0.133, x, x + 180, x + 360, x + 540);

        // let's enable more channels dynamically
        sendCommand("set_ignition_mode 1");
        chart = nextChart();
        assertWave("Switching Aspire into INDIVIDUAL_COILS mode", chart, WaveChart.SPARK_2, 0.133, x);
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
        assertWaveFall(msg, chart, WaveChart.INJECTOR_1, 0.329, x + 180);
        assertWaveFall(msg, chart, WaveChart.INJECTOR_2, 0.329, x);
        assertWaveFall(msg, chart, WaveChart.INJECTOR_4, 0.329, x + 540);

        // above hard limit
        IoUtil.changeRpm(10000);
        chart = nextChart();
        assertWaveNull("hard limit check", chart, WaveChart.INJECTOR_1);
    }

    private static void assertWaveNull(WaveChart chart, String key) {
        assertWaveNull("", chart, key);
    }

    private static void assertWaveNull(String msg, WaveChart chart, String key) {
        assertNull(msg + "chart for " + key, chart.get(key));
    }

    public static void main(String[] args) throws InterruptedException {
        long start = System.currentTimeMillis();
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
        long time = (System.currentTimeMillis() - start) / 1000;
        FileLog.MAIN.logLine("Done in " + time + "secs");
        System.exit(0); // this is a safer method eliminating the issue of non-daemon threads
    }
}
