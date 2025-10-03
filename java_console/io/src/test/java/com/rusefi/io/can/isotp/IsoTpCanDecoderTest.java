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
        byte[] result1 = decoder.decodePacket(new byte[]{0x14, 1, 2, 3, 4});

    }
}
