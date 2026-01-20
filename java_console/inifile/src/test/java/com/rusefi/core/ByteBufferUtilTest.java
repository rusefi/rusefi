package com.rusefi.core;

import org.junit.jupiter.api.Test;

import java.nio.ByteBuffer;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class ByteBufferUtilTest {

    private static void setShortValue(byte[] data8, int value, int offset) {
        data8[offset] = (byte) (value & 0xFF);
        data8[offset + 1] = (byte) (value >> 8);
    }

    @Test
    public void damnItWhichOneIsLittleEndian() {
        byte[] testArrayForLittleEndianByteBuffer = new byte[2];
        // LSB Little-endian System, "Intel"
        ByteBuffer littleEndianBuffer = ByteBufferUtil.littleEndianWrap(testArrayForLittleEndianByteBuffer, 0, 2);
        short testValue = 0x1122;
        littleEndianBuffer.putShort(testValue);
        assertEquals(testArrayForLittleEndianByteBuffer[0], 0x22);

        byte[] testArrayForJavaCodeWhichMimicsCanTxMessageSetShortValue = new byte[2];
        setShortValue(testArrayForJavaCodeWhichMimicsCanTxMessageSetShortValue, testValue, 0);

        assertEquals(testArrayForJavaCodeWhichMimicsCanTxMessageSetShortValue[0], 0x22);
        assertEquals(testArrayForJavaCodeWhichMimicsCanTxMessageSetShortValue[1], testArrayForLittleEndianByteBuffer[1]);
    }
}
