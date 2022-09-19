package com.rusefi.sensor_logs;

import com.opensr5.Logger;
import com.rusefi.FileLog;
import com.rusefi.config.generated.Fields;
import com.rusefi.rusEFIVersion;

import java.io.*;
import java.util.*;
import java.util.function.Function;

/**
 * MLV .mlq binary log file
 * </p>
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class BinarySensorLog<T extends BinaryLogEntry> implements SensorLog {
    private final Function<T, Double> valueProvider;
    private final Collection<T> entries;
    private final TimeProvider timeProvider;
    private DataOutputStream stream;

    private String fileName;

    private int counter;

    public BinarySensorLog(Function<T, Double> valueProvider, Collection<T> sensors) {
        this(valueProvider, sensors, System::currentTimeMillis);
    }

    public BinarySensorLog(Function<T, Double> valueProvider, Collection<T> sensors, TimeProvider timeProvider) {
        this.valueProvider = Objects.requireNonNull(valueProvider, "valueProvider");
        this.entries = Objects.requireNonNull(sensors, "entries");
        this.timeProvider = timeProvider;
    }

    interface TimeProvider {
        long currentTimestamp();
    }

    @Override
    public double getSecondsSinceFileStart() {
        throw new UnsupportedOperationException();
    }

    @Override
    public void writeSensorLogLine() {
        if (stream == null) {
            FileLog.createFolderIfNeeded();
            fileName = Logger.DIR + "rusEFI_gauges_" + FileLog.getDate() + ".mlg";

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
                stream.writeShort((int) (timeProvider.currentTimestamp() * 100));

                ByteArrayOutputStream baos = new ByteArrayOutputStream();
                DataOutputStream dos = new DataOutputStream(baos);

                for (T sensor : entries) {
                    Double value = valueProvider.apply(sensor);
                    if (value == null)
                        throw new NullPointerException("No value for " + sensor);
                    sensor.writeToLog(dos, value);
                }

                byte[] byteArray = baos.toByteArray();
                byte checkSum = 0;
                for (byte b : byteArray) {
                    checkSum += b;
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
        for (BinaryLogEntry entry : entries) {
            fieldsDataSize += entry.getByteSize();
        }

        // 0006h Format version = 01
        stream.write(0);
        stream.write(1);
        // 0008h Timestamp
        stream.writeInt((int) (System.currentTimeMillis() / 1000));
        // 000ch
        int offsetToText = Fields.MLQ_HEADER_SIZE + Fields.MLQ_FIELD_HEADER_SIZE * entries.size();
        stream.writeShort(offsetToText);
        stream.writeShort(0); // reserved?
        // 0010h = offset_to_data
        stream.writeShort(offsetToText + headerText.length());
        // 0012h
        stream.writeShort(fieldsDataSize);
        // 0014h number of fields
        stream.writeShort(entries.size());

        for (BinaryLogEntry sensor : entries) {
            String name = sensor.getName();
            String unit = sensor.getUnit();

            // 0000h
            stream.write(sensor.getByteSize());
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
