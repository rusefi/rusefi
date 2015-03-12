package com.rusefi;

import com.romraider.editor.ecu.ECUEditor;
import com.romraider.util.SettingsManager;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolCmd;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;

import static com.romraider.editor.ecu.ECUEditorManager.getECUEditor;

public class TableEditor extends JPanel {

    public TableEditor() {
        super(new BorderLayout());
        add(new JLabel("Loading tables"), BorderLayout.CENTER);
    }

    public void showContent() {
        removeAll();
        if (!BinaryProtocolCmd.checkForDefinitionFile())
            return;
        try {
            RomRaiderWrapper.init();

            ECUEditor editor = getECUEditor();
            editor.initializeEditorUI();

            add(editor.getContent());

            BinaryProtocol instance = BinaryProtocol.instance;
            if (instance == null)
                throw new NullPointerException("instance");
            ConfigurationImage image = instance.getController();
            ECUEditor.openImage(image.getFileContent(), SettingsManager.getSettings().getEcuDefinitionFiles().elementAt(0),
                    "rusEfi");
        } catch (Exception e) {
            throw new IllegalStateException(e);
        }
        UiUtils.trueLayout(this);
    }
}