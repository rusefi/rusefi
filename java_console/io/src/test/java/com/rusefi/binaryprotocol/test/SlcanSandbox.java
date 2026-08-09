package com.rusefi.binaryprotocol.test;

import com.rusefi.io.can.slcan.SlcanClient;

import java.io.IOException;

/**
 * Manual sandbox for the rusEFI CAN sniffer SLCAN (Lawicel ASCII) channel.
 * <p>
 * Scans all serial ports via {@link SlcanClient#findAndConnect}, opens the first SLCAN port
 * found and prints {@link #FRAMES_TO_PRINT} CAN frames.
 *
 * @see com.rusefi.ui.SlcanUiSandbox Swing variant in the ui module
 * @see SerialSandbox
 * @see SocketCANSandbox
 */
public class SlcanSandbox {
    private static final int FRAMES_TO_PRINT = 10;
    private static final int FRAME_TIMEOUT_MS = 30_000;

    public static void main(String[] args) throws IOException {
        try (SlcanClient client = SlcanClient.findAndConnect(System.out::println)) {
            if (client == null) {
                System.out.println("No SLCAN port found. Is the CAN sniffer enabled in the tune (Connectivity -> CAN Bus)?");
                return;
            }
            System.out.println("Channel open on " + client.getPort() + ", waiting for frames...");

            int count = 0;
            while (count < FRAMES_TO_PRINT) {
                String line = client.readLine(FRAME_TIMEOUT_MS);
                if (line == null) {
                    System.out.println("No frames for " + FRAME_TIMEOUT_MS + "ms, is there CAN traffic?");
                    continue;
                }
                SlcanClient.Frame frame = SlcanClient.Frame.parse(line);
                if (frame != null) {
                    count++;
                    System.out.printf("#%02d %-30s %s%n", count, line, frame.decode());
                } else if (!line.isEmpty()) {
                    System.out.println("Non-frame response: " + SlcanClient.printable(line));
                }
            }
            System.out.println("Done, printed " + count + " frame(s)");
        }
    }
}
