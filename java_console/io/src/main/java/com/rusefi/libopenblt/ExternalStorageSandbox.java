package com.rusefi.libopenblt;

import com.rusefi.libopenblt.transport.XcpSerial;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.zip.CRC32;

/** Explicit-port bench tool. Target must already be in this board's OpenBLT. */
public final class ExternalStorageSandbox {
    static final int QSPI_ADDRESS = 0x90100000;
    static final int QSPI_CAPACITY = 64 * 1024;
    static final int EMMC_ADDRESS = 0xa0000000;
    static final int TEST_FILE_SIZE = 50 * 1024 * 1024;
    static final int CHUNK_SIZE = 16 * 1024;

    public static void main(String[] args) throws Exception {
        if (args.length < 2 || args.length > 3) {
            throw new IllegalArgumentException("Usage: PORT qspi IMAGE | PORT emmc-test | PORT openblt|status|expose|qspitest");
        }
        if (args.length == 2 && (args[1].equals("openblt") || args[1].equals("status")
            || args[1].equals("expose") || args[1].equals("qspitest"))) {
            StorageFirmwareConsole.run(args[0], args[1]);
            return;
        }
        boolean qspi = args[1].equals("qspi");
        if ((!qspi && !args[1].equals("emmc-test")) || (qspi ? args.length != 3 : args.length != 2)) {
            throw new IllegalArgumentException("Usage: PORT qspi IMAGE | PORT emmc-test | PORT openblt|status|expose|qspitest");
        }
        byte[] image = qspi ? Files.readAllBytes(Path.of(args[2])) : null;
        if (qspi) {
            validateQspiImage(image);
        }
        XcpSettings settings = new XcpSettings();
        settings.timeoutT4 = 120_000; // QSPI erase / FAT allocation on eMMC.
        settings.timeoutT6 = 5_000;
        XcpLoader loader = new XcpLoader(new XcpSerial(args[0]), settings);
        loader.requireStationIdCheck("rusefi.premium-quick-test");
        loader.requireStorageLayout(QSPI_ADDRESS, QSPI_CAPACITY, EMMC_ADDRESS, TEST_FILE_SIZE);
        loader.start();
        try {
            uploadAndVerify(loader, qspi ? QSPI_ADDRESS : EMMC_ADDRESS, image);
            loader.stop(); // Only a verified complete upload may be finalized.
        } catch (Exception | Error failure) {
            try {
                loader.abort();
            } catch (Exception closeFailure) {
                failure.addSuppressed(closeFailure);
            }
            throw failure;
        }
        System.out.println(qspi ? "QSPI verified; run qspitest in the firmware console."
            : "50 MiB test50m.bin verified and committed; use sdmode pc to expose eMMC over USB.");
    }

    static void validateQspiImage(byte[] image) {
        if (image.length <= 256 || image.length > QSPI_CAPACITY) {
            throw new IllegalArgumentException("QSPI image must fit the reserved 64 KiB region");
        }
        ByteBuffer header = ByteBuffer.wrap(image).order(ByteOrder.LITTLE_ENDIAN);
        if (header.getInt() != 0x31504958 || header.getInt() != 1 || header.getInt() != image.length - 256) {
            throw new IllegalArgumentException("Invalid XIP1 image header");
        }
        long expected = Integer.toUnsignedLong(header.getInt());
        CRC32 crc = new CRC32();
        crc.update(image, 256, image.length - 256);
        if (crc.getValue() != expected) {
            throw new IllegalArgumentException("Invalid QSPI payload CRC");
        }
    }

    static byte[] testPattern(int offset, int count) {
        byte[] result = new byte[count];
        // Little-endian incrementing uint32 words: independent of chunk size.
        for (int i = 0; i < count; i++) {
            int position = offset + i;
            result[i] = (byte) ((position / 4) >>> (8 * (position % 4)));
        }
        return result;
    }

    static void uploadAndVerify(XcpLoader loader, int address, byte[] image) throws IOException {
        int length = image == null ? TEST_FILE_SIZE : image.length;
        loader.clearMemory(address, length);
        CRC32 crc = new CRC32();
        for (int offset = 0; offset < length;) {
            int count = Math.min(CHUNK_SIZE, length - offset);
            byte[] block = image == null ? testPattern(offset, count) : Arrays.copyOfRange(image, offset, offset + count);
            loader.writeData(address + offset, block);
            if (!Arrays.equals(block, loader.readData(address + offset, count))) {
                throw new IOException("Read-back mismatch at offset " + offset);
            }
            crc.update(block);
            offset += count;
            if (offset % (1024 * 1024) == 0 || offset == length) {
                System.out.printf("Verified %d / %d bytes%n", offset, length);
            }
        }
        System.out.printf("CRC32 %08x%n", crc.getValue());
    }
}
