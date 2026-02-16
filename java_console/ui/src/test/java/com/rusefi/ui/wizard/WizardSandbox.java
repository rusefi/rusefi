package com.rusefi.ui.wizard;

import com.formdev.flatlaf.FlatDarkLaf;
import com.opensr5.ini.IniFileModel;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.UiHelper;

import javax.swing.*;
import java.awt.*;
import java.io.FileNotFoundException;

import static com.rusefi.core.ui.FrameHelper.createFrame;

public class WizardSandbox {
    public static void main(String[] args) throws FileNotFoundException {
        UIContext uiContext = new UIContext();
        String iniPath = "../java_console/ui/src/test/resources/january.ini";
        IniFileModel model = IniFileReaderUtil.readIniFile(iniPath);
        uiContext.iniFileState.setIniFileModelForTest(model);

        UiHelper.configureLaf();

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                FrameHelper fh = createFrame("Wizard Sandbox");

                NumberOfCylindersPanel content = new NumberOfCylindersPanel(uiContext);

                content.setOnCylindersSelected(cylindersCount -> {
                    fh.getFrame().getContentPane().removeAll();
                    FiringOrderPanel firingOrderPanel = new FiringOrderPanel(uiContext, cylindersCount);
                    fh.getFrame().add(firingOrderPanel.getPanel());
                    fh.getFrame().revalidate();
                    fh.getFrame().repaint();
                });

                fh.getFrame().add(content.getPanel());

                fh.getFrame().setSize(800, 600);
                fh.getFrame().setLocationRelativeTo(null);
                fh.getFrame().setVisible(true);
            }
        });
    }
}
