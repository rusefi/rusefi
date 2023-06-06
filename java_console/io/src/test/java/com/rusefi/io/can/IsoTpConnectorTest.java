package com.rusefi.io.can;

import com.rusefi.util.HexBinary;
import org.junit.Test;

import java.util.ArrayList;
import java.util.List;

import static org.junit.Assert.assertEquals;

public class IsoTpConnectorTest {
    @Test
    public void testConnector() {
        byte[] crcWrappedCrcRequest = new byte[]{
                0, 5, 107, 0, 0, 80, 95, 105, -81, -96, 112};

        List<String> packets = new ArrayList<>();

        IsoTpConnector testConnector = new IsoTpConnector() {
            @Override
            public void sendCanData(byte[] hdr, byte[] data, int dataOffset, int dataLength) {
                byte[] total = combineArrays(hdr, data, dataOffset, dataLength);

                String packetAsString = HexBinary.printHexBinary(total);
                packets.add(packetAsString);
            }

            @Override
            public void receiveData() {
            }
        };

        IsoTpConnector.sendStrategy(crcWrappedCrcRequest, testConnector);

        assertEquals(2, packets.size());
        assertEquals("10 0B 00 05 6B 00 00 50 ", packets.get(0));
        assertEquals("21 5F 69 AF A0 70 ", packets.get(1));
    }
}
