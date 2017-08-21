package com.rusefi.binaryprotocol;

import com.opensr5.ConfigurationImage;
import com.rusefi.RomRaiderWrapper;
import com.rusefi.UploadChanges;

import javax.swing.*;
import java.io.File;

/**
 * (c) Andrey Belomutskiy
 * 3/6/2015
 */
public class BinaryProtocolCmd {

    public static boolean checkForDefinitionFile() {
        if (!new File(RomRaiderWrapper.DEFINITION_FILE).exists()) {
            JOptionPane.showMessageDialog(null, RomRaiderWrapper.DEFINITION_FILE + " not found, turning off table editor");
            return false;
        }
        return true;
    }
}
