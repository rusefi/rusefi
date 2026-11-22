package com.rusefi.sensor_logs;

import com.devexperts.logging.FileLogger;
import com.rusefi.UiVersion;
import com.rusefi.config.generated.Integration;

import java.io.*;
import java.util.*;
import java.util.function.Function;

import static com.rusefi.config.generated.VariableRegistryValues.GAUGE_NAME_TIME;

/**
 * MLV .mlg binary log file
 * <p>
 * [tag:mlg_time_field] The first field is always "Time", seconds since the first row, as in TunerStudio
 * and SD card logs. The block timestamps alone are not enough: rusEFI Online (msqur) and other MLG
 * readers look the time up by field name and refuse a log without it.
 * </p>
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class BinarySensorLog<T extends BinaryLogEntry> {
    private final Function<T, Double> valueProvider;
    private final Collection<T> entries;
    private final TimeProvider timeProvider;
    private final String requestedFileName;
    private final byte[] tune;
    private static final int MLG_TYPE_FLOAT = 7;
    private static final int TIME_DATA_SIZE = 4;
    private DataOutputStream stream;

    private String fileName;

    private int counter;
    /** false when the caller's own entries already include a Time field */
    private final boolean writeTime;
    private long firstRowTimestamp = -1;

    public BinarySensorLog(Function<T, Double> valueProvider, Collection<T> sensors) {
        this(valueProvider, sensors, System::currentTimeMillis, null);
    }

    public BinarySensorLog(Function<T, Double> valueProvider, Collection<T> sensors, TimeProvider timeProvider) {
        this(valueProvider, sensors, timeProvider, null);
    }

    BinarySensorLog(Function<T, Double> valueProvider, Collection<T> sensors, TimeProvider timeProvider, String fileName) {
        this(valueProvider, sensors, timeProvider, fileName, null);
    }

    BinarySensorLog(Function<T, Double> valueProvider, Collection<T> sensors, TimeProvider timeProvider,
                    String fileName, byte[] tune) {
        this.valueProvider = Objects.requireNonNull(valueProvider, "valueProvider");
        this.entries = Objects.requireNonNull(sensors, "entries");
        this.timeProvider = timeProvider;
        this.requestedFileName = fileName;
        this.tune = tune == null ? null : tune.clone();
        boolean hasTime = false;
        for (T entry : sensors) {
            hasTime |= GAUGE_NAME_TIME.equalsIgnoreCase(entry.getName());
        }
        writeTime = !hasTime;
    }

    /** Eager writer for callers that own the stream and need to report I/O failures. */
    public BinarySensorLog(Function<T, Double> valueProvider, Collection<T> sensors, OutputStream output)
            throws IOException {
        this(valueProvider, sensors, output, null);
    }

    public BinarySensorLog(Function<T, Double> valueProvider, Collection<T> sensors, OutputStream output, byte[] tune)
            throws IOException {
        this(valueProvider, sensors, System::currentTimeMillis, null, tune);
        stream = new DataOutputStream(Objects.requireNonNull(output, "output"));
        writeHeader();
    }

    interface TimeProvider {
        long currentTimestamp();
    }

    public void writeSensorLogLine() {
        if (stream == null) {
            FileLogger.createFolderIfNeeded();
            fileName = requestedFileName != null
                    ? requestedFileName
                    : FileLogger.DIR + "rusEFI_gauges_" + FileLogger.getDate() + ".mlg";

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
                writeSensorLogLineChecked();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    /** Write one row to an already opened stream, propagating failures to the caller. */
    public void writeSensorLogLineChecked() throws IOException {
        if (stream == null) {
            throw new IOException("Data log is not open");
        }
        stream.write(0);
        stream.write(counter++);
        long now = timeProvider.currentTimestamp();
        stream.writeShort((int) (now * 100));

        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(baos);
        if (writeTime) {
            if (firstRowTimestamp < 0) {
                firstRowTimestamp = now;
            }
            dos.writeFloat((now - firstRowTimestamp) / 1000f);
        }

        for (T sensor : entries) {
            Double value = valueProvider.apply(sensor);
            if (value == null) {
                throw new NullPointerException("No value for " + sensor);
            }
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
    }

    private void writeHeader() throws IOException {
        String headerText = "\"rusEFI " + UiVersion.CONSOLE_VERSION + "\"\n" +
                "\"Capture Date: " + new Date() + "\"\n";
        byte[] info = MlgTune.informationBlock(headerText, tune);

        for (char c : "MLVLG\0".toCharArray()) {
            stream.write(c);
        }

        int fieldCount = entries.size() + (writeTime ? 1 : 0);
        int fieldsDataSize = writeTime ? TIME_DATA_SIZE : 0;
        for (BinaryLogEntry entry : entries) {
            fieldsDataSize += entry.getDataSize();
        }

        // 0006h Format version = 02
        stream.write(0);
        stream.write(2);

        // 0008h Timestamp
        stream.writeInt((int) (System.currentTimeMillis() / 1000));

        int headerSize = Integration.MLQ_HEADER_SIZE + Integration.MLQ_FIELD_HEADER_SIZE * fieldCount;
        // 000ch Info data start - immediately after header
        stream.writeInt(headerSize);

        // 0010h Data begin index - begins immediately after the header text
        int headerWithTextSize = headerSize + info.length;
        stream.writeInt(headerWithTextSize);

        // 0014h Record length
        stream.writeShort(fieldsDataSize);

        // 0016h Number of fields
        stream.writeShort(fieldCount);

        if (writeTime) {
            writeField(MLG_TYPE_FLOAT, GAUGE_NAME_TIME, "s", 1, 3, "");
        }
        for (BinaryLogEntry sensor : entries) {
            writeField(sensor.getByteSize(), sensor.getName(), sensor.getUnit(), sensor.getScale(), 2,
                    sensor.getCategory().getName());
        }

        if (stream.size() != headerSize) {
            throw new IllegalStateException("We are doing something wrong :( stream.size=" + stream.size() + " vs " + headerSize);
        }
        stream.write(info);
    }

    private void writeField(int type, String name, String unit, float scale, int precision, String category)
            throws IOException {
        // 0000h
        stream.write(type);
        // 0001h
        writeLine(stream, name, 34);
        // 0023h
        writeLine(stream, unit, 11);
        // 002Eh scale
        stream.writeFloat(scale);
        // 0032h zeroes
        stream.writeInt(0);
        // 0036h precision
        stream.write(precision);
        // 0037h category string
        writeLine(stream, category, 34);
    }

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
