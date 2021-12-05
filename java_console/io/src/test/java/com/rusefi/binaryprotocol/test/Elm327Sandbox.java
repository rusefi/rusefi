package com.rusefi.binaryprotocol.test;

import com.rusefi.binaryprotocol.BinaryProtocol;
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

        String signature = BinaryProtocol.getSignature(tsStream);
        System.out.println("Got " + signature + " signature via CAN/ELM327");
    }
}
