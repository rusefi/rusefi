package com.rusefi.io.can.slcan;

import java.io.IOException;
import java.util.function.Consumer;

/** Adapter configuration shared by CAN clients after closing and identifying the adapter. */
public final class SlcanSetup {
    @FunctionalInterface
    public interface Command {
        /** Send one command and verify its acknowledgement or ordered version reply. */
        void execute(String command) throws IOException;
    }

    private SlcanSetup() {
    }

    /**
     * Configure a closed channel and open it only after every setup command succeeds.
     * The caller owns version probing, response synchronization and port cleanup.
     * A null or unknown version selects generic Lawicel setup.
     */
    public static void openClosedChannel(String version, int bitrate, Command command,
                                         Consumer<String> log) throws IOException {
        if (bitrate < 0 || bitrate > 8) {
            throw new IllegalArgumentException("SLCAN bitrate must be 0..8");
        }
        command.execute("S" + bitrate);
        if (SlcanVersion.isWeAct(version)) {
            // WeAct defaults to one-shot TX, which can drop an ISO-TP request or
            // flow-control frame when it loses arbitration. A1 is vendor-specific.
            log.accept("WeAct adapter: enabling automatic CAN retransmission");
            command.execute("A1");
        }
        command.execute("O");
    }
}
