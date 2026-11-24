package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.IniField;
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

/** [tag:settings_summary_line] The VE page shows its fuel strategy and load override as one line. */
class SettingsSummaryLineTest {
    @Test
    void vePageShowsStrategyAndOverrideAsOneLineWithLinks() throws Throwable {
        IniFileModel ini = ini();
        ConfigurationImage image = image(ini);
        String strategy = SettingsSummaryLine.displayValue(value(ini, image, "fuelAlgorithm"));
        String override = SettingsSummaryLine.displayValue(value(ini, image, "veOverrideMode"));
        SwingUtilities.invokeAndWait(() -> {
            CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
            List<String> opened = new ArrayList<>();
            widget.setSettingOpener(new SettingsSummaryLine.SettingOpener() {
                @Override
                public String pageName(String fieldKey, String fromPageKey) {
                    assertEquals("veTableDialog", fromPageKey);
                    return "veOverrideMode".equals(fieldKey) ? null : "Vehicle Information";
                }

                @Override
                public void open(String fieldKey, String fromPageKey) {
                    opened.add(fieldKey + " from " + fromPageKey);
                }
            });
            try {
                widget.update("veTableDialog", ini, image);
                JPanel content = widget.getContentPane();
                List<JComboBox<?>> combos = find(content, JComboBox.class);
                assertTrue(combos.isEmpty(), "No editors for the summarized settings: " + combos.size());
                assertNull(named(content, "Fuel"), "The Fuel panel is replaced");
                assertNull(named(content, "Load override"), "The Load override panel is replaced");

                Component line = named(content, "settingsSummaryLine");
                assertNotNull(line);
                String text = text((Container) line);
                assertTrue(text.startsWith("Fuel strategy: " + strategy), text);
                assertTrue(text.endsWith("Override VE table load axis: " + override), text);
                assertFalse(find(content, JTable.class).isEmpty(), "The VE table is still there");

                JButton link = (JButton) named(content, "settingsSummaryLink:fuelAlgorithm");
                assertEquals("Change in Vehicle Information", link.getToolTipText());
                link.doClick();
                assertEquals(java.util.Collections.singletonList("fuelAlgorithm from veTableDialog"), opened);
                assertNull(named(content, "settingsSummaryLink:veOverrideMode"),
                    "A setting no other page shows is plain text");

                // Other pages, and the panels shown on their own, keep their editors.
                widget.update("fuelStrategyPanel", ini, image);
                assertNull(named(widget.getContentPane(), "settingsSummaryLine"));
                assertFalse(find(widget.getContentPane(), JComboBox.class).isEmpty());
            } finally {
                widget.destroy();
            }
        });
    }

    @Test
    void valuesReadAsShown() {
        assertEquals("Speed Density", SettingsSummaryLine.displayValue("\"Speed Density\""));
        assertEquals("On", SettingsSummaryLine.displayValue("\"true\""));
        assertEquals("Off", SettingsSummaryLine.displayValue("\"false\""));
        assertEquals("12.5", SettingsSummaryLine.displayValue("12.5"));
    }

    private static IniFileModel ini() throws Throwable {
        RawIniFile raw;
        try (InputStream stream = SettingsSummaryLineTest.class.getResourceAsStream("/january.ini")) {
            raw = IniFileReaderUtil.read(stream, "january.ini");
        }
        return IniFileReaderUtil.readIniFile(raw, "january.ini", new IniFileMetaInfoImpl(raw));
    }

    private static ConfigurationImage image(IniFileModel ini) throws Throwable {
        return Msq.readTune(new File(SettingsSummaryLineTest.class.getResource("/january_tune.msq").toURI())
            .getAbsolutePath()).asImage(ini);
    }

    private static String value(IniFileModel ini, ConfigurationImage image, String key) {
        IniField field = ini.findIniField(key).orElseThrow(AssertionError::new);
        return ConfigurationImageGetterSetter.getStringValue(field, image);
    }

    private static String text(Container container) {
        StringBuilder text = new StringBuilder();
        for (Component component : container.getComponents()) {
            if (component instanceof JLabel) {
                text.append(((JLabel) component).getText());
            } else if (component instanceof AbstractButton) {
                text.append(((AbstractButton) component).getText());
            } else if (component instanceof Container) {
                text.append(text((Container) component));
            }
        }
        return text.toString();
    }

    private static Component named(Container container, String name) {
        for (Component component : container.getComponents()) {
            if (name.equals(component.getName())) {
                return component;
            }
            if (component instanceof Container) {
                Component found = named((Container) component, name);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    @SuppressWarnings("unchecked")
    private static <T extends Component> List<T> find(Container container, Class<?> type) {
        List<T> found = new ArrayList<>();
        for (Component component : container.getComponents()) {
            if (type.isInstance(component)) {
                found.add((T) component);
            }
            if (component instanceof Container) {
                found.addAll(find((Container) component, type));
            }
        }
        return found;
    }
}
