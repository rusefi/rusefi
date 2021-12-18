package com.rusefi.binaryprotocol.test;

import com.opensr5.ConfigurationImage;
import com.rusefi.io.LinkManager;
import com.rusefi.io.stream.PCanIoStream;

import java.io.IOException;

/**
 * @see Elm327Sandbox
 */
public class PCanSandbox {

    public static void main(String[] args) throws IOException, InterruptedException {
        PCanIoStream tsStream = PCanIoStream.getPCANIoStream();
        if (tsStream == null)
            throw new IOException("No PCAN");

/*
        for (int i = 0; i < 17; i++) {
            String signature = BinaryProtocol.getSignature(tsStream);
            System.out.println("Got " + i + " " + signature + " signature via PCAN");
            if (signature == null || !signature.startsWith(Fields.PROTOCOL_SIGNATURE_PREFIX))
                throw new IllegalStateException("Unexpected S " + signature);
        }
        System.out.println("****************************************");
        System.out.println("*********  PCAN LOOKS GREAT  ***********");
        System.out.println("****************************************");
*/
        LinkManager linkManager = new LinkManager();
/*
        for (int i = 0; i < 4; i++) {
            SandboxCommon.verifyCrcNoPending(tsStream, linkManager);
        }
*/
        ConfigurationImage ci = SandboxCommon.readImage(tsStream, linkManager);

//        System.out.println("We are done");
//        System.exit(0);
    }
}

