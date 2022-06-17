package com.rusefi.binaryprotocol.test;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.io.stream.SocketCANIoStream;

import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;

public class SocketCANSandbox {
    private static final Logging log = getLogging(PCanSandbox.class);

    public static void main(String[] args) throws IOException, InterruptedException {
        AbstractIoStream tsStream = SocketCANIoStream.create();
        if (tsStream == null)
            throw new IOException("No SocketCAN");

/*
        for (int i = 0; i < 17; i++) {
            String signature = BinaryProtocol.getSignature(tsStream);
            System.out.println("Got " + i + " " + signature + " signature via SocketCAN");
            if (signature == null || !signature.startsWith(Fields.PROTOCOL_SIGNATURE_PREFIX))
                throw new IllegalStateException("Unexpected S " + signature);
        }
        log.info("****************************************");
        log.info("******  SocketCAN LOOKS GREAT  *********");
        log.info("****************************************");
*/
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

        ConfigurationImage ci = SandboxCommon.readImage(tsStream, linkManager);
        log.info("Got ConfigurationImage " + ci);

//        System.out.println("We are done");
//        System.exit(0);
    }
}
