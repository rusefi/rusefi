package com.rusefi.sensor_logs;

import com.devexperts.logging.FileLogger;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.Launcher;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.UIContext;

import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.Map;

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

            BinaryProtocol bp = uiContext.getBinaryProtocol();
            Map<String, GaugeModel> gauges = null;
            if (bp != null) {
                IniFileModel iniFile = bp.getIniFileNullable();
                if (iniFile != null) {
                    gauges = iniFile.getGauges();
                }
            }

            logFile.write("Time\t");
            for (BinaryLogEntry sensor : SensorLogger.getSensors(uiContext)) {
                String name = sensor.getName();
                if (gauges != null) {
                    GaugeModel gm = gauges.get(sensor.getName());
                    if (gm != null) {
                        name = gm.getTitle();
                    }
                }
                logFile.write(name + "\t");
            }
            logFile.write("\r\n");

            logFile.write("Time\t");
            for (BinaryLogEntry sensor : SensorLogger.getSensors(uiContext)) {
                String units = sensor.getUnit();
                if (gauges != null) {
                    GaugeModel gm = gauges.get(sensor.getName());
                    if (gm != null) {
                        units = gm.getUnits();
                    }
                }
                logFile.write(units + "\t");
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
            byte[] response = SensorCentral.getInstance().getResponse();
            for (BinaryLogEntry sensor : SensorLogger.getSensors(uiContext)) {
                double value;
                if (sensor instanceof Sensor) {
                    value = SensorCentral.getInstance().getValue((Sensor) sensor);
                    value = ((Sensor) sensor).translateValue(value);
                } else if (sensor instanceof CustomBinaryLogEntry && response != null) {
                    value = ((CustomBinaryLogEntry) sensor).getValue(response);
                } else {
                    value = 0;
                }
                logFile.write(value + "\t");
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

