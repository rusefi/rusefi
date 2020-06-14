package com.rusefi.sensor_logs;

import com.rusefi.FileLog;
import com.rusefi.core.Sensor;

import java.io.*;

public class BinarySensorLog extends SensorLog {
    private BufferedWriter stream;

    public static void main(String[] args) {
        BinarySensorLog l = new BinarySensorLog();
        l.writeSensorLogLine();
    }

    @Override
    public double getSecondsSinceFileStart() {
        throw new UnsupportedOperationException();
    }

    @Override
    void writeSensorLogLine() {
        if (stream == null) {
            FileLog.createFolderIfNeeded();
            String fileName = FileLog.DIR + "rusEFI_gauges_" + FileLog.getDate() + ".mlg";

            try {
                stream = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(fileName)));
            } catch (IOException e) {
                e.printStackTrace();
                stream = null;
            }

            try {
                for (char c : "MLVLG\0".toCharArray()) {
                    stream.write(c);
                }
                stream.write(0);
                stream.write(1);


                for (Sensor sensor : SensorLogger.SENSORS) {
                    String name = SensorLogger.getSensorName(sensor, 0);
                    String unit = sensor.getUnits();

                    writeLine(stream, name, 34);
                    writeLine(stream, unit, 11);

                    stream.write(0);
                    stream.write(0);
                    stream.write(0);
                    stream.write(0);


                }

                stream.close();

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void writeLine(BufferedWriter stream, String name, int length) throws IOException {
        for (int i = 0; i < name.length(); i++) {
            stream.write(name.charAt(i));
        }
        for (int i = name.length(); i < length; i++)
            stream.write(0);
    }
}
