package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.PanelModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

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

        CalibrationDialogWidget widget = new CalibrationDialogWidget();
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
        assertTrue(((BoxLayout) hPanel1.getLayout()).getAxis() == BoxLayout.X_AXIS, "Should be horizontal layout");

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

        CalibrationDialogWidget widget = new CalibrationDialogWidget();
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

        CalibrationDialogWidget widget = new CalibrationDialogWidget();
        widget.update(mainDialog, iniFileModel, null);

        JPanel content = widget.getContentPane();
        JPanel subPanel = (JPanel) content.getComponent(0);

        assertEquals("Sub Dialog UI Name", subPanel.getName());
        assertTrue(subPanel.getBorder() instanceof javax.swing.border.TitledBorder);
        assertEquals("Sub Dialog UI Name", ((javax.swing.border.TitledBorder) subPanel.getBorder()).getTitle());

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

        CalibrationDialogWidget widget = new CalibrationDialogWidget();
        ConfigurationImage ci = new ConfigurationImage(new byte[1000]);
        widget.update(mainDialog, iniFileModel, ci);

        JPanel content = widget.getContentPane();
        assertEquals(1, content.getComponentCount());
        JPanel tablePanel = (JPanel) content.getComponent(0);

        // TuningTableView content contains a JLabel and a JTable (via JTableHeader and JTable)
        // See TuningTableView constructor
        boolean foundLabel = false;
        for (Component c : tablePanel.getComponents()) {
            if (c instanceof JLabel && ((JLabel) c).getText().equals("Table Title")) {
                foundLabel = true;
            }
        }
        assertTrue(foundLabel, "Should find table title label");
    }
}
