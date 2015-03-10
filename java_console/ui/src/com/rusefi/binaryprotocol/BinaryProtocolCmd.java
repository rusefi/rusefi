package com.rusefi.binaryprotocol;

import com.romraider.editor.ecu.ECUEditor;
import com.romraider.util.SettingsManager;
import com.rusefi.ConfigurationImage;
import com.rusefi.Logger;
import com.rusefi.RomRaiderWrapper;
import com.rusefi.UploadChanges;
import com.rusefi.io.serial.PortHolder;
import jssc.SerialPort;

/**
 * (c) Andrey Belomutskiy
 * 3/6/2015
 */
public class BinaryProtocolCmd {
    static BinaryProtocol bp;

    public static void main(String[] args) throws Exception {
        if (args.length != 1) {
            System.out.println("Exactly one parameter expected");
            return;
        }

        String port = args[0];

        Logger logger = UploadChanges.logger;

        SerialPort serialPort;
        serialPort = new SerialPort(port);
        boolean opened = serialPort.openPort();
        if (!opened) {
            logger.error("failed to open " + port);
        }
        bp = new BinaryProtocol(logger, serialPort);

        PortHolder.setupPort(serialPort, 38400);
        logger.info("Looks good");
        bp.switchToBinaryProtocol();

        bp.readImage(BinaryProtocol.IMAGE_SIZE);
        ConfigurationImage image = bp.getController();
//
//        image.saveToFile("rusefi_configuration.bin");
//
        RomRaiderWrapper.startRomRaider();
        ECUEditor.openImage(image.getFileContent(), SettingsManager.getSettings().getEcuDefinitionFiles().elementAt(0),
                "rusEfi");
    }

    public static void scheduleBurn(ConfigurationImage newVersion) {
        UploadChanges.scheduleBurn(newVersion, bp);
    }
}
