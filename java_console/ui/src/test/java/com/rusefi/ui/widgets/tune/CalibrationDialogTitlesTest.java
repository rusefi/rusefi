package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.laf.GradientTitleBorder;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;

class CalibrationDialogTitlesTest {
    @Test
    void curveAndTableViewportWidth() throws Throwable {
        RawIniFile raw;
        try (InputStream stream = getClass().getResourceAsStream("/january.ini")) {
            raw = IniFileReaderUtil.read(stream, "january.ini");
        }
        IniFileModel ini = IniFileReaderUtil.readIniFile(raw, "january.ini",
            new IniFileMetaInfoImpl(raw));
        ConfigurationImage image = Msq.readTune(
            new File(getClass().getResource("/january_tune.msq").toURI()).getAbsolutePath()).asImage(ini);
        SwingUtilities.invokeAndWait(() -> {
            CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
            try {
                for (String key : Arrays.asList("dwellSettings", "veTableTbl", "engineChars", "dwellSettings")) {
                    widget.update(key, ini, image);
                    JPanel viewport = widget.getViewportPane();
                    JPanel frame = (JPanel) viewport.getComponent(0);
                    int preferredWidth = frame.getPreferredSize().width;
                    viewport.setSize(preferredWidth + 400, viewport.getPreferredSize().height);
                    viewport.doLayout();
                    if ("engineChars".equals(key)) {
                        assertEquals(preferredWidth, frame.getWidth(), "Forms stay compact after visiting a table");
                    } else {
                        assertEquals(viewport.getWidth() - 16, frame.getWidth(), key + " fills available width");
                        assertEquals(8, frame.getX());
                    }
                }
            } finally {
                widget.destroy();
            }
        });
    }

    @Test
    void wideViewportDialogLayout() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
            try {
                JPanel dialog = widget.getContentPane();
                dialog.add(new JLabel("Vehicle information"));
                JPanel viewport = widget.getViewportPane();
                JPanel frame = (JPanel) viewport.getComponent(0);
                int preferredWidth = frame.getPreferredSize().width;
                JScrollPane scroll = new JScrollPane(viewport);
                scroll.setSize(preferredWidth + 400, 300);
                scroll.doLayout();
                scroll.getViewport().doLayout();
                viewport.doLayout();
                assertEquals(preferredWidth, frame.getWidth());
                assertEquals((viewport.getWidth() - preferredWidth) / 2, frame.getX());
                assertTrue(frame.getX() > 0);
                assertNotNull(frame.getBorder(), "The complete dialog has an outer frame");
                scroll.setSize(preferredWidth / 2, 300);
                scroll.doLayout();
                scroll.getViewport().doLayout();
                viewport.doLayout();
                assertTrue(frame.getWidth() < preferredWidth, "Narrow viewports shrink the dialog");
                assertTrue(frame.getX() >= 0);
                assertTrue(frame.getX() + frame.getWidth() <= viewport.getWidth());
                scroll.setSize(preferredWidth + 400, 300);
                scroll.doLayout();
                scroll.getViewport().doLayout();
                viewport.doLayout();
                assertEquals(preferredWidth, frame.getWidth(), "Restore full width after resizing");
            } finally {
                widget.destroy();
            }
        });
    }

    /** The SD panel declarations from the INI, reduced to their layout and displayed labels. */
    private static final String SD_DIALOG = String.join("\n",
        "dialog = sdCardHW, \"SD Card Interface\"",
        "field = \"SD Card Logging\"",
        "indicatorPanel = sdPresentIndicators, 1",
        "indicator = { 0 }, \"No SD card\", \"SD card present\", white, black, green, black",
        "indicatorPanel = sdCardActivityIndicators, 2",
        "indicator = { 0 }, \"SD WR\", \"SD WR\", white, black, green, black",
        "indicator = { 0 }, \"SD RD\", \"SD RD\", white, black, green, black",
        "dialog = sdCardLogging, \"SD Card Logging\"",
        "field = \"MLG logger rate\"",
        "dialog = sdConditionalLogging, \"SD Conditional Logging\"",
        "field = \"Conditional logging\"",
        "readoutPanel = sdLoggingStatePanel",
        "readout = sdLoggingState, \"Logging state\", \"\", 0, 8, 0, 0, 1, 1, 0, 0",
        "dialog = sdCardLeft, \"\", yAxis",
        "panel = sdCardHW",
        "panel = sdPresentIndicators",
        "panel = sdCardActivityIndicators",
        "panel = sdCardLogging",
        "dialog = sdCardRight, \"\", yAxis",
        "panel = sdConditionalLogging",
        "panel = sdLoggingStatePanel",
        "dialog = sdCard, \"SD Card\", border",
        "panel = sdCardLeft, West",
        "panel = sdCardRight, East");

    @Test
    void issue10307SdPanelTitles() throws Throwable {
        IniFileModel ini = IniFileReaderUtil.readIniFile(
            IniFileReaderUtil.read(new ByteArrayInputStream(SD_DIALOG.getBytes(StandardCharsets.UTF_8))),
            "issue10307.ini", mock(IniFileMetaInfo.class));
        SwingUtilities.invokeAndWait(() -> {
            CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
            try {
                widget.update("sdCard", ini, null);
                List<String> titles = new ArrayList<>();
                List<String> labels = new ArrayList<>();
                collectText(widget.getContentPane(), titles, labels);
                assertEquals(Arrays.asList("SD Card Interface", "SD Card Logging", "SD Conditional Logging"), titles);
                assertTrue(labels.containsAll(Arrays.asList("No SD card", "SD WR", "SD RD", "Logging state")), labels.toString());
                JPanel left = (JPanel) ((BorderLayout) widget.getContentPane().getLayout())
                    .getLayoutComponent(BorderLayout.WEST);
                assertEquals("sdCardLeft", left.getName(), "Internal component identity remains available");
                assertNull(left.getBorder(), "An untitled layout panel must not reserve a heading or draw a frame");
                assertEquals(0, left.getInsets().top);
            } finally {
                widget.destroy();
            }
        });
    }

    private static void collectText(Component component, List<String> titles, List<String> labels) {
        if (component instanceof JComponent && ((JComponent) component).getBorder() instanceof GradientTitleBorder) {
            titles.add(((GradientTitleBorder) ((JComponent) component).getBorder()).getTitle());
        }
        if (component instanceof JLabel) {
            labels.add(((JLabel) component).getText());
        }
        if (component instanceof Container) {
            for (Component child : ((Container) component).getComponents()) {
                collectText(child, titles, labels);
            }
        }
    }
}
