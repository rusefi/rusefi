package com.rusefi;

import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;

import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;

/**
 * (c) Andrey Belomutskiy
 * 4/15/2016.
 */
public class SensorLogger {
    private static Writer logFile;

    private static Sensor[] SENSORS = {Sensor.RPM,
            Sensor.INT_TEMP,
            Sensor.CLT, Sensor.TPS, Sensor.VBATT,
            Sensor.FUEL_BASE,
            Sensor.T_CHARGE,
            Sensor.DWELL,
            Sensor.CURRENT_VE,
            Sensor.deltaTps,
            Sensor.engineLoadAccelDelta,
            Sensor.tpsAccelFuel,
            Sensor.Injector_duty,
            Sensor.wallFuelAmount,
            Sensor.iatCorrection,
            Sensor.wallFuelCorrection,
            Sensor.idlePosition,
            Sensor.TARGET_AFR,
            Sensor.CHARGE_AIR_MASS,
            Sensor.cltCorrection,
            Sensor.runningFuel,
            Sensor.injectorLagMs,
            Sensor.VSS,
            Sensor.SPEED2RPM,
            Sensor.debugFloatField1,
            Sensor.debugFloatField2,
            Sensor.debugFloatField3,
            Sensor.debugFloatField4,
            Sensor.debugFloatField5,
            Sensor.debugIntField1,
            Sensor.debugIntField2,
            Sensor.debugIntField3,
            Sensor.warningCounter,
            Sensor.lastErrorCode,

            Sensor.MAF,
            Sensor.MAP,
            Sensor.IAT};
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
