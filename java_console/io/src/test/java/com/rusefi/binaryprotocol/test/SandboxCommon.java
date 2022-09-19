package com.rusefi.binaryprotocol.test;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolState;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.StreamConnector;

import java.io.IOException;
import java.util.Arrays;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

import static com.devexperts.logging.Logging.getLogging;

public class SandboxCommon {
    private static final Logging log = getLogging(SandboxCommon.class);
    static {
        log.configureDebugEnabled(false);
    }

    static ConfigurationImage readImage(IoStream tsStream, LinkManager linkManager) throws InterruptedException {
        AtomicReference<ConfigurationImage> configurationImageAtomicReference = new AtomicReference<>();
        CountDownLatch imageLatch = new CountDownLatch(1);

        StreamConnector streamConnector = new StreamConnector(linkManager, () -> tsStream);
        linkManager.setConnector(streamConnector);
        streamConnector.connectAndReadConfiguration(new BinaryProtocol.Arguments(false), new ConnectionStateListener() {
            @Override
            public void onConnectionEstablished() {
                log.info("onConnectionEstablished");

                BinaryProtocol currentStreamState = linkManager.getCurrentStreamState();
                if (currentStreamState == null) {
                    log.info("No BinaryProtocol");
                } else {
                    BinaryProtocolState binaryProtocolState = currentStreamState.getBinaryProtocolState();
                    ConfigurationImage ci = binaryProtocolState.getControllerConfiguration();
                    configurationImageAtomicReference.set(ci);
                    imageLatch.countDown();
                }
            }

            @Override
            public void onConnectionFailed(String s) {
                log.info("onConnectionFailed");
            }
        });

        imageLatch.await(1, TimeUnit.MINUTES);
        ConfigurationImage ci = configurationImageAtomicReference.get();
        log.info("Got ConfigurationImage " + ci + ", size=" + ci.getSize());
        return ci;
    }

    static void verifyCrcNoPending(IoStream tsStream, LinkManager linkManager) {
        BinaryProtocol bp = new BinaryProtocol(linkManager, tsStream);
        linkManager.COMMUNICATION_EXECUTOR.submit(() -> {
            if (tsStream.getDataBuffer().dropPending() != 0)
                log.info("ERROR Extra data before CRC");
            bp.getCrcFromController(Fields.TOTAL_CONFIG_SIZE);
//            bp.getCrcFromController(Fields.TOTAL_CONFIG_SIZE);
//            bp.getCrcFromController(Fields.TOTAL_CONFIG_SIZE);
            if (tsStream.getDataBuffer().dropPending() != 0)
                throw new IllegalStateException("ERROR Extra data after CRC");
        });
    }

    static void verifySignature(IoStream tsStream, String prefix, String suffix) throws IOException {
        String signature = BinaryProtocol.getSignature(tsStream);
        log.info(prefix + "Got " + signature + " signature via " + suffix);
        if (signature == null || !signature.startsWith(Fields.PROTOCOL_SIGNATURE_PREFIX))
            throw new IllegalStateException("Unexpected S " + signature);
    }

    static void runGetProtocolCommand(String prefix, IoStream tsStream) throws IOException {
        IncomingDataBuffer dataBuffer = tsStream.getDataBuffer();
        tsStream.write(new byte[]{Fields.TS_GET_PROTOCOL_VERSION_COMMAND_F});
        tsStream.flush();
        byte[] fResponse = new byte[3];
        dataBuffer.waitForBytes("hello", System.currentTimeMillis(), fResponse.length);
        dataBuffer.getData(fResponse);
        if (log.debugEnabled())
            log.debug(prefix + " Got GetProtocol F response " + IoStream.printByteArray(fResponse));
        if (fResponse[0] != '0' || fResponse[1] != '0' || fResponse[2] != '1')
            throw new IllegalStateException("Unexpected TS_COMMAND_F response " + Arrays.toString(fResponse));
    }
}
