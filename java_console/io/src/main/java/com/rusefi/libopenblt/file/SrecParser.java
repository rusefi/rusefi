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

        public long endAddress() {
            return Integer.toUnsignedLong(address) + data.length;
        }

        @Override
        public String toString() {
            return String.format("Address: 0x%08X, Length: %d", address, data.length);
        }
    }

    private final List<SRecord> mRecords = new ArrayList<>();
    private boolean mHasEmptyDataRecords;

    public void parse(File file) throws IOException {
        mRecords.clear();
        mHasEmptyDataRecords = false;

        try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
            String line;
            int lineNumber = 0;

            while ((line = reader.readLine()) != null) {
                lineNumber++;
                line = line.trim();
                if (line.isEmpty()) {
                    continue;
                }
                if (line.length() < 4 || line.charAt(0) != 'S') {
                    throw new IOException("Invalid S-record at line " + lineNumber);
                }

                char recordType = line.charAt(1);
                String hexData = line.substring(2);

                byte[] bytes = hexStringToByteArray(hexData, lineNumber);
                int count = bytes[0] & 0xFF;

                int addrLen;
                switch (recordType) {
                    case '0':
                    case '1':
                    case '5':
                    case '9':
                        addrLen = 2;
                        break;
                    case '2':
                    case '6':
                    case '8':
                        addrLen = 3;
                        break;
                    case '3':
                    case '7':
                        addrLen = 4;
                        break;
                    default:
                        throw new IOException("Unsupported S-record type at line " + lineNumber + ": S" + recordType);
                }

                if (bytes.length != count + 1 || count < addrLen + 1) {
                    throw new IOException("Invalid S-record length at line " + lineNumber);
                }

                int checksum = 0;
                for (byte value : bytes) {
                    checksum += value & 0xFF;
                }
                if ((checksum & 0xFF) != 0xFF) {
                    throw new IOException("Invalid S-record checksum at line " + lineNumber);
                }

                int address = 0;
                for (int i = 1; i <= addrLen; i++) {
                    address = (address << 8) | (bytes[i] & 0xFF);
                }

                int dataLen = count - addrLen - 1;
                if (Integer.toUnsignedLong(address) + dataLen > 0x1_0000_0000L) {
                    throw new IOException("S-record address overflow at line " + lineNumber);
                }

                if (recordType != '1' && recordType != '2' && recordType != '3') {
                    continue;
                }
                if (dataLen == 0) {
                    mHasEmptyDataRecords = true;
                }

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

    public boolean hasEmptyDataRecords() {
        return mHasEmptyDataRecords;
    }

    private void mergeAdjacentRecords() throws IOException {
        if (mRecords.isEmpty()) return;

        mRecords.sort(Comparator.comparingLong(r -> Integer.toUnsignedLong(r.address)));
        List<SRecord> merged = new ArrayList<>();

        int i = 0;
        while (i < mRecords.size()) {
            SRecord start = mRecords.get(i);
            long totalLength = start.data.length;
            int end = i + 1;

            // Find contiguous run
            while (end < mRecords.size()) {
                SRecord previous = mRecords.get(end - 1);
                SRecord current = mRecords.get(end);
                long currentAddress = Integer.toUnsignedLong(current.address);
                if (currentAddress < previous.endAddress()) {
                    throw new IOException("Overlapping S-records at address 0x"
                        + Long.toHexString(currentAddress));
                }
                if (previous.endAddress() != currentAddress) {
                    break;
                }
                totalLength += mRecords.get(end).data.length;
                end++;
            }

            if (totalLength > Integer.MAX_VALUE) {
                throw new IOException("Contiguous S-record data is too large");
            }

            // Merge all contiguous records in one pass
            byte[] mergedData = new byte[(int) totalLength];
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

    private static byte[] hexStringToByteArray(String s, int lineNumber) throws IOException {
        int len = s.length();
        if (len % 2 != 0) {
            throw new IOException("Odd-length hexadecimal S-record at line " + lineNumber);
        }
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            int high = Character.digit(s.charAt(i), 16);
            int low = Character.digit(s.charAt(i + 1), 16);
            if (high < 0 || low < 0) {
                throw new IOException("Invalid hexadecimal S-record at line " + lineNumber);
            }
            int byteVal = high << 4 | low;
            data[i / 2] = (byte) byteVal;
        }
        return data;
    }
}
