package com.rusefi.openblt;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Minimal Motorola S-record parser (S1/S2/S3 data records, S0 header and
 * S5/S7/S8/S9 terminators). Only what the OpenBLT flasher needs: an ordered
 * list of contiguous segments with their base addresses.
 */
public final class SrecParser {

    public static final class Segment {
        private final int base;
        private final byte[] data;

        Segment(int base, byte[] data) {
            this.base = base;
            this.data = data;
        }

        public int base() {
            return base;
        }

        public byte[] data() {
            return data.clone();
        }

        public int end() {
            return base + data.length;
        }
    }

    public static final class Image {
        private final List<Segment> segments;

        Image(List<Segment> segments) {
            this.segments = Collections.unmodifiableList(segments);
        }

        public List<Segment> segments() {
            return segments;
        }

        public long totalBytes() {
            return segments.stream().mapToLong(s -> s.data.length).sum();
        }
    }

    public static Image parse(Path path) throws IOException {
        List<String> lines = Files.readAllLines(path, StandardCharsets.US_ASCII);
        return parse(lines);
    }

    static Image parse(List<String> lines) {
        List<Segment> segments = new ArrayList<>();

        // Current contiguous run: start address and accumulating buffer.
        int runBase = -1;
        ArrayList<Byte> run = new ArrayList<>();

        boolean sawTerminator = false;

        for (int lineNo = 0; lineNo < lines.size(); lineNo++) {
            String line = lines.get(lineNo).trim();
            if (line.isEmpty()) {
                continue;
            }
            if (line.charAt(0) != 'S') {
                throw new IllegalArgumentException("Not an S-record at line " + (lineNo + 1) + ": " + line);
            }
            char type = line.charAt(1);
            if (type == '0' || type == '5' || type == '7' || type == '8' || type == '9') {
                // Header/terminator records carry no flash data.
                if (type != '0') {
                    sawTerminator = true;
                }
                continue;
            }
            if (type != '1' && type != '2' && type != '3') {
                throw new IllegalArgumentException("Unsupported S-record type S" + type + " at line " + (lineNo + 1));
            }

            int addrBytes = type == '1' ? 2 : type == '2' ? 3 : 4;
            int headerLen = 2 + 2 + addrBytes * 2; // "S" + type + count + address
            if (line.length() < headerLen + 2 || (line.length() - headerLen) % 2 != 0) {
                throw new IllegalArgumentException("Malformed S-record at line " + (lineNo + 1) + ": " + line);
            }

            int byteCount = parseHex(line, 2, 1);
            int expected = addrBytes + (line.length() - headerLen) / 2; // addr + data + checksum
            if (byteCount != expected) {
                throw new IllegalArgumentException("Byte count mismatch at line " + (lineNo + 1)
                        + ": header says " + byteCount + ", actual " + expected);
            }

            int address = parseHex(line, 4, addrBytes);
            int dataLen = (line.length() - headerLen) / 2 - 1;
            byte[] data = new byte[dataLen];
            for (int i = 0; i < dataLen; i++) {
                data[i] = (byte) parseHex(line, headerLen + i * 2, 1);
            }
            int storedChecksum = parseHex(line, headerLen + dataLen * 2, 1);
            int checksum = byteCount;
            for (int i = 0; i < addrBytes; i++) {
                checksum += (address >> (8 * i)) & 0xFF;
            }
            for (byte b : data) {
                checksum += b & 0xFF;
            }
            if (((checksum ^ 0xFF) & 0xFF) != storedChecksum) {
                throw new IllegalArgumentException("Checksum mismatch at line " + (lineNo + 1));
            }

            if (runBase == -1) {
                runBase = address;
                run = new ArrayList<>();
            }
            if (address != runBase + run.size()) {
                // Non-contiguous: close the current run and start a new one.
                segments.add(new Segment(runBase, toBytes(run)));
                runBase = address;
                run = new ArrayList<>();
            }
            for (byte b : data) {
                run.add(b);
            }
        }

        if (runBase != -1 && !run.isEmpty()) {
            segments.add(new Segment(runBase, toBytes(run)));
        }

        if (segments.isEmpty() || !sawTerminator) {
            throw new IllegalArgumentException("SREC file contains no data records or is missing a terminator");
        }

        return new Image(segments);
    }

    private static byte[] toBytes(List<Byte> bytes) {
        byte[] out = new byte[bytes.size()];
        for (int i = 0; i < out.length; i++) {
            out[i] = bytes.get(i);
        }
        return out;
    }

    /** Parses a hex substring of {@code width} bytes starting at {@code charIndex}. */
    private static int parseHex(String line, int charIndex, int width) {
        int value = 0;
        for (int i = 0; i < width * 2; i++) {
            char c = line.charAt(charIndex + i);
            int digit = Character.digit(c, 16);
            if (digit < 0) {
                throw new IllegalArgumentException("Invalid hex digit '" + c + "' in S-record: " + line);
            }
            value = (value << 4) | digit;
        }
        return value;
    }

    private SrecParser() {
    }
}
