package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IndicatorModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.PanelModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.config.FieldType;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.laf.GradientTitleBorder;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

public class CalibrationDialogWidgetTest {

    @Test
    public void testLayout() {
        // Mock IniFileModel
        IniFileModel iniFileModel = mock(IniFileModel.class);
        when(iniFileModel.getCurves()).thenReturn(Collections.emptyMap());
        Map<String, DialogModel> dialogs = new HashMap<>();
        when(iniFileModel.getDialogs()).thenReturn(dialogs);

        // Setup panels
        List<PanelModel> panels = new ArrayList<>();
        // Vertical
        panels.add(new PanelModel("v1", "north", null, null));
        // Horizontal group
        panels.add(new PanelModel("h1", "west", null, null));
        panels.add(new PanelModel("h2", "center", null, null));
        panels.add(new PanelModel("h3", "east", null, null));
        // Vertical again
        panels.add(new PanelModel("v2", "south", null, null));
        // Mixed horizontal
        panels.add(new PanelModel("h4", "west", null, null));
        panels.add(new PanelModel("v3", "north", null, null));
        panels.add(new PanelModel("h5", "east", null, null));

        DialogModel mainDialog = new DialogModel("main", "Main", Collections.emptyList(), Collections.emptyList(), panels, null);

        CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
        widget.update(mainDialog, iniFileModel, null);

        JPanel content = widget.getContentPane();
        Component[] components = content.getComponents();

        // Expecting:
        // 0: v1 (JPanel)
        // 1: horizontalPanel (JPanel)
        // 2: v2 (JPanel)
        // 3: h4 (in horizontalPanel)
        // 4: v3 (JPanel)
        // 5: h5 (in horizontalPanel)
        assertEquals(6, components.length, "Should have 6 top-level components");

        assertTrue(components[0] instanceof JPanel);
        assertEquals("v1", components[0].getName());

        assertTrue(components[1] instanceof JPanel);
        JPanel hPanel1 = (JPanel) components[1];
        assertEquals(3, hPanel1.getComponentCount(), "First horizontal panel should have 3 sub-panels");
        assertEquals("h1", hPanel1.getComponent(0).getName());
        assertEquals("h2", hPanel1.getComponent(1).getName());
        assertEquals("h3", hPanel1.getComponent(2).getName());
        assertTrue(hPanel1.getLayout() instanceof com.rusefi.ui.util.WrapLayout, "Should be wrap layout");

        assertTrue(components[2] instanceof JPanel);
        assertEquals("v2", components[2].getName());

        assertTrue(components[3] instanceof JPanel);
        JPanel hPanel2 = (JPanel) components[3];
        assertEquals(1, hPanel2.getComponentCount(), "Second horizontal panel should have 1 sub-panel");
        assertEquals("h4", hPanel2.getComponent(0).getName());

        assertTrue(components[4] instanceof JPanel);
        assertEquals("v3", components[4].getName());

        assertTrue(components[5] instanceof JPanel);
        JPanel hPanel3 = (JPanel) components[5];
        assertEquals(1, hPanel3.getComponentCount(), "Third horizontal panel should have 1 sub-panel");
        assertEquals("h5", hPanel3.getComponent(0).getName());
    }

    @Test
    public void testXAxisLayoutHint() {
        IniFileModel iniFileModel = mock(IniFileModel.class);
        when(iniFileModel.getCurves()).thenReturn(Collections.emptyMap());

        List<PanelModel> panels = new ArrayList<>();
        panels.add(new PanelModel("p1", null, null, null));
        panels.add(new PanelModel("p2", null, null, null));

        // Dialog with xAxis layout hint
        DialogModel mainDialog = new DialogModel("main", "Main", new ArrayList<DialogModel.Field>(), new ArrayList<DialogModel.Command>(), panels, (String) null, "xAxis");

        CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
        widget.update(mainDialog, iniFileModel, null);

        JPanel content = widget.getContentPane();
        assertTrue(content.getLayout() instanceof BoxLayout);
        assertEquals(BoxLayout.X_AXIS, ((BoxLayout) content.getLayout()).getAxis(), "Top level should be horizontal");
        assertEquals(2, content.getComponentCount());
    }

    @Test
    public void testSubDialogTitleAndLayout() {
        IniFileModel iniFileModel = mock(IniFileModel.class);
        when(iniFileModel.getCurves()).thenReturn(Collections.emptyMap());

        DialogModel subDialog = new DialogModel("sub", "Sub Dialog UI Name", new ArrayList<DialogModel.Field>(), new ArrayList<DialogModel.Command>(), new ArrayList<PanelModel>(), (String) null, "xAxis");
        Map<String, DialogModel> dialogs = new HashMap<>();
        dialogs.put("sub", subDialog);
        when(iniFileModel.getDialogs()).thenReturn(dialogs);

        List<PanelModel> panels = new ArrayList<>();
        panels.add(new PanelModel("sub", null, null, null));

        DialogModel mainDialog = new DialogModel("main", "Main", new ArrayList<DialogModel.Field>(), new ArrayList<DialogModel.Command>(), panels, (String) null, (String) null);

        CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
        widget.update(mainDialog, iniFileModel, null);

        JPanel content = widget.getContentPane();
        JPanel subPanel = (JPanel) content.getComponent(0);

        assertEquals("Sub Dialog UI Name", subPanel.getName());
        assertTrue(subPanel.getBorder() instanceof GradientTitleBorder);
        assertEquals("Sub Dialog UI Name", ((GradientTitleBorder) subPanel.getBorder()).getTitle());

        assertTrue(subPanel.getLayout() instanceof BoxLayout);
        assertEquals(BoxLayout.X_AXIS, ((BoxLayout) subPanel.getLayout()).getAxis(), "Sub-panel should be horizontal due to layout hint");
    }

    @Test
    public void testTableModelPanel() {
        IniFileModel iniFileModel = mock(IniFileModel.class);
        when(iniFileModel.getCurves()).thenReturn(Collections.emptyMap());

        // Mock TableModel
        TableModel tableModel = new TableModel("table1", "map1", "Table Title",
                null, "X", "Y", "xBins", null, false, "yBins", null, false, "zBins",
                null, null, null, null);
        Map<String, TableModel> tables = new HashMap<>();
        tables.put("table1", tableModel);
        when(iniFileModel.getTables()).thenReturn(tables);
        when(iniFileModel.getTable("table1")).thenReturn(tableModel);

        // Mock Bins and Data fields
        ArrayIniField xBinsField = new ArrayIniField("xBins", 0, FieldType.FLOAT, 1, 1, "unit", 1, "0", "100", "1");
        ArrayIniField yBinsField = new ArrayIniField("yBins", 4, FieldType.FLOAT, 1, 1, "unit", 1, "0", "100", "1");
        ArrayIniField zBinsField = new ArrayIniField("zBins", 8, FieldType.FLOAT, 1, 1, "unit", 1, "0", "100", "1");

        when(iniFileModel.findIniField("xBins")).thenReturn(java.util.Optional.of(xBinsField));
        when(iniFileModel.findIniField("yBins")).thenReturn(java.util.Optional.of(yBinsField));
        when(iniFileModel.findIniField("zBins")).thenReturn(java.util.Optional.of(zBinsField));

        List<PanelModel> panels = new ArrayList<>();
        panels.add(new PanelModel("table1", null, null, null));

        DialogModel mainDialog = new DialogModel("main", "Main", new ArrayList<DialogModel.Field>(), new ArrayList<DialogModel.Command>(), panels, (String) null, (String) null);

        CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
        ConfigurationImage ci = new ConfigurationImage(new byte[1000]);
        widget.update(mainDialog, iniFileModel, ci);

        JPanel content = widget.getContentPane();
        assertEquals(1, content.getComponentCount());
        JPanel tablePanel = (JPanel) content.getComponent(0);

        // TuningTableView content contains a topPanel which contains a JLabel
        boolean foundLabel = false;
        for (Component c : tablePanel.getComponents()) {
            if (c instanceof JPanel) {
                for (Component sub : ((JPanel) c).getComponents()) {
                    if (sub instanceof JLabel && ((JLabel) sub).getText().equals("Table Title")) {
                        foundLabel = true;
                        break;
                    }
                }
            }
            if (foundLabel) break;
        }
        assertTrue(foundLabel, "Should find table title label");
    }

    @Test
    public void testUpdateWithTableKey() {
        IniFileModel iniFileModel = mock(IniFileModel.class);
        when(iniFileModel.getCurves()).thenReturn(Collections.emptyMap());
        when(iniFileModel.getDialogs()).thenReturn(Collections.emptyMap());

        // Mock TableModel
        TableModel tableModel = new TableModel("table1", "map1", "Table Title",
                null, "X", "Y", "xBins", null, false, "yBins", null, false, "zBins",
                null, null, null, null);
        when(iniFileModel.getTable("table1")).thenReturn(tableModel);

        // Mock Bins and Data fields
        ArrayIniField xBinsField = new ArrayIniField("xBins", 0, FieldType.FLOAT, 1, 1, "unit", 1, "0", "100", "1");
        ArrayIniField yBinsField = new ArrayIniField("yBins", 4, FieldType.FLOAT, 1, 1, "unit", 1, "0", "100", "1");
        ArrayIniField zBinsField = new ArrayIniField("zBins", 8, FieldType.FLOAT, 1, 1, "unit", 1, "0", "100", "1");

        when(iniFileModel.findIniField("xBins")).thenReturn(java.util.Optional.of(xBinsField));
        when(iniFileModel.findIniField("yBins")).thenReturn(java.util.Optional.of(yBinsField));
        when(iniFileModel.findIniField("zBins")).thenReturn(java.util.Optional.of(zBinsField));

        CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
        ConfigurationImage ci = new ConfigurationImage(new byte[1000]);
        widget.update("table1", iniFileModel, ci);

        JPanel content = widget.getContentPane();
        assertEquals(1, content.getComponentCount());
        JPanel tablePanel = (JPanel) content.getComponent(0);

        boolean foundLabel = false;
        for (Component c : tablePanel.getComponents()) {
            if (c instanceof JPanel) {
                for (Component sub : ((JPanel) c).getComponents()) {
                    if (sub instanceof JLabel && ((JLabel) sub).getText().equals("Table Title")) {
                        foundLabel = true;
                        break;
                    }
                }
            }
            if (foundLabel) break;
        }
        assertTrue(foundLabel, "Should find table title label when updating by key");
    }
    @Test
    public void testCommandButtonAndPanelTitle() {
        IniFileModel iniFileModel = mock(IniFileModel.class);
        when(iniFileModel.getCurves()).thenReturn(Collections.emptyMap());

        // Sub-dialog with commands and NO UI name (but a key)
        List<DialogModel.Command> commands = new ArrayList<>();
        commands.add(new DialogModel.Command("Spark #1", "cmd_test_spk1"));

        // Use key "testSpark" and uiName "" (empty string) to mimic "dialog = testSpark, """
        DialogModel subDialog = new DialogModel("testSpark", "", Collections.emptyList(), commands, new ArrayList<>(), null);
        Map<String, DialogModel> dialogs = new HashMap<>();
        dialogs.put("testSpark", subDialog);
        when(iniFileModel.getDialogs()).thenReturn(dialogs);

        // Main dialog with the panel
        List<PanelModel> panels = new ArrayList<>();
        panels.add(new PanelModel("testSpark", null, null, null));
        DialogModel mainDialog = new DialogModel("main", "Main", Collections.emptyList(), Collections.emptyList(), panels, null);

        CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
        widget.update(mainDialog, iniFileModel, null);

        JPanel content = widget.getContentPane();
        assertEquals(1, content.getComponentCount());
        JPanel panelWidget = (JPanel) content.getComponent(0);

        // Check title - it should fall back to key "testSpark"
        assertTrue(panelWidget.getBorder() instanceof GradientTitleBorder);
        String title = ((GradientTitleBorder) panelWidget.getBorder()).getTitle();
        assertEquals("testSpark", title, "Title should fall back to key if UI name is empty");

        // Check command button
        boolean foundButton = false;
        for (Component c : panelWidget.getComponents()) {
            if (c instanceof JPanel) {
                for (Component sub : ((JPanel) c).getComponents()) {
                    if (sub instanceof JButton && ((JButton) sub).getText().equals("Spark #1")) {
                        foundButton = true;
                        break;
                    }
                }
            }
            if (foundButton) break;
        }
        assertTrue(foundButton, "Should find command button 'Spark #1'");
    }

    @Test
    public void testIsCheckboxEnum() {
        assertCheckbox("No", "Yes", true);
        assertCheckbox("Yes", "No", true);
        assertCheckbox("Disabled", "Enabled", true);
        assertCheckbox("Enabled", "Disabled", true);

        assertCheckbox("false", "true", false);
        assertCheckbox("No", "Maybe", false);
        assertCheckbox("No", "Yes", "Third", false);
    }

    private void assertCheckbox(String v1, String v2, boolean expected) {
        EnumIniField field = createEnumField(v1, v2);
        assertEquals(expected, CalibrationFieldFactory.isCheckboxEnum(field), "Values: " + v1 + ", " + v2);
    }

    private void assertCheckbox(String v1, String v2, String v3, boolean expected) {
        EnumIniField field = createEnumField(v1, v2, v3);
        assertEquals(expected, CalibrationFieldFactory.isCheckboxEnum(field), "Values: " + v1 + ", " + v2 + ", " + v3);
    }

    private EnumIniField createEnumField(String... values) {
        Map<Integer, String> map = new HashMap<>();
        for (int i = 0; i < values.length; i++) {
            map.put(i, values[i]);
        }
        EnumIniField.EnumKeyValueMap enumMap = new EnumIniField.EnumKeyValueMap(map);
        return new EnumIniField("test", 0, FieldType.INT8, enumMap, 0, 0);
    }

    @Test
    public void testBackgroundColor() {
        IniFileModel iniFileModel = mock(IniFileModel.class);
        when(iniFileModel.getCurves()).thenReturn(Collections.emptyMap());

        // Field starting with # should be blue
        com.opensr5.ini.field.StringIniField blueField = new com.opensr5.ini.field.StringIniField("blue", 0, 10);
        when(iniFileModel.findIniField("blue")).thenReturn(java.util.Optional.of(blueField));

        // Field starting with ! should be red
        com.opensr5.ini.field.StringIniField redField = new com.opensr5.ini.field.StringIniField("red", 10, 10);
        when(iniFileModel.findIniField("red")).thenReturn(java.util.Optional.of(redField));

        // Normal field
        com.opensr5.ini.field.StringIniField normalField = new com.opensr5.ini.field.StringIniField("normal", 20, 10);
        when(iniFileModel.findIniField("normal")).thenReturn(java.util.Optional.of(normalField));

        List<DialogModel.Field> fields = new ArrayList<>();
        fields.add(new DialogModel.Field("blue", "Blue Label"));
        fields.add(new DialogModel.Field("red", "Red Label"));
        fields.add(new DialogModel.Field("normal", "Normal Label"));

        DialogModel mainDialog = new DialogModel("main", "Main", fields, Collections.emptyList());

        ConfigurationImage ci = new ConfigurationImage(new byte[100]);
        // Set values in image
        System.arraycopy("#blue".getBytes(), 0, ci.getContent(), 0, 5);
        System.arraycopy("!red".getBytes(), 0, ci.getContent(), 10, 4);
        System.arraycopy("normal".getBytes(), 0, ci.getContent(), 20, 6);

        CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
        widget.update(mainDialog, iniFileModel, ci);

        JPanel content = widget.getContentPane();
        assertEquals(3, content.getComponentCount());

        // Helper to find JTextField in a row
        java.util.function.Function<JPanel, JTextField> getTextField = row -> {
            for (Component c : row.getComponents()) {
                if (c instanceof JTextField) return (JTextField) c;
            }
            return null;
        };

        JTextField blueTF = getTextField.apply((JPanel) content.getComponent(0));
        assertNotNull(blueTF);
        assertEquals(Color.BLUE, blueTF.getBackground());
        assertEquals(Color.WHITE, blueTF.getForeground());

        JTextField redTF = getTextField.apply((JPanel) content.getComponent(1));
        assertNotNull(redTF);
        assertEquals(Color.RED, redTF.getBackground());
        assertEquals(Color.WHITE, redTF.getForeground());

        JTextField normalTF = getTextField.apply((JPanel) content.getComponent(2));
        assertNotNull(normalTF);
        // Default background varies by LAF, but it shouldn't be Blue or Red
        assertNotEquals(Color.BLUE, normalTF.getBackground());
        assertNotEquals(Color.RED, normalTF.getBackground());
    }

    @Test
    public void testEnumBackgroundColor() {
        IniFileModel iniFileModel = mock(IniFileModel.class);
        when(iniFileModel.getCurves()).thenReturn(Collections.emptyMap());

        // Enum field starting with ! should be red
        Map<Integer, String> enumMap = new HashMap<>();
        enumMap.put(0, "!Red Option");
        enumMap.put(1, "Normal Option");
        EnumIniField redEnumField = new EnumIniField("redEnum", 0, FieldType.INT8, new EnumIniField.EnumKeyValueMap(enumMap), 0, 0);
        when(iniFileModel.findIniField("redEnum")).thenReturn(java.util.Optional.of(redEnumField));

        List<DialogModel.Field> fields = new ArrayList<>();
        fields.add(new DialogModel.Field("redEnum", "Red Enum Label"));

        DialogModel mainDialog = new DialogModel("main", "Main", fields, Collections.emptyList());

        ConfigurationImage ci = new ConfigurationImage(new byte[100]);
        ci.getContent()[0] = 0; // Select the "!Red Option"

        CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
        widget.update(mainDialog, iniFileModel, ci);

        JPanel content = widget.getContentPane();
        JPanel row = (JPanel) content.getComponent(0);
        JComboBox comboBox = null;
        for (Component c : row.getComponents()) {
            if (c instanceof JComboBox) {
                comboBox = (JComboBox) c;
                break;
            }
        }

        assertNotNull(comboBox);
        assertEquals(Color.RED, comboBox.getBackground());
        assertEquals(Color.WHITE, comboBox.getForeground());
    }

    @Test
    public void testGridIndicatorsHaveFixedWidth() {
        IndicatorModel indicator = new IndicatorModel("{value}",
            "A very long disabled indicator label", "A very long enabled indicator label",
            "white", "black", "green", "black");
        IndicatorPanel panel = new IndicatorPanel(Collections.singletonList(indicator), mock(IniFileModel.class), 2);

        assertEquals(IndicatorPanel.GRID_INDICATOR_WIDTH,
            panel.getPanel().getComponent(0).getPreferredSize().width);
    }

    @Test
    public void testLongComboIsWidthLimited() {
        String longOption = "B18 VVT2 or Idle or Low Side output 2 or injector 8 with flyback protection";
        EnumIniField field = createEnumField(longOption, "NONE");
        ConfigurationImage image = new ConfigurationImage(new byte[1]);
        JPanel row = CalibrationFieldFactory.createFieldRow(
            new DialogModel.Field("test", "Output"), field, image, image.clone());

        JComboBox<?> combo = null;
        for (Component component : row.getComponents()) {
            if (component instanceof JComboBox) {
                combo = (JComboBox<?>) component;
                break;
            }
        }
        assertNotNull(combo);
        assertEquals(CalibrationFieldFactory.MAX_COMBO_WIDTH, combo.getPreferredSize().width);
        assertEquals(longOption, combo.getToolTipText());
    }

    @Test
    public void testPinoutButton() {
        EnumIniField field = createEnumField("B16 Low Side output 4", "NONE");
        ConfigurationImage image = new ConfigurationImage(new byte[1]);
        AtomicReference<String> selectedPin = new AtomicReference<>();
        JPanel row = CalibrationFieldFactory.createFieldRow(
            new DialogModel.Field("injectionPins1", "Injection Output 1"),
            field, image, image.clone(), null, selectedPin::set);

        JButton button = getButtonFromRow(row);
        assertNotNull(button);
        assertEquals("W", button.getText());
        assertEquals("Wiring/Pinout", button.getToolTipText());
        assertTrue(button.isEnabled());
        button.doClick();
        assertEquals("B16 Low Side output 4", selectedPin.get());

        JComboBox<?> combo = getComboBoxFromRow(row);
        assertNotNull(combo);
        combo.setSelectedItem("NONE");
        assertFalse(button.isEnabled());
        button.setEnabled(true);
        assertFalse(button.isEnabled());
        selectedPin.set(null);
        button.doClick();
        assertNull(selectedPin.get());

        combo.setSelectedItem("B16 Low Side output 4");
        assertTrue(button.isEnabled());

        JPanel nonPinRow = CalibrationFieldFactory.createFieldRow(
            new DialogModel.Field("algorithm", "Algorithm"),
            field, image, image.clone(), null, selectedPin::set);
        assertNull(getButtonFromRow(nonPinRow));
    }

    @Test
    public void testLongFieldLabelWrapsToTwoRows() {
        String text = "Use absolute fuel pressure for dead time calculation";
        com.opensr5.ini.field.StringIniField field =
            new com.opensr5.ini.field.StringIniField("test", 0, 10);
        ConfigurationImage image = new ConfigurationImage(new byte[10]);
        JPanel row = CalibrationFieldFactory.createFieldRow(
            new DialogModel.Field("test", text), field, image, image.clone(), null, null, 200);

        JLabel label = getLabelFromRow(row);
        assertNotNull(label);
        assertTrue(label.getText().startsWith("<html>"));
        assertEquals(text, label.getToolTipText());
        int lineHeight = label.getFontMetrics(label.getFont()).getHeight();
        assertTrue(label.getPreferredSize().height > lineHeight);
        assertTrue(label.getPreferredSize().height <= lineHeight * 2);
    }

    @Test
    public void testFieldEditorsShareColumn() {
        IniFileModel iniFileModel = mock(IniFileModel.class);
        when(iniFileModel.getCurves()).thenReturn(Collections.emptyMap());

        com.opensr5.ini.field.StringIniField shortField =
            new com.opensr5.ini.field.StringIniField("short", 0, 10);
        com.opensr5.ini.field.StringIniField longField =
            new com.opensr5.ini.field.StringIniField("long", 10, 17);
        when(iniFileModel.findIniField("short")).thenReturn(java.util.Optional.of(shortField));
        when(iniFileModel.findIniField("long")).thenReturn(java.util.Optional.of(longField));

        DialogModel nestedDialog = new DialogModel("nested", "Nested",
            Collections.singletonList(new DialogModel.Field("long", "A much longer field label")),
            Collections.emptyList());
        Map<String, DialogModel> dialogs = new HashMap<>();
        dialogs.put("nested", nestedDialog);
        when(iniFileModel.getDialogs()).thenReturn(dialogs);

        DialogModel mainDialog = new DialogModel("main", "Main",
            Collections.singletonList(new DialogModel.Field("short", "Mode")),
            Collections.emptyList(),
            Collections.singletonList(new PanelModel("nested", "north", null, null)), null);

        CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
        widget.update(mainDialog, iniFileModel, new ConfigurationImage(new byte[27]));

        JPanel content = widget.getContentPane();
        JPanel firstRow = (JPanel) content.getComponent(0);
        JPanel nestedPanel = (JPanel) content.getComponent(1);
        JPanel secondRow = (JPanel) nestedPanel.getComponent(0);
        layoutTree(content);

        JTextField firstEditor = getTextFieldFromRow(firstRow);
        JTextField secondEditor = getTextFieldFromRow(secondRow);
        assertNotNull(firstEditor);
        assertNotNull(secondEditor);
        assertEquals(10, firstEditor.getColumns());
        assertEquals(17, secondEditor.getColumns());
        assertEquals(
            SwingUtilities.convertPoint(firstRow, firstEditor.getLocation(), content).x,
            SwingUtilities.convertPoint(secondRow, secondEditor.getLocation(), content).x);
    }

    private static void layoutTree(Container container) {
        container.setSize(container.getPreferredSize());
        container.doLayout();
        for (Component component : container.getComponents()) {
            if (component instanceof Container) {
                layoutTree((Container) component);
            }
        }
    }

    private static JTextField getTextFieldFromRow(JPanel row) {
        for (Component component : row.getComponents()) {
            if (component instanceof JTextField) {
                return (JTextField) component;
            }
        }
        return null;
    }

    private static JComboBox<?> getComboBoxFromRow(JPanel row) {
        for (Component component : row.getComponents()) {
            if (component instanceof JComboBox) {
                return (JComboBox<?>) component;
            }
        }
        return null;
    }

    private static JButton getButtonFromRow(JPanel row) {
        for (Component component : row.getComponents()) {
            if (component instanceof JButton) {
                return (JButton) component;
            }
        }
        return null;
    }

    @Test
    public void testTextOnlyFieldBackgroundColor() {
        IniFileModel iniFileModel = mock(IniFileModel.class);
        when(iniFileModel.getCurves()).thenReturn(Collections.emptyMap());

        List<DialogModel.Field> fields = new ArrayList<>();
        fields.add(new DialogModel.Field("key1", "!Red Label"));
        fields.add(new DialogModel.Field("key2", "#Blue Label"));
        fields.add(new DialogModel.Field("key3", "Normal Label"));

        DialogModel mainDialog = new DialogModel("main", "Main", fields, Collections.emptyList());

        CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
        widget.update(mainDialog, iniFileModel, null);

        JPanel content = widget.getContentPane();
        assertEquals(3, content.getComponentCount());

        JLabel redLabel = getLabelFromRow((JPanel) content.getComponent(0));
        assertEquals("!Red Label", redLabel.getText());
        assertEquals(Color.RED, redLabel.getBackground());
        assertEquals(Color.WHITE, redLabel.getForeground());
        assertTrue(redLabel.isOpaque());

        JLabel blueLabel = getLabelFromRow((JPanel) content.getComponent(1));
        assertEquals("#Blue Label", blueLabel.getText());
        assertEquals(Color.BLUE, blueLabel.getBackground());
        assertEquals(Color.WHITE, blueLabel.getForeground());
        assertTrue(blueLabel.isOpaque());

        JLabel normalLabel = getLabelFromRow((JPanel) content.getComponent(2));
        assertEquals("Normal Label", normalLabel.getText());
        assertNotEquals(Color.RED, normalLabel.getBackground());
        assertNotEquals(Color.BLUE, normalLabel.getBackground());
    }

    private JLabel getLabelFromRow(JPanel row) {
        for (Component c : row.getComponents()) {
            if (c instanceof JLabel) return (JLabel) c;
        }
        return null;
    }

    @Test
    public void testDecodeIniCommandBytesTypicalPayload() {
        // "Z\x00\x13\x00\x01" → [0x5A, 0x00, 0x13, 0x00, 0x01]
        byte[] result = CalibrationDialogWidget.decodeIniCommandBytes("Z\\x00\\x13\\x00\\x01");
        assertArrayEquals(new byte[]{0x5A, 0x00, 0x13, 0x00, 0x01}, result);
    }

    @Test
    public void testDecodeIniCommandBytesAllEscapes() {
        // Payload with all-escape content
        byte[] result = CalibrationDialogWidget.decodeIniCommandBytes("\\x00\\xFF\\x1e\\x22");
        assertArrayEquals(new byte[]{0x00, (byte) 0xFF, 0x1e, 0x22}, result);
    }

    @Test
    public void testDecodeIniCommandBytesOnlyLiteral() {
        // No escape sequences, just ASCII
        byte[] result = CalibrationDialogWidget.decodeIniCommandBytes("ZAB");
        assertArrayEquals(new byte[]{'Z', 'A', 'B'}, result);
    }

    @Test
    public void testDecodeIniCommandBytesEmpty() {
        byte[] result = CalibrationDialogWidget.decodeIniCommandBytes("");
        assertArrayEquals(new byte[0], result);
    }

    @Test
    public void testLinkLabel() {
        IniFileModel iniFileModel = mock(IniFileModel.class);
        when(iniFileModel.getCurves()).thenReturn(Collections.emptyMap());

        List<DialogModel.Field> fields = new ArrayList<>();
        fields.add(new DialogModel.Field("key1", "!<html><a href=https://rusefi.com/s/vvt>https://rusefi.com/s/vvt</a></html>"));

        DialogModel mainDialog = new DialogModel("main", "Main", fields, Collections.emptyList());

        CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
        widget.update(mainDialog, iniFileModel, null);

        JPanel content = widget.getContentPane();
        assertEquals(1, content.getComponentCount());

        JLabel linkLabel = getLabelFromRow((JPanel) content.getComponent(0));
        assertEquals("https://rusefi.com/s/vvt", linkLabel.getText());
        assertEquals(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR), linkLabel.getCursor());
        assertEquals(1, linkLabel.getMouseListeners().length);
    }
}
