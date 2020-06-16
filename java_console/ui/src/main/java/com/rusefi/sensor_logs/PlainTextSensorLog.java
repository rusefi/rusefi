package com.rusefi.sensor_logs;

import com.opensr5.ConfigurationImage;
import com.rusefi.FileLog;
import com.rusefi.Launcher;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolHolder;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.config.ConfigField;

import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;

/**
 * Legacy human readable CSV log file
 * </p>
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class PlainTextSensorLog implements SensorLog {
    private Writer logFile;

    private long fileStartTime;

    private void startIfNeeded() {
        if (logFile == null) {
            /*
             * we only start file header once we have first bunch of data
             */
            startSensorLogFile();
        }
    }

    private void startSensorLogFile() {
        FileLog.createFolderIfNeeded();
        String fileName = FileLog.DIR + "rusEFI_gauges_" + FileLog.getDate() + ".msl";

        fileStartTime = System.currentTimeMillis();
        try {
            logFile = new FileWriter(fileName);

            logFile.write("\"rusEFI console" + Launcher.CONSOLE_VERSION + " firmware " + Launcher.firmwareVersion.get() + "\"\r\n");
            logFile.write("Captured " + FileLog.getDate() + "\r\n");

            int debugMode = -1;
            BinaryProtocol bp = BinaryProtocolHolder.getInstance().getCurrentStreamState();
            if (bp != null) {
                ConfigurationImage ci = bp.getControllerConfiguration();
                if (ci != null) {
                    debugMode = ConfigField.getIntValue(ci, Fields.DEBUGMODE);
                }
            }
            System.out.println("debug mode " + debugMode);
            logFile.write("Time\t");
            for (Sensor sensor : SensorLogger.SENSORS) {
                logFile.write(SensorLogger.getSensorName(sensor, debugMode) + "\t");
            }
            logFile.write("\r\n");

            logFile.write("Time\t");
            for (Sensor sensor : SensorLogger.SENSORS) {
                logFile.write(sensor.getUnits() + "\t");
            }
            logFile.write("\r\n");
            logFile.flush();


        } catch (IOException e) {
            e.printStackTrace();
            logFile = null;
        }
    }

    @Override
    public double getSecondsSinceFileStart() {
        long msSinceFileStart = System.currentTimeMillis() - fileStartTime;
        return msSinceFileStart / 1000.0;
    }

    @Override
    public void writeSensorLogLine() {
        startIfNeeded();
        if (logFile == null)
            return;

        try {
            logFile.write(getSecondsSinceFileStart() + "\t");
            for (Sensor sensor : SensorLogger.SENSORS) {
                logFile.write(SensorCentral.getInstance().getValue(sensor) + "\t");
            }
            logFile.write("\r\n");
            logFile.flush();
        } catch (IOException e) {
            e.printStackTrace();
            logFile = null;
        }
    }

    @Override
    public void close() {
        BinarySensorLog.close(logFile);
        logFile = null;
    }
}

