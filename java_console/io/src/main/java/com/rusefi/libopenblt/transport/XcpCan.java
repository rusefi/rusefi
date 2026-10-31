package com.rusefi.libopenblt.transport;

import com.rusefi.io.can.CanAddress;
import com.rusefi.io.can.ClassicCanFrame;
import com.rusefi.io.can.RawCanPort;

import java.io.IOException;
import java.util.Arrays;
import java.util.Optional;
import java.util.concurrent.TimeUnit;

/** XCP transport carried in one Classic CAN frame per request and response. */
public class XcpCan implements IXcpTransport {
    private static final int MAX_PAYLOAD_BYTES = 8;

    private final Object lock = new Object();
    private final RawCanPort port;
    private final CanAddress requestAddress;
    private final CanAddress responseAddress;
    private boolean connected;

    public XcpCan(RawCanPort port, CanAddress requestAddress, CanAddress responseAddress) {
        this.port = java.util.Objects.requireNonNull(port, "port");
        this.requestAddress = java.util.Objects.requireNonNull(requestAddress, "requestAddress");
        this.responseAddress = java.util.Objects.requireNonNull(responseAddress, "responseAddress");
    }

    @Override
    public void connect() throws IOException {
        synchronized (lock) {
            if (connected) {
                throw new IllegalStateException("Cannot connect when already connected");
            }
            port.open(responseAddress);
            connected = true;
        }
    }

    @Override
    public void disconnect() throws IOException {
        synchronized (lock) {
            if (connected) {
                try {
                    port.close();
                } finally {
                    connected = false;
                }
            }
        }
    }

    @Override
    public byte[] sendPacket(byte[] request, int timeoutMs, int expectResponseBytes) throws IOException {
        if (request == null || request.length > MAX_PAYLOAD_BYTES) {
            throw new IllegalArgumentException("XCP CAN request must contain at most " + MAX_PAYLOAD_BYTES + " bytes");
        }
        if (expectResponseBytes < 0 || expectResponseBytes > MAX_PAYLOAD_BYTES) {
            throw new IllegalArgumentException("XCP CAN expected response must contain from 0 to " + MAX_PAYLOAD_BYTES + " bytes");
        }
        if (timeoutMs < 0) {
            throw new IllegalArgumentException("timeoutMs must not be negative");
        }

        synchronized (lock) {
            checkConnected();
            port.send(new ClassicCanFrame(requestAddress, request));

            long deadline = System.nanoTime() + TimeUnit.MILLISECONDS.toNanos(timeoutMs);
            while (true) {
                long remainingNanos = deadline - System.nanoTime();
                if (remainingNanos <= 0) {
                    throw timeout(request, timeoutMs);
                }

                long remainingMillis = TimeUnit.NANOSECONDS.toMillis(remainingNanos);
                int remainingMs = (int) Math.min(Integer.MAX_VALUE, Math.max(1, remainingMillis));
                Optional<ClassicCanFrame> frame = port.receive(remainingMs);
                if (!frame.isPresent()) {
                    throw timeout(request, timeoutMs);
                }
                if (!responseAddress.equals(frame.get().getAddress())) {
                    continue;
                }

                byte[] response = frame.get().getPayload();
                // Some deployed OpenBLT targets pad logical responses to a classic-CAN DLC of eight.
                if (response.length < expectResponseBytes) {
                    throw new IOException("Unexpected XCP CAN response length for " + command(request)
                        + ": expected at least " + expectResponseBytes
                        + " but got " + response.length);
                }
                return response.length == expectResponseBytes
                    ? response
                    : Arrays.copyOf(response, expectResponseBytes);
            }
        }
    }

    private void checkConnected() {
        if (!connected) {
            throw new IllegalStateException("CAN port is not open");
        }
    }

    private static IOException timeout(byte[] request, int timeoutMs) {
        return new IOException("XCP CAN timeout waiting for response to " + command(request) + ", timeoutMs=" + timeoutMs);
    }

    private static String command(byte[] request) {
        return request.length == 0 ? "empty request" : "command 0x" + Integer.toHexString(request[0] & 0xFF);
    }
}
