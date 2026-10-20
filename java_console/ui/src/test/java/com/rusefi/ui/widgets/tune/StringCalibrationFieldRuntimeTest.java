package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.config.FieldType;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.lang.reflect.InvocationTargetException;

import static org.junit.jupiter.api.Assertions.*;

/** Runtime characterization of regular calibration field editing. */
public class StringCalibrationFieldRuntimeTest {
    private static final String VIN = "12345678901234567";
    private static final String OTHER_VIN = "76543210987654321";
    private static final String OVERSIZED_VIN = VIN + "8";

    @Test
    public void appendingEighteenthVinCharacterKeepsTextWithinImageLimit() throws Exception {
        onEdt(() -> {
            Editor editor = createEditor(new StringIniField("vinNumber", 0, 17), 17);
            editor.textField.setText(VIN);
            editor.textField.getDocument().insertString(editor.textField.getDocument().getLength(), "8", null);

            assertEquals(VIN, editor.textField.getText());
            assertEquals(VIN, readBack(editor));
        });
    }

    @Test
    public void oversizedVinPasteIsTruncatedToImageLimit() throws Exception {
        onEdt(() -> {
            Editor editor = createEditor(new StringIniField("vinNumber", 0, 17), 17);
            editor.textField.selectAll();
            editor.textField.replaceSelection(OVERSIZED_VIN);

            assertEquals(VIN, editor.textField.getText());
            assertEquals(VIN, readBack(editor));
        });
    }

    @Test
    public void selectAllReplacementWithSeventeenVinCharactersUpdatesImage() throws Exception {
        onEdt(() -> {
            Editor editor = createEditor(new StringIniField("vinNumber", 0, 17), 17);
            editor.textField.setText(VIN);
            editor.textField.selectAll();
            editor.textField.replaceSelection(OTHER_VIN);

            assertEquals(OTHER_VIN, editor.textField.getText());
            assertEquals(OTHER_VIN, readBack(editor));
        });
    }

    @Test
    public void oversizedReplacementOfExistingVinKeepsEditorAndImageSynchronized() throws Exception {
        onEdt(() -> {
            Editor editor = createEditor(new StringIniField("vinNumber", 0, 17), 17);
            editor.textField.setText(OTHER_VIN);
            editor.textField.selectAll();
            editor.textField.replaceSelection(OVERSIZED_VIN);

            assertEquals(VIN, editor.textField.getText());
            assertEquals(VIN, readBack(editor));
        });
    }

    @Test
    public void oversizedPartialReplacementUsesSelectedSpaceAndPreservesSurroundingText() throws Exception {
        onEdt(() -> {
            Editor editor = createEditor(new StringIniField("vinNumber", 0, 17), 17);
            editor.textField.setText(VIN);
            editor.textField.select(2, 5);
            editor.textField.replaceSelection("ABCDE");

            String expected = VIN.substring(0, 2) + "ABC" + VIN.substring(5);
            assertEquals(expected, editor.textField.getText());
            assertEquals(expected, readBack(editor));
        });
    }

    @Test
    public void deletingAndReenteringVinKeepsEditorAndImageSynchronized() throws Exception {
        onEdt(() -> {
            Editor editor = createEditor(new StringIniField("vinNumber", 0, 17), 17);
            editor.textField.setText(VIN);
            editor.textField.selectAll();
            editor.textField.replaceSelection("");
            assertEquals("", readBack(editor));
            editor.textField.replaceSelection(OTHER_VIN);

            assertEquals(OTHER_VIN, editor.textField.getText());
            assertEquals(OTHER_VIN, readBack(editor));
        });
    }

    @Test
    public void shorterStringFieldTruncatesTextToItsDeclaredSize() throws Exception {
        onEdt(() -> {
            Editor editor = createEditor(new StringIniField("shortName", 0, 5), 8);
            editor.textField.setText("123456");

            assertEquals("12345", editor.textField.getText());
            assertEquals("12345", readBack(editor));
        });
    }

    @Test
    public void longStringFieldInputLimitUsesDeclaredSizeNotDisplayColumnCap() throws Exception {
        onEdt(() -> {
            Editor editor = createEditor(new StringIniField("longName", 0, 40), 40);
            assertEquals(32, editor.textField.getColumns());
            char[] characters = new char[33];
            java.util.Arrays.fill(characters, 'x');
            String text = new String(characters);
            editor.textField.setText(text);

            assertEquals(text, editor.textField.getText());
            assertEquals(text, readBack(editor));
        });
    }

    @Test
    public void numericCalibrationFieldRemainsEditable() throws Exception {
        onEdt(() -> {
            ScalarIniField scalar = new ScalarIniField("scalar", 0, "", FieldType.INT8, 1, "0", 0);
            Editor editor = createEditor(scalar, 1);
            editor.textField.setText("12");

            assertEquals("12", editor.textField.getText());
            assertEquals("12", readBack(editor));
        });
    }

    private static Editor createEditor(IniField field, int imageSize) {
        ConfigurationImage image = new ConfigurationImage(imageSize);
        JPanel row = CalibrationFieldFactory.createFieldRow(
            new DialogModel.Field(field.getName(), field.getName()), field, image, image);
        JTextField textField = findTextField(row);
        assertNotNull(textField);
        return new Editor(textField, field, image);
    }

    private static String readBack(Editor editor) {
        return ConfigurationImageGetterSetter.getStringValue(editor.field, editor.image);
    }

    private static JTextField findTextField(Container container) {
        for (Component component : container.getComponents()) {
            if (component instanceof JTextField) {
                return (JTextField) component;
            }
            if (component instanceof Container) {
                JTextField nested = findTextField((Container) component);
                if (nested != null) {
                    return nested;
                }
            }
        }
        return null;
    }

    private static void onEdt(ThrowingRunnable action) throws Exception {
        if (SwingUtilities.isEventDispatchThread()) {
            action.run();
            return;
        }
        try {
            SwingUtilities.invokeAndWait(() -> {
                try {
                    action.run();
                } catch (RuntimeException | Error e) {
                    throw e;
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            });
        } catch (InvocationTargetException e) {
            Throwable cause = e.getCause();
            if (cause instanceof Exception) {
                throw (Exception) cause;
            }
            if (cause instanceof Error) {
                throw (Error) cause;
            }
            throw e;
        }
    }

    private interface ThrowingRunnable {
        void run() throws Exception;
    }

    private static final class Editor {
        private final JTextField textField;
        private final IniField field;
        private final ConfigurationImage image;

        private Editor(JTextField textField, IniField field, ConfigurationImage image) {
            this.textField = textField;
            this.field = field;
            this.image = image;
        }
    }
}
