package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.core.ui.FrameHelper;
import com.opensr5.io.DataListener;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.widgets.tune.CalibrationDialogWidget;
import com.rusefi.ui.widgets.tune.MainMenuTreeWidget;

import javax.swing.*;
import jakarta.xml.bind.JAXBException;
import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;


import static com.rusefi.ui.basic.UiHelper.commonUiStartup;

/**
 * Visual sandbox for the top-level calibration menu tree.
 */
public class TopLevelMenuSandbox {
    private static final String INI_RESOURCE = "/january.ini";
    private static final String TUNE_RESOURCE = "/january_tune.msq";

    public static void main(String[] args) throws JAXBException, IOException, IniParsingException, java.net.URISyntaxException {
        commonUiStartup();

        // Load test resources from the classpath so the working directory does not matter
        IniFileModel model = readIniFromClasspath(INI_RESOURCE);
        Msq msq = Msq.readTune(resourceFile(TUNE_RESOURCE).getAbsolutePath());
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

    private static IniFileModel readIniFromClasspath(String resource) throws IOException, IniParsingException {
        try (InputStream is = TopLevelMenuSandbox.class.getResourceAsStream(resource)) {
            if (is == null)
                throw new IllegalStateException("Resource not found on classpath: " + resource);
            RawIniFile content = IniFileReaderUtil.read(is, resource);
            return IniFileReaderUtil.readIniFile(content, resource, new IniFileMetaInfoImpl(content));
        }
    }

    private static File resourceFile(String resource) throws java.net.URISyntaxException {
        URL url = TopLevelMenuSandbox.class.getResource(resource);
        if (url == null)
            throw new IllegalStateException("Resource not found on classpath: " + resource);
        return new File(url.toURI());
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

