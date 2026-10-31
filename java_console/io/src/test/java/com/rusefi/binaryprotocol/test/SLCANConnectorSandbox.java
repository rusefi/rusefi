package com.rusefi.binaryprotocol.test;

import com.opensr5.ConfigurationImage;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.can.SLCANConnector;

import java.util.concurrent.TimeUnit;

/**
 * Talks to a real rusEFI ECU through a standalone SLCAN adapter using the PCAN ISO-TP path.
 * Arguments: adapter port, optional SLCAN bitrate index (default 6 = 500 kbit/s),
 * optional expected ECU signature. Reads only; does not write or burn a tune.
 */
public final class SLCANConnectorSandbox {
    public static void main(String[] args) throws Exception {
        if (args.length < 1 || args.length > 3) {
            throw new IllegalArgumentException("Usage: SLCANConnectorSandbox <adapter-port> [bitrate-index=6] [expected-signature]");
        }
        int bitrate = args.length > 1 ? Integer.parseInt(args[1]) : 6;
        try (IoStream stream = SLCANConnector.createStream(args[0], bitrate);
             LinkManager manager = new LinkManager().setNeedPullText(false).setNeedPullLiveData(false)) {
            long started = System.nanoTime();
            for (int i = 0; i < 17; i++) {
                String signature = BinaryProtocol.getSignature(stream);
                if (signature == null || !signature.startsWith(Integration.PROTOCOL_SIGNATURE_PREFIX)
                        || (args.length > 2 && !args[2].equals(signature))) {
                    throw new IllegalStateException("Unexpected ECU signature over SLCAN: " + signature);
                }
                System.out.println("SLCAN signature " + (i + 1) + "/17: " + signature);
            }
            System.out.println("17 signature queries passed in "
                + TimeUnit.NANOSECONDS.toMillis(System.nanoTime() - started) + " ms");
            ConfigurationImage image = SandboxCommon.readImage(stream, manager);
            if (image == null) {
                throw new IllegalStateException("Configuration read failed");
            }
            manager.submit(() -> {
                if (!manager.getBinaryProtocol().requestOutputChannels()) {
                    throw new IllegalStateException("Output-channel read failed");
                }
            }).get(15, TimeUnit.SECONDS);
            System.out.println("SLCAN PASS: 17 signatures, configuration (" + image.getSize()
                + " bytes), and output channels");
        }
    }
}
