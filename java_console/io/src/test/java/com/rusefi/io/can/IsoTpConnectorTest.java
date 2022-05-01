package com.rusefi.io.can;

import com.rusefi.io.IoStream;
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

                String packetAsString = IoStream.printHexBinary(Elm327Connector.byteToString(total));
                packets.add(packetAsString);

            }

            @Override
            public void receiveData() {
            }
        };

        IsoTpConnector.sendStrategy(crcWrappedCrcRequest, testConnector);

        assertEquals(2, packets.size());
        //                       10    0B                                  50
        assertEquals("31 30 30 42 30 30 30 35 36 42 30 30 30 30 35 30 0D ", packets.get(0));
        //                       21    5F    69    AF    A0    70
        assertEquals("32 31 35 46 36 39 41 46 41 30 37 30 0D ", packets.get(1));
    }
}
