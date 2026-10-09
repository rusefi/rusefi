package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import javax.swing.text.html.HTML;
import javax.swing.text.html.HTMLDocument;
import java.awt.*;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;

class TopicHelpTest {
    private static IniFileModel readIni(String text) throws Throwable {
        return IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8))),
            "topicHelp.ini", mock(IniFileMetaInfo.class));
    }

    @Test
    void namedDialogHelp() throws Throwable {
        IniFileModel ini = readIni("[UserDefined]\n"
            + "dialog = example, \"Example\"\n"
            + "topicHelp = exampleHelp\n"
            + "field = \"Existing content\"\n"
            + "help = exampleHelp, \"Example help\"\n"
            + "text = \"First line<br>Second line\"\n"
            + "webHelp = \"https://rusefi.com/s/knock\"\n");
        assertEquals("exampleHelp", ini.getDialogs().get("example").getTopicHelp());
        assertNotNull(ini.getContextHelp("exampleHelp"));
        SwingUtilities.invokeAndWait(() -> {
            CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
            try {
                widget.update("example", ini, null);
                List<JButton> buttons = helpButtons(widget.getContentPane());
                assertEquals(1, buttons.size());
                JButton button = buttons.get(0);
                assertEquals("Example help", button.getAccessibleContext().getAccessibleName());
                assertEquals(1, button.getActionListeners().length);
                JEditorPane editor = (JEditorPane) CalibrationFieldFactory.createHtmlHelpScrollPane(
                    button.getToolTipText()).getViewport().getView();
                assertTrue(editor.getText().contains("First line<br>Second line"));
                HTMLDocument document = (HTMLDocument) editor.getDocument();
                assertEquals("https://rusefi.com/s/knock",
                    document.getIterator(HTML.Tag.A).getAttributes().getAttribute(HTML.Attribute.HREF));
                assertFalse(editor.isEditable());
                assertEquals(1, editor.getHyperlinkListeners().length);
            } finally {
                widget.destroy();
            }
        });
    }

    @Test
    void webHelpAndMissingHelpDoNotLeakAcrossNavigation() throws Throwable {
        IniFileModel ini = readIni("[UserDefined]\n"
            + "dialog = web, \"Web\"\ntopicHelp = \"https://rusefi.com/s/fuel\"\nfield = \"Web content\"\n"
            + "dialog = missing, \"Missing\"\ntopicHelp = unknownHelp\nfield = \"Missing help content\"\n"
            + "dialog = empty, \"Empty\"\nfield = \"Still usable\"\n");
        SwingUtilities.invokeAndWait(() -> {
            CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
            try {
                widget.update("web", ini, null);
                assertEquals(1, helpButtons(widget.getContentPane()).size());
                assertTrue(helpButtons(widget.getContentPane()).get(0).getToolTipText()
                    .contains("href='https://rusefi.com/s/fuel'"));
                widget.update("missing", ini, null);
                assertTrue(helpButtons(widget.getContentPane()).isEmpty());
                widget.update("empty", ini, null);
                assertTrue(helpButtons(widget.getContentPane()).isEmpty());
                assertEquals(1, widget.getContentPane().getComponentCount());
                widget.update("web", ini, null);
                widget.reset();
                assertTrue(helpButtons(widget.getContentPane()).isEmpty());
            } finally {
                widget.destroy();
            }
        });
    }

    @Test
    void nestedDialogHelpPreservesBorderLayoutPanels() throws Throwable {
        IniFileModel ini = readIni("[UserDefined]\n"
            + "dialog = child, \"Child\"\ntopicHelp = childHelp\nfield = \"Child field\"\n"
            + "dialog = parent, \"Parent\", border\ntopicHelp = parentHelp\npanel = child, North\n"
            + "help = childHelp, \"Child help\"\ntext = \"Child instructions\"\n"
            + "help = parentHelp, \"Parent help\"\ntext = \"Parent instructions\"\n");
        SwingUtilities.invokeAndWait(() -> {
            CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
            try {
                widget.update("parent", ini, null);
                List<JButton> buttons = helpButtons(widget.getContentPane());
                assertEquals(2, buttons.size());
                assertEquals("Parent help", buttons.get(0).getAccessibleContext().getAccessibleName());
                assertEquals("Child help", buttons.get(1).getAccessibleContext().getAccessibleName());
                JPanel body = (JPanel) ((BorderLayout) widget.getContentPane().getLayout())
                    .getLayoutComponent(BorderLayout.CENTER);
                Component child = ((BorderLayout) body.getLayout()).getLayoutComponent(BorderLayout.NORTH);
                assertNotNull(child, "Help must not replace the existing North panel");
                assertEquals("Child", child.getName());
            } finally {
                widget.destroy();
            }
        });
    }

    @Test
    void standaloneAndEmbeddedTableAndCurveHelp() throws Throwable {
        IniFileModel ini = readIni("[Constants]\npage = 1\n"
            + "x = array, U08, 0, [2], \"\", 1, 0, 0, 100, 0\n"
            + "y = array, U08, 2, [2], \"\", 1, 0, 0, 100, 0\n"
            + "z = array, U08, 4, [2x2], \"\", 1, 0, 0, 100, 0\n"
            + "[TableEditor]\ntable = tableExample, tableMap, \"Table\", 1\n"
            + "topicHelp = sharedHelp\nxBins = x\nyBins = y\nzBins = z\n"
            + "[CurveEditor]\ncurve = curveExample, \"Curve\"\ntopicHelp = sharedHelp\n"
            + "columnLabel = \"X\", \"Y\"\nxAxis = 0, 10, 5\nyAxis = 0, 10, 5\nxBins = x\nyBins = y\n"
            + "curve = noHelpCurve, \"No help\"\n"
            + "columnLabel = \"X\", \"Y\"\nxAxis = 0, 10, 5\nyAxis = 0, 10, 5\nxBins = x\nyBins = y\n"
            + "[UserDefined]\ndialog = combined, \"Combined\"\npanel = tableExample\npanel = curveExample\n"
            + "help = sharedHelp, \"Shared help\"\ntext = \"Editor instructions\"\n");
        assertEquals("sharedHelp", ini.getCurves().get("curveExample").getTopicHelp());
        assertNull(ini.getCurves().get("noHelpCurve").getTopicHelp());
        ConfigurationImage image = new ConfigurationImage(new byte[]{0, 10, 0, 10, 1, 2, 3, 4});
        SwingUtilities.invokeAndWait(() -> {
            CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
            try {
                for (String key : new String[]{"tableExample", "curveExample"}) {
                    widget.update(key, ini, image);
                    assertEquals(1, helpButtons(widget.getContentPane()).size(), key);
                }
                widget.update("noHelpCurve", ini, image);
                assertTrue(helpButtons(widget.getContentPane()).isEmpty());
                widget.update("combined", ini, image);
                assertEquals(2, helpButtons(widget.getContentPane()).size());
            } finally {
                widget.destroy();
            }
        });
    }

    private static List<JButton> helpButtons(Component component) {
        List<JButton> result = new ArrayList<>();
        if (component instanceof JButton && "topicHelpButton".equals(component.getName())) {
            result.add((JButton) component);
        }
        if (component instanceof Container) {
            for (Component child : ((Container) component).getComponents()) {
                result.addAll(helpButtons(child));
            }
        }
        return result;
    }
}
