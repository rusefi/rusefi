package com.rusefi.io.can.isotp;

import com.rusefi.io.can.IsoTpConnectorTest;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * @see IsoTpConnectorTest for opposite
 */
public class IsoTpCanDecoderTest {
    @Test
    public void decodeSingleFrame() {
        IsoTpCanDecoder decoder = new TestIsoTpCanDecoder();
        byte[] result1 = decoder.decodePacket(new byte[]{0x4, 1, 2, 3, 4});
        assertArrayEquals(new byte[]{1, 2, 3, 4}, result1);
    }

    @Test
    public void decodeMultiFrame() {
        IsoTpCanDecoder decoder = new TestIsoTpCanDecoder();
        byte[] result1 = decoder.decodePacket(new byte[]{0x10, 0x0B, 0x59, 0x02, (byte) 0x89, (byte) 0xA1, 0x41, 0x00});
        assertArrayEquals(new byte[]{0x59, 0x02, (byte) 0x89, (byte) 0xA1, 0x41, 0x00}, result1);

        byte[] result2 = decoder.decodePacket(new byte[]{0x21, 0x08, (byte) 0xA1, 0x46, 0x00, 0x08, 0x00, 0x00});
        assertArrayEquals(new byte[]{0x08, (byte) 0xA1, 0x46, 0x00, 0x08}, result2);
    }

    @Test
    public void decodeSingleFrameExt() {
        IsoTpCanDecoder decoder = new TestIsoTpCanDecoder(1);
        byte[] result1 = decoder.decodePacket(new byte[]{0x18, 0x03, 0x22, (byte) 0xF1, (byte) 0x90, 0x00, 0, 0});
        assertArrayEquals(new byte[]{0x22, (byte) 0xF1, (byte) 0x90}, result1);
    }

    @Test
    public void decodeMultiFrameExt() {
        IsoTpCanDecoder decoder = new TestIsoTpCanDecoder(1);
        byte[] result1 = decoder.decodePacket(new byte[]{(byte) 0xF1, 0x10, 0x14, 0x62, (byte) 0xF1, (byte) 0x90, 0x57, 0x42});
        assertArrayEquals(new byte[]{0x62, (byte) 0xF1, (byte) 0x90, 0x57, 0x42}, result1);

        byte[] result2 = decoder.decodePacket(new byte[]{(byte) 0xF1, 0x21, 0x41, 0x33, 0x43, 0x31, 0x43, 0x35});
        assertArrayEquals(new byte[]{0x41, 0x33, 0x43, 0x31, 0x43, 0x35}, result2);
    }
}
