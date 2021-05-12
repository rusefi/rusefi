package com.rusefi.sensor_logs;

import com.opensr5.Logger;
import com.rusefi.FileLog;
import com.rusefi.config.FieldType;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.rusEFIVersion;

import java.io.*;
import java.util.*;
import java.util.function.Function;
import java.util.function.Predicate;
import java.util.stream.Collectors;

/**
 * MLV .mlq binary log file
 * </p>
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class BinarySensorLog implements SensorLog {
    private final Function<Sensor, Double> valueProvider;
    private final Collection<Sensor> sensors;
    private DataOutputStream stream;

    private String fileName;

    private int counter;

    public BinarySensorLog() {
        this(sensor -> SensorCentral.getInstance().getValue(sensor), SensorLogger.SENSORS);
    }

    public BinarySensorLog(Function<Sensor, Double> valueProvider, Sensor... sensors) {
        this.valueProvider = valueProvider;
        this.sensors = filterOutSensorsWithoutType(Objects.requireNonNull(sensors, "sensors"));
    }

    private Collection<Sensor> filterOutSensorsWithoutType(Sensor[] sensors) {
        return Arrays.stream(sensors).filter(new Predicate<Sensor>() {
            @Override
            public boolean test(Sensor sensor) {
                return sensor.getType() != null;
            }
        }).collect(Collectors.toCollection(ArrayList::new));
    }

    @Override
    public double getSecondsSinceFileStart() {
        throw new UnsupportedOperationException();
    }

    @Override
    public void writeSensorLogLine() {
        if (stream == null) {
            FileLog.createFolderIfNeeded();
            fileName = Logger.DIR + "rusEFI_gauges_" + Logger.getDate() + ".mlg";

            try {
                stream = new DataOutputStream(new FileOutputStream(fileName));
                writeHeader();
            } catch (Throwable e) {
                e.printStackTrace();
                stream = null;
            }
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

                if (counter % 20 == 0) {
                    // for not flush on each block of data but still flush
                    stream.flush();
                }

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void writeHeader() throws IOException {
        String headerText = "\"rusEFI " + rusEFIVersion.CONSOLE_VERSION + "\"\n" +
                "\"Capture Date: " + new Date() + "\"\n";

        for (char c : "MLVLG\0".toCharArray()) {
            stream.write(c);
        }

        int fieldsDataSize = 0;
        for (Sensor sensor : sensors) {
            FieldType type = sensor.getType();
            if (type == null)
                throw new NullPointerException("No type for " + sensor);
            fieldsDataSize += type.getStorageSize();
        }

        // 0006h Format version = 01
        stream.write(0);
        stream.write(1);
        // 0008h Timestamp
        stream.writeInt((int) (System.currentTimeMillis() / 1000));
        // 000ch
        int offsetToText = Fields.MLQ_HEADER_SIZE + Fields.MLQ_FIELD_HEADER_SIZE * sensors.size();
        stream.writeShort(offsetToText);
        stream.writeShort(0); // reserved?
        // 0010h = offset_to_data
        stream.writeShort(offsetToText + headerText.length());
        // 0012h
        stream.writeShort(fieldsDataSize);
        // 0014h number of fields
        stream.writeShort(sensors.size());

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
        if (stream.size() != offsetToText)
            throw new IllegalStateException("We are doing something wrong :( stream.size=" + stream.size());
        writeLine(stream, headerText, headerText.length());
    }

    private int getSensorTypeValue(Sensor sensor) {
        switch (sensor.getType()) {
            case UINT8:
                return 0;
            case INT8:
                return 1;
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

    @Override
    public void close() {
        close(stream);
        stream = null;
    }

    public static void close(Closeable closeable) {
        try {
            if (closeable != null) {
                closeable.close();
            }
        } catch (IOException e) {
            // ignoring
        }
    }

    public String getFileName() {
        return fileName;
    }

    private void writeLine(DataOutputStream stream, String name, int length) throws IOException {
        for (int i = 0; i < Math.min(name.length(), length); i++) {
            stream.write(name.charAt(i));
        }
        for (int i = name.length(); i < length; i++)
            stream.write(0);
    }
}
