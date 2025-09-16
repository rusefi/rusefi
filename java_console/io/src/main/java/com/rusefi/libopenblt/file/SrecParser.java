package com.rusefi.libopenblt.file;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class SrecParser {
    public static class SRecord {
        public final int address;
        public final byte[] data;

        public SRecord(int address, byte[] data) {
            this.address = address;
            this.data = data;
        }

        public int endAddress() {
            return address + data.length;
        }

        @Override
        public String toString() {
            return String.format("Address: 0x%08X, Length: %d", address, data.length);
        }
    }

    private final List<SRecord> mRecords = new ArrayList<>();

    public void parse(File file) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
            String line;

            while ((line = reader.readLine()) != null) {
                line = line.trim();
                if (line.isEmpty() || line.charAt(0) != 'S') continue;

                char recordType = line.charAt(1);
                String hexData = line.substring(2);

                byte[] bytes = hexStringToByteArray(hexData);
                int count = bytes[0] & 0xFF;

                int addrLen;
                switch (recordType) {
                    case '1': addrLen = 2; break;
                    case '2': addrLen = 3; break;
                    case '3': addrLen = 4; break;
                    default: continue; // skip unsupported types
                }

                if (count < addrLen + 1) {
                    throw new IOException("Invalid record length in line: " + line);
                }

                int address = 0;
                for (int i = 1; i <= addrLen; i++) {
                    address = (address << 8) | (bytes[i] & 0xFF);
                }

                int dataLen = count - addrLen - 1;
                byte[] data = new byte[dataLen];
                System.arraycopy(bytes, 1 + addrLen, data, 0, dataLen);

                mRecords.add(new SRecord(address, data));
            }
        }

        mergeAdjacentRecords();
    }

    public List<SRecord> getSegments() {
        return new ArrayList<>(mRecords);
    }

    private void mergeAdjacentRecords() {
        if (mRecords.isEmpty()) return;

        mRecords.sort(Comparator.comparingInt(r -> r.address));
        List<SRecord> merged = new ArrayList<>();

        int i = 0;
        while (i < mRecords.size()) {
            SRecord start = mRecords.get(i);
            int totalLength = start.data.length;
            int end = i + 1;

            // Find contiguous run
            while (end < mRecords.size() && mRecords.get(end - 1).endAddress() == mRecords.get(end).address) {
                totalLength += mRecords.get(end).data.length;
                end++;
            }

            // Merge all contiguous records in one pass
            byte[] mergedData = new byte[totalLength];
            int offset = 0;
            for (int j = i; j < end; j++) {
                byte[] segment = mRecords.get(j).data;
                System.arraycopy(segment, 0, mergedData, offset, segment.length);
                offset += segment.length;
            }

            merged.add(new SRecord(start.address, mergedData));
            i = end;
        }

        mRecords.clear();
        mRecords.addAll(merged);
    }

    private static byte[] hexStringToByteArray(String s) {
        int len = s.length();
        if (len % 2 != 0) throw new IllegalArgumentException("Hex string has odd length.");
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            int byteVal = Character.digit(s.charAt(i), 16) << 4 | Character.digit(s.charAt(i + 1), 16);
            data[i / 2] = (byte) byteVal;
        }
        return data;
    }
}
