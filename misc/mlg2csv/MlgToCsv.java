/**
 * MlgToCsv - converts rusEFI binary MLG log files (format v2, "MLVLG") to CSV
 * that MegaLogViewer (MS/HD) can open.
 *
 * Format reference: firmware/console/binary_mlg_log/mlg_types.h and
 * binary_mlg_logging.cpp
 *
 * Usage: java MlgToCsv <input.mlg> [output.csv]
 *
 * No dependencies beyond a JDK:
 *   javac MlgToCsv.java
 *   java MlgToCsv in.mlg out.csv
 */

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

public class MlgToCsv {
    private static final class Field {
        final String name;
        final int type;
        final int size;
        final float scale;
        final float transform;
        final int digits;

        Field(String name, int type, float scale, float transform, int digits) {
            this.name = name;
            this.type = type;
            this.size = sizeForType(type);
            this.scale = scale;
            this.transform = transform;
            this.digits = Math.max(0, Math.min(6, digits));
        }
    }

    // MLG v2 scalar types: 0=U08, 1=S08, 2=U16, 3=S16, 4=U32, 5=S32, 6=S64, 7=F32
    private static int sizeForType(int t) {
        switch (t) {
            case 0: case 1: return 1;
            case 2: case 3: return 2;
            case 4: case 5: case 7: return 4;
            case 6: return 8;
            default: throw new IllegalStateException("unsupported scalar type " + t);
        }
    }

    private static String cstr(byte[] buf, int offset, int length) {
        int end = offset;
        while (end < offset + length && buf[end] != 0) {
            end++;
        }
        return new String(buf, offset, end - offset, StandardCharsets.US_ASCII);
    }

    private static int beU16(byte[] b, int off) {
        return ((b[off] & 0xFF) << 8) | (b[off + 1] & 0xFF);
    }

    private static long beU32(byte[] b, int off) {
        return ((long) (b[off] & 0xFF) << 24) | ((b[off + 1] & 0xFF) << 16)
                | ((b[off + 2] & 0xFF) << 8) | (b[off + 3] & 0xFF);
    }

    private static float beFloat(byte[] b, int off) {
        return Float.intBitsToFloat((int) beU32(b, off));
    }

    private static long beU64(byte[] b, int off) {
        long v = 0;
        for (int i = 0; i < 8; i++) {
            v = (v << 8) | (b[off + i] & 0xFF);
        }
        return v;
    }

    private static double readScaledValue(byte[] b, int off, Field f) {
        double raw;
        switch (f.type) {
            case 0: raw = b[off] & 0xFF; break;
            case 1: raw = b[off]; break;
            case 2: raw = beU16(b, off); break;
            case 3: raw = (short) beU16(b, off); break;
            case 4: raw = beU32(b, off); break;
            case 5: raw = (int) beU32(b, off); break;
            case 6: raw = beU64(b, off); break;
            case 7: raw = beFloat(b, off); break;
            default: raw = 0;
        }
        return (raw + f.transform) * f.scale;
    }

    private static String formatValue(double v) {
        if (Double.isNaN(v)) {
            return "";
        }
        String s = String.format(Locale.ROOT, "%.6f", v);
        if (s.indexOf('.') >= 0) {
            s = s.replaceAll("0+$", "").replaceAll("\\.$", "");
        }
        return s;
    }

    private static String sanitize(String s) {
        return s.replace("\"", "").replace(",", " ").trim();
    }

    public static void main(String[] args) throws IOException {
        if (args.length < 1) {
            System.err.println("Usage: java MlgToCsv <input.mlg> [output.csv]");
            System.exit(1);
        }

        String inPath = args[0];
        String outPath = args.length > 1 ? args[1] : inPath.replaceFirst("\\.mlg$", "") + ".csv";

        byte[] b = Files.readAllBytes(Paths.get(inPath));

        if (b.length < 24 || b[0] != 'M' || b[1] != 'L' || b[2] != 'V' || b[3] != 'L' || b[4] != 'G') {
            throw new IOException("not an MLVLG file: " + inPath);
        }

        int dataBegin = (int) beU32(b, 16);
        int recordLength = beU16(b, 20);
        int fieldCount = beU16(b, 22);

        List<Field> fields = new ArrayList<>();
        int off = 24;
        for (int i = 0; i < fieldCount; i++) {
            int type = b[off] & 0xFF;
            String name = cstr(b, off + 1, 34);
            // units: b[off+35..45), display style: b[off+45]
            float scale = beFloat(b, off + 46);
            float transform = beFloat(b, off + 50);
            int digits = b[off + 54];
            fields.add(new Field(name, type, scale, transform, digits));
            off += 89;
        }

        System.out.println("fields: " + fieldCount + ", recordLength: " + recordLength
                + ", dataBegin: " + dataBegin + ", fileSize: " + b.length);

        StringBuilder sb = new StringBuilder(1024 * 1024);
        sb.append("time");
        for (Field f : fields) {
            sb.append(',').append(sanitize(f.name));
        }
        sb.append('\n');

        int pos = dataBegin;
        long recordCounter = 0;
        int prevTs = -1;
        double timeSec = 0;
        int skipped = 0;
        int checksumBad = 0;

        while (pos + 4 + recordLength + 1 <= b.length) {
            int blockType = b[pos] & 0xFF;
            // rolling counter: b[pos + 1]
            int ts = beU16(b, pos + 2);

            if (blockType != 0) {
                // unknown block type: skip its payload defensively
                pos += 4 + recordLength + 1;
                skipped++;
                continue;
            }

            // unwrap the 16-bit 10us timestamp (wraps every 0.65536 s)
            if (prevTs >= 0) {
                int delta = (ts - prevTs) & 0xFFFF;
                timeSec += delta * 10e-6;
            }
            prevTs = ts;

            int payloadOff = pos + 4;
            int chk = b[payloadOff + recordLength] & 0xFF;
            int sum = 0;
            for (int i = 0; i < recordLength; i++) {
                sum = (sum + (b[payloadOff + i] & 0xFF)) & 0xFF;
            }
            if (sum != chk) {
                checksumBad++;
            }

            sb.append(formatValue(timeSec));
            int fieldOff = payloadOff;
            for (Field f : fields) {
                sb.append(',').append(formatValue(readScaledValue(b, fieldOff, f)));
                fieldOff += f.size;
            }
            sb.append('\n');

            pos += 4 + recordLength + 1;
            recordCounter++;
        }

        System.out.println("records: " + recordCounter + ", skipped: " + skipped
                + ", checksum mismatches: " + checksumBad
                + ", trailing bytes: " + (b.length - pos));

        Files.write(Paths.get(outPath), sb.toString().getBytes(StandardCharsets.US_ASCII));
        System.out.println("written: " + outPath);
    }
}
