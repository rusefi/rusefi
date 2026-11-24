package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;

/**
 * [tag:unburned_marks] The Tuning tab end to end (#10671): edits mark their rows and are counted in the
 * toolbar, and Discard clears both.
 */
public class TuningUnburnedChangesTest {
    private static final String INI = "[Constants]\n"
        + "signature = \"unburned-test\"\n"
        + "nPages = 1\npageSize = 12\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
        + "injectorFlow = scalar, F32, 0, \"cc/min\", 1, 0, 0, 2000, 1\n"
        + "enableA = bits, U32, 4, [0:0], \"Disabled\", \"Enabled\"\n"
        + "enableB = bits, U32, 4, [1:1], \"Disabled\", \"Enabled\"\n"
        + "[Menu]\nmenu = \"Fuel\"\nsubMenu = injection, \"Injection\"\n"
        + "[UserDefined]\ndialog = injection, \"Injection\"\n"
        + "field = \"Injector flow\", injectorFlow\n"
        + "field = \"Enable A\", enableA\n"
        + "field = \"Enable B\", enableB\n";

    @Test
    void editsAreMarkedAndCountedUntilDiscarded() throws Throwable {
        IniFileModel ini = readIni();
        UIContext context = new UIContext();
        AtomicReference<TuningPane> tuning = new AtomicReference<>();
        AtomicReference<Throwable> failure = new AtomicReference<>();

        SwingUtilities.invokeAndWait(() -> {
            context.setOfflineMode(true);
            context.setIniFileModelForOffline(ini);
            TuningPane pane = new TuningPane(context);
            tuning.set(pane);
            ConfigurationImage image = ConfigurationImageTestHelper.newLittleEndian(new byte[12]);
            image.getByteBuffer(0, 4).putFloat(440);
            pane.seedOfflineImage(image, "injection");
        });
        SwingUtilities.invokeAndWait(() -> {
            try {
                TuningPane pane = tuning.get();
                assertEquals("Offline tune", stateLabel(pane).getText());

                field(pane, "injectorFlow", JTextField.class).setText("470");
                assertEquals("Local changes not burned to ECU (1 value)", stateLabel(pane).getText(),
                    "the label counts the edit just made, not the one before");
                assertEquals(Collections.singletonList("injectorFlow"),
                    pane.getDialogForTests().getUnburnedFieldKeysForTests());
                assertEquals(Collections.singleton("injection"), pane.getUnburnedPageKeysForTests(),
                    "the menu page is dotted");

                field(pane, "enableB", JCheckBox.class).doClick();
                assertEquals("Local changes not burned to ECU (2 values)", stateLabel(pane).getText());
                assertEquals(Arrays.asList("injectorFlow", "enableB"),
                    pane.getDialogForTests().getUnburnedFieldKeysForTests());

                field(pane, "injectorFlow", JTextField.class).setText("440");
                assertEquals("Local changes not burned to ECU (1 value)", stateLabel(pane).getText(),
                    "a value typed back to the loaded one is no longer counted");

                button(pane, "Discard changes").doClick();
                assertEquals("Offline tune", stateLabel(pane).getText());
                assertEquals(Collections.emptyList(), pane.getDialogForTests().getUnburnedFieldKeysForTests());
                assertEquals(Collections.emptySet(), pane.getUnburnedPageKeysForTests(),
                    "Discard clears the menu dot too");
            } catch (Throwable t) {
                failure.set(t);
            }
        });
        if (failure.get() != null) {
            throw failure.get();
        }
    }

    /** Undo and Redo replace the tune: the label, the row marks and the menu dot follow it. */
    @Test
    void undoAndRedoRefreshTheMarks() throws Throwable {
        IniFileModel ini = readIni();
        UIContext context = new UIContext();
        AtomicReference<TuningPane> tuning = new AtomicReference<>();
        AtomicReference<Throwable> failure = new AtomicReference<>();

        SwingUtilities.invokeAndWait(() -> {
            context.setOfflineMode(true);
            context.setIniFileModelForOffline(ini);
            TuningPane pane = new TuningPane(context);
            tuning.set(pane);
            ConfigurationImage image = ConfigurationImageTestHelper.newLittleEndian(new byte[12]);
            image.getByteBuffer(0, 4).putFloat(440);
            pane.seedOfflineImage(image, "injection");
        });
        SwingUtilities.invokeAndWait(() -> {
            try {
                TuningPane pane = tuning.get();
                // one undo step, as a Pinout assignment makes
                assertTrue(pane.applyEdit(image -> image.getByteBuffer(0, 4).putFloat(470)));
                assertEquals("Local changes not burned to ECU (1 value)", stateLabel(pane).getText());
                assertEquals(Collections.singletonList("injectorFlow"),
                    pane.getDialogForTests().getUnburnedFieldKeysForTests());
                assertEquals(Collections.singleton("injection"), pane.getUnburnedPageKeysForTests());
                JLabel loaded = (JLabel) all(pane.getDialogForTests().getContentPane()).stream()
                    .filter(c -> "unburnedValue".equals(c.getName())).findFirst().orElseThrow(AssertionError::new);
                assertTrue(loaded.getText().startsWith("Loaded: 440"),
                    "offline the reference is the loaded tune: " + loaded.getText());

                button(pane, "Undo").doClick();
                assertEquals("Offline tune", stateLabel(pane).getText(), "Undo back to the loaded tune");
                assertEquals(Collections.emptyList(), pane.getDialogForTests().getUnburnedFieldKeysForTests());
                assertEquals(Collections.emptySet(), pane.getUnburnedPageKeysForTests());

                button(pane, "Redo").doClick();
                assertEquals("Local changes not burned to ECU (1 value)", stateLabel(pane).getText());
                assertEquals(Collections.singletonList("injectorFlow"),
                    pane.getDialogForTests().getUnburnedFieldKeysForTests());
                assertEquals(Collections.singleton("injection"), pane.getUnburnedPageKeysForTests());
            } catch (Throwable t) {
                failure.set(t);
            }
        });
        if (failure.get() != null) {
            throw failure.get();
        }
    }

    private static JLabel stateLabel(TuningPane pane) {
        for (Component c : all(pane.getContent())) {
            if (c instanceof JLabel) {
                String text = ((JLabel) c).getText();
                if ("Offline tune".equals(text) || text != null && text.startsWith("Local changes")) {
                    return (JLabel) c;
                }
            }
        }
        throw new AssertionError("no toolbar state label");
    }

    private static <T extends Component> T field(TuningPane pane, String fieldKey, Class<T> type) {
        for (Component c : all(pane.getDialogForTests().getContentPane())) {
            if (c instanceof JComponent && fieldKey.equals(((JComponent) c).getClientProperty("rusefi.fieldKey"))) {
                for (Component child : all((Container) c)) {
                    if (type.isInstance(child)) {
                        return type.cast(child);
                    }
                }
            }
        }
        throw new AssertionError("no " + type.getSimpleName() + " for " + fieldKey);
    }

    private static JButton button(TuningPane pane, String text) {
        for (Component c : all(pane.getContent())) {
            if (c instanceof JButton && text.equals(((JButton) c).getText())) {
                return (JButton) c;
            }
        }
        throw new AssertionError("no button " + text);
    }

    private static List<Component> all(Container parent) {
        List<Component> result = new ArrayList<>();
        for (Component c : parent.getComponents()) {
            result.add(c);
            if (c instanceof Container) {
                result.addAll(all((Container) c));
            }
        }
        return result;
    }

    private static IniFileModel readIni() throws Throwable {
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(INI.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(raw, "unburned-test.ini", new IniFileMetaInfoImpl(raw));
    }
}
