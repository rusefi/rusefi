package com.rusefi.io.can.isotp;

import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.assertTrue;

public class IsoTpCanDecoderTest {
    @Test
    public void decodeSingleFrame() {
        IsoTpCanDecoder decoder = new IsoTpCanDecoder();
        byte[] result1 = decoder.decodePacket(new byte[]{0x4, 1, 2, 3, 4});
        assertTrue(Arrays.equals(result1, new byte[]{1, 2, 3, 4}));
    }

    @Test
    public void decodeMultiFrame() {
        IsoTpCanDecoder decoder = new IsoTpCanDecoder();
        byte[] result1 = decoder.decodePacket(new byte[]{0x10, 0x0B, 0x59, 0x02, (byte) 0x89, (byte) 0xA1, 0x41, 0x00  });
        assertTrue(Arrays.equals(result1, new byte[]{ 0x59, 0x02, (byte) 0x89, (byte) 0xA1, 0x41, 0x00}));

        byte[] result2 = decoder.decodePacket(new byte[]{0x21, 0x08, (byte) 0xA1, 0x46, 0x00, 0x08, 0x00, 0x00  });
        assertTrue(Arrays.equals(result2, new byte[]{ 0x08, (byte) 0xA1, 0x46, 0x00, 0x08}));
    }
}
