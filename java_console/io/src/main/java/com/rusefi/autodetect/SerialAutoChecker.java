package com.rusefi.autodetect;

import com.devexperts.logging.Logging;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.serial.SerialIoStreamJSerialComm;

import java.io.IOException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Function;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;

public class SerialAutoChecker implements Runnable {
    private final static Logging log = Logging.getLogging(SerialAutoChecker.class);
    private final String serialPort;
    private final CountDownLatch portFound;
    private final AtomicReference<String> result;
    private final Function<IoStream, Void> callback;
    public static String SIGNATURE;

    public SerialAutoChecker(String serialPort, CountDownLatch portFound, AtomicReference<String> result, Function<IoStream, Void> callback) {
        this.serialPort = serialPort;
        this.portFound = portFound;
        this.result = result;
        this.callback = callback;
    }

    public SerialAutoChecker(String serialPort, CountDownLatch portFound, AtomicReference<String> result) {
        this(serialPort, portFound, result, null);
    }

    @Override
    public void run() {
        IoStream stream = SerialIoStreamJSerialComm.openPort(serialPort);
        IncomingDataBuffer incomingData = stream.getDataBuffer();
        boolean isPortFound = false;
        try {
            HelloCommand.send(stream);
            byte[] response = incomingData.getPacket("");
            if (!checkResponseCode(response, (byte) Fields.TS_RESPONSE_OK))
                return;
            String signature = new String(response, 1, response.length - 1);
            SIGNATURE = signature;
            log.info("Got signature=" + signature + " from " + serialPort);
            if (signature.startsWith(Fields.PROTOCOL_SIGNATURE_PREFIX)) {
                if (callback != null) {
                    callback.apply(stream);
                }
                isPortFound = true;
            }
        } catch (IOException ignore) {
            return;
        } finally {
            stream.close();
        }
        if (isPortFound) {
            /**
             * propagating result after closing the port so that it could be used right away
             */
            result.set(serialPort);
            portFound.countDown();
        }
    }
}
