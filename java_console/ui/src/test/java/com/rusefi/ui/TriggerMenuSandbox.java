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
 * Visual sandbox showing only "trigger" submenu panel.
 */
public class TriggerMenuSandbox {
    private static final String INI_RESOURCE = "/january.ini";
    private static final String TUNE_RESOURCE = "/january_tune.msq";

    public static void main(String[] args) throws JAXBException, IOException, IniParsingException, java.net.URISyntaxException {
        commonUiStartup();

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

            runAwt(uiContext, model, ci);
        });
    }

    private static IniFileModel readIniFromClasspath(String resource) throws IOException, IniParsingException {
        try (InputStream is = TriggerMenuSandbox.class.getResourceAsStream(resource)) {
            if (is == null)
                throw new IllegalStateException("Resource not found on classpath: " + resource);
            RawIniFile content = IniFileReaderUtil.read(is, resource);
            return IniFileReaderUtil.readIniFile(content, resource, new IniFileMetaInfoImpl(content));
        }
    }

    private static File resourceFile(String resource) throws java.net.URISyntaxException {
        URL url = TriggerMenuSandbox.class.getResource(resource);
        if (url == null)
            throw new IllegalStateException("Resource not found on classpath: " + resource);
        return new File(url.toURI());
    }

    private static void runAwt(UIContext uiContext, IniFileModel model, ConfigurationImage ci) {
        FrameHelper frameHelper = new FrameHelper(JDialog.EXIT_ON_CLOSE);

        CalibrationDialogWidget panel = new CalibrationDialogWidget(uiContext);
        // "triggerConfiguration" is the dialog key for "Trigger" submenu in january.ini
        panel.update("triggerConfiguration");

        JScrollPane scrollPane = new JScrollPane(panel.getContentPane());
        frameHelper.showFrame(scrollPane);

        // Ensure we have a valid trigger type for visualization if needed
        model.findIniField("trigger_type").ifPresent(field -> {
            com.opensr5.ConfigurationImageGetterSetter.setValue2(field, ci, "trigger_type", "60-2");
        });
        panel.update("triggerConfiguration");
    }
}
