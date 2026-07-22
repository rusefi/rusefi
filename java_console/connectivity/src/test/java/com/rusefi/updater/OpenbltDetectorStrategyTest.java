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
    private static class FakeOpenbltStream extends AbstractIoStream {
        private final IncomingDataBuffer dataBuffer = new IncomingDataBuffer("test", getStreamStats());
        private final String signature;
        private final List<byte[]> writes = new ArrayList<>();

        private FakeOpenbltStream(String signature) {
            this.signature = signature;
        }

        @Override
        public void write(byte[] bytes) {
            writes.add(bytes.clone());
            if (writes.size() == 1) {
                dataBuffer.addData(new byte[]{8, (byte) 0xFF, 0, 0, 0, 0, 0, 0, 0});
            } else {
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
    public void readsBoardSignatureAfterConnecting() throws Exception {
        FakeOpenbltStream stream = new FakeOpenbltStream("rusefi.uaefi");

        OpenbltDetectorStrategy.OpenbltInfo info =
            OpenbltDetectorStrategy.detectOpenbltWithSignature(stream);

        assertTrue(info.isOpenblt);
        assertEquals("rusefi", info.brand);
        assertEquals("uaefi", info.board);
        assertEquals("rusefi.uaefi", info.raw);
        assertArrayEquals(new byte[]{2, (byte) 0xFF, 0}, stream.writes.get(0));
        assertArrayEquals(new byte[]{1, (byte) 0xBF}, stream.writes.get(1));
    }

    @Test
    public void malformedSignatureDoesNotClaimABoard() throws Exception {
        OpenbltDetectorStrategy.OpenbltInfo info =
            OpenbltDetectorStrategy.detectOpenbltWithSignature(new FakeOpenbltStream("uaefi"));

        assertTrue(info.isOpenblt);
        assertNull(info.brand);
        assertNull(info.board);
    }

    @Test
    public void silentLegacyBootloaderRemainsDetected() throws Exception {
        OpenbltDetectorStrategy.OpenbltInfo info =
            OpenbltDetectorStrategy.detectOpenbltWithSignature(new FakeOpenbltStream(null));

        assertTrue(info.isOpenblt);
        assertNull(info.raw);
        assertNull(info.board);
    }
}
