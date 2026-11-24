package com.rusefi.ui;

import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.SwingUtilities;
import java.awt.Component;
import java.awt.Container;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Undo, Redo and Discard replace Tuning's tune; views following the working tune through
 * {@link UIContext#fireConfigImageChanged} (Pinout's Tune use, {@link UIContext}'s current image) must see it,
 * or they keep showing the replaced tune.
 */
class TuningUndoNotificationTest {
    private static final String B6 = "B6 Low Side output";
    private static final String B7 = "B7 Low Side output";

    @Test
    void undoRedoAndDiscardNotifyConfigImageListeners() throws Throwable {
        IniFileModel ini = readIni();
        UIContext context = new UIContext();
        AtomicReference<TuningPane> tuningRef = new AtomicReference<>();
        List<String> notifiedFan = new ArrayList<>();

        try {
            SwingUtilities.invokeAndWait(() -> {
                context.setOfflineMode(true);
                context.setIniFileModelForOffline(ini);
                TuningPane tuning = new TuningPane(context);
                tuningRef.set(tuning);
                // fan on B7 as loaded
                tuning.seedOfflineImage(ConfigurationImageTestHelper.newLittleEndian(new byte[]{2}), "fanSettings");
                context.addConfigImageListener(image -> notifiedFan.add(fan(ini, image)));

                // the user picks B6 in the Fan page's pin selector
                JComboBox<?> fanSelector = components(tuning.getContent(), JComboBox.class).stream()
                    .filter(combo -> B7.equals(combo.getSelectedItem())).findFirst()
                    .orElseThrow(() -> new AssertionError("the Fan page shows its pin selector"));
                fanSelector.setSelectedItem(B6);
                assertFalse(notifiedFan.isEmpty(), "an edit notifies");
                assertEquals(B6, notifiedFan.get(notifiedFan.size() - 1));
                assertEquals(B6, fan(ini, workingTune(tuning)));
            });

            // an edit becomes an undo step once the user pauses (TuningToolbarWidget's idle timer)
            long deadline = System.currentTimeMillis() + 10_000;
            while (!undoEnabled(tuningRef.get())) {
                assertTrue(System.currentTimeMillis() < deadline, "the edit becomes undoable");
                Thread.sleep(20);
            }

            SwingUtilities.invokeAndWait(() -> {
                TuningPane tuning = tuningRef.get();
                int before = notifiedFan.size();
                button(tuning, "Undo").doClick();
                assertEquals(B7, fan(ini, workingTune(tuning)), "Undo restores the fan to B7");
                assertEquals(before + 1, notifiedFan.size(), "Undo notifies once");
                assertEquals(B7, notifiedFan.get(notifiedFan.size() - 1), "with the restored tune");
                assertEquals(B7, fan(ini, context.getCurrentConfigurationImage()));

                button(tuning, "Redo").doClick();
                assertEquals(B6, fan(ini, workingTune(tuning)));
                assertEquals(before + 2, notifiedFan.size(), "Redo notifies once");
                assertEquals(B6, notifiedFan.get(notifiedFan.size() - 1));

                button(tuning, "Discard changes").doClick();
                assertEquals(B7, fan(ini, workingTune(tuning)), "Discard restores the loaded tune");
                assertEquals(before + 3, notifiedFan.size(), "Discard notifies once");
                assertEquals(B7, notifiedFan.get(notifiedFan.size() - 1));
                assertEquals(B7, fan(ini, context.getCurrentConfigurationImage()));
            });
        } finally {
            SwingUtilities.invokeAndWait(() -> {
                if (tuningRef.get() != null) {
                    tuningRef.get().destroy();
                }
                context.getLinkManager().close();
            });
        }
    }

    private static boolean undoEnabled(TuningPane tuning) throws Exception {
        AtomicReference<Boolean> enabled = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> enabled.set(button(tuning, "Undo").isEnabled()));
        return enabled.get();
    }

    private static ConfigurationImage workingTune(TuningPane tuning) {
        return tuning.getDialogForTests().getWorkingImage();
    }

    private static String fan(IniFileModel ini, ConfigurationImage image) {
        return ConfigurationImageGetterSetter.getStringValue(ini.getAllIniFields().get("fanPin"), image)
            .replace("\"", "");
    }

    private static JButton button(TuningPane tuning, String text) {
        for (JButton button : components(tuning.getContent(), JButton.class)) {
            if (text.equals(button.getText())) {
                return button;
            }
        }
        throw new AssertionError("no " + text + " button");
    }

    private static <T extends Component> List<T> components(Component root, Class<T> type) {
        List<T> result = new ArrayList<>();
        if (type.isInstance(root)) {
            result.add(type.cast(root));
        }
        if (root instanceof Container) {
            for (Component child : ((Container) root).getComponents()) {
                result.addAll(components(child, type));
            }
        }
        return result;
    }

    private static IniFileModel readIni() throws Throwable {
        String text = "[Constants]\n"
            + "signature = \"tuning-undo-test\"\n"
            + "nPages = 1\npageSize = 1\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
            + "fanPin = bits, U08, 0, [0:1], \"NONE\", \"" + B6 + "\", \"" + B7 + "\"\n"
            + "[Menu]\nmenu = \"Setup\"\nsubMenu = fanSettings, \"Fan\"\n"
            + "[UserDefined]\n"
            + "dialog = fanSettings, \"Fan\"\nfield = \"Fan output\", fanPin\n";
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(raw, "tuning-undo-test.ini", new IniFileMetaInfoImpl(raw));
    }
}
