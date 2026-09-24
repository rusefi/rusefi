package com.rusefi.io.can;

import com.rusefi.io.IoStream;
import com.rusefi.io.serial.BufferedSerialIoStream;

import java.io.EOFException;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.Optional;
import java.util.concurrent.TimeUnit;
import java.util.function.Supplier;

/** SLCAN transport, sharing the PCAN stream's ISO-TP implementation. */
public final class SLCANConnector implements RawCanPort {
    private final String port;
    private final int bitrate;
    private final Supplier<IoStream> opener;
    private final StringBuilder line = new StringBuilder();
    private volatile IoStream serial;
    private CanAddress receiveAddress;
    private boolean canable;

    private SLCANConnector(String port, int bitrate) {
        this(port, bitrate, () -> BufferedSerialIoStream.openPort(port));
    }

    SLCANConnector(String port, int bitrate, Supplier<IoStream> opener) {
        if (bitrate < 0 || bitrate > 8) {
            throw new IllegalArgumentException("SLCAN bitrate must be 0..8");
        }
        this.port = port;
        this.bitrate = bitrate;
        this.opener = opener;
    }

    public static PCanIoStream createStream(String port, int bitrate) throws IOException {
        PCanIoStream stream = PCanIoStream.createStream(System.out::println, new SLCANConnector(port, bitrate));
        if (stream == null) {
            throw new IOException("Cannot initialize SLCAN on " + port);
        }
        return stream;
    }

    @Override
    public void open(CanAddress receiveAddress) throws IOException {
        this.receiveAddress = receiveAddress;
        serial = opener.get();
        if (serial == null) {
            throw new IOException("Cannot open " + port);
        }
        try {
            // Explicit adapter selection: stop any previous streaming session before identification.
            writeLine("C");
            drain(200);
            writeLine("V");
            long deadline = deadline(1000);
            String version = null;
            while (remaining(deadline) > 0) {
                String response = readLine(deadline);
                if (response == null) {
                    break;
                }
                if (isVersion(response)) {
                    version = response;
                    break;
                }
            }
            if (version == null) {
                throw new IOException("No recognized SLCAN version on " + port);
            }
            canable = version.contains("github.com/normaldotcom/canable2");
            System.out.println(port + " SLCAN version: " + version);
            drain(100); // Discard any trailing version acknowledgement before setup.
            expectOk("S" + bitrate);
            expectOk("O");
        } catch (IOException | RuntimeException e) {
            close();
            throw e;
        }
    }

    static boolean isVersion(String value) {
        return value.matches("V[0-9A-Fa-f]{4}") || value.contains("github.com/normaldotcom/canable2");
    }

    private void drain(int timeoutMs) throws IOException {
        long deadline = deadline(timeoutMs);
        while (remaining(deadline) > 0 && readLine(deadline) != null) {
            // Bounded even if the adapter was streaming before C.
        }
        line.setLength(0);
    }

    private void expectOk(String command) throws IOException {
        // CANable does not acknowledge setup commands. Its ordered V reply is a barrier.
        writeLine(command);
        if (canable) {
            writeLine("V");
        }
        long deadline = deadline(1000);
        while (remaining(deadline) > 0) {
            String response = readLine(deadline);
            if (response == null) {
                break;
            }
            if ((!canable && response.isEmpty()) || (canable && isVersion(response))) {
                return;
            }
            if (response.equals("\u0007")) {
                throw new IOException("SLCAN rejected " + command);
            }
            // CAN traffic may arrive immediately after O.
        }
        throw new IOException("No acknowledgement for SLCAN " + command);
    }

    private synchronized void writeLine(String value) throws IOException {
        IoStream current = serial;
        if (current == null || current.isClosed()) {
            throw new IOException("SLCAN port is closed");
        }
        current.write((value + "\r").getBytes(StandardCharsets.US_ASCII));
        current.flush();
    }

    @Override
    public void send(ClassicCanFrame frame) throws IOException {
        writeLine(encode(frame));
    }

    static String encode(ClassicCanFrame frame) {
        CanAddress address = frame.getAddress();
        byte[] payload = frame.getPayload();
        StringBuilder result = new StringBuilder(String.format(address.isExtended() ? "T%08X" : "t%03X", address.getId()));
        result.append(payload.length);
        for (byte value : payload) {
            result.append(String.format("%02X", value & 0xff));
        }
        return result.toString();
    }

    static Optional<ClassicCanFrame> decode(String value) {
        // Only data frames enter ISO-TP. Reject RTR, FD, malformed lengths and non-hex data.
        if (!value.matches("(t[0-9A-Fa-f]{3}|T[0-9A-Fa-f]{8})[0-8][0-9A-Fa-f]*")) {
            return Optional.empty();
        }
        boolean extended = value.charAt(0) == 'T';
        int idEnd = extended ? 9 : 4;
        int count = Character.digit(value.charAt(idEnd), 16);
        int expected = idEnd + 1 + 2 * count;
        if (value.length() != expected && value.length() != expected + 4) {
            return Optional.empty();
        }
        try {
            CanAddress address = new CanAddress(Integer.parseInt(value.substring(1, idEnd), 16), extended);
            byte[] payload = new byte[count];
            for (int i = 0; i < count; i++) {
                int offset = idEnd + 1 + 2 * i;
                payload[i] = (byte) Integer.parseInt(value.substring(offset, offset + 2), 16);
            }
            return Optional.of(new ClassicCanFrame(address, payload));
        } catch (IllegalArgumentException e) {
            return Optional.empty();
        }
    }

    @Override
    public Optional<ClassicCanFrame> receive(int timeoutMs) throws IOException {
        long deadline = deadline(timeoutMs);
        while (remaining(deadline) > 0) {
            String response = readLine(deadline);
            if (response == null) {
                break;
            }
            if (response.equals("\u0007")) {
                throw new IOException("SLCAN adapter reported an error");
            }
            Optional<ClassicCanFrame> frame = decode(response);
            if (frame.isPresent() && frame.get().getAddress().equals(receiveAddress)
                    && frame.get().getPayload().length > 0) {
                return frame;
            }
            // Ignore acknowledgements and unrelated bus traffic.
        }
        return Optional.empty();
    }

    private String readLine(long deadline) throws IOException {
        while (remaining(deadline) > 0) {
            IoStream current = serial;
            if (current == null || current.isClosed()) {
                throw new IOException("SLCAN disconnected");
            }
            final int value;
            try {
                value = current.getDataBuffer().readByte(remaining(deadline)) & 0xff;
            } catch (EOFException timeout) {
                return null; // Preserve a partial line across receive calls.
            }
            if (value == 7) {
                line.setLength(0);
                return "\u0007";
            }
            if (value == '\r') {
                String result = line.toString();
                line.setLength(0);
                return result;
            }
            if (value != '\n') {
                line.append((char) value);
            }
            if (line.length() > 256) {
                throw new IOException("Oversized SLCAN response");
            }
        }
        return null;
    }

    private static long deadline(int timeoutMs) {
        return System.nanoTime() + TimeUnit.MILLISECONDS.toNanos(timeoutMs);
    }

    private static int remaining(long deadline) {
        long nanos = deadline - System.nanoTime();
        return nanos <= 0 ? 0 : (int) Math.max(1, TimeUnit.NANOSECONDS.toMillis(nanos));
    }

    @Override
    public synchronized void close() {
        IoStream current = serial;
        if (current == null) {
            return;
        }
        try {
            writeLine("C");
        } catch (IOException ignored) {
            // Disconnection must still release the host port.
        } finally {
            serial = null;
            current.close();
        }
    }
}
