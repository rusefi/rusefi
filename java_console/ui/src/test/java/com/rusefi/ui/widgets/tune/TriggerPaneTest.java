package com.rusefi.ui.widgets.tune;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import javax.swing.border.LineBorder;
import java.awt.*;

import static org.junit.jupiter.api.Assertions.*;

public class TriggerPaneTest {
    @Test
    public void testPrimaryTriggerPanelDetection() throws IniParsingException {
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
            "\tdialog = trigger_primary, \"Primary Trigger\"\n" +
            "\t\tfield = \"Field 1\", field1\n";

        RawIniFile lines = IniFileReaderUtil.read(new java.io.ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderUtil.readIniFile(lines, "test.ini", new com.opensr5.ini.IniFileMetaInfoImpl(lines));

        CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
        widget.update("trigger_primary", model, null);

        JPanel content = widget.getContentPane();

        JPanel panelWidget = findPanelByName(content, "Primary Trigger");

        assertNotNull(panelWidget, "Should find a panel widget named 'Primary Trigger'");
        assertTrue(hasOrangeEastPanel(panelWidget), "Should find an east panel with orange border in Primary Trigger panel widget");

        // Test with sub-panel
        string = "[Constants]\n" +
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

        lines = IniFileReaderUtil.read(new java.io.ByteArrayInputStream(string.getBytes()));
        model = IniFileReaderUtil.readIniFile(lines, "test2.ini", new com.opensr5.ini.IniFileMetaInfoImpl(lines));

        widget = new CalibrationDialogWidget(new UIContext());
        widget.update("mainDialog", model, null);

        content = widget.getContentPane();
        panelWidget = null;
        for (Component c : content.getComponents()) {
            if (c instanceof JPanel && "Sub Panel".equals(c.getName())) {
                panelWidget = (JPanel) c;
                break;
            }
        }
        assertNotNull(panelWidget, "Should find a sub-panel named 'Sub Panel'");
        assertTrue(hasOrangeEastPanel(panelWidget), "Should find an east panel with orange border in Primary Trigger sub-panel");
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
            "[SettingContextHelp]\n" +
            "; SettingContextHelpEnd\n" +
            "\n" +
            "\tdialog = trigger_cams, \"Cam Inputs\"\n" +
            "\t\tfield = \"Field 1\", field1\n";

        RawIniFile lines = IniFileReaderUtil.read(new java.io.ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderUtil.readIniFile(lines, "test.ini", new com.opensr5.ini.IniFileMetaInfoImpl(lines));

        CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
        widget.update("trigger_cams", model, null);

        JPanel content = widget.getContentPane();
        JPanel panelWidget = findPanelByName(content, "Cam Inputs");

        assertNotNull(panelWidget, "Should find a panel widget named 'Cam Inputs'");
        assertTrue(hasOrangeEastPanel(panelWidget), "Should find an east panel with orange border in Cam Inputs panel");
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

    private boolean hasOrangeEastPanel(JPanel panelWidget) {
        if (panelWidget.getLayout() instanceof BorderLayout) {
            Component east = ((BorderLayout) panelWidget.getLayout()).getLayoutComponent(BorderLayout.EAST);
            if (east instanceof JPanel) {
                JPanel eastPanel = (JPanel) east;
                if (eastPanel.getBorder() instanceof LineBorder) {
                    LineBorder lb = (LineBorder) eastPanel.getBorder();
                    return Color.ORANGE.equals(lb.getLineColor());
                }
            }
        }
        return false;
    }
}
