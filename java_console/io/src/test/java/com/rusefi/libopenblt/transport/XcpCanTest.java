package com.rusefi.libopenblt.transport;

import com.rusefi.io.can.CanAddress;
import com.rusefi.io.can.ClassicCanFrame;
import com.rusefi.io.can.RawCanPort;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class XcpCanTest {
    private static final CanAddress REQUEST = new CanAddress(0x10667, true);
    private static final CanAddress RESPONSE = new CanAddress(0x107e1, true);

    @Test
    void opensResponseFilterSendsRequestAndSkipsUnrelatedFrames() throws IOException {
        FakeRawCanPort port = new FakeRawCanPort();
        port.frames.add(new ClassicCanFrame(new CanAddress(0x123, false), new byte[]{9}));
        port.frames.add(new ClassicCanFrame(RESPONSE, new byte[]{1, 2}));
        XcpCan transport = new XcpCan(port, REQUEST, RESPONSE);

        transport.connect();
        byte[] response = transport.sendPacket(new byte[]{(byte) 0xff}, 100, 2);

        assertEquals(RESPONSE, port.openAddress);
        assertEquals(new ClassicCanFrame(REQUEST, new byte[]{(byte) 0xff}), port.sent.get(0));
        assertArrayEquals(new byte[]{1, 2}, response);
    }

    @Test
    void rejectsUnexpectedResponseLength() throws IOException {
        FakeRawCanPort port = new FakeRawCanPort();
        port.frames.add(new ClassicCanFrame(RESPONSE, new byte[]{1}));
        XcpCan transport = connected(port);

        assertThrows(IOException.class, () -> transport.sendPacket(new byte[]{1}, 100, 2));
    }

    @Test
    void timeoutDoesNotWaitAgainAfterPortReportsNoFrame() throws IOException {
        FakeRawCanPort port = new FakeRawCanPort();
        XcpCan transport = connected(port);

        assertThrows(IOException.class, () -> transport.sendPacket(new byte[]{1}, 100, 1));

        assertEquals(1, port.receiveTimeouts.size());
        assertTrue(port.receiveTimeouts.get(0) >= 0 && port.receiveTimeouts.get(0) <= 100);
    }

    @Test
    void disconnectClosesPortOnlyOnce() throws IOException {
        FakeRawCanPort port = new FakeRawCanPort();
        XcpCan transport = connected(port);

        transport.disconnect();
        transport.disconnect();

        assertEquals(1, port.closeCount);
        assertThrows(IllegalStateException.class, () -> transport.sendPacket(new byte[]{1}, 1, 1));
    }

    @Test
    void rejectsPayloadsThatCannotFitInClassicCan() throws IOException {
        XcpCan transport = connected(new FakeRawCanPort());

        assertThrows(IllegalArgumentException.class, () -> transport.sendPacket(new byte[9], 1, 1));
        assertThrows(IllegalArgumentException.class, () -> transport.sendPacket(new byte[1], 1, 9));
    }

    private static XcpCan connected(FakeRawCanPort port) throws IOException {
        XcpCan transport = new XcpCan(port, REQUEST, RESPONSE);
        transport.connect();
        return transport;
    }

    private static class FakeRawCanPort implements RawCanPort {
        final ArrayDeque<ClassicCanFrame> frames = new ArrayDeque<>();
        final List<ClassicCanFrame> sent = new ArrayList<>();
        final List<Integer> receiveTimeouts = new ArrayList<>();
        CanAddress openAddress;
        int closeCount;

        @Override
        public void open(CanAddress receiveAddress) {
            openAddress = receiveAddress;
        }

        @Override
        public void send(ClassicCanFrame frame) {
            sent.add(frame);
        }

        @Override
        public Optional<ClassicCanFrame> receive(int timeoutMs) {
            receiveTimeouts.add(timeoutMs);
            return frames.isEmpty() ? Optional.empty() : Optional.of(frames.remove());
        }

        @Override
        public void close() {
            closeCount++;
        }
    }
}
