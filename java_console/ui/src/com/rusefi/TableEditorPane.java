package com.rusefi;

import com.romraider.editor.ecu.ECUEditor;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolCmd;
import com.rusefi.io.ConfigurationImageFile;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;

import static com.romraider.editor.ecu.ECUEditorManager.getECUEditor;

public class TableEditorPane extends JPanel {

    public TableEditorPane() {
        super(new BorderLayout());
        UiUtils.showLoadingMessage(this);
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
            byte[] fileContent = ConfigurationImageFile.getFileContent(image);
            ECUEditor.openImage(fileContent);
        } catch (Exception e) {
            throw new IllegalStateException(e);
        }
        UiUtils.trueLayout(this);
    }
}