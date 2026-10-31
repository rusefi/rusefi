package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.UiHelper;

import javax.swing.*;
import java.io.FileNotFoundException;
import java.nio.file.Files;
import java.nio.file.Paths;

import static com.rusefi.core.ui.FrameHelper.createFrame;

/**
 * Visual sandbox for the setup wizard, offline (no ECU): a mocked {@link BinaryProtocol}
 * holds an in-memory configuration image, and "uploads" just replace that image.
 * <p>
 * Hosts the real {@link WizardContainer} so the sandbox always runs the exact same step
 * flow as the console wizard — do not wire individual step panels here by hand.
 * <p>
 * Uses a current generated INI (which declares the {@code wizard*} progress flags) rather
 * than the january.ini snapshot; without those flags {@link WizardContainer} would treat
 * every step as already satisfied and jump straight to the completion card.
 */
public class WizardSandbox {
    private static final String GENERATED_INI = "firmware/tunerstudio/generated/rusefi_f407-discovery.ini";

    public static void main(String[] args) throws FileNotFoundException {
        UIContext uiContext = createOfflineContext();
        int width = args.length > 0 ? Integer.parseInt(args[0]) : 800;
        int height = args.length > 1 ? Integer.parseInt(args[1]) : 600;

        UiHelper.configureLaf();

        SwingUtilities.invokeLater(() -> {
            FrameHelper fh = createFrame("Wizard Sandbox");

            WizardContainer wizard = new WizardContainer(uiContext);
            wizard.setOnWizardExit(() -> System.exit(0));
            fh.getFrame().add(wizard);
            wizard.startWizard();

            fh.getFrame().setSize(width, height);
            fh.getFrame().setLocationRelativeTo(null);
            fh.getFrame().setVisible(true);
        });
    }

    /** No-ECU {@link UIContext}: generated INI + blank config image behind a mocked {@link BinaryProtocol}. */
    static UIContext createOfflineContext() throws FileNotFoundException {
        IniFileModel model = IniFileReaderUtil.readIniFile(resolveIniPath());
        return createOfflineContext(model);
    }

    static UIContext createOfflineContext(IniFileModel model) {
        UIContext uiContext = new UIContext();
        uiContext.iniFileState.setIniFileModelForTest(model);

        // Blank image: all wizard flags read "no" so every step is shown
        int totalSize = 0;
        for (int page = 0; page < model.getMetaInfo().getnPages(); page++) {
            totalSize += model.getMetaInfo().getPageSize(page);
        }
        ConfigurationImage blankImage = new ConfigurationImage(totalSize);

        AbstractIoStream deadStream = new AbstractIoStream() {
            @Override
            public void setInputListener(DataListener listener) {
            }

            @Override
            public IncomingDataBuffer getDataBuffer() {
                return null;
            }

            @Override
            public void write(byte[] bytes) {
            }
        };

        BinaryProtocol offlineBp = new BinaryProtocol(uiContext.getLinkManager(), deadStream) {
            @Override
            public void uploadChanges(ConfigurationImage newVersion) {
                setConfigurationImage(newVersion);
            }

            @Override
            public IniFileModel getIniFileNullable() {
                return model;
            }
        };
        offlineBp.setConfigurationImage(blankImage);
        uiContext.getLinkManager().setBinaryProtocolForTests(offlineBp);
        return uiContext;
    }

    /** Supports launching from either the repository root or the java_console directory. */
    private static String resolveIniPath() throws FileNotFoundException {
        for (String candidate : new String[]{GENERATED_INI, "../" + GENERATED_INI}) {
            if (Files.exists(Paths.get(candidate))) {
                return candidate;
            }
        }
        throw new FileNotFoundException(GENERATED_INI + " not found; run from the repository root");
    }
}
