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
import java.io.File;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

/** "Generate base VE..." shares the page's Help row instead of taking a row above the table. */
class VeGeneratorButtonTest {
    private static final String GENERATE = "Generate base VE...";

    @Test
    void generatorButtonJoinsThePagesHelpRowOrKeepsItsOwn() throws Throwable {
        RawIniFile raw;
        try (InputStream stream = getClass().getResourceAsStream("/january.ini")) {
            raw = IniFileReaderUtil.read(stream, "january.ini");
        }
        IniFileModel ini = IniFileReaderUtil.readIniFile(raw, "january.ini", new IniFileMetaInfoImpl(raw));
        ConfigurationImage image = Msq.readTune(
            new File(getClass().getResource("/january_tune.msq").toURI()).getAbsolutePath()).asImage(ini);
        SwingUtilities.invokeAndWait(() -> {
            CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
            try {
                // veTableDialog has topicHelp, and holds veTableTbl as a panel.
                widget.update("veTableDialog", ini, image);
                List<JButton> generators = buttons(widget.getContentPane(), GENERATE);
                assertEquals(1, generators.size());
                JButton help = buttons(widget.getContentPane(), "Help").get(0);
                assertSame(help.getParent(), generators.get(0).getParent(), "One row for Help and the generator");
                assertEquals(2, generators.get(0).getParent().getComponentCount());

                // The table on its own has no topic help in this INI: the button keeps its row.
                widget.update("veTableTbl", ini, image);
                generators = buttons(widget.getContentPane(), GENERATE);
                assertEquals(1, generators.size());
                assertTrue(buttons(widget.getContentPane(), "Help").isEmpty());
                assertEquals(1, generators.get(0).getParent().getComponentCount(), "A row of its own");

                // Other tables get no generator, and revisiting a page does not add a second one.
                widget.update("ignitionTableTbl", ini, image);
                assertTrue(buttons(widget.getContentPane(), GENERATE).isEmpty());
                widget.update("veTableDialog", ini, image);
                assertEquals(1, buttons(widget.getContentPane(), GENERATE).size());
            } finally {
                widget.destroy();
            }
        });
    }

    private static List<JButton> buttons(Container container, String text) {
        List<JButton> found = new ArrayList<>();
        for (Component component : container.getComponents()) {
            if (component instanceof JButton && text.equals(((JButton) component).getText())) {
                found.add((JButton) component);
            }
            if (component instanceof Container) {
                found.addAll(buttons((Container) component, text));
            }
        }
        return found;
    }
}
