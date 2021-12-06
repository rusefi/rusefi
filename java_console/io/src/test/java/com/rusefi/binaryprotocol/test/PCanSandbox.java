package com.rusefi.binaryprotocol.test;

import com.opensr5.ConfigurationImage;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolState;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.StreamConnector;
import peak.can.basic.*;

import java.io.IOException;

/**
 * @see Elm327Sandbox
 */
public class PCanSandbox {

    public static final TPCANHandle CHANNEL = TPCANHandle.PCAN_USBBUS1;

    public static void main(String[] args) throws IOException, InterruptedException {
        PCANBasic can = new PCANBasic();
        can.initializeAPI();
        TPCANStatus status = can.Initialize(CHANNEL, TPCANBaudrate.PCAN_BAUD_500K, TPCANType.PCAN_TYPE_NONE, 0, (short) 0);
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            System.out.println(status);
            return;
        }
        System.out.println("Hello PCAN " + can);

        PCanIoStream tsStream = new PCanIoStream(can);

/*
        {
            String signature = BinaryProtocol.getSignature(tsStream);
            System.out.println("Got " + signature + " signature via PCAN");
            if (signature == null || !signature.startsWith(Fields.PROTOCOL_SIGNATURE_PREFIX))
                throw new IllegalStateException("Unexpected S " + signature);
        }

        {
            String signature = BinaryProtocol.getSignature(tsStream);
            System.out.println("AGAIN, got " + signature + " signature via PCAN");
            if (signature == null || !signature.startsWith(Fields.PROTOCOL_SIGNATURE_PREFIX))
                throw new IllegalStateException("Unexpected S " + signature);
        }


        System.out.println("****************************************");
        System.out.println("*********  PCAN LOOKS GREAT  ***********");
        System.out.println("****************************************");
*/
        LinkManager linkManager = new LinkManager();
        BinaryProtocol bp = new BinaryProtocol(linkManager, tsStream);
        linkManager.COMMUNICATION_EXECUTOR.submit(() -> {
            if (tsStream.getDataBuffer().dropPending() != 0)
                System.out.println("ERROR Extra data before CRC");
            bp.getCrcFromController(Fields.TOTAL_CONFIG_SIZE);
//            bp.getCrcFromController(Fields.TOTAL_CONFIG_SIZE);
//            bp.getCrcFromController(Fields.TOTAL_CONFIG_SIZE);
            if (tsStream.getDataBuffer().dropPending() != 0)
                System.out.println("ERROR Extra data after CRC");
        });

/*
        StreamConnector streamConnector = new StreamConnector(linkManager, () -> tsStream);
        linkManager.setConnector(streamConnector);
        streamConnector.connectAndReadConfiguration(new ConnectionStateListener() {
            @Override
            public void onConnectionEstablished() {
                System.out.println("onConnectionEstablished");

                BinaryProtocol currentStreamState = linkManager.getCurrentStreamState();
                if (currentStreamState == null) {
                    System.out.println("No BinaryProtocol");
                } else {
                    BinaryProtocolState binaryProtocolState = currentStreamState.getBinaryProtocolState();
                    ConfigurationImage ci = binaryProtocolState.getControllerConfiguration();
                    System.out.println("Got ConfigurationImage " + ci);
                    System.exit(0);
                }
            }

            @Override
            public void onConnectionFailed() {
                System.out.println("onConnectionFailed");
            }
        });
*/
    }

}

