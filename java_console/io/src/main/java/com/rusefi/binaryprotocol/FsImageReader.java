package com.rusefi.binaryprotocol;

import com.rusefi.config.generated.Integration;

/**
 * Reads the embedded mass-storage filesystem image over the TunerStudio protocol.
 *
 * The ECU exposes the FAT image which carries the compressed .ini (see
 * firmware/hw_layer/mass_storage/create_ini_image.sh) via two read-only pages accessible
 * with the READ32 command; external displays such as lvgl dashboards pull the image this
 * way to self-configure against the exact firmware they are talking to.
 *
 * Page identifiers must match firmware/console/binary/tunerstudio.h.
 */
public class FsImageReader {
    // two MSB bytes are "RE", see tunerstudio.h
    public static final int TS_PAGE_FS_IMAGE_SIZE = 0x52450000;
    public static final int TS_PAGE_FS_IMAGE_DATA = 0x52450001;

    // mirrors lvgl dashboard client behavior: optimistically try a chunk larger than
    // BLOCKING_FACTOR first - the firmware answers TS_RESPONSE_OUT_OF_RANGE (it must NOT
    // criticalError) and we fall back to a chunk its response buffer can actually hold
    private static final int READ_CHUNK_SIZE_FAST = 4080;
    private static final int READ_CHUNK_SIZE_FALLBACK = 1000;

    /**
     * Must be invoked on the {@link com.rusefi.io.LinkManager} communication thread.
     *
     * @return the whole filesystem image, CRC32-validated against the ECU
     * @throws IllegalStateException on any protocol failure or CRC mismatch
     */
    public static byte[] readImage(BinaryProtocol bp) {
        int totalSize = readImageSize(bp);
        if (totalSize <= 0) {
            throw new IllegalStateException("FS image not available, size=" + totalSize);
        }

        byte[] image = readImageData(bp, totalSize, READ_CHUNK_SIZE_FAST);
        if (image == null) {
            image = readImageData(bp, totalSize, READ_CHUNK_SIZE_FALLBACK);
        }
        if (image == null) {
            throw new IllegalStateException("FS image read failed with both chunk sizes");
        }

        int expectedCrc = readImageCrc(bp, totalSize);
        int actualCrc = IoHelper.getCrc32(image);
        if (expectedCrc != actualCrc) {
            throw new IllegalStateException(String.format("FS image CRC mismatch: ECU 0x%08X local 0x%08X",
                    expectedCrc, actualCrc));
        }
        return image;
    }

    /**
     * @return size of the filesystem image in bytes, zero if the firmware has no embedded image
     */
    public static int readImageSize(BinaryProtocol bp) {
        byte[] response = bp.executeCommand(Integration.TS_READ32_COMMAND,
                makePageOffsetCountPayload(TS_PAGE_FS_IMAGE_SIZE, 0, 4),
                "read FS image size");
        assertOkResponse(response, 4, "FS image size");
        // little-endian, same as the raw uint32_t the firmware writes
        return (response[1] & 0xff)
                | ((response[2] & 0xff) << 8)
                | ((response[3] & 0xff) << 16)
                | ((response[4] & 0xff) << 24);
    }

    /**
     * @return the image, or null when the ECU rejects this chunk size with an error code
     */
    private static byte[] readImageData(BinaryProtocol bp, int totalSize, int chunkSize) {
        byte[] image = new byte[totalSize];
        int offset = 0;
        while (offset < totalSize) {
            int toRead = Math.min(chunkSize, totalSize - offset);
            byte[] response = bp.executeCommand(Integration.TS_READ32_COMMAND,
                    makePageOffsetCountPayload(TS_PAGE_FS_IMAGE_DATA, offset, toRead),
                    "read FS image offset=" + offset + " size=" + toRead);
            if (response == null) {
                throw new IllegalStateException("FS image data at offset " + offset + ": no response");
            }
            if (response[0] != Integration.TS_RESPONSE_OK) {
                // e.g. TS_RESPONSE_OUT_OF_RANGE for a chunk larger than the response buffer
                return null;
            }
            if (response.length != toRead + 1) {
                throw new IllegalStateException("FS image data at offset " + offset + ": expected "
                        + toRead + " bytes, got " + (response.length - 1));
            }
            System.arraycopy(response, 1, image, offset, toRead);
            offset += toRead;
        }
        return image;
    }

    private static int readImageCrc(BinaryProtocol bp, int totalSize) {
        byte[] response = bp.executeCommand(Integration.TS_CRC32_CHECK_COMMAND,
                makePageOffsetCountPayload(TS_PAGE_FS_IMAGE_DATA, 0, totalSize),
                "FS image CRC");
        assertOkResponse(response, 4, "FS image CRC");
        // CRC reply is big-endian
        return ((response[1] & 0xff) << 24)
                | ((response[2] & 0xff) << 16)
                | ((response[3] & 0xff) << 8)
                | (response[4] & 0xff);
    }

    private static void assertOkResponse(byte[] response, int expectedPayloadSize, String msg) {
        if (response == null) {
            throw new IllegalStateException(msg + ": no response");
        }
        if (response[0] != Integration.TS_RESPONSE_OK) {
            throw new IllegalStateException(msg + ": error code " + (response[0] & 0xff));
        }
        if (response.length != expectedPayloadSize + 1) {
            throw new IllegalStateException(msg + ": expected " + expectedPayloadSize
                    + " bytes, got " + (response.length - 1));
        }
    }

    /**
     * READ32/CRC32 commands take three little-endian uint32 arguments: page, offset, count
     */
    private static byte[] makePageOffsetCountPayload(int page, int offset, int count) {
        byte[] payload = new byte[12];
        putIntLe(payload, 0, page);
        putIntLe(payload, 4, offset);
        putIntLe(payload, 8, count);
        return payload;
    }

    private static void putIntLe(byte[] buffer, int offset, int value) {
        buffer[offset] = (byte) value;
        buffer[offset + 1] = (byte) (value >> 8);
        buffer[offset + 2] = (byte) (value >> 16);
        buffer[offset + 3] = (byte) (value >> 24);
    }
}
