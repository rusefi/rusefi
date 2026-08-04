package com.rusefi.binaryprotocol.test;

import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.BufferedSerialIoStream;

import java.io.EOFException;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.Set;

/**
 * Manual sandbox for the rusEFI CAN sniffer SLCAN (Lawicel ASCII) channel.
 * <p>
 * Scans all serial ports, skips the one(s) which answer the TunerStudio HELLO with a rusEFI
 * signature (that's the primary console VCP), probes the rest with the SLCAN 'V' (version)
 * command, then opens the first SLCAN port found and prints {@link #FRAMES_TO_PRINT} CAN frames.
 * <p>
 * Firmware side: firmware/controllers/can/can_sniffer.cpp, protocol notes in can_sniffer.md.
 * Note the sniffer requires an 'S' bitrate command before 'O' even though the bitrate value
 * itself is ignored (hardware is controlled by the ECU), and 'C' resets that state.
 *
 * @see SerialSandbox
 * @see SocketCANSandbox
 */
public class SlcanSandbox {
    private static final int FRAMES_TO_PRINT = 10;
    private static final int RESPONSE_TIMEOUT_MS = 700;
    private static final int FRAME_TIMEOUT_MS = 30_000;
    private static final char CR = '\r';
    private static final char BELL = 7;

    public static void main(String[] args) throws IOException {
        Set<String> ports = LinkManager.getCommPorts();
        System.out.println("Scanning ports: " + ports);

        for (String port : ports) {
            try (IoStream stream = BufferedSerialIoStream.openPort(port)) {
                if (stream == null) {
                    System.out.println(port + ": failed to open, skipping");
                    continue;
                }
                String signature = SerialAutoChecker.checkResponse(stream, null);
                if (signature != null) {
                    System.out.println(port + ": rusEFI TS console [" + signature + "], skipping");
                    continue;
                }
                // the TS HELLO probe above may have confused the SLCAN parser, drain leftovers
                stream.getDataBuffer().dropPending();

                String version = command(stream, "V");
                if (version == null || version.isEmpty() || version.charAt(0) != 'V') {
                    System.out.println(port + ": not SLCAN (V response: " + printable(version) + ")");
                    continue;
                }
                System.out.println(port + ": SLCAN detected, version response " + version);
                printFrames(stream);
                return;
            }
        }
        System.out.println("No SLCAN port found. Is the CAN sniffer enabled in the tune (Connectivity -> CAN Bus)?");
    }

    private static void printFrames(IoStream stream) throws IOException {
        // close first in case a previous session left the terminal open; error ack is fine here
        command(stream, "C");
        // 'O' refuses to open until some 'S' bitrate is sent, the actual value is ignored by the ECU
        expectOk(stream, "S6");
        expectOk(stream, "O");
        System.out.println("Channel open, waiting for frames...");

        int count = 0;
        while (count < FRAMES_TO_PRINT) {
            String line = readLine(stream, FRAME_TIMEOUT_MS);
            if (line == null) {
                System.out.println("No frames for " + FRAME_TIMEOUT_MS + "ms, is there CAN traffic?");
                continue;
            }
            if (line.isEmpty()) {
                continue;
            }
            char type = line.charAt(0);
            if (type == 't' || type == 'T' || type == 'r' || type == 'R') {
                count++;
                System.out.printf("#%02d %-30s %s%n", count, line, decodeFrame(line));
            } else {
                System.out.println("Non-frame response: " + printable(line));
            }
        }
        command(stream, "C");
        System.out.println("Done, printed " + count + " frame(s)");
    }

    /**
     * @return decoded human-readable form of a t/T/r/R SLCAN line, best effort
     */
    private static String decodeFrame(String line) {
        try {
            char type = line.charAt(0);
            boolean extended = type == 'T' || type == 'R';
            boolean rtr = type == 'r' || type == 'R';
            int idLen = extended ? 8 : 3;
            int id = Integer.parseInt(line.substring(1, 1 + idLen), 16);
            int dlc = Integer.parseInt(line.substring(1 + idLen, 2 + idLen), 16);
            String result = String.format("ID=0x%X DLC=%d", id, dlc);
            if (!rtr) {
                int dataStart = 2 + idLen;
                StringBuilder data = new StringBuilder();
                for (int i = 0; i < dlc; i++) {
                    if (i > 0)
                        data.append(' ');
                    data.append(line, dataStart + 2 * i, dataStart + 2 * i + 2);
                }
                result += " DATA=[" + data + "]";
                int leftover = line.length() - (dataStart + 2 * dlc);
                if (leftover == 4) {
                    result += " TS=" + line.substring(dataStart + 2 * dlc);
                }
            } else {
                result += " RTR";
            }
            return result;
        } catch (RuntimeException e) {
            return "failed to decode: " + e;
        }
    }

    private static void expectOk(IoStream stream, String cmd) throws IOException {
        String response = command(stream, cmd);
        // OK ack is a bare CR which reads back as an empty line
        if (response == null || !response.isEmpty()) {
            throw new IOException("SLCAN '" + cmd + "' failed, response: " + printable(response));
        }
    }

    /**
     * Sends one SLCAN command and returns the CR-terminated response without the CR,
     * "" for a bell error ack, null on timeout.
     */
    private static String command(IoStream stream, String cmd) throws IOException {
        stream.write((cmd + CR).getBytes(StandardCharsets.US_ASCII));
        stream.flush();
        return readLine(stream, RESPONSE_TIMEOUT_MS);
    }

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

    private static String printable(String response) {
        if (response == null) {
            return "<timeout>";
        }
        if (response.equals(String.valueOf(BELL))) {
            return "<bell/error>";
        }
        return "'" + response + "'";
    }
}
