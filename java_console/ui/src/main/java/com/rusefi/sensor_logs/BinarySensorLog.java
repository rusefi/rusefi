package com.rusefi.sensor_logs;

import com.rusefi.FileLog;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.rusEFIVersion;

import java.io.*;
import java.util.Date;
import java.util.function.Function;

public class BinarySensorLog extends SensorLog {
    private final Function<Sensor, Double> valueProvider;
    private final Sensor[] sensors;
    private DataOutputStream stream;

    private int counter;

    public BinarySensorLog() {
        this(new Function<Sensor, Double>() {
            @Override
            public Double apply(Sensor sensor) {
                return SensorCentral.getInstance().getValue(sensor);
            }
        }, SensorLogger.SENSORS);
    }

    public BinarySensorLog(Function<Sensor, Double> valueProvider, Sensor... sensors) {
        this.valueProvider = valueProvider;
        this.sensors = sensors;
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
                stream = new DataOutputStream(new FileOutputStream(fileName));
            } catch (IOException e) {
                e.printStackTrace();
                stream = null;
            }
            writeHeader();
        }

        if (stream != null) {
            try {
                stream.write(0);
                stream.write(counter++);
                stream.writeShort((int) (System.currentTimeMillis() * 100));

                ByteArrayOutputStream baos = new ByteArrayOutputStream();
                DataOutputStream dos = new DataOutputStream(baos);

                for (Sensor sensor : sensors) {
                    double value = valueProvider.apply(sensor);
                    sensor.writeToLog(dos, value);
                }

                byte[] byteArray = baos.toByteArray();
                byte checkSum = 0;
                for (int i = 0; i < byteArray.length; i++) {
                    checkSum += byteArray[i];
                }
                stream.write(byteArray);
                stream.write(checkSum);

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void writeHeader() {
        try {

            String headerText = "\"rusEFI " + rusEFIVersion.CONSOLE_VERSION + "\"\n" +
                    "\"Capture Date: " + new Date() + "\"\n";

            for (char c : "MLVLG\0".toCharArray()) {
                stream.write(c);
            }

            int fieldsDataSize = 0;
            for (Sensor sensor : sensors) {
                fieldsDataSize += sensor.getType().getStorageSize();
            }

            // 0006h Format version = 01
            stream.write(0);
            stream.write(1);
            // 0008h Timestamp
            stream.writeInt(0);
            // 000ch
            int offsetToText = Fields.MLQ_HEADER_SIZE + Fields.MLQ_FIELD_HEADER_SIZE * sensors.length;
            stream.writeShort(offsetToText);
            stream.writeShort(0); // reserved?
            // 0010h = offset_to_data
            stream.writeShort(offsetToText + headerText.length());
            // 0012h
            stream.writeShort(fieldsDataSize);
            // 0014h number of fields
            stream.writeShort(sensors.length);

            for (Sensor sensor : sensors) {
                String name = SensorLogger.getSensorName(sensor, 0);
                String unit = sensor.getUnits();

                // 0000h
                stream.write(getSensorTypeValue(sensor));
                // 0001h
                writeLine(stream, name, 34);
                // 0023h
                writeLine(stream, unit, 11);
                // 002Eh scale
                stream.writeFloat(1); // todo: multiplier?
                // 0032h zeroes
                stream.writeInt(0);
                // 0036h precision
                stream.write(2);
            }
            writeLine(stream, headerText, 0);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private int getSensorTypeValue(Sensor sensor) {
        switch (sensor.getType()) {
            case UINT16:
                return 2;
            case INT16:
                return 3;
            case INT:
                return 4;
            case FLOAT:
                return 7;
            default:
                throw new UnsupportedOperationException("" + sensor.getType());
        }
    }

    public void close() {
        close(stream);
    }

    public static void close(Closeable closeable) {
        try {
            closeable.close();
        } catch (IOException e) {
            // ignoring
        }
    }

    private void writeLine(DataOutputStream stream, String name, int length) throws IOException {
        for (int i = 0; i < name.length(); i++) {
            stream.write(name.charAt(i));
        }
        for (int i = name.length(); i < length; i++)
            stream.write(0);
    }
}
