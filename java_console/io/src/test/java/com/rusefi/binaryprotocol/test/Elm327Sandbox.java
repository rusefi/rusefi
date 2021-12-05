package com.rusefi.binaryprotocol.test;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.can.Elm327Connector;
import com.rusefi.io.serial.BaudRateHolder;
import com.rusefi.io.serial.SerialIoStream;

import java.io.IOException;

import static com.rusefi.io.can.Elm327Connector.ELM327_DEFAULT_BAUDRATE;

public class Elm327Sandbox {
    public static void main(String[] args) throws InterruptedException, IOException {
        BaudRateHolder.INSTANCE.baudRate = ELM327_DEFAULT_BAUDRATE;
        String serialPort = "COM5";
        Elm327Connector connector = new Elm327Connector(SerialIoStream.openPort(serialPort));
        connector.start("sandbox");

        IoStream tsStream = connector.getTsStream();

        IncomingDataBuffer dataBuffer = tsStream.getDataBuffer();
        System.out.println("Hello new connection " + dataBuffer.getPendingCount());

        runFcommand("First time", tsStream);
        whyDoWeNeedToSleepBetweenCommands();

        runFcommand("Second time", tsStream);
        whyDoWeNeedToSleepBetweenCommands();

        {
            String signature = BinaryProtocol.getSignature(tsStream);
            System.out.println("Got " + signature + " signature via CAN/ELM327");
        }

        whyDoWeNeedToSleepBetweenCommands();

        {
            String signature = BinaryProtocol.getSignature(tsStream);
            System.out.println("Let's do it again! Got " + signature + " signature via CAN/ELM327");
        }
    }

    private static void runFcommand(String prefix, IoStream tsStream) throws IOException {
        IncomingDataBuffer dataBuffer = tsStream.getDataBuffer();
        tsStream.sendPacket(new byte[]{Fields.TS_COMMAND_F});
        byte[] fResponse = new byte[3];
        dataBuffer.waitForBytes("hello", System.currentTimeMillis(), fResponse.length);
        dataBuffer.getData(fResponse);
        System.out.println(prefix + " Got F response " + IoStream.printHexBinary(fResponse));
    }

    /**
     * TODO: HUH? what's that about?!
     */
    private static void whyDoWeNeedToSleepBetweenCommands() throws InterruptedException {
        Thread.sleep(200);
    }
}
