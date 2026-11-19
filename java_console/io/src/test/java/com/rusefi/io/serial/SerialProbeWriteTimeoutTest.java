package com.rusefi.io.serial;

import com.fazecast.jSerialComm.SerialPort;
import com.rusefi.io.IoStream;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class SerialProbeWriteTimeoutTest {
    @Test
    void probeWriteTimeoutPolicyBeforeInitialConnect() throws Exception {
        // Initial OpenBLT CONNECT needs a finite native TX timeout before its first write.
        // This fake records native calls; it does not simulate Windows driver timing.
        RecordingPort port = new RecordingPort();
        try (IoStream stream = BufferedSerialIoStream.openPort("FAKE_PROBE", true, port)) {
            assertNotNull(stream);
            assertNotNull(stream.getDataBuffer(), "probe retains buffered input");
            stream.write(new byte[]{2, (byte) 0xFF, 0});
            assertEquals(1, port.timeoutCalls);
            assertEquals(SerialPort.TIMEOUT_WRITE_BLOCKING, port.mode);
            assertEquals(0, port.readTimeoutMs);
            assertEquals(500, port.writeTimeoutMs);
            assertEquals(Arrays.asList("baud", "open", "timeouts", "flush", "write"), port.calls);
            assertArrayEquals(new byte[]{2, (byte) 0xFF, 0}, port.written);
        }
        assertEquals(1, port.closeCalls);
    }

    @Test
    void ordinaryRuntimeOpenerKeepsExistingTimeoutPolicy() throws Exception {
        RecordingPort port = new RecordingPort();
        try (IoStream stream = BufferedSerialIoStream.openPort("FAKE_RUNTIME", false, port)) {
            assertNotNull(stream);
            stream.write(new byte[]{1, 2, 3});
            assertEquals(0, port.timeoutCalls);
            assertEquals(Arrays.asList("baud", "open", "flush", "write"), port.calls);
        }
        assertEquals(1, port.closeCalls);
    }

    @Test
    void posixProbeKeepsExistingNonblockingWritePolicy() throws Exception {
        // POSIX ignores the TX budget and WRITE_BLOCKING adds an unbounded tcdrain.
        RecordingPort port = new RecordingPort();
        port.supportsWriteTimeout = false;
        try (IoStream stream = BufferedSerialIoStream.openPort("FAKE_POSIX_PROBE", true, port)) {
            assertNotNull(stream);
            stream.write(new byte[]{2, (byte) 0xFF, 0});
            assertEquals(0, port.timeoutCalls);
            assertEquals(0, port.mode);
            assertEquals(Arrays.asList("baud", "open", "flush", "write"), port.calls);
            assertArrayEquals(new byte[]{2, (byte) 0xFF, 0}, port.written);
        }
        assertEquals(1, port.closeCalls);
    }

    @Test
    void posixRuntimeOpenerKeepsExistingTimeoutPolicy() throws Exception {
        RecordingPort port = new RecordingPort();
        port.supportsWriteTimeout = false;
        try (IoStream stream = BufferedSerialIoStream.openPort("FAKE_POSIX_RUNTIME", false, port)) {
            assertNotNull(stream);
            stream.write(new byte[]{1, 2, 3});
            assertEquals(0, port.timeoutCalls);
            assertEquals(0, port.mode);
            assertEquals(Arrays.asList("baud", "open", "flush", "write"), port.calls);
        }
        assertEquals(1, port.closeCalls);
    }

    @Test
    void rejectedProbeTimeoutClosesOwnedPortWithoutFlushingOrWriting() {
        RecordingPort port = new RecordingPort();
        port.acceptTimeouts = false;

        assertNull(BufferedSerialIoStream.openPort("FAKE_REJECTED", true, port));

        assertEquals(1, port.closeCalls);
        assertNull(port.written);
        assertEquals(Arrays.asList("baud", "open", "timeouts", "close"), port.calls);
    }

    @Test
    void throwingTimeoutConfigurationAlsoClosesOwnedPort() {
        RecordingPort port = new RecordingPort();
        port.timeoutFailure = new IllegalStateException("fake native configuration failure");

        assertSame(port.timeoutFailure, assertThrows(IllegalStateException.class,
            () -> BufferedSerialIoStream.openPort("FAKE_CONFIG_FAILURE", true, port)));

        assertEquals(1, port.closeCalls);
        assertNull(port.written);
        assertEquals(Arrays.asList("baud", "open", "timeouts", "close"), port.calls);
    }

    @Test
    void unsuccessfulOpenDoesNotConfigureOrCloseAnUnownedPort() {
        RecordingPort port = new RecordingPort();
        port.opened = false;

        assertNull(BufferedSerialIoStream.openPort("FAKE_UNAVAILABLE", true, port));

        assertEquals(0, port.timeoutCalls);
        assertEquals(0, port.closeCalls);
        assertEquals(Arrays.asList("baud", "open"), port.calls);
    }

    @Test
    void shortZeroAndFailedNativeWritesThrowAndReleaseProbeOwnership() throws Exception {
        for (int written : new int[]{2, 0, -1}) {
            RecordingPort port = new RecordingPort();
            port.writeResult = written;
            try (IoStream stream = BufferedSerialIoStream.openPort("FAKE_SHORT_WRITE", true, port)) {
                assertNotNull(stream);
                IOException error = assertThrows(IOException.class,
                    () -> stream.write(new byte[]{2, (byte) 0xFF, 0}));
                assertEquals("write failed: wrote " + written + " but expected 3", error.getMessage());
                assertEquals(0, port.closeCalls, "write failure does not bypass the owner's cleanup");
            }
            assertEquals(1, port.closeCalls);
            assertEquals(Arrays.asList("baud", "open", "timeouts", "flush", "write", "close"), port.calls);
        }
    }

    private static class RecordingPort extends SerialIoStream.NativePort {
        private final List<String> calls = new ArrayList<>();
        private int timeoutCalls;
        private int mode;
        private int readTimeoutMs;
        private int writeTimeoutMs;
        private int closeCalls;
        private byte[] written;
        private boolean opened = true;
        private boolean supportsWriteTimeout = true;
        private boolean acceptTimeouts = true;
        private RuntimeException timeoutFailure;
        private Integer writeResult;

        private RecordingPort() {
            super(null);
        }

        @Override
        void setBaudRate(int baudRate) {
            calls.add("baud");
            assertEquals(BaudRateHolder.INSTANCE.baudRate, baudRate);
        }

        @Override
        boolean openPort() {
            calls.add("open");
            return opened;
        }

        @Override
        boolean supportsWriteTimeout() {
            return supportsWriteTimeout;
        }

        @Override
        boolean setComPortTimeouts(int mode, int readTimeoutMs, int writeTimeoutMs) {
            calls.add("timeouts");
            timeoutCalls++;
            this.mode = mode;
            this.readTimeoutMs = readTimeoutMs;
            this.writeTimeoutMs = writeTimeoutMs;
            if (timeoutFailure != null) {
                throw timeoutFailure;
            }
            return acceptTimeouts;
        }

        @Override
        void flushIOBuffers() {
            calls.add("flush");
        }

        @Override
        void closePort() {
            calls.add("close");
            closeCalls++;
        }

        @Override
        int writeBytes(byte[] bytes) {
            calls.add("write");
            written = bytes.clone();
            return writeResult == null ? bytes.length : writeResult;
        }
    }
}
