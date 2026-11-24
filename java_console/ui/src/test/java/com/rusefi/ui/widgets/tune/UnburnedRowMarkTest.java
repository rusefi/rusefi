package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;

/**
 * [tag:unburned_marks] Setting rows changed since the last burn are marked, show the burned value and
 * can be set back to it (#10671).
 */
public class UnburnedRowMarkTest {
    private static final String INI = "[Constants]\npage = 1\n"
        + "injectorFlow = scalar, F32, 0, \"cc/min\", 1, 0, 0, 2000, 1\n"
        + "enableA = bits, U32, 4, [0:0], \"Disabled\", \"Enabled\"\n"
        + "enableB = bits, U32, 4, [1:1], \"Disabled\", \"Enabled\"\n"
        + "injMode = bits, U08, 8, [0:1], \"Sequential\", \"Batch\", \"Simultaneous\", \"Off\"\n"
        + "[UserDefined]\ndialog = injection, \"Injection\"\n"
        + "field = \"Injector flow\", injectorFlow\n"
        + "field = \"Enable A\", enableA\n"
        + "field = \"Enable B\", enableB\n"
        + "field = \"Injection mode\", injMode\n";

    private static IniFileModel ini() throws IniParsingException {
        return IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(INI.getBytes(StandardCharsets.US_ASCII))),
            "unburned.ini", mock(IniFileMetaInfo.class));
    }

    private static ConfigurationImage burned() {
        ConfigurationImage image = ConfigurationImageTestHelper.newLittleEndian(new byte[12]);
        image.getByteBuffer(0, 4).putFloat(440);
        return image;
    }

    /** Runs {@code body} on the EDT with the injection page shown from a copy of {@code burned}. */
    private static void onPage(ConfigurationImage burned, WidgetTest body) throws Exception {
        AtomicReference<Throwable> failure = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
            try {
                IniFileModel ini = ini();
                widget.setBurnedImage(() -> burned);
                widget.update(ini.getDialogs().get("injection"), ini, burned.clone());
                body.run(widget);
            } catch (Throwable t) {
                failure.set(t);
            } finally {
                widget.destroy();
            }
        });
        if (failure.get() != null) {
            throw new AssertionError(failure.get());
        }
    }

    private interface WidgetTest {
        void run(CalibrationDialogWidget widget) throws Exception;
    }

    @Test
    void unchangedPageHasNoMarks() throws Exception {
        onPage(burned(), widget -> assertEquals(Collections.emptyList(), widget.getUnburnedFieldKeysForTests()));
    }

    @Test
    void editedSettingIsMarkedWithItsBurnedValue() throws Exception {
        onPage(burned(), widget -> {
            find(row(widget, "injectorFlow"), JTextField.class).setText("470");

            assertEquals(Collections.singletonList("injectorFlow"), widget.getUnburnedFieldKeysForTests());
            String burnedText = find(row(widget, "injectorFlow"), JLabel.class, "unburnedValue").getText();
            assertTrue(burnedText.startsWith("Burned: 440"), burnedText);
            assertTrue(burnedText.endsWith("cc/min"), burnedText);
        });
    }

    @Test
    void settingTypedBackToItsBurnedValueIsUnmarked() throws Exception {
        onPage(burned(), widget -> {
            JTextField flow = find(row(widget, "injectorFlow"), JTextField.class);
            flow.setText("470");
            assertFalse(widget.getUnburnedFieldKeysForTests().isEmpty());

            flow.setText("440");
            assertEquals(Collections.emptyList(), widget.getUnburnedFieldKeysForTests());
        });
    }

    @Test
    void onlyTheToggledOnOffSettingIsMarked() throws Exception {
        onPage(burned(), widget -> {
            find(row(widget, "enableB"), JCheckBox.class).doClick();

            assertEquals(Collections.singletonList("enableB"), widget.getUnburnedFieldKeysForTests());
            assertEquals("Burned: Off", find(row(widget, "enableB"), JLabel.class, "unburnedValue").getText());
        });
    }

    @Test
    void revertSetsTheBurnedValueBackAndReportsTheEdit() throws Exception {
        ConfigurationImage burned = burned();
        onPage(burned, widget -> {
            AtomicInteger edits = new AtomicInteger();
            widget.setOnConfigChange(image -> edits.incrementAndGet());
            JCheckBox enableA = find(row(widget, "enableA"), JCheckBox.class);
            JCheckBox enableB = find(row(widget, "enableB"), JCheckBox.class);
            enableA.doClick();
            enableB.doClick();
            int before = edits.get();

            find(row(widget, "enableB"), JButton.class, "unburnedRevert").doClick();

            assertFalse(enableB.isSelected());
            assertTrue(enableA.isSelected(), "the setting sharing the word keeps its edit");
            assertEquals(Collections.singletonList("enableA"), widget.getUnburnedFieldKeysForTests());
            assertTrue(edits.get() > before, "a revert is an edit");
        });
    }

    @Test
    void revertRestoresTheExactBurnedValueEvenWhenShownRounded() throws Exception {
        ConfigurationImage burned = burned();
        burned.getByteBuffer(0, 4).putFloat(440.04f); // shown as 440.0 with one digit
        onPage(burned, widget -> {
            find(row(widget, "injectorFlow"), JTextField.class).setText("470");
            find(row(widget, "injectorFlow"), JButton.class, "unburnedRevert").doClick();

            assertEquals(440.04f, widget.getWorkingImage().getByteBuffer(0, 4).getFloat());
            assertEquals(Collections.emptyList(), widget.getUnburnedFieldKeysForTests());
        });
    }

    @Test
    void revertOfADropDownSelectsTheBurnedChoice() throws Exception {
        onPage(burned(), widget -> {
            @SuppressWarnings("unchecked")
            JComboBox<Object> mode = find(row(widget, "injMode"), JComboBox.class);
            mode.setSelectedItem("Batch");
            assertEquals("Burned: Sequential", find(row(widget, "injMode"), JLabel.class, "unburnedValue").getText());

            find(row(widget, "injMode"), JButton.class, "unburnedRevert").doClick();
            assertEquals("Sequential", mode.getSelectedItem());
            assertEquals(Collections.emptyList(), widget.getUnburnedFieldKeysForTests());
        });
    }

    @Test
    void newBurnedTuneClearsTheMarks() throws Exception {
        AtomicReference<ConfigurationImage> burned = new AtomicReference<>(burned());
        onPage(burned.get(), widget -> {
            widget.setBurnedImage(burned::get);
            find(row(widget, "injectorFlow"), JTextField.class).setText("470");
            assertFalse(widget.getUnburnedFieldKeysForTests().isEmpty());

            burned.set(widget.getWorkingImage().clone()); // as after a burn
            widget.refreshUnburnedMarks();
            assertEquals(Collections.emptyList(), widget.getUnburnedFieldKeysForTests());
        });
    }

    @Test
    void withoutABurnedTuneNothingIsMarked() throws Exception {
        onPage(burned(), widget -> {
            widget.setBurnedImage(null);
            find(row(widget, "injectorFlow"), JTextField.class).setText("470");
            assertEquals(Collections.emptyList(), widget.getUnburnedFieldKeysForTests());
        });
    }

    @Test
    void offlineTheReferenceIsTheLoadedTune() throws Exception {
        ConfigurationImage burned = burned();
        AtomicReference<Throwable> failure = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = new UIContext();
            context.setOfflineMode(true);
            CalibrationDialogWidget widget = new CalibrationDialogWidget(context);
            try {
                IniFileModel ini = ini();
                widget.setBurnedImage(() -> burned);
                widget.update(ini.getDialogs().get("injection"), ini, burned.clone());
                find(row(widget, "injectorFlow"), JTextField.class).setText("470");
                String text = find(row(widget, "injectorFlow"), JLabel.class, "unburnedValue").getText();
                assertTrue(text.startsWith("Loaded: 440"), text);
                assertEquals(UnburnedWording.LOADED.revertTooltip,
                    find(row(widget, "injectorFlow"), JButton.class, "unburnedRevert").getToolTipText());

                context.setOfflineMode(false); // connected: the reference is the burned tune again
                widget.refreshUnburnedMarks();
                text = find(row(widget, "injectorFlow"), JLabel.class, "unburnedValue").getText();
                assertTrue(text.startsWith("Burned: 440"), text);
            } catch (Throwable t) {
                failure.set(t);
            } finally {
                widget.destroy();
            }
        });
        if (failure.get() != null) {
            throw new AssertionError(failure.get());
        }
    }

    @Test
    void openInTuningHighlightKeepsTheMark() throws Exception {
        onPage(burned(), widget -> {
            find(row(widget, "injectorFlow"), JTextField.class).setText("470");
            assertTrue(widget.highlightField("injectorFlow"));
            assertEquals(Collections.singletonList("injectorFlow"), widget.getUnburnedFieldKeysForTests());
        });
    }

    private static JComponent row(CalibrationDialogWidget widget, String fieldKey) {
        for (Component c : all(widget.getContentPane())) {
            if (c instanceof JComponent && fieldKey.equals(((JComponent) c).getClientProperty("rusefi.fieldKey"))) {
                return (JComponent) c;
            }
        }
        throw new AssertionError("no row " + fieldKey);
    }

    private static <T extends Component> T find(Container parent, Class<T> type) {
        return find(parent, type, null);
    }

    private static <T extends Component> T find(Container parent, Class<T> type, String name) {
        for (Component c : all(parent)) {
            if (type.isInstance(c) && (name == null || name.equals(c.getName()))) {
                return type.cast(c);
            }
        }
        throw new AssertionError("no " + type.getSimpleName() + " " + name);
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
}
