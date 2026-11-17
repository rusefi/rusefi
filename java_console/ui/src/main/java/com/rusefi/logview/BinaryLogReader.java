package com.rusefi.logview;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

import static com.rusefi.config.generated.VariableRegistryValues.GAUGE_NAME_TIME;

/**
 * Reader for version 2 binary operating logs. ECU-written logs (firmware binary_mlg_logging.cpp) carry a
 * Time field; console-recorded logs (BinarySensorLog) do not, so their time comes from the data blocks'
 * 16-bit timestamps, as MegaLogViewer reads it.
 */
public final class BinaryLogReader {
    private static final int HEADER_SIZE = 24;
    private static final int FIELD_SIZE = 89;
    /** Data block timestamps count 10 us steps. */
    static final double SECONDS_PER_TIMESTAMP_STEP = 1e-5;

    private BinaryLogReader() {
    }

    public static CsvLogReader read(InputStream source) throws IOException {
        DataInputStream in = new DataInputStream(source);
        byte[] header = new byte[HEADER_SIZE];
        in.readFully(header);
        ByteBuffer metadata = ByteBuffer.wrap(header);
        if (!"MLVLG\0".equals(new String(header, 0, 6, StandardCharsets.US_ASCII))
                || metadata.getShort(6) != 2) {
            throw new IOException("Only MLVLG version 2 logs are supported");
        }

        long infoBegin = Integer.toUnsignedLong(metadata.getInt(12));
        long dataBegin = Integer.toUnsignedLong(metadata.getInt(16));
        int recordLength = Short.toUnsignedInt(metadata.getShort(20));
        int fieldCount = Short.toUnsignedInt(metadata.getShort(22));
        long descriptorEnd = HEADER_SIZE + (long) FIELD_SIZE * fieldCount;
        if (fieldCount == 0 || infoBegin < descriptorEnd || dataBegin < infoBegin) {
            throw new IOException("Invalid binary log header offsets");
        }

        Field[] fields = new Field[fieldCount];
        int payloadSize = 0;
        int timeField = -1;
        for (int i = 0; i < fieldCount; i++) {
            byte[] descriptor = new byte[FIELD_SIZE];
            in.readFully(descriptor);
            fields[i] = new Field(descriptor);
            payloadSize += fields[i].size;
            if (fields[i].name.equalsIgnoreCase(GAUGE_NAME_TIME)) {
                timeField = i;
            }
        }
        if (payloadSize != recordLength) {
            throw new IOException("Binary log record length " + recordLength + " does not match its fields (" + payloadSize + ")");
        }
        // without a Time field, column 0 is filled from the block timestamps
        boolean timeFromBlocks = timeField < 0;
        int columns = timeFromBlocks ? fieldCount + 1 : fieldCount;

        byte[] discard = new byte[8192];
        long remaining = dataBegin - descriptorEnd;
        while (remaining > 0) {
            int length = (int) Math.min(remaining, discard.length);
            in.readFully(discard, 0, length);
            remaining -= length;
        }

        String[] names = new String[columns];
        String[] units = new String[columns];
        int[] destination = new int[fieldCount];
        if (timeFromBlocks) {
            names[0] = GAUGE_NAME_TIME;
            units[0] = "s";
        }
        for (int i = 0, next = 1; i < fieldCount; i++) {
            int output = i == timeField ? 0 : next++;
            destination[i] = output;
            names[output] = fields[i].name;
            units[output] = fields[i].unit;
        }

        byte[] payload = new byte[recordLength];
        List<double[]> rows = new ArrayList<>();
        int previousTimestamp = -1;
        long timestampSteps = 0;
        int blockType;
        while ((blockType = in.read()) != -1) {
            if (blockType != 0) {
                throw new IOException("Unsupported binary log block type " + blockType);
            }
            in.readUnsignedByte();
            int timestamp = in.readUnsignedShort();
            if (previousTimestamp >= 0) {
                timestampSteps += timestampStep(previousTimestamp, timestamp);
            }
            previousTimestamp = timestamp;
            in.readFully(payload);
            int checksum = in.readUnsignedByte();
            int sum = 0;
            for (byte value : payload) {
                sum += value & 0xff;
            }
            if ((sum & 0xff) != checksum) {
                throw new IOException("Binary log checksum mismatch at record " + rows.size());
            }

            ByteBuffer values = ByteBuffer.wrap(payload);
            double[] row = new double[columns];
            for (int i = 0; i < fieldCount; i++) {
                row[destination[i]] = fields[i].read(values);
            }
            if (timeFromBlocks) {
                row[0] = timestampSteps * SECONDS_PER_TIMESTAMP_STEP;
            }
            rows.add(row);
        }
        return CsvLogReader.fromRows(names, units, rows);
    }

    /**
     * 10 us steps between two consecutive block timestamps. The 16-bit counter wraps every 655 ms, so a
     * longer pause between two records cannot be told apart from a shorter one.
     */
    static int timestampStep(int previous, int current) {
        return (current - previous) & 0xffff;
    }

    private static String cString(byte[] source, int offset, int length) {
        int end = offset;
        while (end < offset + length && source[end] != 0) {
            end++;
        }
        return new String(source, offset, end - offset, StandardCharsets.US_ASCII);
    }

    private static final class Field {
        final int type;
        final int size;
        final String name;
        final String unit;
        final float scale;
        final float transform;

        Field(byte[] descriptor) throws IOException {
            type = descriptor[0] & 0xff;
            switch (type) {
                case 0: case 1: size = 1; break;
                case 2: case 3: size = 2; break;
                case 4: case 5: case 7: size = 4; break;
                case 6: size = 8; break;
                default: throw new IOException("Unsupported binary log field type " + type);
            }
            name = cString(descriptor, 1, 34);
            unit = cString(descriptor, 35, 10);
            ByteBuffer attributes = ByteBuffer.wrap(descriptor);
            scale = attributes.getFloat(46);
            transform = attributes.getFloat(50);
            if (!Float.isFinite(scale) || !Float.isFinite(transform)) {
                throw new IOException("Invalid binary log scale for " + name);
            }
        }

        double read(ByteBuffer values) {
            double raw;
            switch (type) {
                case 0: raw = values.get() & 0xff; break;
                case 1: raw = values.get(); break;
                case 2: raw = values.getShort() & 0xffff; break;
                case 3: raw = values.getShort(); break;
                case 4: raw = Integer.toUnsignedLong(values.getInt()); break;
                case 5: raw = values.getInt(); break;
                case 6: raw = values.getLong(); break;
                case 7: raw = values.getFloat(); break;
                default: throw new IllegalStateException("Unsupported binary log field type " + type);
            }
            return (raw + transform) * scale;
        }
    }
}
