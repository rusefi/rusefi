package com.rusefi;


import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.waves.EngineChart;
import com.rusefi.waves.EngineReport;

import static com.rusefi.TestingUtils.nextChart;
import static com.rusefi.IoUtil.sendCommand;
import static com.rusefi.IoUtil.sleep;
import static com.rusefi.TestingUtils.*;
import static com.rusefi.waves.EngineReport.isCloseEnough;

/**
 * rusEfi firmware simulator functional test suite
 * <p/>
 * java -cp rusefi_console.jar com.rusefi.AutoTest
 *
 * @author Andrey Belomutskiy
 *         3/5/14
 */
public class AutoTest {
    public static final int COMPLEX_COMMAND_RETRY = 10000;
    static int currentEngineType;

    static void mainTestBody() {
        sendCommand("fl 1"); // just in case it was disabled
        testBmwE34();
        testSachs();
        testMitsu();
        testCitroenBerlingo();
        testMazda626();
        test2003DodgeNeon();
        testFordAspire();
        testMazdaProtege();
        test1995DodgeNeon();
        testFord6();
        testFordFiesta();
    }

    private static void testSachs() {
        setEngineType(29);
        String msg = "BMW";
        IoUtil.changeRpm(1200);
        // todo: add more content
    }

    private static void testBmwE34() {
        setEngineType(25);
        sendCommand("chart 1");
        String msg = "BMW";
        EngineChart chart;
        IoUtil.changeRpm(200);
        chart = nextChart();
        double x = 173.988;
        // something is wrong here - it's a 6 cylinder here, why 4 cylinder cycle?
        assertWave(msg, chart, EngineChart.SPARK_1, 0.0199666, x, x + 180, x + 360, x + 540);

        IoUtil.changeRpm(1200);
        chart = nextChart();

        x = 688.464;
        // something is wrong here - it's a 6 cylinder here, why 4 cylinder cycle?
        assertWave(msg, chart, EngineChart.SPARK_1, 0.0597999999, x, x + 180, x + 360, x + 540);

        x = 101;
        // 6 cylinder
        assertWave(msg, chart, EngineChart.MAP_AVERAGING, 0.139, x, x + 120, x + 240, x + 360, x + 480, x + 600);
    }

    private static void testMitsu() {
        setEngineType(16);
        String msg = "Mitsubishi";
        IoUtil.changeRpm(200);

        IoUtil.changeRpm(1200);
        // todo: add more content
    }

    private static void testCitroenBerlingo() {
        setEngineType(15);
        String msg = "Citroen";
        IoUtil.changeRpm(1200);
        // todo: add more content
    }

    static void setEngineType(int type) {
        currentEngineType = type;
        sendCommand("set_engine_type " + type, COMPLEX_COMMAND_RETRY, 30);
        sleep(10);
        sendCommand("enable self_stimulation");
    }

    private static void testMazda626() {
        setEngineType(28);
        String msg = "mazda 626 default cranking";
        IoUtil.changeRpm(200);
        EngineChart chart;
        chart = nextChart();

        double x = 102;
        assertWave(msg, chart, EngineChart.SPARK_1, 0.1944, x, x + 180, x + 360, x + 540);
    }

    private static void test2003DodgeNeon() {
        setEngineType(23);
        sendCommand("set suckedOffCoef 0");
        sendCommand("set addedToWallCoef 0");
        if (!TestingUtils.isRealHardware)
            sendCommand("set_mock_map_voltage 1");
        EngineChart chart;
        String msg = "2003 Neon cranking ";
        IoUtil.changeRpm(200);

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
        assertWave(true, msg, chart, EngineChart.INJECTOR_1, 0.006266666666666905, 0.02, widthRatio, x, x + 180, x + 360, x + 540);
        assertWave(true, msg, chart, EngineChart.INJECTOR_2, 0.006266666666666905, 0.02, widthRatio, x, x + 180, x + 360, x + 540);
        assertWave(true, msg, chart, EngineChart.INJECTOR_3, 0.006266666666666905, 0.02, widthRatio, x, x + 180, x + 360, x + 540);
        assertWave(true, msg, chart, EngineChart.INJECTOR_4, 0.006266666666666905, 0.02, widthRatio, x, x + 180, x + 360, x + 540);

        msg = "2003 Neon running";
        IoUtil.changeRpm(2000);
        chart = nextChart();
        x = 101.0;
        assertWave(true, msg, chart, EngineChart.SPARK_1, 0.13299999999999998, 0.02, EngineReport.RATIO, x + 180, x + 540);
        assertWaveNull(msg, chart, EngineChart.SPARK_2);
        assertWave(true, msg, chart, EngineChart.SPARK_3, 0.13299999999999998, 0.02, EngineReport.RATIO, x, x + 360);
        assertWaveNull(msg, chart, EngineChart.SPARK_4);

        x = 106.0;
        assertWave(true, msg, chart, EngineChart.INJECTOR_1, 0.208666666666667, 0.04, 0.2, x + 360);
        assertWave(true, msg, chart, EngineChart.INJECTOR_2, 0.208666666666667, 0.04, 0.2, x + 180);
        assertWave(true, msg, chart, EngineChart.INJECTOR_3, 0.208666666666667, 0.04, 0.2, x + 540);
        assertWave(true, msg, chart, EngineChart.INJECTOR_4, 0.208666666666667, 0.04, 0.2, x);

        sendCommand("enable trigger_only_front");
        chart = nextChart();
        assertWave(true, msg, chart, EngineChart.INJECTOR_1, 0.208666666666667, 0.04, 0.2, x + 360);
        assertWave(true, msg, chart, EngineChart.INJECTOR_2, 0.208666666666667, 0.04, 0.2, x + 180);
        assertWave(true, msg, chart, EngineChart.INJECTOR_3, 0.208666666666667, 0.04, 0.2, x + 540);
        assertWave(true, msg, chart, EngineChart.INJECTOR_4, 0.208666666666667, 0.04, 0.2, x);
    }

    private static void testMazdaProtege() {
        setEngineType(14);
        EngineChart chart;
        IoUtil.changeRpm(200);
        String msg = "ProtegeLX cranking";
        chart = nextChart();
        double x = 107;
        assertWave(msg, chart, EngineChart.SPARK_1, 0.194433, x, x + 180, x + 360, x + 540);
        x = 0;
        assertWaveFall(msg, chart, EngineChart.INJECTOR_1, 0.004666666666, x, x + 180, x + 360, x + 540);
        assertWaveFall(msg, chart, EngineChart.INJECTOR_2, 0.004666666666, x, x + 180, x + 360, x + 540);

        msg = "ProtegeLX running";
        IoUtil.changeRpm(2000);
        chart = nextChart();
        x = 127.92;
        assertWave(msg, chart, EngineChart.SPARK_1, 0.13333333333333333, x, x + 180, x + 360, x + 540);
        x = 0;
        assertWaveFall(msg, chart, EngineChart.INJECTOR_1, 0.036999999999999825, x + 180, x + 540);
        assertWaveFall(msg, chart, EngineChart.INJECTOR_2, 0.036999999999999825, x, x + 360);
    }

    private static void test1995DodgeNeon() {
        setEngineType(2);
        EngineChart chart;
        sendComplexCommand("set_whole_fuel_map 3");
        sendComplexCommand("set_individual_coils_ignition");
        /**
         * note that command order matters - RPM change resets wave chart
         */
        IoUtil.changeRpm(2000);
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
        if (!TestingUtils.isRealHardware)
            sendCommand("set_mock_map_voltage 1");
        sendComplexCommand("set_algorithm 3");
        chart = nextChart();
        x = -70;
        assertWaveFall(msg, chart, EngineChart.INJECTOR_4, 0.463, x + 540);
    }

    private static void testFordFiesta() {
        setEngineType(4);
        EngineChart chart;
        IoUtil.changeRpm(2000);
        chart = nextChart();

        String msg = "Fiesta";
        double x = 312;
        assertWave("wasted spark #1 with Fiesta", chart, EngineChart.SPARK_1, 0.1333333, x, x + 360);
        assertWaveNull(chart, EngineChart.SPARK_2);
        assertWave("wasted spark #3 with Fiesta", chart, EngineChart.SPARK_3, 0.1333333, x + 180, x + 540);
        assertWaveNull(msg, chart, EngineChart.SPARK_4);
    }

    private static void testFord6() {
        setEngineType(7);
        EngineChart chart;
        IoUtil.changeRpm(2000);
        chart = nextChart();

        String msg = "ford 6";

        double x = 7;
        assertWave(msg, chart, EngineChart.SPARK_1, 0.01666, x, x + 120, x + 240, x + 360, x + 480, x + 600);

        assertWaveNull(msg, chart, EngineChart.TRIGGER_2);
        sendComplexCommand("set_trigger_type 1"); // TT_FORD_ASPIRE
        chart = nextChart();
        assertTrue(msg, chart.get(EngineChart.TRIGGER_2) != null);
    }

    private static void testFordAspire() {
        setEngineType(3);
        String msg;
        EngineChart chart;
        // todo: interesting changeRpm(100);
        sendComplexCommand("set_cranking_rpm 500");
        IoUtil.changeRpm(200);

        double x;
        chart = nextChart();
        x = 55;
        assertWave("aspire default cranking ", chart, EngineChart.SPARK_1, 0.1944, x, x + 180, x + 360, x + 540);


        IoUtil.changeRpm(600);
        chart = nextChart();
        x = 76;
        assertWave("aspire default running ", chart, EngineChart.SPARK_1, 0.04, x, x + 180, x + 360, x + 540);

        IoUtil.changeRpm(200);

        sendCommand("set_cranking_charge_angle 65");
        sendCommand("set_cranking_timing_angle -31");

        chart = nextChart();
        x = 55;
        assertWave("aspire cranking", chart, EngineChart.SPARK_1, 0.18, x, x + 180, x + 360, x + 540);

        sendCommand("set_cranking_timing_angle -40");
        chart = nextChart();
        x = 64;
        assertWave("aspire", chart, EngineChart.SPARK_1, 0.18, x, x + 180, x + 360, x + 540);
        sendCommand("set_cranking_timing_angle 149");

        sendCommand("set_cranking_charge_angle 40");
        chart = nextChart();
        x = 80;
        assertWave("aspire", chart, EngineChart.SPARK_1, 40.0 / 360, x, x + 180, x + 360, x + 540);
        sendCommand("set_cranking_charge_angle 65");

        IoUtil.changeRpm(600);
        sendComplexCommand("set_cranking_rpm 700");
        chart = nextChart();
        x = 55;
        assertWave("cranking@600", chart, EngineChart.SPARK_1, 0.18, x, x + 180, x + 360, x + 540);

        IoUtil.changeRpm(2000);
        sendCommand("set_whole_fuel_map 1.57");

        chart = nextChart();

        msg = "aspire running";

        assertWaveFall(msg, chart, EngineChart.INJECTOR_1, 0.086, 238.75);
        assertWaveFall(msg, chart, EngineChart.INJECTOR_2, 0.086, 53.04);
        assertWaveFall(msg, chart, EngineChart.INJECTOR_3, 0.086, 417.04);
        assertWaveFall(msg, chart, EngineChart.INJECTOR_4, 0.086, 594.04);

        x = 16.44;
        assertWave(chart, EngineChart.SPARK_1, 0.133, x, x + 180, x + 360, x + 540);

        sendCommand("set_fuel_map 2200 4 15.66");
        sendCommand("set_fuel_map 2000 4 15.66");
        sendCommand("set_fuel_map 2200 4.2 15.66");
        sendCommand("set_fuel_map 2000 4.2 15.66");
        // mock 2 means 4 on the gauge because of the divider. should we simplify this?
        if (!TestingUtils.isRealHardware)
            sendCommand("set_mock_maf_voltage 2");
        sendComplexCommand("set_global_trigger_offset_angle 175");
        chart = nextChart();

        assertWaveFall(msg + " fuel", chart, EngineChart.INJECTOR_1, 0.555, 238.75);
        assertWaveFall(msg + " fuel", chart, EngineChart.INJECTOR_2, 0.555, 53.04);
        assertWaveFall(msg + " fuel", chart, EngineChart.INJECTOR_3, 0.555, 417.04);
        assertWaveFall(msg + " fuel", chart, EngineChart.INJECTOR_4, 0.555, 594.04);

        x = 39.12;
        assertWave(chart, EngineChart.SPARK_1, 0.133, x, x + 180, x + 360, x + 540);
        assertWaveNull(chart, EngineChart.SPARK_2);

        sendComplexCommand("set_global_trigger_offset_angle 130");
        sendComplexCommand("set_injection_offset 369");
        chart = nextChart();
        x = 39.12;
        assertWave(chart, EngineChart.SPARK_1, 0.133, x, x + 180, x + 360, x + 540);

        // let's enable more channels dynamically
        sendComplexCommand("set_individual_coils_ignition");
        chart = nextChart();
        assertWave("Switching Aspire into INDIVIDUAL_COILS mode", chart, EngineChart.SPARK_2, 0.133, x + 540);
        assertWave(chart, EngineChart.SPARK_3, 0.133, x + 180);

        sendCommand("set_whole_timing_map 520");
        chart = nextChart();
        x = 58.92;
        assertWave(chart, EngineChart.SPARK_2, 0.133, x);

        // switching to Speed Density
        if (!TestingUtils.isRealHardware)
            sendCommand("set_mock_maf_voltage 2");
        if (!TestingUtils.isRealHardware)
            sendCommand("set_mock_map_voltage 1");
        sendComplexCommand("set_algorithm 3");
        nextChart();
        chart = nextChart();
        assertEquals(1, SensorCentral.getInstance().getValue(Sensor.MAP));
        //assertEquals(1, SensorCentral.getInstance().getValue(Sensor.));
        x = 8.88;
        assertWaveFall(msg + " fuel SD #1", chart, EngineChart.INJECTOR_1, 0.329, x + 180);
        assertWaveFall(msg + " fuel SD #2", chart, EngineChart.INJECTOR_2, 0.329, x);
        assertWaveFall(msg + " fuel SD #3", chart, EngineChart.INJECTOR_3, 0.329, x + 360);
        assertWaveFall(msg + " fuel SD #4", chart, EngineChart.INJECTOR_4, 0.329, x + 540);

        // above hard limit
        IoUtil.changeRpm(10000);
        chart = nextChart();
        assertWaveNull("hard limit check", chart, EngineChart.INJECTOR_1);
    }

    private static void assertEquals(double expected, double actual) {
        assertEquals("", expected, actual);
    }

    private static void assertEquals(String msg, double expected, double actual) {
        if (isCloseEnough(expected, actual))
            throw new IllegalStateException(msg + " Expected " + expected + " but got " + actual);
    }

    private static void sendComplexCommand(String command) {
        sendCommand(command, COMPLEX_COMMAND_RETRY, Timeouts.CMD_TIMEOUT);
    }

    private static void assertWaveNull(EngineChart chart, String key) {
        assertWaveNull("", chart, key);
    }

    private static void assertWaveNull(String msg, EngineChart chart, String key) {
        assertNull(msg + "chart for " + key, chart.get(key));
    }

    public static void main(String[] args) throws InterruptedException {
        Thread.setDefaultUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
            @Override
            public void uncaughtException(Thread t, Throwable e) {
                e.printStackTrace();
                System.exit(-1);
            }
        });

        long start = System.currentTimeMillis();
        FileLog.SIMULATOR_CONSOLE.start();
        FileLog.MAIN.start();

        boolean failed = false;
        try {
            IoUtil.launchSimulator(true);
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
