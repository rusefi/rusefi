package com.rusefi.libopenblt;

import org.junit.jupiter.api.Test;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;
import java.util.zip.CRC32;
import static org.junit.jupiter.api.Assertions.*;

class ExternalStorageSandboxTest {
    @Test void validatesManifestAndRejectsCorruptCodeBeforeConnecting() {
        byte[] image = new byte[260];
        image[256] = 42;
        CRC32 crc = new CRC32();
        crc.update(image, 256, 4);
        ByteBuffer.wrap(image).order(ByteOrder.LITTLE_ENDIAN)
            .putInt(0x31504958).putInt(1).putInt(4).putInt((int) crc.getValue());
        assertDoesNotThrow(() -> ExternalStorageSandbox.validateQspiImage(image));
        image[256] ^= 1;
        assertThrows(IllegalArgumentException.class, () -> ExternalStorageSandbox.validateQspiImage(image));
        assertThrows(IllegalArgumentException.class, () -> ExternalStorageSandbox.validateQspiImage(new byte[256]));
        assertThrows(IllegalArgumentException.class, () -> ExternalStorageSandbox.validateQspiImage(new byte[65537]));
    }

    @Test void patternCanBeVerifiedAcrossUnalignedTransportChunks() {
        byte[] all = ExternalStorageSandbox.testPattern(0, 1029);
        assertArrayEquals(new byte[]{0,0,0,0,1,0,0,0,2,0,0,0}, Arrays.copyOf(all, 12));
        assertArrayEquals(Arrays.copyOfRange(all, 61, 1029), ExternalStorageSandbox.testPattern(61, 968));
        assertEquals(50 * 1024 * 1024, ExternalStorageSandbox.TEST_FILE_SIZE);
    }
}
