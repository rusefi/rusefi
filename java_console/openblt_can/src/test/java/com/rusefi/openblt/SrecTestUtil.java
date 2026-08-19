package com.rusefi.openblt;

import java.util.ArrayList;
import java.util.List;

/** Builds minimal Motorola S-record files for the parser/flasher tests. */
public final class SrecTestUtil {

    /** Builds one S3 data record line with a correct checksum. */
    static String s3(int address, byte[] data) {
        return srecLine('3', address, data);
    }

    /** Builds one S1 data record line (16-bit address). */
    static String s1(int address, byte[] data) {
        return srecLine('1', address, data);
    }

    static String s0(String header) {
        byte[] data = header.getBytes(java.nio.charset.StandardCharsets.US_ASCII);
        StringBuilder sb = new StringBuilder("S0");
        int count = 3 + data.length; // addr(2) + data + checksum
        sb.append(String.format("%02X", count));
        sb.append("0000");
        for (byte b : data) {
            sb.append(String.format("%02X", b));
        }
        sb.append(checksum(count, 0x0000, data));
        return sb.toString();
    }

    static String terminator() {
        return "S70500000000FA";
    }

    static List<String> image(int address, byte[] data) {
        List<String> lines = new ArrayList<>();
        lines.add(s0("test"));
        for (int off = 0; off < data.length; off += 16) {
            int len = Math.min(16, data.length - off);
            byte[] chunk = new byte[len];
            System.arraycopy(data, off, chunk, 0, len);
            lines.add(s3(address + off, chunk));
        }
        lines.add(terminator());
        return lines;
    }

    /** Full file content for a list of lines. */
    static String file(List<String> lines) {
        return String.join("\n", lines) + "\n";
    }

    private static String srecLine(char type, int address, byte[] data) {
        int addrBytes = type == '1' ? 2 : 4;
        StringBuilder sb = new StringBuilder("S").append(type);
        int count = addrBytes + data.length + 1;
        sb.append(String.format("%02X", count));
        sb.append(String.format("%0" + (addrBytes * 2) + "X", address));
        for (byte b : data) {
            sb.append(String.format("%02X", b));
        }
        int sum = count;
        for (int i = 0; i < addrBytes; i++) {
            sum += (address >> (8 * i)) & 0xFF;
        }
        for (byte b : data) {
            sum += b & 0xFF;
        }
        sb.append(String.format("%02X", (~sum) & 0xFF));
        return sb.toString();
    }

    private static String checksum(int count, int address, byte[] data) {
        int sum = count;
        for (int i = 0; i < 2; i++) {
            sum += (address >> (8 * i)) & 0xFF;
        }
        for (byte b : data) {
            sum += b & 0xFF;
        }
        return String.format("%02X", (~sum) & 0xFF);
    }

    private SrecTestUtil() {
    }
}
