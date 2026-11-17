package com.rusefi.ui.widgets;

import com.formdev.flatlaf.FlatLightLaf;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;

import static org.junit.jupiter.api.Assertions.*;

class JSplitButtonTest {
    @Test
    void updateAndRecoveryCaptionsFitTheSplitButtonPreferredWidthAtNormalAndLargeFonts() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            LookAndFeel original = UIManager.getLookAndFeel();
            try {
                assertCaptionsFitAtSupportedSizes("default LAF");
                UIManager.setLookAndFeel(new FlatLightLaf());
                assertCaptionsFitAtSupportedSizes("FlatLaf");
            } catch (UnsupportedLookAndFeelException e) {
                throw new AssertionError(e);
            } finally {
                if (original != null) {
                    try {
                        UIManager.setLookAndFeel(original);
                    } catch (UnsupportedLookAndFeelException e) {
                        throw new AssertionError(e);
                    }
                }
            }
        });
    }

    private static void assertCaptionsFitAtSupportedSizes(String lafName) {
        Icon upload48 = new ImageIcon(new BufferedImage(48, 48, BufferedImage.TYPE_INT_ARGB));
        JSplitButton split = new JSplitButton("Update ECU firmware", upload48);
        JButton applicationUpdate = new JButton("Update application");
        JButton main = findButton(split, "Update ECU firmware");
        JButton arrow = findButton(split, "");
        assertNotNull(main, lafName);
        assertNotNull(arrow, lafName);
        assertEquals(48, main.getIcon().getIconWidth(), lafName);
        for (float fontSize : new float[]{13.0f, 24.0f}) {
            Font font = main.getFont().deriveFont(fontSize);
            main.setFont(font);
            arrow.setFont(font);
            applicationUpdate.setFont(font);

            for (String caption : new String[]{"Update ECU firmware", "Recover ECU…", "Recover ECU"}) {
                split.setText(caption);

                // ProgramSelector assigns adjacent firmware controls the current split preferred size.
                Dimension sharedWidth = split.getPreferredSize();
                applicationUpdate.setPreferredSize(sharedWidth);
                assertEquals(sharedWidth, applicationUpdate.getPreferredSize(),
                    "adjacent control uses the shared width in " + lafName);
                split.setSize(sharedWidth);
                split.doLayout();

                assertTrue(arrow.getWidth() > 0, "dropdown retains its own width in " + lafName);
                assertEquals(caption, laidOutButtonText(main),
                    "the complete assigned caption must fit in " + lafName
                        + " at " + fontSize + "pt; bounds=" + main.getBounds()
                        + ", insets=" + main.getInsets() + ", dropdown width=" + arrow.getWidth());
            }
        }
    }

    private static String laidOutButtonText(JButton button) {
        Insets insets = button.getInsets();
        Rectangle view = new Rectangle(insets.left, insets.top,
            button.getWidth() - insets.left - insets.right,
            button.getHeight() - insets.top - insets.bottom);
        Rectangle iconBounds = new Rectangle();
        Rectangle textBounds = new Rectangle();
        return SwingUtilities.layoutCompoundLabel(button, button.getFontMetrics(button.getFont()), button.getText(),
            button.getIcon(), button.getVerticalAlignment(), button.getHorizontalAlignment(),
            button.getVerticalTextPosition(), button.getHorizontalTextPosition(), view, iconBounds, textBounds,
            button.getIconTextGap());
    }

    private static JButton findButton(Container container, String text) {
        for (Component component : container.getComponents()) {
            if (component instanceof JButton && text.equals(((JButton) component).getText())) {
                return (JButton) component;
            }
            if (component instanceof Container) {
                JButton found = findButton((Container) component, text);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }
}
