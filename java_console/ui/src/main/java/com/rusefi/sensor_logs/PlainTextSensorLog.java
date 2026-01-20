package com.rusefi.sensor_logs;

import com.devexperts.logging.FileLogger;
import com.rusefi.Launcher;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.UIContext;

import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;

/**
 * Legacy human readable CSV log file
 * </p>
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class PlainTextSensorLog implements SensorLog {
    private final UIContext uiContext;
    private Writer logFile;

    private long fileStartTime;

    public PlainTextSensorLog(UIContext uiContext) {
        this.uiContext = uiContext;
    }

    private void startIfNeeded() {
        if (logFile == null) {
            /*
             * we only start file header once we have first bunch of data
             */
            startSensorLogFile();
        }
    }

    private void startSensorLogFile() {
        FileLogger.createFolderIfNeeded();
        String fileName = FileLogger.DIR + "rusEFI_gauges_" + FileLogger.getDate() + ".msl";

        fileStartTime = System.currentTimeMillis();
        try {
            logFile = new FileWriter(fileName);

            logFile.write("\"rusEFI console" + Launcher.CONSOLE_VERSION + " firmware " + Launcher.firmwareVersion.get() + "\"\r\n");
            logFile.write("Captured " + FileLogger.getDate() + "\r\n");

            BinaryProtocol bp = uiContext.getLinkManager().getCurrentStreamState();

            logFile.write("Time\t");
            for (Sensor sensor : SensorLogger.SENSORS) {
                logFile.write(SensorLogger.getSensorName(sensor, -1) + "\t");
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
                logFile.write( sensor.getLogValue(SensorCentral.getInstance().getValue(sensor)) + "\t");
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

