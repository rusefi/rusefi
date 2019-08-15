package com.rusefi.autodetect;

import com.opensr5.Logger;
import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolCommands;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.serial.PortHolder;

import java.io.IOException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicReference;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;

class SerialAutoChecker implements Runnable {
    private final String serialPort;
    private final CountDownLatch portFound;
    private final AtomicReference<String> result;

    public SerialAutoChecker(String serialPort, CountDownLatch portFound, AtomicReference<String> result) {
        this.serialPort = serialPort;
        this.portFound = portFound;
        this.result = result;
    }

    @Override
    public void run() {
        PortHolder.EstablishConnection establishConnection = new PortHolder.EstablishConnection(serialPort).invoke();
        if (!establishConnection.isConnected())
            return;
        IoStream stream = establishConnection.getStream();
        Logger logger = FileLog.LOGGER;
        IncomingDataBuffer incomingData = new IncomingDataBuffer(logger);
        stream.setInputListener(incomingData::addData);
        try {
            BinaryProtocol.sendPacket(new byte[]{BinaryProtocolCommands.COMMAND_HELLO}, logger, stream);
            byte[] response = incomingData.getPacket(logger, "", false, System.currentTimeMillis());
            if (!checkResponseCode(response, BinaryProtocolCommands.RESPONSE_OK))
                return;
            String message = new String(response, 1, response.length - 1);
            System.out.println("Got " + message + " from " + serialPort);
            if (message.startsWith(Fields.TS_SIGNATURE)) {
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
