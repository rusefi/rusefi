package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.core.ui.FrameHelper;
import com.opensr5.io.DataListener;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.widgets.tune.CalibrationDialogWidget;
import com.rusefi.ui.widgets.tune.MainMenuTreeWidget;

import javax.swing.*;
import javax.xml.bind.JAXBException;
import java.awt.*;
import java.io.FileNotFoundException;
import java.io.IOException;


import static com.rusefi.ui.basic.UiHelper.commonUiStartup;

public class TopLevelMenuSandbox {
    public static void main(String[] args) throws JAXBException, FileNotFoundException {
        commonUiStartup();

        String iniPath = "../java_console/ui/src/test/resources/january.ini";
        IniFileModel model = IniFileReaderUtil.readIniFile(iniPath);
        Msq msq = Msq.readTune("../java_console/ui/src/test/resources/january_tune.msq");
        ConfigurationImage ci = msq.asImage(model);

        SwingUtilities.invokeLater(() -> {
            UIContext uiContext = new UIContext();
            uiContext.iniFileState.setIniFileModelForTest(model);

            AbstractIoStream tsStream = new AbstractIoStream() {
                @Override
                public void setInputListener(DataListener listener) {
                }

                @Override
                public IncomingDataBuffer getDataBuffer() {
                    return null;
                }

                @Override
                public void write(byte[] bytes) throws IOException {
                }
            };

            uiContext.getLinkManager().setBinaryProtocolForTests(new BinaryProtocol(uiContext.getLinkManager(), tsStream) {
                @Override
                public ConfigurationImage getControllerConfiguration() {
                    return ci;
                }

                @Override
                public IniFileModel getIniFileNullable() {
                    return model;
                }
            });

            runAwt(uiContext);
        });
    }

    private static void runAwt(UIContext uiContext) {
        FrameHelper frameHelper = new FrameHelper(JDialog.EXIT_ON_CLOSE);

        MainMenuTreeWidget left = new MainMenuTreeWidget(uiContext);

        CalibrationDialogWidget right = new CalibrationDialogWidget(uiContext);
        JScrollPane rightScrollPane = new JScrollPane(right.getContentPane());

        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, left.getContentPane(), rightScrollPane);
        splitPane.setResizeWeight(0.3);

        left.setOnSelect(subMenu -> {
            right.update(subMenu.getKey());
        });

        frameHelper.showFrame(splitPane);
        left.selectSubMenu("dwellSettings");
    }
}
