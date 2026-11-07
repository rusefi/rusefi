package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;

class PanelExpressionTest {
    private static final String CONSTANTS = "[Constants]\npage = 1\n"
        + "visibleFlag = scalar, U08, 0, \"\", 1, 0, 0, 1, 0\n"
        + "enabledFlag = scalar, U08, 1, \"\", 1, 0, 0, 1, 0\n"
        + "innerFlag = scalar, U08, 2, \"\", 1, 0, 0, 1, 0\n"
        + "[UserDefined]\n";

    private static final String CHILD = "dialog = child, \"Child\"\n"
        + "field = \"Inner\", innerFlag, { 1 }\n";

    @Test
    void panelFollowsItsVisibleExpressionWhileValuesAreEdited() throws Throwable {
        IniFileModel ini = parse(CONSTANTS + CHILD
            + "dialog = main, \"Main\"\n"
            + "field = \"Visible\", visibleFlag\n"
            + "panel = child, { 1 }, { visibleFlag }\n");
        withWidget(ini, "main", widget -> {
            JPanel content = widget.getContentPane();
            JTextField visibleEditor = editor((Container) content.getComponent(0));
            Component panel = content.getComponent(1);
            assertFalse(panel.isVisible(), "A false visible expression hides the panel");

            visibleEditor.setText("1");
            assertTrue(panel.isVisible(), "Editing the value shows the panel again");

            visibleEditor.setText("0");
            assertFalse(panel.isVisible());
        });
    }

    @Test
    void disabledPanelDisablesChildrenDespiteTheirOwnExpressions() throws Throwable {
        IniFileModel ini = parse(CONSTANTS + CHILD
            + "dialog = main, \"Main\"\n"
            + "field = \"Enabled\", enabledFlag\n"
            + "panel = child, { enabledFlag }\n");
        withWidget(ini, "main", widget -> {
            JPanel content = widget.getContentPane();
            JTextField enabledEditor = editor((Container) content.getComponent(0));
            Container panel = (Container) content.getComponent(1);
            JTextField inner = editor((Container) panel.getComponent(0));
            assertTrue(panel.isVisible());
            assertFalse(inner.isEnabled(), "A false enabled expression disables children unconditionally");

            enabledEditor.setText("1");
            assertTrue(inner.isEnabled(), "A true enabled expression leaves children to their own expressions");

            enabledEditor.setText("0");
            assertFalse(inner.isEnabled());
        });
    }

    @Test
    void panelWithoutExpressionsIsUnaffected() throws Throwable {
        IniFileModel ini = parse(CONSTANTS + CHILD
            + "dialog = main, \"Main\"\n"
            + "field = \"Visible\", visibleFlag\n"
            + "panel = child\n");
        withWidget(ini, "main", widget -> {
            JPanel content = widget.getContentPane();
            Container panel = (Container) content.getComponent(1);
            JTextField inner = editor((Container) panel.getComponent(0));
            assertTrue(panel.isVisible());
            assertTrue(inner.isEnabled());

            editor((Container) content.getComponent(0)).setText("1");
            assertTrue(panel.isVisible());
            assertTrue(inner.isEnabled());
        });
    }

    @Test
    void nestedDisabledPanelWinsOverEnabledOuterPanel() throws Throwable {
        IniFileModel ini = parse(CONSTANTS + CHILD
            + "dialog = middle, \"Middle\"\n"
            + "panel = child, { innerFlag }\n"
            + "dialog = main, \"Main\"\n"
            + "field = \"Enabled\", enabledFlag\n"
            + "field = \"Inner flag\", innerFlag\n"
            + "panel = middle, { enabledFlag }\n");
        withWidget(ini, "main", widget -> {
            JPanel content = widget.getContentPane();
            JTextField enabledEditor = editor((Container) content.getComponent(0));
            JTextField innerFlagEditor = editor((Container) content.getComponent(1));
            Container middle = (Container) content.getComponent(2);
            JTextField inner = editor((Container) ((Container) middle.getComponent(0)).getComponent(0));
            assertFalse(inner.isEnabled());

            enabledEditor.setText("1");
            assertFalse(inner.isEnabled(), "Enabling the outer panel does not enable the still disabled inner one");

            innerFlagEditor.setText("1");
            assertTrue(inner.isEnabled());

            enabledEditor.setText("0");
            assertFalse(inner.isEnabled());
        });
    }

    // Mirrors the idleSolenoid dialog of tunerstudio.template.ini.
    @Test
    void secondaryIdleSolenoidPinIsEnabledByDoubleSolenoidModeOrItsOwnPin() throws Throwable {
        IniFileModel ini = parse("[Constants]\npage = 1\n"
            + "idle_solenoidPin = scalar, U08, 0, \"\", 1, 0, 0, 255, 0\n"
            + "secondSolenoidPin = scalar, U08, 1, \"\", 1, 0, 0, 255, 0\n"
            + "useStepperIdle = scalar, U08, 2, \"\", 1, 0, 0, 1, 0\n"
            + "isDoubleSolenoidIdle = scalar, U08, 3, \"\", 1, 0, 0, 1, 0\n"
            + "[UserDefined]\ndialog = idleSolenoid, \"Solenoid\"\n"
            + "field = \"Primary\", idle_solenoidPin\n"
            + "field = \"Secondary\", secondSolenoidPin, "
            + "{ secondSolenoidPin != 0 || (!useStepperIdle && isDoubleSolenoidIdle ) }\n"
            + "field = \"Stepper\", useStepperIdle\n"
            + "field = \"Double\", isDoubleSolenoidIdle\n");
        withWidget(ini, "idleSolenoid", widget -> {
            JPanel content = widget.getContentPane();
            JTextField primary = editor((Container) content.getComponent(0));
            JTextField secondary = editor((Container) content.getComponent(1));
            JTextField stepper = editor((Container) content.getComponent(2));
            JTextField doubleSolenoid = editor((Container) content.getComponent(3));
            assertFalse(secondary.isEnabled());

            primary.setText("1");
            assertFalse(secondary.isEnabled(), "The primary pin does not enable the secondary output");

            doubleSolenoid.setText("1");
            assertTrue(secondary.isEnabled());

            stepper.setText("1");
            assertFalse(secondary.isEnabled());

            secondary.setText("1");
            assertTrue(secondary.isEnabled(), "An assigned secondary pin stays editable so it can be cleared");
        });
    }

    private interface WidgetAction {
        void run(CalibrationDialogWidget widget);
    }

    private static void withWidget(IniFileModel ini, String dialog, WidgetAction action) throws Throwable {
        SwingUtilities.invokeAndWait(() -> {
            CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
            try {
                widget.update(dialog, ini, new ConfigurationImage(16));
                action.run(widget);
            } finally {
                widget.destroy();
            }
        });
    }

    private static JTextField editor(Container row) {
        for (Component child : row.getComponents()) {
            if (child instanceof JTextField) {
                return (JTextField) child;
            }
        }
        throw new AssertionError("No text editor in " + row);
    }

    private static IniFileModel parse(String text) throws Throwable {
        return IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(new ByteArrayInputStream(
            text.getBytes(StandardCharsets.US_ASCII))), "panelExpression.ini", mock(IniFileMetaInfo.class));
    }
}
