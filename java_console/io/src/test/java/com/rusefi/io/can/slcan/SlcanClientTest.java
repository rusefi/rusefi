package com.rusefi.io.can.slcan;

import org.junit.jupiter.api.Test;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.IoHelper;
import com.rusefi.io.serial.AbstractIoStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import static org.junit.jupiter.api.Assertions.*;

public class SlcanClientTest {
    @Test
    public void testParseFrame() {
        assertNotNull(SlcanClient.Frame.parse("t1230"));
        assertNotNull(SlcanClient.Frame.parse("T123456780"));
        assertNotNull(SlcanClient.Frame.parse("r1230"));
        assertNotNull(SlcanClient.Frame.parse("R123456780"));

        // Valid frame with data
        SlcanClient.Frame f = SlcanClient.Frame.parse("t1232AABB");
        assertNotNull(f);
        assertEquals(0x123, f.id);
        assertEquals(2, f.dlc);
        assertArrayEquals(new byte[]{(byte)0xAA, (byte)0xBB}, f.data);

        // Invalid frames
        assertNull(SlcanClient.Frame.parse("V123"));
        assertNull(SlcanClient.Frame.parse(""));
        assertNull(SlcanClient.Frame.parse("x1230"));
    }

    @Test
    public void explicitStreamInitializesAndClosesLogicalChannel() throws Exception {
        FakeStream stream = new FakeStream();
        try (SlcanClient client = SlcanClient.connect(stream, "COM42", line -> {})) {
            assertEquals("COM42", client.getPort());
            assertEquals("V1220", client.getVersion());
            assertEquals(Arrays.asList("V", "C", "S6", "O"), stream.commands);
            client.pollStatus();
            assertEquals("F00", client.readLine(10));
        }
        assertEquals(Arrays.asList("V", "C", "S6", "O", "F", "C"), stream.commands);
        assertTrue(stream.isClosed());
    }

    @Test
    public void initializationFailureClosesPort() {
        FakeStream stream = new FakeStream();
        stream.rejectOpen = true;
        assertThrows(IOException.class, () -> SlcanClient.connect(stream, "COM42", line -> {}));
        assertEquals(Arrays.asList("V", "C", "S6", "O"), stream.commands);
        assertTrue(stream.isClosed());
    }

    @Test
    public void consolePortIsRejectedBeforeSendingSlcanCommands() {
        FakeStream stream = new FakeStream();
        stream.console = true;
        assertThrows(IOException.class, () -> SlcanClient.connect(stream, "COM42", line -> {}));
        assertTrue(stream.commands.isEmpty());
        assertTrue(stream.isClosed());
    }

    private static class FakeStream extends AbstractIoStream {
        final IncomingDataBuffer buffer = new IncomingDataBuffer("slcan-test", getStreamStats());
        final List<String> commands = new ArrayList<>();
        boolean console;
        boolean rejectOpen;

        @Override
        public IncomingDataBuffer getDataBuffer() { return buffer; }

        @Override
        public void setInputListener(DataListener listener) { }

        @Override
        public void write(byte[] bytes) throws IOException {
            if (bytes[0] == 0) {
                if (console) {
                    buffer.addData(IoHelper.makeCrc32Packet(
                            "\u0000rusEFI test".getBytes(StandardCharsets.US_ASCII)));
                    return;
                }
                throw new IOException("No TS response");
            }
            String command = new String(bytes, StandardCharsets.US_ASCII).trim();
            commands.add(command);
            String reply = "V".equals(command) ? "V1220\r"
                    : "F".equals(command) ? "F00\r"
                    : rejectOpen && "O".equals(command) ? "\u0007" : "\r";
            buffer.addData(reply.getBytes(StandardCharsets.US_ASCII));
        }
    }
}
