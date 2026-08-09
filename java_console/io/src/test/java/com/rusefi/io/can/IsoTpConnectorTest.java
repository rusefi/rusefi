package com.rusefi.io.can;

import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.io.can.isotp.IsoTpConnector;
import com.rusefi.util.HexBinary;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class IsoTpConnectorTest {
    @Test
    public void testConnector() {
        byte[] crcWrappedCrcRequest = new byte[]{
                0, 5, 107, 0, 0, 80, 95, 105, -81, -96, 112};

        List<String> packets = new ArrayList<>();

        IsoTpConnector testConnector = new IsoTpConnector(VariableRegistryValues.CAN_ECU_SERIAL_RX_ID) {
            @Override
            public void sendCanData(byte[] total) {
                String packetAsString = HexBinary.printHexBinary(total);
                packets.add(packetAsString);
            }
        };

        IsoTpConnector.sendStrategy(crcWrappedCrcRequest, testConnector);

        assertEquals(2, packets.size());
        assertEquals("10 0B 00 05 6B 00 00 50 ", packets.get(0));
        assertEquals("21 5F 69 AF A0 70 ", packets.get(1));
    }

    @Test
    public void testFlowControlWaitCalledBetweenFirstAndConsecutive() {
        // 13 bytes -> FIRST (6 bytes) + one CONSECUTIVE frame (7 bytes)
        byte[] payload = new byte[13];
        for (int i = 0; i < payload.length; i++) {
            payload[i] = (byte) i;
        }

        List<String> packets = new ArrayList<>();
        // 0 = sendCanData (frame sent), 1 = receiveData (flow control wait)
        List<Integer> events = new ArrayList<>();

        IsoTpConnector testConnector = new IsoTpConnector(VariableRegistryValues.CAN_ECU_SERIAL_RX_ID) {
            @Override
            public void sendCanData(byte[] total) {
                events.add(0);
                packets.add(HexBinary.printHexBinary(total));
            }

            @Override
            public void receiveData() {
                events.add(1);
            }
        };

        IsoTpConnector.sendStrategy(payload, testConnector);

        // FIRST frame, then flow control wait, then the consecutive burst
        assertEquals(Arrays.asList(0, 1, 0), events);
        assertEquals(2, packets.size());
        assertEquals("10 0D 00 01 02 03 04 05 ", packets.get(0));
        assertEquals("21 06 07 08 09 0A 0B 0C ", packets.get(1));
    }
}
