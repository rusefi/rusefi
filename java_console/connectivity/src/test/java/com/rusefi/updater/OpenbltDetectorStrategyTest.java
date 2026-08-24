package com.rusefi.updater;

import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.io.serial.AbstractIoStream;
import org.junit.jupiter.api.Test;

import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class OpenbltDetectorStrategyTest {
    private static final byte XCP_CMD_CONNECT = (byte) 0xFF;
    private static final byte XCP_CMD_GET_SIGNATURE = (byte) 0xBF;
    private static final byte XCP_CMD_GET_VERSION = '?';

    private static class FakeOpenbltStream extends AbstractIoStream {
        private final IncomingDataBuffer dataBuffer = new IncomingDataBuffer("test", getStreamStats());
        private final String signature;
        // banner returned for the '?' probe; null simulates a legacy bootloader
        // that does not understand the new command (no reply -> read timeout).
        private final String versionBanner;
        private final List<byte[]> writes = new ArrayList<>();

        private FakeOpenbltStream(String signature, String versionBanner) {
            this.signature = signature;
            this.versionBanner = versionBanner;
        }

        @Override
        public void write(byte[] bytes) {
            writes.add(bytes.clone());
            byte cmd = bytes.length >= 2 ? bytes[1] : bytes[0];
            if (cmd == XCP_CMD_CONNECT) {
                dataBuffer.addData(new byte[]{8, (byte) 0xFF, 0, 0, 0, 0, 0, 0, 0});
            } else if (cmd == XCP_CMD_GET_VERSION) {
                if (versionBanner == null) {
                    return; // legacy bootloader silently ignores the probe
                }
                byte[] text = versionBanner.getBytes(StandardCharsets.US_ASCII);
                byte[] response = new byte[text.length + 1];
                response[0] = (byte) text.length;
                System.arraycopy(text, 0, response, 1, text.length);
                dataBuffer.addData(response);
            } else if (cmd == XCP_CMD_GET_SIGNATURE) {
                if (signature == null) {
                    return;
                }
                byte[] text = signature.getBytes(StandardCharsets.US_ASCII);
                byte[] response = new byte[text.length + 2];
                response[0] = (byte) (text.length + 1);
                response[1] = (byte) 0xFF;
                System.arraycopy(text, 0, response, 2, text.length);
                dataBuffer.addData(response);
            }
        }

        @Override
        public void setInputListener(DataListener listener) {
        }

        @Override
        public IncomingDataBuffer getDataBuffer() {
            return dataBuffer;
        }
    }

    @Test
    public void readsBoardSignatureAndVersionAndFeatures() throws Exception {
        FakeOpenbltStream stream = new FakeOpenbltStream("rusefi.uaefi", "blt 1.16.0 build=42 features=custom_led,example_feature note=ignored");

        OpenbltDetectorStrategy.OpenbltInfo info =
            OpenbltDetectorStrategy.detectOpenbltWithSignature(stream);

        assertTrue(info.isOpenblt);
        assertEquals("rusefi", info.brand);
        assertEquals("uaefi", info.board);
        assertEquals("rusefi.uaefi", info.raw);
        assertEquals("1.16.0", info.version);
        assertEquals(List.of("custom_led", "example_feature"), info.features);
        // connect, '?' probe, 0xBF signature
        assertArrayEquals(new byte[]{2, (byte) 0xFF, 0}, stream.writes.get(0));
        assertArrayEquals(new byte[]{1, XCP_CMD_GET_VERSION}, stream.writes.get(1));
        assertArrayEquals(new byte[]{1, XCP_CMD_GET_SIGNATURE}, stream.writes.get(2));
    }

    @Test
    public void bootloaderWithoutFeatures() throws Exception {
        FakeOpenbltStream stream = new FakeOpenbltStream("rusefi.uaefi", "blt 1.16.0");

        OpenbltDetectorStrategy.OpenbltInfo info =
            OpenbltDetectorStrategy.detectOpenbltWithSignature(stream);

        assertTrue(info.isOpenblt);
        assertEquals("1.16.0", info.version);
        assertNull(info.features);
    }

    @Test
    public void legacyBootloaderWithoutProbeSupportStillDetected() throws Exception {
        // legacy reply: no '?' banner at all -> version/features fall back to null
        FakeOpenbltStream stream = new FakeOpenbltStream("rusefi.uaefi", null);

        OpenbltDetectorStrategy.OpenbltInfo info =
            OpenbltDetectorStrategy.detectOpenbltWithSignature(stream);

        assertTrue(info.isOpenblt);
        assertEquals("rusefi", info.brand);
        assertNull(info.version);
        assertNull(info.features);
    }

    @Test
    public void bannerRemainsAvailableWhenOptionalSignatureTimesOut() throws Exception {
        FakeOpenbltStream stream = new FakeOpenbltStream(null, "blt 1.16.0 features=example_feature");

        OpenbltDetectorStrategy.OpenbltInfo info =
            OpenbltDetectorStrategy.detectOpenbltWithSignature(stream);

        assertTrue(info.isOpenblt);
        assertNull(info.raw);
        assertEquals("1.16.0", info.version);
        assertEquals(List.of("example_feature"), info.features);
    }

    @Test
    public void malformedSignatureDoesNotClaimABoard() throws Exception {
        OpenbltDetectorStrategy.OpenbltInfo info =
            OpenbltDetectorStrategy.detectOpenbltWithSignature(new FakeOpenbltStream("uaefi", null));

        assertTrue(info.isOpenblt);
        assertNull(info.brand);
        assertNull(info.board);
    }

    @Test
    public void silentLegacyBootloaderRemainsDetected() throws Exception {
        OpenbltDetectorStrategy.OpenbltInfo info =
            OpenbltDetectorStrategy.detectOpenbltWithSignature(new FakeOpenbltStream(null, null));

        assertTrue(info.isOpenblt);
        assertNull(info.raw);
        assertNull(info.board);
    }
}
