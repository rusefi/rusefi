package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;

import static org.junit.jupiter.api.Assertions.*;

class CalibrationDialogLabelWidthTest {
    /** Too narrow for label and editor side by side on Vehicle Information, wide enough for its editors. */
    private static final int NARROW = 460;

    /** A label in a titled panel, whose border inset narrows its label column. */
    @Test
    void labelInTitledPanelIsNotCutOff() throws Exception, com.rusefi.ini.reader.IniParsingException {
        IniFileModel ini;
        try (java.io.InputStream stream = getClass().getResourceAsStream("/january.ini")) {
            RawIniFile raw = IniFileReaderUtil.read(stream, "january.ini");
            ini = IniFileReaderUtil.readIniFile(raw, "january.ini", new IniFileMetaInfoImpl(raw));
        }
        ConfigurationImage image = Msq.readTune(new java.io.File(
            getClass().getResource("/january_tune.msq").toURI()).getAbsolutePath()).asImage(ini);
        SwingUtilities.invokeAndWait(() -> {
            CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
            // [tag:settings_summary_line] the VE page shows this panel as a line; Vehicle Information keeps it
            widget.update("engineChars", ini, image);
            JLabel label = findLabel(widget.getContentPane(), "Fuel strategy");
            assertNotNull(label, "The label is shown whole, not wrapped into a one-line row");
            JLabel natural = new JLabel(label.getText());
            CalibrationFieldFactory.applyStyle(natural);
            // Measured off screen; the shown text can be slightly wider, so the column needs room to spare.
            assertTrue(label.getPreferredSize().width
                >= natural.getPreferredSize().width + CalibrationFieldFactory.LABEL_TEXT_SLACK);
        });
    }

    @Test
    void narrowDialogPutsTheEditorUnderItsLabel() throws Exception, com.rusefi.ini.reader.IniParsingException {
        IniFileModel ini = loadIni();
        ConfigurationImage image = Msq.readTune(new java.io.File(
            getClass().getResource("/january_tune.msq").toURI()).getAbsolutePath()).asImage(ini);
        SwingUtilities.invokeAndWait(() -> {
            CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
            widget.update("engineChars", ini, image);
            JScrollPane scroll = new JScrollPane(widget.getViewportPane());
            for (int width : new int[]{1200, NARROW}) {
                scroll.setSize(width, 800);
                for (int pass = 0; pass < 2; pass++) {
                    // the second pass sees the rows' heights at their new widths
                    invalidateTree(scroll);
                    layoutTree(scroll);
                }
                JLabel label = findLabel(widget.getContentPane(), "Fuel strategy");
                JPanel row = (JPanel) label.getParent();
                JComboBox<?> combo = findCombo(row);
                assertNotNull(combo);
                assertEquals(combo.getPreferredSize().width, combo.getWidth(), width + ": the editor keeps its width");
                assertTrue(combo.getX() + combo.getWidth() <= row.getWidth(), width + ": the editor is inside its row");
                assertTrue(combo.getY() + combo.getHeight() <= row.getHeight(), width + ": the row is tall enough");
                if (width == NARROW) {
                    assertTrue(combo.getY() >= label.getY() + label.getHeight(), "A narrow row puts the editor under the label");
                    assertEquals(label.getX(), combo.getX(), "The editor lines up with its label");
                } else {
                    assertTrue(combo.getX() > label.getX() + label.getWidth(), "A wide row keeps one line");
                }
            }
        });
    }

    private static IniFileModel loadIni() throws Exception, com.rusefi.ini.reader.IniParsingException {
        try (java.io.InputStream stream = CalibrationDialogLabelWidthTest.class.getResourceAsStream("/january.ini")) {
            RawIniFile raw = IniFileReaderUtil.read(stream, "january.ini");
            return IniFileReaderUtil.readIniFile(raw, "january.ini", new IniFileMetaInfoImpl(raw));
        }
    }

    private static JComboBox<?> findCombo(Container container) {
        for (Component component : container.getComponents()) {
            if (component instanceof JComboBox) {
                return (JComboBox<?>) component;
            }
        }
        return null;
    }

    private static void layoutTree(Container container) {
        container.doLayout();
        for (Component component : container.getComponents()) {
            if (component instanceof Container) {
                layoutTree((Container) component);
            }
        }
    }

    private static void invalidateTree(Container container) {
        for (Component component : container.getComponents()) {
            if (component instanceof Container) {
                invalidateTree((Container) component);
            }
        }
        container.invalidate();
    }

    private static JLabel findLabel(Container container, String text) {
        for (Component component : container.getComponents()) {
            if (component instanceof JLabel && text.equals(((JLabel) component).getText())) {
                return (JLabel) component;
            }
            if (component instanceof Container) {
                JLabel found = findLabel((Container) component, text);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }
}
