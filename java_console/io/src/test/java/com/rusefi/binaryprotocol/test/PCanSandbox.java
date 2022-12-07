package com.rusefi.binaryprotocol.test;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.io.stream.PCanIoStream;

import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.Timeouts.SECOND;

/**
 * @see Elm327Sandbox
 */
public class PCanSandbox {
    private static final Logging log = getLogging(PCanSandbox.class);

    public static void main(String[] args) throws IOException, InterruptedException {
        AbstractIoStream tsStream = PCanIoStream.createStream();
        if (tsStream == null)
            throw new IOException("Error creating PCAN stream");


        for (int i = 0; i < 17; i++) {
            String signature = BinaryProtocol.getSignature(tsStream);
            System.out.println("Got " + i + " " + signature + " signature via PCAN");
            if (signature == null || !signature.startsWith(Fields.PROTOCOL_SIGNATURE_PREFIX))
                throw new IllegalStateException("Unexpected S " + signature);
        }
        log.info("****************************************");
        log.info("*********  PCAN LOOKS GREAT  ***********");
        log.info("****************************************");
        LinkManager linkManager = new LinkManager();
/*
        for (int i = 0; i < 4; i++) {
            SandboxCommon.verifyCrcNoPending(tsStream, linkManager);
        }
*/

        BinaryProtocol bp = new BinaryProtocol(linkManager, tsStream);
        linkManager.submit(() -> {
            boolean response = bp.requestOutputChannels();
            log.info("requestOutputChannels " + response);
        });

        long start = System.currentTimeMillis();
        ConfigurationImage ci = SandboxCommon.readImage(tsStream, linkManager);
        log.info("Got ConfigurationImage " + ci + " in " + (System.currentTimeMillis() - start) + "ms");

        Thread.sleep(5 * SECOND);

        System.out.println("We are done");
        System.exit(0);
    }
}

