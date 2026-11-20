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
    /** The widest label sits in a titled panel, whose border inset narrows its label column. */
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
            widget.update("veTableDialog", ini, image);
            JLabel label = findLabel(widget.getContentPane(), "Override VE table load axis");
            assertNotNull(label, "The label is shown whole, not wrapped into a one-line row");
            JLabel natural = new JLabel(label.getText());
            CalibrationFieldFactory.applyStyle(natural);
            assertTrue(label.getPreferredSize().width >= natural.getPreferredSize().width);
        });
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
