package com.rusefi.mcp;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

/**
 * Host-side MLVLG v2 / text TunerStudio MSL converter.
 * Binary decoding is adapted from misc/mlg2csv/MlgToCsv.java on at32-vovansss;
 * CSV columns follow unit_tests/test-framework/msl_to_csv.cpp (logged fields only).
 */
public final class MslToCsv {
    private MslToCsv() { }

    public static final class Result {
        public final Path path;
        public final String inputFormat;
        public final long recordCount;
        public final int fieldCount;

        private Result(Path path, String inputFormat, long recordCount, int fieldCount) {
            this.path = path;
            this.inputFormat = inputFormat;
            this.recordCount = recordCount;
            this.fieldCount = fieldCount;
        }
    }

    public static Path defaultOutput(Path input) {
        String name = input.getFileName().toString();
        int dot = name.lastIndexOf('.');
        return input.resolveSibling((dot > 0 ? name.substring(0, dot) : name) + ".csv");
    }

    /** Streams input and publishes CSV only after successful conversion; never overwrites a file. */
    public static Result convert(Path input, Path output) throws IOException {
        Path target = output.toAbsolutePath().normalize();
        if (input.toAbsolutePath().normalize().equals(target) || Files.exists(target)) {
            throw new IOException("Output already exists or is the input file: " + target);
        }
        Path temporary = Files.createTempFile(target.getParent(), ".msl-to-csv-", ".tmp");
        try {
            Result result;
            try (BufferedInputStream in = new BufferedInputStream(Files.newInputStream(input));
                 BufferedWriter out = Files.newBufferedWriter(temporary, StandardCharsets.UTF_8)) {
                in.mark(5);
                byte[] magic = new byte[5];
                int count = 0;
                int next;
                while (count < magic.length && (next = in.read()) != -1) {
                    magic[count++] = (byte) next;
                }
                in.reset();
                boolean binary = count == 5 && "MLVLG".equals(new String(magic, StandardCharsets.US_ASCII));
                result = binary ? convertBinary(in, out, target) : convertText(in, out, target);
            }
            Files.move(temporary, target);
            return result;
        } finally {
            Files.deleteIfExists(temporary);
        }
    }

    private static final class Field {
        final int type;
        final int size;
        final String name;
        final String units;
        final float scale;
        final float transform;
        final int digits;

        Field(byte[] descriptor) throws IOException {
            ByteBuffer data = ByteBuffer.wrap(descriptor);
            type = descriptor[0] & 0xff;
            switch (type) {
                case 0: case 1: size = 1; break;
                case 2: case 3: size = 2; break;
                case 4: case 5: case 7: size = 4; break;
                case 6: size = 8; break;
                default: throw new IOException("Unsupported MLG scalar type: " + type);
            }
            name = cstr(descriptor, 1, 34);
            units = cstr(descriptor, 35, 10);
            scale = data.getFloat(46);
            transform = data.getFloat(50);
            digits = Math.max(0, Math.min(12, descriptor[54]));
            if (!Float.isFinite(scale) || !Float.isFinite(transform)) {
                throw new IOException("Invalid scale/transform for " + name);
            }
        }

        double readScaledValue(ByteBuffer data) {
            double raw;
            switch (type) {
                case 0: raw = data.get() & 0xff; break;
                case 1: raw = data.get(); break;
                case 2: raw = data.getShort() & 0xffff; break;
                case 3: raw = data.getShort(); break;
                case 4: raw = Integer.toUnsignedLong(data.getInt()); break;
                case 5: raw = data.getInt(); break;
                case 6: raw = data.getLong(); break;
                case 7: raw = data.getFloat(); break;
                default: throw new IllegalStateException("Unsupported type " + type);
            }
            return (raw + transform) * scale;
        }
    }

    private static Result convertBinary(InputStream source, BufferedWriter out, Path target) throws IOException {
        DataInputStream in = new DataInputStream(source);
        byte[] header = new byte[24];
        in.readFully(header);
        ByteBuffer h = ByteBuffer.wrap(header);
        if (header[5] != 0 || h.getShort(6) != 2) {
            throw new IOException("Only MLVLG version 2 is supported");
        }
        long dataBegin = Integer.toUnsignedLong(h.getInt(16));
        int recordLength = Short.toUnsignedInt(h.getShort(20));
        int fieldCount = Short.toUnsignedInt(h.getShort(22));
        long metadataEnd = 24L + 89L * fieldCount;
        if (fieldCount == 0 || dataBegin < metadataEnd) {
            throw new IOException("Invalid MLG header offsets/field count");
        }
        List<Field> fields = new ArrayList<>();
        List<String> headings = new ArrayList<>();
        int expectedLength = 0;
        for (int i = 0; i < fieldCount; i++) {
            byte[] descriptor = new byte[89];
            in.readFully(descriptor);
            Field field = new Field(descriptor);
            fields.add(field);
            headings.add(heading(field.name, field.units));
            expectedLength += field.size;
        }
        if (expectedLength != recordLength) {
            throw new IOException("MLG record length does not match field descriptors");
        }
        long remaining = dataBegin - metadataEnd;
        byte[] info = new byte[8192];
        while (remaining > 0) {
            int length = (int) Math.min(remaining, info.length);
            in.readFully(info, 0, length);
            remaining -= length;
        }
        writeRow(out, headings.toArray(new String[0]));
        byte[] payload = new byte[recordLength];
        long records = 0;
        int blockType;
        while ((blockType = in.read()) != -1) {
            if (blockType != 0) {
                throw new IOException("Unsupported MLG block type " + blockType + " at record " + records);
            }
            in.readUnsignedByte(); // rolling counter
            in.readUnsignedShort(); // no synthetic time column: preserve the logged fields
            in.readFully(payload);
            int checksum = in.readUnsignedByte();
            int sum = 0;
            for (byte value : payload) {
                sum += value & 0xff;
            }
            if ((sum & 0xff) != checksum) {
                throw new IOException("MLG checksum mismatch at record " + records);
            }
            ByteBuffer values = ByteBuffer.wrap(payload);
            String[] row = new String[fieldCount];
            for (int i = 0; i < fieldCount; i++) {
                Field field = fields.get(i);
                double value = field.readScaledValue(values);
                row[i] = Double.isNaN(value) ? "" : String.format(Locale.ROOT, "%." + field.digits + "f", value);
            }
            writeRow(out, row);
            records++;
        }
        return new Result(target, "mlg", records, fieldCount);
    }

    private static Result convertText(InputStream source, BufferedWriter out, Path target) throws IOException {
        BufferedReader in = new BufferedReader(new InputStreamReader(source, StandardCharsets.UTF_8));
        String line;
        String[] names = null;
        while ((line = in.readLine()) != null) {
            if (line.startsWith("\uFEFF")) {
                line = line.substring(1);
            }
            if (!line.isEmpty() && !line.startsWith("\"") && !line.startsWith("#") && line.contains("\t")) {
                names = line.split("\t", -1);
                break;
            }
        }
        if (names == null) {
            throw new IOException("No field header found in text MSL");
        }
        line = in.readLine();
        if (line == null) {
            throw new IOException("Missing text MSL units row");
        }
        String[] units = line.split("\t", -1);
        if (units.length != names.length) {
            throw new IOException("Text MSL units count does not match fields");
        }
        for (int i = 0; i < names.length; i++) {
            names[i] = heading(names[i], units[i]);
        }
        writeRow(out, names);
        long records = 0;
        while ((line = in.readLine()) != null) {
            if (line.isEmpty()) {
                continue;
            }
            String[] row = line.split("\t", -1);
            if (row.length != names.length) {
                throw new IOException("Text MSL field count mismatch at record " + records);
            }
            writeRow(out, row);
            records++;
        }
        return new Result(target, "msl", records, names.length);
    }

    private static String cstr(byte[] data, int offset, int length) {
        int end = offset;
        while (end < offset + length && data[end] != 0) {
            end++;
        }
        return new String(data, offset, end - offset, StandardCharsets.US_ASCII);
    }

    private static String heading(String name, String units) {
        return units.isEmpty() ? name : name + " (" + units + ")";
    }

    private static void writeRow(BufferedWriter out, String[] cells) throws IOException {
        for (int i = 0; i < cells.length; i++) {
            if (i > 0) {
                out.write(',');
            }
            String cell = cells[i];
            if (cell.contains(",") || cell.contains("\"") || cell.contains("\r") || cell.contains("\n")) {
                out.write('"');
                out.write(cell.replace("\"", "\"\""));
                out.write('"');
            } else {
                out.write(cell);
            }
        }
        out.write('\n');
    }

    public static void main(String[] args) throws IOException {
        if (args.length < 1 || args.length > 2) {
            throw new IllegalArgumentException("Usage: MslToCsv input.mlg|input.msl [output.csv]");
        }
        Path input = Paths.get(args[0]);
        Result result = convert(input, args.length == 2 ? Paths.get(args[1]) : defaultOutput(input));
        System.out.println("Written " + result.recordCount + " records, " + result.fieldCount + " fields to " + result.path);
    }
}
