package com.rusefi.io.can;

import org.junit.jupiter.api.Test;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.io.serial.AbstractIoStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import static org.junit.jupiter.api.Assertions.*;

class SLCANConnectorTest {
    @Test
    void canableSetupUsesVersionBarriersAndCloseIsIdempotent() throws Exception {
        FakeSerial serial = new FakeSerial(true);
        SLCANConnector connector = new SLCANConnector("fake", 6, () -> serial);
        connector.open(new CanAddress(0x720, false));
        connector.close();
        connector.close();
        assertEquals(Arrays.asList("C", "V", "S6", "V", "O", "V", "C"), serial.commands);
        assertTrue(serial.isClosed());
    }

    @Test
    void lawicelSetupAndReceivePreservePartialLinesAndFilterBusTraffic() throws Exception {
        FakeSerial serial = new FakeSerial(false);
        try (SLCANConnector connector = new SLCANConnector("fake", 6, () -> serial)) {
            connector.open(new CanAddress(0x720, false));
            assertEquals(Arrays.asList("C", "V", "S6", "O"), serial.commands);
            serial.reply("z\rT00000720201AA\rr7208\rt123201AA\rt7202");
            assertFalse(connector.receive(10).isPresent());
            serial.reply("01AA\r");
            assertEquals(new ClassicCanFrame(new CanAddress(0x720, false), new byte[]{1, (byte) 0xaa}),
                connector.receive(100).get());
            serial.reply("\u0007");
            assertThrows(IOException.class, () -> connector.receive(100));
        }
    }

    @Test
    void failedSetupReleasesSerialPort() {
        FakeSerial serial = new FakeSerial(false);
        serial.rejectOpen = true;
        SLCANConnector connector = new SLCANConnector("fake", 6, () -> serial);
        assertThrows(IOException.class, () -> connector.open(new CanAddress(0x720, false)));
        assertTrue(serial.isClosed());
    }

    private static final class FakeSerial extends AbstractIoStream {
        final IncomingDataBuffer buffer = new IncomingDataBuffer("SLCAN test", getStreamStats());
        final List<String> commands = new ArrayList<>();
        final boolean canable;
        boolean rejectOpen;

        FakeSerial(boolean canable) {
            this.canable = canable;
        }

        @Override
        public IncomingDataBuffer getDataBuffer() {
            return buffer;
        }

        @Override
        public void setInputListener(DataListener listener) {
        }

        void reply(String text) {
            buffer.addData(text.getBytes(StandardCharsets.US_ASCII));
        }

        @Override
        public void write(byte[] bytes) {
            String command = new String(bytes, StandardCharsets.US_ASCII).trim();
            commands.add(command);
            if (command.equals("V")) {
                reply(canable ? "16e7497-dirty github.com/normaldotcom/canable2.git\r" : "V1220\r");
            } else if (rejectOpen && command.equals("O")) {
                reply("\u0007");
            } else if (!canable) {
                reply("\r");
            }
        }
    }

    @Test
    void encodesIsoTpHelloAndExtendedFrames() {
        assertEquals("t1238070001532060EFC3", SLCANConnector.encode(new ClassicCanFrame(
            new CanAddress(0x123, false), new byte[]{7, 0, 1, 0x53, 0x20, 0x60, (byte) 0xef, (byte) 0xc3})));
        assertEquals("T001ABCDE201FF", SLCANConnector.encode(new ClassicCanFrame(
            new CanAddress(0x1abcde, true), new byte[]{1, (byte) 0xff})));
    }

    @Test
    void decodesClassicFramesWithOptionalTimestamp() {
        ClassicCanFrame expected = new ClassicCanFrame(new CanAddress(0x123, false), new byte[]{0x30, 0, 0});
        assertEquals(expected, SLCANConnector.decode("t1233300000").get());
        assertEquals(expected, SLCANConnector.decode("t1233300000ABCD").get());
        assertEquals(new CanAddress(0x1abcde, true), SLCANConnector.decode("T001ABCDE0").get().getAddress());
    }

    @Test
    void ignoresAcksRtrFdAndMalformedFrames() {
        for (String value : new String[]{"", "z", "Z", "r1238", "d1230", "tFFF0", "TFFFFFFFF0",
                "t1239", "t1232AA", "t1231GG", "t123100123", "t123100ZZZZ"}) {
            assertFalse(SLCANConnector.decode(value).isPresent(), value);
        }
    }

    @Test
    void recognizesLawicelAndActualCanableBanner() {
        assertTrue(SLCANConnector.isVersion("V1220"));
        assertTrue(SLCANConnector.isVersion("16e7497-dirty github.com/normaldotcom/canable2.git"));
        assertFalse(SLCANConnector.isVersion("V"));
        assertFalse(SLCANConnector.isVersion("rusEFI master"));
    }
}
