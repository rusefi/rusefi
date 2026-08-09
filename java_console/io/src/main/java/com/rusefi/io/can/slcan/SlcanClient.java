package com.rusefi.io.can.slcan;

import com.devexperts.logging.Logging;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.BufferedSerialIoStream;
import org.jetbrains.annotations.Nullable;

import java.io.Closeable;
import java.io.EOFException;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.function.Consumer;

/**
 * Client for the rusEFI CAN sniffer SLCAN (Lawicel ASCII) channel which some boards expose
 * as a secondary USB CDC virtual COM port next to the primary TunerStudio console port.
 * <p>
 * Protocol notes, see firmware/controllers/can/can_sniffer.cpp and can_sniffer.md:
 * <ul>
 * <li>'O' (open) refuses to work until some 'S' bitrate command is sent, even though the bitrate
 * value itself is ignored (CAN hardware is controlled by the ECU), and 'C' resets that state --
 * hence the C -&gt; S6 -&gt; O init sequence which also recovers from a stale open terminal;</li>
 * <li>OK ack is a bare CR (reads back as an empty line), error ack is a bell 0x07 character.</li>
 * </ul>
 * Used by SlcanSandbox (console) and SlcanUiSandbox (Swing).
 */
public class SlcanClient implements Closeable {
    private static final Logging log = Logging.getLogging(SlcanClient.class);

    private static final int RESPONSE_TIMEOUT_MS = 700;
    private static final char CR = '\r';
    private static final char BELL = 7;

    private final IoStream stream;
    private final String port;
    private final String version;

    private SlcanClient(IoStream stream, String port, String version) {
        this.stream = stream;
        this.port = port;
        this.version = version;
    }

    public String getPort() {
        return port;
    }

    /**
     * @return response to the 'V' probe, e.g. "V1220"
     */
    public String getVersion() {
        return version;
    }

    /**
     * Scans all serial ports, skips port(s) which answer the TunerStudio HELLO with a rusEFI
     * signature (that's the primary console VCP), probes the rest with the SLCAN 'V' (version)
     * command and opens the channel on the first SLCAN port found.
     *
     * @param logger receives a human-readable line per inspected port
     * @return connected client with the channel open, or null if no SLCAN port was found
     */
    @Nullable
    public static SlcanClient findAndConnect(Consumer<String> logger) {
        for (String port : LinkManager.getCommPorts()) {
            IoStream stream = BufferedSerialIoStream.openPort(port);
            if (stream == null) {
                logger.accept(port + ": failed to open, skipping");
                continue;
            }
            boolean keepOpen = false;
            try {
                String signature = SerialAutoChecker.checkResponse(stream, null);
                if (signature != null) {
                    logger.accept(port + ": rusEFI TS console [" + signature + "], skipping");
                    continue;
                }
                // the TS HELLO probe above may have confused the SLCAN parser, drain leftovers
                stream.getDataBuffer().dropPending();

                String version = command(stream, "V");
                if (version == null || version.isEmpty() || version.charAt(0) != 'V') {
                    logger.accept(port + ": not SLCAN (V response: " + printable(version) + ")");
                    continue;
                }
                logger.accept(port + ": SLCAN detected, version response " + version);
                SlcanClient client = new SlcanClient(stream, port, version);
                client.openChannel();
                keepOpen = true;
                return client;
            } catch (IOException e) {
                logger.accept(port + ": IO error: " + e);
            } finally {
                if (!keepOpen) {
                    stream.close();
                }
            }
        }
        return null;
    }

    private void openChannel() throws IOException {
        // close first in case a previous session left the terminal open; error ack is fine here
        command(stream, "C");
        expectOk("S6");
        expectOk("O");
        log.info(port + ": SLCAN channel open");
    }

    /**
     * Fire-and-forget 'F' status poll usable as a liveness probe: writing to a disconnected
     * port throws. The "Fxx" response line is left in the stream for the normal read loop,
     * where it parses as a non-frame line and gets ignored -- reading it here would race
     * against already-queued frame lines.
     */
    public void pollStatus() throws IOException {
        send("F");
    }

    /**
     * @return next CR-terminated line without the CR ("" for an OK ack), a one-character
     * bell string for an error ack, null on timeout
     */
    @Nullable
    public String readLine(int timeoutMs) throws IOException {
        return readLine(stream, timeoutMs);
    }

    @Override
    public void close() {
        try {
            // hardware stays under ECU control, this only closes the logical terminal
            command(stream, "C");
        } catch (IOException ignored) {
        }
        stream.close();
    }

    private void expectOk(String cmd) throws IOException {
        String response = command(stream, cmd);
        if (response == null || !response.isEmpty()) {
            throw new IOException("SLCAN '" + cmd + "' failed on " + port + ", response: " + printable(response));
        }
    }

    private void send(String cmd) throws IOException {
        stream.write((cmd + CR).getBytes(StandardCharsets.US_ASCII));
        stream.flush();
    }

    /**
     * Sends one SLCAN command and returns the response, see {@link #readLine(int)} for the contract.
     */
    @Nullable
    private static String command(IoStream stream, String cmd) throws IOException {
        stream.write((cmd + CR).getBytes(StandardCharsets.US_ASCII));
        stream.flush();
        return readLine(stream, RESPONSE_TIMEOUT_MS);
    }

    @Nullable
    private static String readLine(IoStream stream, int timeoutMs) throws IOException {
        StringBuilder line = new StringBuilder();
        while (true) {
            byte b;
            try {
                b = stream.getDataBuffer().readByte(timeoutMs);
            } catch (EOFException timeout) {
                return null;
            }
            if (b == BELL) {
                return String.valueOf(BELL);
            }
            if (b == CR) {
                return line.toString();
            }
            line.append((char) (b & 0xFF));
        }
    }

    public static String printable(String response) {
        if (response == null) {
            return "<timeout>";
        }
        if (response.equals(String.valueOf(BELL))) {
            return "<bell/error>";
        }
        return "'" + response + "'";
    }

    /**
     * One parsed t/T/r/R SLCAN line.
     */
    public static class Frame {
        public final String raw;
        public final boolean extended;
        public final boolean rtr;
        public final int id;
        public final int dlc;
        public final byte[] data;
        /**
         * four hex characters when Z1 timestamp mode is active, null otherwise
         */
        @Nullable
        public final String timestamp;

        private Frame(String raw, boolean extended, boolean rtr, int id, int dlc, byte[] data, @Nullable String timestamp) {
            this.raw = raw;
            this.extended = extended;
            this.rtr = rtr;
            this.id = id;
            this.dlc = dlc;
            this.data = data;
            this.timestamp = timestamp;
        }

        /**
         * @return parsed frame or null if the line is not a well-formed frame line
         */
        @Nullable
        public static Frame parse(String line) {
            if (line == null || line.isEmpty()) {
                return null;
            }
            char type = line.charAt(0);
            if (type != 't' && type != 'T' && type != 'r' && type != 'R') {
                return null;
            }
            boolean extended = type == 'T' || type == 'R';
            boolean rtr = type == 'r' || type == 'R';
            int idLen = extended ? 8 : 3;
            try {
                if (line.length() < 2 + idLen) {
                    return null;
                }
                int id = Integer.parseInt(line.substring(1, 1 + idLen), 16);
                int dlc = Integer.parseInt(line.substring(1 + idLen, 2 + idLen), 16);
                if (dlc > 8) {
                    return null;
                }
                int dataStart = 2 + idLen;
                byte[] data = new byte[rtr ? 0 : dlc];
                if (line.length() < dataStart + 2 * data.length) {
                    return null;
                }
                for (int i = 0; i < data.length; i++) {
                    data[i] = (byte) Integer.parseInt(line.substring(dataStart + 2 * i, dataStart + 2 * i + 2), 16);
                }
                int leftover = line.length() - (dataStart + 2 * data.length);
                String timestamp = leftover == 4 ? line.substring(line.length() - 4) : null;
                return new Frame(line, extended, rtr, id, dlc, data, timestamp);
            } catch (NumberFormatException e) {
                return null;
            }
        }

        /**
         * @return human-readable form, e.g. "ID=0x300 DLC=6 DATA=[02 AF 4E 41 70 45]"
         */
        public String decode() {
            StringBuilder result = new StringBuilder(String.format("ID=0x%X DLC=%d", id, dlc));
            if (rtr) {
                result.append(" RTR");
            } else {
                result.append(" DATA=[");
                for (int i = 0; i < data.length; i++) {
                    if (i > 0) {
                        result.append(' ');
                    }
                    result.append(String.format("%02X", data[i]));
                }
                result.append(']');
            }
            if (timestamp != null) {
                result.append(" TS=").append(timestamp);
            }
            return result.toString();
        }

        @Override
        public String toString() {
            return "Frame{" + raw + "}";
        }
    }
}
