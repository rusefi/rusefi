package com.rusefi.sensor_logs;

import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;

import java.util.Arrays;
import java.util.List;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 4/15/2016.
 */
public class SensorLogger {
    protected static Sensor[] SENSORS = {Sensor.RPM,
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
            Sensor.tCharge,
            Sensor.veValue,

            Sensor.DWELL,

            Sensor.baseFuel,
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

            Sensor.engineMakeCodeNameCrc16,
            Sensor.tuneCrc16,
    };

    private static List<SensorLog> sensorLogs = Arrays.asList(new PlainTextSensorLog(), new BinarySensorLog());

    private static boolean isInitialized;

    private SensorLogger() {
    }

    static {
        init();
    }

    public synchronized static void init() {
        if (isInitialized) {
            return;
        }
        isInitialized = true;
        SensorCentral.getInstance().addListener(Sensor.TIME_SECONDS, new SensorCentral.SensorListener() {
            @Override
            public void onSensorUpdate(double value) {
                if (ConnectionStatusLogic.INSTANCE.getValue() != ConnectionStatusValue.CONNECTED)
                    return;
                for (SensorLog sensorLog : sensorLogs)
                    sensorLog.writeSensorLogLine();
            }
        });
    }

    public static double getSecondsSinceFileStart() {
        return sensorLogs.get(0).getSecondsSinceFileStart();
    }

    static String getSensorName(Sensor sensor, int debugMode) {
        if (sensor == Sensor.debugFloatField1 && isPidDebugMode(debugMode)) {
            return "PID: output";
        }
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
        if (sensor == Sensor.debugIntField5 && isPidDebugMode(debugMode)) {
            return "PID: feed forward";
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
