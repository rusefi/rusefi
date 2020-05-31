package com.rusefi.autodetect;

import com.opensr5.Logger;
import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolCommands;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.serial.SerialIoStreamJSerialComm;

import java.io.IOException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicReference;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;

public class SerialAutoChecker implements Runnable {
    private final String serialPort;
    private final CountDownLatch portFound;
    private final AtomicReference<String> result;
    public static String SIGNATURE;

    public SerialAutoChecker(String serialPort, CountDownLatch portFound, AtomicReference<String> result) {
        this.serialPort = serialPort;
        this.portFound = portFound;
        this.result = result;
    }

    @Override
    public void run() {
        IoStream stream = SerialIoStreamJSerialComm.openPort(serialPort);
        Logger logger = FileLog.LOGGER;
        IncomingDataBuffer incomingData = BinaryProtocol.createDataBuffer(stream, logger);
        try {
            stream.sendPacket(new byte[]{BinaryProtocolCommands.COMMAND_HELLO}, logger);
            byte[] response = incomingData.getPacket(logger, "", false);
            if (!checkResponseCode(response, BinaryProtocolCommands.RESPONSE_OK))
                return;
            String signature = new String(response, 1, response.length - 1);
            SIGNATURE = signature;
            System.out.println("Got " + signature + " from " + serialPort);
            String signatureWithoutMinorVersion = Fields.TS_SIGNATURE.substring(0, Fields.TS_SIGNATURE.length() - 2);
            if (signature.startsWith(signatureWithoutMinorVersion)) {
                result.set(serialPort);
                portFound.countDown();
            }
        } catch (IOException | InterruptedException ignore) {
            return;
        } finally {
            stream.close();
        }

    }
}
