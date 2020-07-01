package com.rusefi;

import com.opensr5.ConfigurationImage;
import com.romraider.editor.ecu.ECUEditor;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolCmd;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;

import static com.romraider.editor.ecu.ECUEditorManager.getECUEditor;


public class RomEditorPane extends JPanel {
/*
    private final UIContext uiContext;

    public RomEditorPane(UIContext uiContext) {
        super(new BorderLayout());
        this.uiContext = uiContext;
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

            BinaryProtocol instance = LinkManager.getCurrentStreamStateStatic();
            if (instance == null)
                throw new NullPointerException("instance");
            ConfigurationImage image = instance.getControllerConfiguration();
            byte[] fileContent = ConfigurationImageFile.getFileContent(image);
            ECUEditor.openImage(fileContent);
        } catch (Exception e) {
            throw new IllegalStateException(e);
        }
        UiUtils.trueLayout(this);
    }
*/

}