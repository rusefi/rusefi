package com.rusefi.autodetect;

import com.opensr5.Logger;
import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocolCommands;
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
    private final Logger logger;
    private final String serialPort;
    private final CountDownLatch portFound;
    private final AtomicReference<String> result;
    private final Function<IoStream, Void> callback;
    public static String SIGNATURE;

    public SerialAutoChecker(Logger logger, String serialPort, CountDownLatch portFound, AtomicReference<String> result, Function<IoStream, Void> callback) {
        this.logger = logger;
        this.serialPort = serialPort;
        this.portFound = portFound;
        this.result = result;
        this.callback = callback;
    }

    public SerialAutoChecker(Logger logger, String serialPort, CountDownLatch portFound, AtomicReference<String> result) {
        this(logger, serialPort, portFound, result, null);
    }

    @Override
    public void run() {
        IoStream stream = SerialIoStreamJSerialComm.openPort(serialPort, logger);
        Logger logger = FileLog.LOGGER;
        IncomingDataBuffer incomingData = stream.getDataBuffer();
        try {
            HelloCommand.send(stream, logger);
            byte[] response = incomingData.getPacket(logger, "", false);
            if (!checkResponseCode(response, BinaryProtocolCommands.RESPONSE_OK))
                return;
            String signature = new String(response, 1, response.length - 1);
            SIGNATURE = signature;
            System.out.println("Got signature=" + signature + " from " + serialPort);
            if (signature.startsWith(Fields.PROTOCOL_SIGNATURE_PREFIX)) {
                if (callback != null) {
                    callback.apply(stream);
                }
                result.set(serialPort);
                portFound.countDown();
            }
        } catch (IOException ignore) {
            return;
        } finally {
            stream.close();
        }

    }
}
