package com.rusefi.binaryprotocol;

import com.opensr5.ConfigurationImage;
import com.opensr5.Logger;
import com.opensr5.io.ConfigurationImageFile;
import com.romraider.editor.ecu.ECUEditor;
import com.rusefi.RomRaiderWrapper;
import com.rusefi.TsPageSize;
import com.rusefi.UploadChanges;
import com.rusefi.io.serial.PortHolder;
import jssc.SerialPort;

/**
 * (c) Andrey Belomutskiy
 * 6/21/2017
 */
public class BinaryProtocolCmdSandbox {

    public static void main(String[] args) throws Exception {
        if (args.length != 1) {
            System.out.println("Exactly one parameter expected: port");
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
        BinaryProtocol bp = new BinaryProtocol(logger, serialPort);

        PortHolder.setupPort(serialPort, 38400);
        logger.info("Binary looks good!");
        bp.switchToBinaryProtocol();

        bp.readImage(TsPageSize.IMAGE_SIZE);
        //
//        image.saveToFile("rusefi_configuration.bin");
//
        doShowImage(bp.getController());
    }

    private static void doShowImage(ConfigurationImage image) throws Exception {
        if (!BinaryProtocolCmd.checkForDefinitionFile())
            return;
        RomRaiderWrapper.startRomRaider();
        ECUEditor.openImage(ConfigurationImageFile.getFileContent(image));
    }
}
