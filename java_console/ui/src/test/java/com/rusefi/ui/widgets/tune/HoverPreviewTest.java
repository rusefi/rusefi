package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.config.FieldType;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.*;

class HoverPreviewTest {
    private static final Rectangle SCREEN = new Rectangle(0, 0, 1000, 800);

    @Test
    void cardGoesRightOfTheButtonOrLeftWhenThereIsNoRoom() {
        Dimension card = new Dimension(300, 200);
        assertEquals(new Point(128, 100), HoverPreview.place(new Rectangle(100, 100, 20, 20), card, SCREEN),
            "to the right, top-aligned");
        assertEquals(new Point(552, 100), HoverPreview.place(new Rectangle(860, 100, 20, 20), card, SCREEN),
            "flipped left near the right edge");
        assertEquals(new Point(128, 600), HoverPreview.place(new Rectangle(100, 750, 20, 20), card, SCREEN),
            "kept on screen near the bottom");
    }

    @Test
    void connectorButtonAsksTheLinkForTheChosenPinsCard() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<String> asked = new ArrayList<>();
            JLabel card = new JLabel("card");
            PinoutLink link = new PinoutLink() {
                @Override
                public void show(String pin) {
                }

                @Override
                public JComponent preview(String pin) {
                    asked.add(pin);
                    return pin.startsWith("B16") ? card : null;
                }
            };
            ConfigurationImage image = new ConfigurationImage(new byte[1]);
            JPanel row = CalibrationFieldFactory.createFieldRow(
                new DialogModel.Field("injectionPins1", "Injection Output 1"),
                enumField("B16 Low Side output 4", "C1 not on this board", "NONE"), image, image.clone(), null, link);
            JButton button = button(row);
            JComboBox<?> combo = combo(row);
            HoverPreview preview = HoverPreview.of(button);
            assertNotNull(preview, "The connector button previews the pin on hover");

            assertSame(card, preview.createCard());
            assertEquals("B16 Low Side output 4", asked.get(0), "Without quotes, as Pinout names it");

            combo.setSelectedItem("C1 not on this board");
            assertNull(preview.createCard(), "No card when the board's pinout lacks the pin: the tooltip stays");

            asked.clear();
            combo.setSelectedItem("NONE");
            assertNull(preview.createCard());
            assertTrue(asked.isEmpty(), "No pin chosen: Pinout is not asked");

            assertFalse(preview.show(), "Nothing shows for a button that is not on screen");
            assertFalse(preview.isShowing());
        });
    }

    private static EnumIniField enumField(String... values) {
        Map<Integer, String> map = new HashMap<>();
        for (int i = 0; i < values.length; i++) {
            map.put(i, values[i]);
        }
        return new EnumIniField("test", 0, FieldType.INT8, new EnumIniField.EnumKeyValueMap(map), 0, 1);
    }

    private static JButton button(JPanel row) {
        for (Component component : row.getComponents()) {
            if (component instanceof JButton) {
                return (JButton) component;
            }
        }
        throw new AssertionError("no button");
    }

    private static JComboBox<?> combo(JPanel row) {
        for (Component component : row.getComponents()) {
            if (component instanceof JComboBox) {
                return (JComboBox<?>) component;
            }
        }
        throw new AssertionError("no combo");
    }
}
