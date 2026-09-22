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
    public void requestedPortRecoversBeforeVersionProbeWithoutBinaryProbe() throws Exception {
        FakeStream stream = new FakeStream();
        // If a TunerStudio binary probe were attempted, this stream would identify as a console.
        stream.console = true;
        try (SlcanClient client = SlcanClient.connectExplicit(stream, "COM42", line -> {})) {
            assertEquals("COM42", client.getPort());
            assertEquals("V1220", client.getVersion());
            assertEquals(0, stream.binaryProbeAttempts);
            assertEquals(Arrays.asList("C", "V", "S6", "O"), stream.commands);
            client.pollStatus();
            assertEquals("F00", client.readLine(10));
        }
        assertEquals(Arrays.asList("C", "V", "S6", "O", "F", "C"), stream.commands);
        assertTrue(stream.isClosed());
    }

    @Test
    public void autoDiscoveryStillProbesForConsole() throws Exception {
        FakeStream stream = new FakeStream();
        try (SlcanClient client = SlcanClient.connect(stream, "COM42", line -> {})) {
            assertEquals(1, stream.binaryProbeAttempts);
            assertEquals(Arrays.asList("V", "C", "S6", "O"), stream.commands);
        }
    }

    @Test
    public void initializationFailureClosesPort() {
        FakeStream stream = new FakeStream();
        stream.rejectOpen = true;
        assertThrows(IOException.class, () -> SlcanClient.connectExplicit(stream, "COM42", line -> {}));
        assertEquals(Arrays.asList("C", "V", "S6", "O"), stream.commands);
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

    @Test
    public void busIdentityAndLegacyAmbiguity() {
        for (int bus = 0; bus < 3; bus++) {
            String prefix = new String[]{"", "&", "$"}[bus];
            for (String frame : new String[]{"t1232AABB", "T000001232AABB", "r1238", "R000001238"}) {
                SlcanClient.Frame parsed = SlcanClient.Frame.parse(prefix + frame + "ABCD");
                assertNotNull(parsed);
                assertEquals(Integer.valueOf(bus), parsed.busIndex);
                assertEquals(prefix + frame + "ABCD", parsed.raw);
                assertEquals("ABCD", parsed.timestamp);
                assertEquals(0x123, parsed.id);
                assertTrue(parsed.decode().startsWith("CAN" + (bus + 1) + " "));
            }
        }
        SlcanClient.Frame legacy = SlcanClient.Frame.parse("t1232AABB");
        assertEquals(Integer.valueOf(0), legacy.busIndex);
        assertEquals(Integer.valueOf(1), SlcanClient.Frame.parse("&t1230").busIndex);
        assertEquals(Integer.valueOf(2), SlcanClient.Frame.parse("$t1230").busIndex);
        for (String bad : new String[]{"&", "$", "&&t1230", "$&t1230", "&V1220", "$t1232AA"}) {
            assertNull(SlcanClient.Frame.parse(bad), bad);
        }
    }

    private static class FakeStream extends AbstractIoStream {
        final IncomingDataBuffer buffer = new IncomingDataBuffer("slcan-test", getStreamStats());
        final List<String> commands = new ArrayList<>();
        boolean console;
        boolean rejectOpen;
        int binaryProbeAttempts;
        String formatReply = "I1\r";

        @Override
        public IncomingDataBuffer getDataBuffer() { return buffer; }

        @Override
        public void setInputListener(DataListener listener) { }

        @Override
        public void write(byte[] bytes) throws IOException {
            if (bytes[0] == 0) {
                binaryProbeAttempts++;
                if (console) {
                    buffer.addData(IoHelper.makeCrc32Packet(
                            "\u0000rusEFI test".getBytes(StandardCharsets.US_ASCII)));
                    return;
                }
                throw new IOException("No TS response");
            }
            String command = new String(bytes, StandardCharsets.US_ASCII).trim();
            commands.add(command);
            if ("I".equals(command)) {
                if (formatReply != null) {
                    buffer.addData(formatReply.getBytes(StandardCharsets.US_ASCII));
                }
                return;
            }
            String reply = "V".equals(command) ? "V1220\r"
                    : "F".equals(command) ? "F00\r"
                    : rejectOpen && "O".equals(command) ? "\u0007" : "\r";
            buffer.addData(reply.getBytes(StandardCharsets.US_ASCII));
        }
    }
}
