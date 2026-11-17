package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.opensr5.io.DataListener;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.widgets.tune.CalibrationDialogWidget;
import com.rusefi.ui.widgets.tune.CalibrationPanelWorkspace;
import com.rusefi.ui.widgets.tune.MainMenuTreeWidget;

import javax.swing.*;
import jakarta.xml.bind.JAXBException;
import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;


import static com.rusefi.ui.basic.UiHelper.commonUiStartup;

/**
 * Visual sandbox for {@link MainMenuTreeWidget}, the curve UI control
 * {@link com.rusefi.ui.widgets.tune.CurveWidget}, and the table UI control
 * {@link com.rusefi.ui.widgets.tune.TuningTableView}.
 */
public class TopLevelMenuSandbox {
    private static final String INI_RESOURCE = "/26october.ini";
    private static final String TUNE_RESOURCE = "/26october.msq";

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

        PersistentConfiguration config = PersistentConfiguration.getConfig();
        CalibrationPanelWorkspace workspace = new CalibrationPanelWorkspace(uiContext,
            config.getRoot().getChild("top_level_menu_sandbox").getChild(CalibrationPanelWorkspace.CONFIG_KEY));
        CalibrationDialogWidget right = workspace.getPrimary();
        right.setActive(true);
        workspace.setOnConfigChange(image -> {
            left.refreshExpressions(image);
            uiContext.fireConfigImageChanged(image);
        });

        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, left.getContentPane(), workspace.getContent());
        splitPane.setResizeWeight(0.3);

        left.setOnSelect(subMenu -> {
            ConfigurationImage image = right.getWorkingImage();
            right.update(subMenu.getKey(), uiContext.iniFileState.getIniFileModel(),
                image == null ? uiContext.getBinaryProtocol().getControllerConfiguration() : image);
        });

        // Avoid WSL2 mouse-position offsets in maximized windows by using 90% of the usable screen.
        Rectangle screen = GraphicsEnvironment.getLocalGraphicsEnvironment().getMaximumWindowBounds();
        JFrame frame = frameHelper.getFrame();
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent event) {
                workspace.destroy();
                config.save();
            }
        });
        frame.setSize((int) (screen.width * 0.9), (int) (screen.height * 0.9));
        frame.setLocation(screen.x + (screen.width - frame.getWidth()) / 2,
                screen.y + (screen.height - frame.getHeight()) / 2);
        frameHelper.showFrame(splitPane, false);
        left.selectSubMenu("dwellSettings");
    }
}
