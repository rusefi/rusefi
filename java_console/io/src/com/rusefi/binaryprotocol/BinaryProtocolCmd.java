package com.rusefi.binaryprotocol;

import com.rusefi.ConfigurationImage;
import com.rusefi.Logger;
import com.rusefi.io.DataListener;
import com.rusefi.io.serial.PortHolder;
import com.rusefi.io.serial.SerialPortReader;
import etch.util.CircularByteBuffer;
import jssc.SerialPort;
import jssc.SerialPortException;

import java.io.EOFException;
import java.io.IOException;
import java.util.Arrays;

/**
 * (c) Andrey Belomutskiy
 * 3/6/2015
 */
public class BinaryProtocolCmd {
    private static SerialPort serialPort;

    public static void main(String[] args) throws SerialPortException, InterruptedException, IOException {
        if (args.length != 1) {
            System.out.println("Exactly one parameter expected");
            return;
        }

        String port = args[0];

        Logger logger = Logger.STDOUT;

        serialPort = new SerialPort(port);
        boolean opened = serialPort.openPort();
        if (!opened) {
            logger.error("failed to open " + port);
        }
        BinaryProtocol bp = new BinaryProtocol(logger, serialPort);


        PortHolder.setupPort(serialPort, 38400);
        logger.info("Looks good");
        bp.sendQueryCommand();
        bp.receivePacket();

        ConfigurationImage image = new ConfigurationImage(14008);

        bp.readImage(image);

        image.saveToFile("rusefi_configuration.bin");

    }

}
