package com.opensr5.ini.test;

import com.opensr5.ini.*;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;

import static com.opensr5.ini.test.IniFileReaderTest.EPS;
import static com.opensr5.ini.test.IniFileReaderTest.readLines;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class IniDialogTest {
    @Test
    public void testDialogWithPanels() {
        String string = "[Constants]\n" +
            "page = 1\n" +
            "field1 = scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n" +
            "field2 = scalar, F32, 4, \"unit\", 1, 0, 0, 100, 1\n" +
            "[SettingContextHelp]\n" +
            "; SettingContextHelpEnd\n" +
            "\tdialog = subDialog1, \"Sub Dialog 1\"\n" +
            "\t\tfield = \"Field 1\", field1\n" +
            "\n" +
            "\tdialog = subDialog2, \"Sub Dialog 2\"\n" +
            "\t\tfield = \"Field 2\", field2\n" +
            "\t\tcommandButton = \"Enable internal trigger simulation\", cmd_enable_self_stim\n" +
            "\t\tcommandButton = \"Enable external trigger simulation\", cmd_enable_ext_stim" +
            "\n" +
            "\tdialog = mainDialog, \"Main Dialog\", yAxis\n" +
            "\t\tpanel = subDialog1\n" +
            "\t\tpanel = subDialog2, Center\n" +
            "\n" +
            "\tdialog = complexDialog, \"Complex Dialog\", border\n" +
            "\t\tpanel = subDialog1, West\n" +
            "\t\tpanel = subDialog2, Center, {field1 > 50}, {field2 < 100}\n" +
            "\t\tpanel = subDialog3, {field1 > 50}, {field2 < 100}\n" +
            ""
            ;

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // Verify dialogs were created
        assertEquals(4, model.getDialogs().size());

        // Test mainDialog with panels (use dialog key, not UI name)
        DialogModel mainDialog = model.getDialogs().get("mainDialog");
        assertNotNull(mainDialog);
        assertEquals(2, mainDialog.getPanels().size());
        assertEquals(0, mainDialog.getFields().size());

        // Check first panel (no placement)
        PanelModel panel1 = mainDialog.getPanels().get(0);
        assertEquals("subDialog1", panel1.getPanelName());
        assertNull(panel1.getPlacement());
        assertNull(panel1.getEnableExpression());
        assertNull(panel1.getVisibleExpression());

        // Check second panel (with placement)
        PanelModel panel2 = mainDialog.getPanels().get(1);
        assertEquals("subDialog2", panel2.getPanelName());
        assertEquals("Center", panel2.getPlacement());
        assertNull(panel2.getEnableExpression());
        assertNull(panel2.getVisibleExpression());

        assertEquals("yAxis", mainDialog.getLayoutHint());

        DialogModel subDialog2 = model.getDialogs().get("subDialog2");
        assertEquals(2, subDialog2.getCommandsOfCurrentDialog().size());
        assertEquals("Enable internal trigger simulation", subDialog2.getCommandsOfCurrentDialog().get(0).getUiName());
        assertEquals("cmd_enable_self_stim", subDialog2.getCommandsOfCurrentDialog().get(0).getCommand());
        assertEquals("Enable external trigger simulation", subDialog2.getCommandsOfCurrentDialog().get(1).getUiName());
        assertEquals("cmd_enable_ext_stim", subDialog2.getCommandsOfCurrentDialog().get(1).getCommand());

        // Test complexDialog with panels with expressions (use dialog key)
        DialogModel complexDialog = model.getDialogs().get("complexDialog");
        assertNotNull(complexDialog);
        assertEquals(3, complexDialog.getPanels().size());

        // Check first panel (West placement, no expressions)
        PanelModel complexPanel1 = complexDialog.getPanels().get(0);
        assertEquals("subDialog1", complexPanel1.getPanelName());
        assertEquals("West", complexPanel1.getPlacement());
        assertNull(complexPanel1.getEnableExpression());
        assertNull(complexPanel1.getVisibleExpression());

        // Check third panel (Center placement, with enable and visible expressions)
        PanelModel complexPanel3 = complexDialog.getPanels().get(2);
        assertEquals("subDialog3", complexPanel3.getPanelName());
        assertEquals(null, complexPanel3.getPlacement());
        assertEquals("{field1 > 50}", complexPanel3.getEnableExpression());
        assertEquals("{field2 < 100}", complexPanel3.getVisibleExpression());

        // Check second panel (Center placement, with enable and visible expressions)
        PanelModel complexPanel2 = complexDialog.getPanels().get(1);
        assertEquals("subDialog2", complexPanel2.getPanelName());
        assertEquals("Center", complexPanel2.getPlacement());
        assertEquals("{field1 > 50}", complexPanel2.getEnableExpression());
        assertEquals("{field2 < 100}", complexPanel2.getVisibleExpression());

        assertEquals("border", complexDialog.getLayoutHint());
    }

    @Test
    public void testDialogPanelFieldPath() {
        // This test demonstrates the hierarchy: Dialog -> Panel -> Field
        // A parent dialog references child dialogs via panels, which contain fields
        String string = "[Constants]\n" +
            "page = 1\n" +
            "injectorFlow = scalar, F32, 0, \"cc/min\", 1, 0, 0, 2000, 1\n" +
            "injectorPressure = scalar, F32, 4, \"kPa\", 1, 0, 0, 1000, 1\n" +
            "cylinderCount = scalar, U08, 8, \"cylinders\", 1, 0, 1, 12, 0\n" +
            "displacement = scalar, F32, 12, \"L\", 1, 0, 0, 10, 2\n" +
            "[SettingContextHelp]\n" +
            "; SettingContextHelpEnd\n" +
            "\n" +
            "dialog = injectorSettings, \"Injector Configuration\"\n" +
            "\t\tfield = \"Flow Rate\", injectorFlow\n" +
            "\t\tfield = \"Pressure\", injectorPressure\n" +
            "\n" +
            "\tdialog = engineSpecs, \"Engine Specifications\"\n" +
            "\t\tfield = \"Number of Cylinders\", cylinderCount\n" +
            "\t\tfield = \"Displacement\", displacement\n" +
            "\n" +
            "\tdialog = fuelSystemTab, \"Fuel System\", yAxis\n" +
            "\t\tpanel = injectorSettings\n" +
            "\t\tpanel = engineSpecs\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // Verify all dialogs were created
        assertEquals(3, model.getDialogs().size());

        // Get the parent dialog (contains panels) - use dialog key
        DialogModel fuelSystemTab = model.getDialogs().get("fuelSystemTab");
        assertNotNull(fuelSystemTab);
        assertEquals("fuelSystemTab", fuelSystemTab.getKey());
        assertEquals(0, fuelSystemTab.getFields().size()); // Parent has no direct fields
        assertEquals(2, fuelSystemTab.getPanels().size()); // Parent has 2 panels

        // Verify panel references
        PanelModel panel1 = fuelSystemTab.getPanels().get(0);
        assertEquals("injectorSettings", panel1.getPanelName());

        PanelModel panel2 = fuelSystemTab.getPanels().get(1);
        assertEquals("engineSpecs", panel2.getPanelName());

        // Get the first child dialog via panel reference (use dialog key)
        DialogModel injectorSettings = model.getDialogs().get("injectorSettings");
        assertNotNull(injectorSettings);
        assertEquals("injectorSettings", injectorSettings.getKey());
        assertEquals(2, injectorSettings.getFields().size()); // Child has fields
        assertEquals(0, injectorSettings.getPanels().size()); // Child has no panels

        // Verify fields in first child dialog
        DialogModel.Field field1 = injectorSettings.getFields().get(0);
        assertEquals("injectorFlow", field1.getKey());
        assertEquals("Flow Rate", field1.getUiName());

        DialogModel.Field field2 = injectorSettings.getFields().get(1);
        assertEquals("injectorPressure", field2.getKey());
        assertEquals("Pressure", field2.getUiName());

        // Get the second child dialog via panel reference (use dialog key)
        DialogModel engineSpecs = model.getDialogs().get("engineSpecs");
        assertNotNull(engineSpecs);
        assertEquals("engineSpecs", engineSpecs.getKey());
        assertEquals(2, engineSpecs.getFields().size());
        assertEquals(0, engineSpecs.getPanels().size());

        // Verify fields in second child dialog
        DialogModel.Field field3 = engineSpecs.getFields().get(0);
        assertEquals("cylinderCount", field3.getKey());
        assertEquals("Number of Cylinders", field3.getUiName());

        DialogModel.Field field4 = engineSpecs.getFields().get(1);
        assertEquals("displacement", field4.getKey());
        assertEquals("Displacement", field4.getUiName());

        // Demonstrate the complete path: fuelSystemTab -> injectorSettings -> injectorFlow
        // This simulates how a UI would navigate the hierarchy:
        // 1. Start with parent dialog
        assertEquals("fuelSystemTab", fuelSystemTab.getKey());

        // 2. Get first panel reference
        PanelModel firstPanel = fuelSystemTab.getPanels().get(0);
        assertEquals("injectorSettings", firstPanel.getPanelName());

        // 3. Resolve the child dialog using the simplified lookup method
        DialogModel childDialog = firstPanel.resolveDialog(model);
        assertNotNull(childDialog);
        assertEquals("injectorSettings", childDialog.getKey());

        // 4. Access fields in the child dialog
        assertEquals(2, childDialog.getFields().size());
        assertEquals("injectorFlow", childDialog.getFields().get(0).getKey());

        // Demonstrate simplified one-liner navigation
        DialogModel secondChild = fuelSystemTab.getPanels().get(1).resolveDialog(model);
        assertNotNull(secondChild);
        assertEquals("engineSpecs", secondChild.getKey());
        assertEquals("cylinderCount", secondChild.getFields().get(0).getKey());
    }

    @Test
    public void testGaugeConfigurationWithStringFunctionExpressions() {
        String string = "[Constants]\n" +
            "page = 1\n" +
            "gpPwmNote1 = string, ASCII, 200, 16\n" +
            "[GaugeConfigurations]\n" +
            "gaugeCategory = GPPWMGauges\n" +
            "gppwmGauge1 = gppwmOutput1, { stringValue(gpPwmNote1) }, \"%\", 0, 100, 0, 0, 100, 100, 1, 1\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // Test that gauge with stringValue() expression is parsed successfully
        GaugeModel gauge = model.getGauge("gppwmGauge1");
        assertNotNull(gauge);
        assertEquals("gppwmGauge1", gauge.getName());
        assertEquals("gppwmOutput1", gauge.getChannel());

        // Title should keep the expression as-is (evaluation happens at runtime)
        assertEquals("{ stringValue(gpPwmNote1) }", gauge.getTitle());

        // Units is a plain string (no expression)
        assertEquals("%", gauge.getUnits());

        // Numeric values
        assertEquals(0.0, gauge.getLowValue(), EPS);
        assertEquals(100.0, gauge.getHighValue(), EPS);
        assertEquals(1, gauge.getValueDecimalPlaces());
        assertEquals(1, gauge.getLabelDecimalPlaces());

        // Gauge should indicate it needs runtime evaluation due to stringValue() in title
        assertTrue(gauge.needsOutputChannelEvaluation());

        // Check that title is stored as an expression (contains function call)
        assertTrue(gauge.getTitleValue().isExpression());
        assertEquals("{ stringValue(gpPwmNote1) }", gauge.getTitleValue().getStringValue());

        // Units is stored as a plain string (no expression markers)
        assertTrue(gauge.getUnitsValue().isString());
        assertEquals("%", gauge.getUnitsValue().getStringValue());
    }

    @Test
    public void testGetDialogKeyByTitle() {
        String string = "[Constants]\n" +
            "page = 1\n" +
            "veTable = array, F32, 0, [16x16], \"value\", 1, 0, 0, 100, 2\n" +
            "field1 = scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n" +
            "[SettingContextHelp]\n" +
            "; SettingContextHelpEnd\n" +
            "\thelp = veTableDialogHelp, \"Volumetric Efficiency\"\n" +
            "\t\ttext = \"VE help text\"\n" +
            "\t\twebHelp = \"https://rusefi.com/s/fuel\"\n" +
            "\n" +
            "\tdialog = veTableDialog, \"VE Table\"\n" +
            "\t\ttopicHelp = \"veTableDialogHelp\"\n" +
            "\t\tfield = \"VE Table\", veTable\n" +
            "\n" +
            "\tdialog = engineChars, \"Base Engine Settings\"\n" +
            "\t\tfield = \"Field 1\", field1\n" +
            "\n" +
            "\tdialog = idleSettings, \"Idle Control\"\n" +
            "\t\tfield = \"Field 1\", field1\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        // Test finding dialog key by title
        String veDialogKey = model.getDialogKeyByTitle("VE Table");
        assertEquals("veTableDialog", veDialogKey);

        String engineCharsKey = model.getDialogKeyByTitle("Base Engine Settings");
        assertEquals("engineChars", engineCharsKey);

        String idleSettingsKey = model.getDialogKeyByTitle("Idle Control");
        assertEquals("idleSettings", idleSettingsKey);

        // Test with non-existent title
        String nonExistent = model.getDialogKeyByTitle("Non Existent Dialog");
        assertNull(nonExistent);

        // Test with empty title
        String emptyTitle = model.getDialogKeyByTitle("");
        assertNull(emptyTitle);
    }
}
