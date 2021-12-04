package com.rusefi.binaryprotocol.test;

import com.rusefi.io.IoStream;
import com.rusefi.io.can.Elm327Connector;
import com.rusefi.io.serial.BaudRateHolder;
import com.rusefi.io.serial.SerialIoStreamJSerialComm;

import java.io.IOException;

import static com.rusefi.Timeouts.SECOND;
import static com.rusefi.io.can.Elm327Connector.ELM327_DEFAULT_BAUDRATE;
import static com.rusefi.io.can.Elm327Connector.ELM_EOL;

public class Elm327Sandbox {
    public static void main(String[] args) throws InterruptedException, IOException {
        BaudRateHolder.INSTANCE.baudRate = ELM327_DEFAULT_BAUDRATE;
        String serialPort = "COM5";

        IoStream stream = SerialIoStreamJSerialComm.openPort(serialPort);
        stream.setInputListener(freshData -> System.out.println("onDataArrived"));

        stream.write((Elm327Connector.HELLO + ELM_EOL).getBytes());
        Thread.sleep(6 * SECOND);
    }
}
