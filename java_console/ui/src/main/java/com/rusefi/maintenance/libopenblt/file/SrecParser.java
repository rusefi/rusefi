package com.rusefi.maintenance.libopenblt.file;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class SrecParser {
    public static class SRecord {
        public int address;
        public byte[] data;

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

        SRecord current = mRecords.get(0);

        for (int i = 1; i < mRecords.size(); i++) {
            SRecord next = mRecords.get(i);

            if (current.endAddress() == next.address) {
                // Merge next in to current
                byte[] newData = new byte[current.data.length + next.data.length];
                System.arraycopy(current.data, 0, newData, 0, current.data.length);
                System.arraycopy(next.data, 0, newData, current.data.length, next.data.length);
                current = new SRecord(current.address, newData);
            } else {
                merged.add(current);
                current = next;
            }
        }

        merged.add(current);
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
