package com.rusefi.binaryprotocol;

import com.romraider.editor.ecu.ECUEditor;
import com.rusefi.*;
import com.rusefi.io.ConfigurationImageFile;
import com.rusefi.io.serial.PortHolder;
import jssc.SerialPort;

import javax.swing.*;
import java.io.File;

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
        logger.info("Binary looks good!");
        bp.switchToBinaryProtocol();

        bp.readImage(TsPageSize.IMAGE_SIZE);
        //
//        image.saveToFile("rusefi_configuration.bin");
//
        doShowImage(bp.getController());
    }

    public static void doShowImage(ConfigurationImage image) throws Exception {
        if (!checkForDefinitionFile())
            return;
        RomRaiderWrapper.startRomRaider();
        ECUEditor.openImage(ConfigurationImageFile.getFileContent(image));
    }

    public static boolean checkForDefinitionFile() {
        if (!new File(RomRaiderWrapper.DEFINITION_FILE).exists()) {
            JOptionPane.showMessageDialog(null, RomRaiderWrapper.DEFINITION_FILE + " not found, turning off table editor");
            return false;
        }
        return true;
    }

    public static void scheduleUpload(ConfigurationImage newVersion) {
        UploadChanges.scheduleUpload(newVersion, null);
    }
}
