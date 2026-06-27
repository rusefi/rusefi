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

public class PrimaryTriggerPanelTest {
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
        // We trigger the update by dialog key
        widget.update("trigger_primary", model, null);

        JPanel content = widget.getContentPane();

        // When updating by dialog key "trigger_primary", content name is NOT set to "Primary Trigger"
        // and fillPanel is called on content.
        // We need the detection to happen in update(DialogModel) as well.

        // The Primary Trigger dialog content is added to 'content'.
        // Since CalibrationDialogWidget.update(String key) calls update(DialogModel),
        // and fillPanel adds a panelWidget to 'content'.

        // Let's find the panelWidget named "Primary Trigger"
        JPanel panelWidget = null;
        for (Component c : content.getComponents()) {
            if (c instanceof JPanel) {
                JPanel jp = (JPanel) c;
                if ("Primary Trigger".equals(jp.getName())) {
                    panelWidget = jp;
                    break;
                }
                // Also check children if it's wrapped in a ScrollPane or something
                if (c instanceof JViewport) {
                    // ...
                }
            }
            if (c instanceof JScrollPane) {
                Component view = ((JScrollPane) c).getViewport().getView();
                if (view instanceof JPanel) {
                    for (Component cc : ((JPanel) view).getComponents()) {
                        if (cc instanceof JPanel && "Primary Trigger".equals(cc.getName())) {
                            panelWidget = (JPanel) cc;
                            break;
                        }
                    }
                }
            }
        }

        // If not found yet, maybe the content itself IS the panelWidget if it was a dialog update?
        if (panelWidget == null && "Primary Trigger".equals(content.getName())) {
            panelWidget = content;
        }

        // Wait, update("trigger_primary") calls update(dialog)
        // update(dialog) calls fillPanel(contentPane, dialog, ...)
        // fillPanel adds FIELDS directly to contentPane if they are fields.
        // But if the dialog has PANELS, it calls renderPanelEntry which adds panelWidgets.

        // Let's print names of components in content for debugging if it fails
        if (panelWidget == null) {
            System.out.println("[DEBUG_LOG] Content layout: " + content.getLayout());
            System.out.println("[DEBUG_LOG] Content name: " + content.getName());
            for (Component c : content.getComponents()) {
                System.out.println("[DEBUG_LOG] Component: " + c.getClass().getName() + " name: " + c.getName());
            }
        }

        assertNotNull(panelWidget, "Should find a panel widget named 'Primary Trigger'");

        // We expect an east panel with orange border in panelWidget
        boolean foundEastPanel = false;
        if (panelWidget.getLayout() instanceof BorderLayout) {
            Component east = ((BorderLayout) panelWidget.getLayout()).getLayoutComponent(BorderLayout.EAST);
            if (east instanceof JPanel) {
                JPanel eastPanel = (JPanel) east;
                if (eastPanel.getBorder() instanceof LineBorder) {
                    LineBorder lb = (LineBorder) eastPanel.getBorder();
                    if (Color.ORANGE.equals(lb.getLineColor())) {
                        foundEastPanel = true;
                    }
                }
            }
        }

        assertTrue(foundEastPanel, "Should find an east panel with orange border in Primary Trigger panel widget");

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

        foundEastPanel = false;
        if (panelWidget.getLayout() instanceof BorderLayout) {
            Component east = ((BorderLayout) panelWidget.getLayout()).getLayoutComponent(BorderLayout.EAST);
            if (east instanceof JPanel) {
                JPanel eastPanel = (JPanel) east;
                if (eastPanel.getBorder() instanceof LineBorder) {
                    LineBorder lb = (LineBorder) eastPanel.getBorder();
                    if (Color.ORANGE.equals(lb.getLineColor())) {
                        foundEastPanel = true;
                    }
                }
            }
        }
        assertTrue(foundEastPanel, "Should find an east panel with orange border in Primary Trigger sub-panel");
    }
}
