package com.rusefi.binaryprotocol.test;

import com.rusefi.binaryprotocol.IoHelper;
import org.junit.Test;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;

import static org.junit.Assert.assertEquals;

public class IoHelperTest {
    @Test
    public void testSwap16() throws IOException {
        byte packet[] = {-12, 0};
        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(packet));
        short a = dis.readShort();
        int x = IoHelper.swap16(a);
        assertEquals(244, x);
    }
}
