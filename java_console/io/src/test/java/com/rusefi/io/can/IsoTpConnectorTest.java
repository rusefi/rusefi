package com.rusefi.io.can;

import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.io.can.isotp.IsoTpConnector;
import com.rusefi.util.HexBinary;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
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
}
