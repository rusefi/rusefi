package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;

import static org.junit.jupiter.api.Assertions.*;

public class TriggerPaneTest {
    @Test
    public void testTriggerImageChannelsAndCamFallback() throws IniParsingException {
        assertNotNull(TriggerPaneTest.class.getResource("/triggers.txt"));

        IniFileModel model = createTriggerTypeModel();
        ConfigurationImage image = new ConfigurationImage(new byte[100]);
        TriggerImageHelper helper = new TriggerImageHelper();

        image.getContent()[0] = 1; // TT_FORD_ASPIRE has two channels
        helper.updateTriggerImage(model, image, TriggerImageHelper.Channel.PRIMARY);
        assertTrue(helper.getPanel().getComponent(0) instanceof JPanel);
        helper.updateTriggerImage(model, image, TriggerImageHelper.Channel.SECONDARY);
        assertTrue(helper.getPanel().getComponent(0) instanceof JPanel);

        image.getContent()[1] = 9; // VVT_NISSAN_VQ maps to TT_VVT_NISSAN_VQ35
        helper.updateVvtTriggerImage(model, image);
        JPanel previews = (JPanel) helper.getPanel().getComponent(0);
        assertEquals(1, previews.getComponentCount());

        image.getContent()[0] = 2; // TT_DODGE_NEON_1995 has only a primary channel
        helper.updateTriggerImage(model, image, TriggerImageHelper.Channel.SECONDARY);
        Component fallback = helper.getPanel().getComponent(0);
        assertTrue(fallback instanceof JLabel);
        assertEquals("No cam wheel for this trigger", ((JLabel) fallback).getText());
    }

    private static IniFileModel createTriggerTypeModel() throws IniParsingException {
        String ini = "[Constants]\n" +
            "signature = \"rusEFI\"\n" +
            "ochBlockSize = 100\n" +
            "nPages = 1\n" +
            "pageSize = 100\n" +
            "pageReadCommand = \"r\"\n" +
            "page = 1\n" +
            "trigger_type = scalar, U08, 0, \"\", 1, 0, 0, 255, 0\n" +
            "vvtMode1 = scalar, U08, 1, \"\", 1, 0, 0, 32, 0\n" +
            "vvtMode2 = scalar, U08, 2, \"\", 1, 0, 0, 32, 0\n";
        RawIniFile lines = IniFileReaderUtil.read(new java.io.ByteArrayInputStream(ini.getBytes()));
        return IniFileReaderUtil.readIniFile(lines, "trigger-test.ini", new com.opensr5.ini.IniFileMetaInfoImpl(lines));
    }

    @Test
    public void testPrimaryTriggerPanelDetection() throws IniParsingException {
        String string = "[Constants]\n" +
            "signature = \"rusEFI\"\n" +
            "ochBlockSize = 100\n" +
            "nPages = 1\n" +
            "pageSize = 100\n" +
            "pageReadCommand = \"r\"\n" +
            "page = 1\n" +
            "trigger_type = bits, U08, 0, [0:3], \"TT_TOOTHED_WHEEL_60_2\", \"TT_TOOTHED_WHEEL_36_1\", \"Skipped\", \"Other\"\n" +
            "field1 = scalar, F32, 4, \"unit\", 1, 0, 0, 100, 1\n" +
            "[SettingContextHelp]\n" +
            "; SettingContextHelpEnd\n" +
            "\n" +
            "\tdialog = trigger_primary, \"Primary Trigger\"\n" +
            "\t\tfield = \"Trigger Type\", trigger_type\n" +
            "\t\tfield = \"Field 1\", field1\n";

        RawIniFile lines = IniFileReaderUtil.read(new java.io.ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderUtil.readIniFile(lines, "test.ini", new com.opensr5.ini.IniFileMetaInfoImpl(lines));

        ConfigurationImage ci = new ConfigurationImage(new byte[100]);
        // trigger_type is at offset 0, value 8 is "TT_TOOTHED_WHEEL_60_2"
        ci.getContent()[0] = 8;

        CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
        widget.update("trigger_primary", model, ci);

        JPanel content = widget.getContentPane();

        JPanel panelWidget = findPanelByName(content, "Primary Trigger");

        assertNotNull(panelWidget, "Should find a panel widget named 'Primary Trigger'");
        assertTrue(hasEastPanel(panelWidget), "Should find a trigger preview beside the Primary Trigger fields");

        // Verify image panel exists in east panel
        JPanel eastPanel = getEastPanel(panelWidget);
        assertNotNull(eastPanel);
        assertEquals(1, eastPanel.getComponentCount(), "East panel should contain trigger image panel");
        JPanel triggerImagePanel = (JPanel) eastPanel.getComponent(0);
        assertTrue(triggerImagePanel.getComponentCount() > 0, "Trigger image panel should contain the wheel");
        assertTrue(hasLabel(content, "Trigger Type"), "Trigger configuration fields should remain editable beside the wheel");
    }

    private JPanel getEastPanel(JPanel panelWidget) {
        if (panelWidget.getLayout() instanceof BorderLayout) {
            Component east = ((BorderLayout) panelWidget.getLayout()).getLayoutComponent(BorderLayout.EAST);
            if (east instanceof JPanel) {
                return (JPanel) east;
            }
        }
        return null;
    }

    @Test
    public void testSubPanelDetection() throws IniParsingException {
        // Test with sub-panel
        String string = "[Constants]\n" +
            "signature = \"rusEFI\"\n" +
            "ochBlockSize = 100\n" +
            "nPages = 1\n" +
            "pageSize = 100\n" +
            "pageReadCommand = \"r\"\n" +
            "page = 1\n" +
            "field1 = scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n" +
            "[SettingContextHelp]\n" +
            "; SettingContextHelpEnd\n" +
            "\n" +
            "\tdialog = trigger_primary, \"Sub Panel\"\n" +
            "\t\tfield = \"Field 1\", field1\n" +
            "\tdialog = mainDialog, \"Main Dialog\"\n" +
            "\t\tpanel = trigger_primary\n";

        RawIniFile lines = IniFileReaderUtil.read(new java.io.ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderUtil.readIniFile(lines, "test2.ini", new com.opensr5.ini.IniFileMetaInfoImpl(lines));

        CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
        widget.update("mainDialog", model, null);

        JPanel content = widget.getContentPane();
        JPanel panelWidget = null;
        for (Component c : content.getComponents()) {
            if (c instanceof JPanel && "Sub Panel".equals(c.getName())) {
                panelWidget = (JPanel) c;
                break;
            }
        }
        assertNotNull(panelWidget, "Should find a sub-panel named 'Sub Panel'");
        assertTrue(hasEastPanel(panelWidget), "Should find a trigger preview beside the Primary Trigger sub-panel");
    }

    @Test
    public void testCamInputsPanelDetection() throws IniParsingException {
        String string = "[Constants]\n" +
            "signature = \"rusEFI\"\n" +
            "ochBlockSize = 100\n" +
            "nPages = 1\n" +
            "pageSize = 100\n" +
            "pageReadCommand = \"r\"\n" +
            "page = 1\n" +
            "field1 = scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n" +
            "vvtMode1 = scalar, U08, 4, \"\", 1, 0, 0, 32, 0\n" +
            "vvtMode2 = scalar, U08, 5, \"\", 1, 0, 0, 32, 0\n" +
            "[SettingContextHelp]\n" +
            "; SettingContextHelpEnd\n" +
            "\n" +
            "\tdialog = trigger_cams, \"Cam Inputs\"\n" +
            "\t\tfield = \"Cam mode\", vvtMode1\n" +
            "\t\tfield = \"Exhaust cam mode\", vvtMode2\n" +
            "\t\tfield = \"Field 1\", field1\n";

        RawIniFile lines = IniFileReaderUtil.read(new java.io.ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderUtil.readIniFile(lines, "test.ini", new com.opensr5.ini.IniFileMetaInfoImpl(lines));
        ConfigurationImage image = new ConfigurationImage(new byte[100]);
        image.getContent()[4] = 9; // VVT_NISSAN_VQ
        image.getContent()[5] = 8; // VVT_BARRA_3_PLUS_1

        CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
        widget.update("trigger_cams", model, image);

        JPanel content = widget.getContentPane();
        JPanel panelWidget = findPanelByName(content, "Cam Inputs");

        assertNotNull(panelWidget, "Should find a panel widget named 'Cam Inputs'");
        assertTrue(hasEastPanel(panelWidget), "Should find a trigger preview beside the Cam Inputs fields");
        JPanel triggerImagePanel = (JPanel) getEastPanel(panelWidget).getComponent(0);
        JPanel previews = (JPanel) triggerImagePanel.getComponent(0);
        assertEquals(2, previews.getComponentCount(), "Both active cam modes should render a wheel");
    }

    private JPanel findPanelByName(JPanel container, String name) {
        if (name.equals(container.getName())) {
            return container;
        }
        for (Component c : container.getComponents()) {
            if (c instanceof JPanel) {
                JPanel jp = (JPanel) c;
                if (name.equals(jp.getName())) {
                    return jp;
                }
            }
            if (c instanceof JScrollPane) {
                Component view = ((JScrollPane) c).getViewport().getView();
                if (view instanceof JPanel) {
                    JPanel result = findPanelByName((JPanel) view, name);
                    if (result != null) return result;
                }
            }
        }
        return null;
    }

    private boolean hasEastPanel(JPanel panelWidget) {
        if (panelWidget.getLayout() instanceof BorderLayout) {
            Component east = ((BorderLayout) panelWidget.getLayout()).getLayoutComponent(BorderLayout.EAST);
            return east instanceof JPanel;
        }
        return false;
    }

    private boolean hasLabel(Container container, String text) {
        for (Component component : container.getComponents()) {
            if (component instanceof JLabel && text.equals(((JLabel) component).getText())) {
                return true;
            }
            if (component instanceof Container && hasLabel((Container) component, text)) {
                return true;
            }
        }
        return false;
    }
}
