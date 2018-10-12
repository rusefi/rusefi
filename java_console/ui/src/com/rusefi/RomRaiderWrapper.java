package com.rusefi;


import com.romraider.ECUExec;
import com.romraider.Settings;
import com.romraider.editor.ecu.ECUEditor;
import com.romraider.editor.ecu.ECUEditorManager;
import com.romraider.swing.LookAndFeelManager;
import com.romraider.util.LogManager;
import com.romraider.util.SettingsManager;

import java.io.File;

import static com.romraider.editor.ecu.ECUEditorManager.getECUEditor;

/**
 * (c) Andrey Belomutskiy 2013-2018
 * 3/6/2015
 */
public class RomRaiderWrapper {
    public static final String DEFINITION_FILE = "rusefi.xml";

    public static void main(String[] args) {
        startRomRaider();

        ECUExec.openRom(ECUEditorManager.getECUEditor(), "rusefi_configuration.bin");
    }

    public static ECUEditor startRomRaider() {
        init();

//        LookAndFeelManager.initLookAndFeel();

        ECUEditor editor = getECUEditor();
        editor.initializeEditorUI();
        return editor;
    }

    public static void init() {
        Settings settings = SettingsManager.getSettings();
        settings.getEcuDefinitionFiles().clear();
        settings.addEcuDefinitionFile(new File(DEFINITION_FILE));
        LogManager.initDebugLogging();
    }
}
