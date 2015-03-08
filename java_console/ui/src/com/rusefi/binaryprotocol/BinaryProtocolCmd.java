package com.rusefi.binaryprotocol;

import com.romraider.Settings;
import com.romraider.editor.ecu.ECUEditor;
import com.romraider.util.SettingsManager;
import com.rusefi.ConfigurationImage;
import com.rusefi.Logger;
import com.rusefi.RomRaiderWrapper;
import com.rusefi.io.serial.PortHolder;
import jssc.SerialPort;
import jssc.SerialPortException;

import java.io.IOException;

/**
 * (c) Andrey Belomutskiy
 * 3/6/2015
 */
public class BinaryProtocolCmd {

    public static void main(String[] args) throws Exception {
        if (args.length != 1) {
            System.out.println("Exactly one parameter expected");
            return;
        }

        String port = args[0];

        Logger logger = Logger.STDOUT;

        SerialPort serialPort;
        serialPort = new SerialPort(port);
        boolean opened = serialPort.openPort();
        if (!opened) {
            logger.error("failed to open " + port);
        }
        BinaryProtocol bp = new BinaryProtocol(logger, serialPort);

        PortHolder.setupPort(serialPort, 38400);
        logger.info("Looks good");
        bp.exchange(new byte[]{'S'});

        ConfigurationImage image = new ConfigurationImage(14008);

        bp.readImage(image);

        image.saveToFile("rusefi_configuration.bin");

        RomRaiderWrapper.startRomRaider();

        ECUEditor.openImage(image.getFileContent(), SettingsManager.getSettings().getEcuDefinitionFiles().elementAt(0),
                "rusEfi");

    }

}
