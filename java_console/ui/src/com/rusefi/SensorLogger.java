package com.rusefi;

import com.opensr5.ConfigurationImage;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolHolder;
import com.rusefi.config.Fields;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.ConnectionStatus;
import com.rusefi.ui.config.ConfigField;

import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;

/**
 * (c) Andrey Belomutskiy 2013-2018
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
            Sensor.debugIntField4,
            Sensor.debugIntField5,

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

        SensorCentral.getInstance().addListener(Sensor.TIME_SECONDS, new SensorCentral.SensorListener() {
            @Override
            public void onSensorUpdate(double value) {
                if (ConnectionStatus.INSTANCE.getValue() != ConnectionStatus.Value.CONNECTED)
                    return;
                if (logFile == null) {
                    /*
                     * we only start file header once we have first bunch of data
                     */
                    startSensorLogFile();
                }

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

            int debugMode = -1;
            BinaryProtocol bp = BinaryProtocolHolder.getInstance().get();
            if (bp != null) {
                ConfigurationImage ci = bp.getController();
                if (ci != null) {
                    debugMode = ConfigField.getIntValue(ci, Fields.DEBUGMODE);
                }
            }
            System.out.println("debug mode " + debugMode);
            logFile.write("Time\t");
            for (Sensor sensor : SENSORS) {
                logFile.write(getSensorName(sensor, debugMode) + "\t");
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

    private static String getSensorName(Sensor sensor, int debugMode) {
        if (sensor == Sensor.debugFloatField2 && isPidDebugMode(debugMode)) {
            return "PID: I-term";
        }
        if (sensor == Sensor.debugFloatField3 && isPidDebugMode(debugMode)) {
            return "PID: prevError";
        }
        if (sensor == Sensor.debugFloatField4 && isPidDebugMode(debugMode)) {
            return "PID: I setting";
        }
        if (sensor == Sensor.debugFloatField5 && isPidDebugMode(debugMode)) {
            return "PID: D setting";
        }
        if (sensor == Sensor.debugFloatField6 && isPidDebugMode(debugMode)) {
            return "PID: D-term";
        }
        if (sensor == Sensor.debugIntField1 && isPidDebugMode(debugMode)) {
            return "PID: P setting";
        }
        if (sensor == Sensor.debugIntField2 && isPidDebugMode(debugMode)) {
            return "PID: offset";
        }
        if (sensor == Sensor.debugIntField3 && isPidDebugMode(debugMode)) {
            return "PID: counter";
        }
        if (sensor == Sensor.debugIntField4 && isPidDebugMode(debugMode)) {
            return "PID: period";
        }
        return sensor.getName();
    }

    private static boolean isPidDebugMode(int debugMode) {
        // nasty implementation hard-coded debug_mode_e values
        return debugMode == 0
                || debugMode == 3
                || debugMode == 7
                || debugMode == 11
                || debugMode == 17;
    }
}
