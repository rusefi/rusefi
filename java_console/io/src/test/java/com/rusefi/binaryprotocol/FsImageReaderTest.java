package com.rusefi.binaryprotocol;

import com.rusefi.config.ConsoleConstants;
import com.rusefi.config.generated.Integration;
import org.junit.jupiter.api.Test;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;
import java.util.zip.CRC32;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.Mockito.*;

class FsImageReaderTest {
    private final BinaryProtocol protocol = mock(BinaryProtocol.class);

    @Test
    void zeroSizeImageIsRejectedWithoutReadingData() {
        stubSize(0);
        IllegalStateException error = assertThrows(IllegalStateException.class,
                () -> FsImageReader.readImage(protocol));
        assertEquals("FS image not available, size=0", error.getMessage());
        verify(protocol).executeCommand(eq(ConsoleConstants.TS_READ32_COMMAND),
                eq(request(0x52450000, 0, 4)), anyString());
        verifyNoMoreInteractions(protocol);
    }

    @Test
    void readsMultipleChunksAndValidatesCrc() {
        byte[] image = image(5000);
        stubSize(image.length);
        stubData(image, 0, 4080);
        stubData(image, 4080, 920);
        stubCrc(image, crc(image));

        assertArrayEquals(image, FsImageReader.readImage(protocol));
        verify(protocol).executeCommand(eq(ConsoleConstants.TS_CRC32_CHECK_COMMAND),
                eq(request(0x52450001, 0, image.length)), anyString());
    }

    @Test
    void rejectedLargeReadFallsBackToSmallerChunks() {
        byte[] image = image(2500);
        stubSize(image.length);
        when(protocol.executeCommand(eq(ConsoleConstants.TS_READ32_COMMAND),
                eq(request(0x52450001, 0, 2500)), anyString()))
                .thenReturn(new byte[]{(byte) Integration.TS_RESPONSE_OUT_OF_RANGE});
        stubData(image, 0, 1000);
        stubData(image, 1000, 1000);
        stubData(image, 2000, 500);
        stubCrc(image, crc(image));

        assertArrayEquals(image, FsImageReader.readImage(protocol));
    }

    @Test
    void rejectsCrcMismatch() {
        byte[] image = image(512);
        stubSize(image.length);
        stubData(image, 0, image.length);
        stubCrc(image, crc(image) ^ 1);

        IllegalStateException error = assertThrows(IllegalStateException.class,
                () -> FsImageReader.readImage(protocol));
        assertTrue(error.getMessage().startsWith("FS image CRC mismatch:"));
    }

    private void stubSize(int size) {
        when(protocol.executeCommand(eq(ConsoleConstants.TS_READ32_COMMAND),
                eq(request(0x52450000, 0, 4)), anyString()))
                .thenReturn(reply(ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt(size).array()));
    }

    private void stubData(byte[] image, int offset, int length) {
        when(protocol.executeCommand(eq(ConsoleConstants.TS_READ32_COMMAND),
                eq(request(0x52450001, offset, length)), anyString()))
                .thenReturn(reply(Arrays.copyOfRange(image, offset, offset + length)));
    }

    private void stubCrc(byte[] image, int crc) {
        when(protocol.executeCommand(eq(ConsoleConstants.TS_CRC32_CHECK_COMMAND),
                eq(request(0x52450001, 0, image.length)), anyString()))
                .thenReturn(reply(ByteBuffer.allocate(4).putInt(crc).array()));
    }

    private static byte[] request(int page, int offset, int count) {
        return ByteBuffer.allocate(12).order(ByteOrder.LITTLE_ENDIAN)
                .putInt(page).putInt(offset).putInt(count).array();
    }

    private static byte[] reply(byte[] data) {
        byte[] response = new byte[data.length + 1];
        response[0] = (byte) Integration.TS_RESPONSE_OK;
        System.arraycopy(data, 0, response, 1, data.length);
        return response;
    }

    private static byte[] image(int size) {
        byte[] image = new byte[size];
        for (int i = 0; i < size; i++) {
            image[i] = (byte) (i * 31);
        }
        return image;
    }

    private static int crc(byte[] image) {
        CRC32 crc = new CRC32();
        crc.update(image);
        return (int) crc.getValue();
    }
}
