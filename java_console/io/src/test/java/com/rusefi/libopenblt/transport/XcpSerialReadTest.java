package com.rusefi.libopenblt.transport;

import org.junit.jupiter.api.Test;
import java.io.IOException;
import java.util.Arrays;
import static org.junit.jupiter.api.Assertions.*;

class XcpSerialReadTest {
    @Test void readsUnsignedResponseLength() throws Exception {
        for (int length : new int[]{127, 128, 193, 240, 255}) {
            byte[] response = XcpSerial.readResponse((byte) length, data -> {
                Arrays.fill(data, (byte) 0xff);
                return data.length;
            });
            assertEquals(length, response.length);
            assertEquals((byte) 0xff, response[length - 1]);
        }
    }

    @Test void rejectsShortAndEmptyResponses() {
        assertThrows(IOException.class, () -> XcpSerial.readResponse((byte) 20, data -> 4));
        assertThrows(IOException.class, () -> XcpSerial.readResponse((byte) 20, data -> -1));
        assertThrows(IOException.class, () -> XcpSerial.readResponse((byte) 0, data -> 0));
    }
}
