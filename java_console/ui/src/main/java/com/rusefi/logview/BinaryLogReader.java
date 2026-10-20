package com.rusefi.logview;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

/** Reader for version 2 binary operating logs. */
public final class BinaryLogReader {
    private static final int HEADER_SIZE = 24;
    private static final int FIELD_SIZE = 89;

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
            if (fields[i].name.equalsIgnoreCase("Time")) {
                timeField = i;
            }
        }
        if (payloadSize != recordLength || timeField < 0) {
            throw new IOException("Binary log needs a Time field and matching record length");
        }

        byte[] discard = new byte[8192];
        long remaining = dataBegin - descriptorEnd;
        while (remaining > 0) {
            int length = (int) Math.min(remaining, discard.length);
            in.readFully(discard, 0, length);
            remaining -= length;
        }

        String[] names = new String[fieldCount];
        String[] units = new String[fieldCount];
        int[] destination = new int[fieldCount];
        for (int i = 0, next = 1; i < fieldCount; i++) {
            int output = i == timeField ? 0 : next++;
            destination[i] = output;
            names[output] = fields[i].name;
            units[output] = fields[i].unit;
        }

        byte[] payload = new byte[recordLength];
        List<double[]> rows = new ArrayList<>();
        int blockType;
        while ((blockType = in.read()) != -1) {
            if (blockType != 0) {
                throw new IOException("Unsupported binary log block type " + blockType);
            }
            in.readUnsignedByte();
            in.readUnsignedShort();
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
            double[] row = new double[fieldCount];
            for (int i = 0; i < fieldCount; i++) {
                row[destination[i]] = fields[i].read(values);
            }
            rows.add(row);
        }
        return CsvLogReader.fromRows(names, units, rows);
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
