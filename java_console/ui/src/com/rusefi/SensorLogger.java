package com.rusefi;

import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;

/**
 * (c) Andrey Belomutskiy
 * 4/15/2016.
 */
public class SensorLogger {
    private static Writer logFile;

    private static Sensor[] SENSORS = {Sensor.RPM, Sensor.CLT, Sensor.TPS, Sensor.VBATT};
    private static long fileStartTime;

    private SensorLogger() {
    }

    public static void init() {
        startSensorLogFile();

        SensorCentral.getInstance().addListener(Sensor.TIME_SECONDS, new SensorCentral.SensorListener() {
            @Override
            public void onSensorUpdate(double value) {
                writeSensorLogLine();
            }
        });

    }

    private static void writeSensorLogLine() {
        if (logFile == null)
            return;

        long msSinceFileStart = System.currentTimeMillis() - fileStartTime;

        try {
            logFile.write((msSinceFileStart / 1000.0) + "\t");
            for (Sensor sensor : SENSORS) {
                logFile.write(SensorCentral.getInstance().getValue(sensor) + "\t");
            }
            logFile.write("\r\n");
            logFile.flush();
        } catch (IOException e) {
            e.printStackTrace();
            logFile = null;
        }
    }

    private static void startSensorLogFile() {
        String fileName = FileLog.DIR + FileLog.getDate() + ".msl";

        fileStartTime = System.currentTimeMillis();
        try {
            logFile = new FileWriter(fileName);

            logFile.write("\"rusEfi console" + Launcher.CONSOLE_VERSION + "\"\r\n");
            logFile.write("Captured " + FileLog.getDate() + "\r\n");

            logFile.write("Time\t");
            for (Sensor sensor : SENSORS) {
                logFile.write(sensor.name() + "\t");
            }
            logFile.write("\r\n");

            logFile.write("Time\t");
            for (Sensor sensor : SENSORS) {
                logFile.write("v\t");
            }
            logFile.write("\r\n");
            logFile.flush();


        } catch (IOException e) {
            e.printStackTrace();
            logFile = null;
        }
    }
}
