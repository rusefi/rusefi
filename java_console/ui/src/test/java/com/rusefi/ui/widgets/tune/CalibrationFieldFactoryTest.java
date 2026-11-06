package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.config.FieldType;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.Collections;

import static org.junit.jupiter.api.Assertions.*;

public class CalibrationFieldFactoryTest {

    @Test
    public void stripStylePrefixRemovesRedAndBlueMarkers() {
        assertEquals("red text", CalibrationFieldFactory.stripStylePrefix("!red text"));
        assertEquals("red text", CalibrationFieldFactory.stripStylePrefix("! red text"));
        assertEquals("blue text", CalibrationFieldFactory.stripStylePrefix("#blue text"));
        assertEquals("plain text", CalibrationFieldFactory.stripStylePrefix("plain text"));
        assertEquals("", CalibrationFieldFactory.stripStylePrefix(""));
        assertNull(CalibrationFieldFactory.stripStylePrefix(null));
    }

    @Test
    public void redLabelRowDoesNotDisplayExclamationMark() {
        DialogModel.Field field = new DialogModel.Field("!ECU reboot needed", "!ECU reboot needed");
        JPanel row = CalibrationFieldFactory.createLabelRow(field);

        JLabel label = findLabel(row);
        assertNotNull(label);
        assertEquals("ECU reboot needed", label.getText());
        // the marker still drives the styling
        assertEquals(Color.RED, label.getBackground());
        assertEquals(Color.WHITE, label.getForeground());
    }

    @Test
    public void blueLabelRowDoesNotDisplayHashMark() {
        DialogModel.Field field = new DialogModel.Field("#Batch injection", "#Batch injection");
        JPanel row = CalibrationFieldFactory.createLabelRow(field);

        JLabel label = findLabel(row);
        assertNotNull(label);
        assertEquals("Batch injection", label.getText());
        assertEquals(Color.BLUE, label.getBackground());
    }

    @Test
    public void styledLabelOfFieldWithConstantDoesNotDisplayMarker() {
        DialogModel.Field field = new DialogModel.Field("myField", "!Red setting", null, null);
        EnumIniField enumField = new EnumIniField("myField", 0, FieldType.INT8,
            new EnumIniField.EnumKeyValueMap(Collections.singletonMap(0, "off")), 0, 0);
        JPanel row = CalibrationFieldFactory.createFieldRow(field, enumField, new ConfigurationImage(4),
            new ConfigurationImage(4));

        JLabel label = findLabel(row);
        assertNotNull(label);
        assertEquals("Red setting", label.getText());
        assertEquals(Color.RED, label.getBackground());
    }

    @Test
    public void plainLabelRowIsUnchanged() {
        DialogModel.Field field = new DialogModel.Field("Just a label", "Just a label");
        JPanel row = CalibrationFieldFactory.createLabelRow(field);

        JLabel label = findLabel(row);
        assertNotNull(label);
        assertEquals("Just a label", label.getText());
    }

    private static JLabel findLabel(Container container) {
        for (Component component : container.getComponents()) {
            if (component instanceof JLabel) {
                return (JLabel) component;
            }
            if (component instanceof Container) {
                JLabel nested = findLabel((Container) component);
                if (nested != null) {
                    return nested;
                }
            }
        }
        return null;
    }
}
