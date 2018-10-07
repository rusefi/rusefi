package com.rusefi;

import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;

import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;

/**
 * (c) Andrey Belomutskiy 2013-2017
 * 4/15/2016.
 */
public class SensorLogger {
    private static Writer logFile;

    private static Sensor[] SENSORS = {Sensor.RPM,
            Sensor.INT_TEMP,

            Sensor.engineMode,
            Sensor.CLT,
            Sensor.TPS,
            Sensor.VBATT,
            Sensor.IAT,
            Sensor.MAF,
            Sensor.MAP,
            Sensor.AFR,

            Sensor.PPS,
            Sensor.ETB_CONTROL_QUALITY,

            Sensor.idlePosition,

            Sensor.TARGET_AFR,
            Sensor.T_CHARGE,
            Sensor.CURRENT_VE,
            Sensor.ENGINE_LOAD,

            Sensor.DWELL,
            Sensor.TIMING,

            Sensor.FUEL_BASE,
            Sensor.actualLastInjection,
            Sensor.ignitionAdvance,

            Sensor.deltaTps,
            Sensor.tpsAccelFuel,
            Sensor.engineLoadAccelDelta,
            Sensor.injectorDutyCycle,
            Sensor.coilDutyCycle,

            Sensor.wallFuelAmount,
            Sensor.wallFuelCorrection,

            Sensor.iatCorrection,
            Sensor.cltCorrection,

            Sensor.CHARGE_AIR_MASS,
            Sensor.runningFuel,
            Sensor.injectorLagMs,

            Sensor.vvtPosition,
            Sensor.VSS,
            Sensor.SPEED2RPM,

            Sensor.debugFloatField1,
            Sensor.debugFloatField2,
            Sensor.debugFloatField3,
            Sensor.debugFloatField4,
            Sensor.debugFloatField5,
            Sensor.debugFloatField6,
            Sensor.debugFloatField7,
            Sensor.debugIntField1,
            Sensor.debugIntField2,
            Sensor.debugIntField3,

            Sensor.errorCodeCounter,
            Sensor.lastErrorCode,

    };
    private static long fileStartTime;

    private SensorLogger() {
    }

    private static boolean isRunning;

    public static void init() {
        if (isRunning)
            return;
        isRunning = true;

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

            logFile.write("\"rusEfi console" + Launcher.CONSOLE_VERSION + " firmware " + Launcher.firmwareVersion.get() + "\"\r\n");
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
