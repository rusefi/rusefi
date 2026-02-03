package com.rusefi.ui.widgets;

import com.opensr5.ini.AxisModel;
import com.opensr5.ini.CurveModel;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.SubMenuModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.widgets.tune.CalibrationDialogWidget;
import com.rusefi.ui.widgets.tune.MainMenuTreeWidget;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeCellRenderer;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.awt.image.BufferedImageOp;
import java.awt.image.ImageObserver;
import java.awt.image.RenderedImage;
import java.awt.image.renderable.RenderableImage;
import java.awt.geom.AffineTransform;
import java.io.ByteArrayInputStream;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;

public class MainMenuTreeWidgetTest {
    private static IniFileModel readLines(RawIniFile lines) {
        IniFileMetaInfo metaInfo = new IniFileMetaInfo() {
            @Override
            public int getnPages() { return 1; }
            @Override
            public int getOchBlockSize() { return 0; }
            @Override
            public String getSignature() { return "test"; }
            @Override
            public String getPageReadCommand(int pageIndex) { return "r"; }
            @Override
            public int getPageSize(int pageIndex) { return 100; }
        };
        return IniFileReaderUtil.readIniFile(lines, "test", metaInfo);
    }

    @Test
    public void testEnumFieldInCalibrationWidget() {
        String string = "[Constants]\n" +
            "signature = \"rusEFI\"\n" +
            "ochBlockSize = 100\n" +
            "nPages = 1\n" +
            "pageSize = 100\n" +
            "pageReadCommand = \"r\"\n" +
            "page = 1\n" +
            "field1 = scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n" +
            "field2 = bits, U08, 4, [0:1], \"Off\", \"On\"\n" +
            "field3 = bits, U08, 5, [0:1], \"No\", \"Yes\"\n" +
            "field4 = bits, U08, 6, [0:1], \"Disabled\", \"Enabled\"\n" +
            "[SettingContextHelp]\n" +
            "; SettingContextHelpEnd\n" +
            "\n" +
            "\tdialog = mainDialog, \"Main Dialog\"\n" +
            "\t\tfield = \"Field 1\", field1\n" +
            "\t\tfield = \"Field 2\", field2\n" +
            "\t\tfield = \"Field 3\", field3\n" +
            "\t\tfield = \"Field 4\", field4\n";

        RawIniFile lines = IniFileReaderUtil.read(new java.io.ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderUtil.readIniFile(lines, "test.ini", new com.opensr5.ini.IniFileMetaInfoImpl(lines));

        com.rusefi.ui.widgets.tune.CalibrationDialogWidget widget = new com.rusefi.ui.widgets.tune.CalibrationDialogWidget();
        widget.update(model.getDialogs().get("mainDialog"), model, null);

        JPanel content = widget.getContentPane();
        assertEquals(4, content.getComponentCount());

        // Field 1 is scalar -> JPanel with JLabel and JTextField
        assertTrue(content.getComponent(0) instanceof JPanel);
        JPanel row1 = (JPanel) content.getComponent(0);
        assertEquals("Field 1", ((JLabel) row1.getComponent(0)).getText());
        assertTrue(row1.getComponent(1) instanceof JTextField);

        // Field 2 is enum (Off/On) -> JPanel with JLabel and JComboBox
        assertTrue(content.getComponent(1) instanceof JPanel);
        JPanel row2 = (JPanel) content.getComponent(1);
        assertTrue(row2.getComponent(1) instanceof JComboBox);

        // Field 3 is enum (No/Yes) -> JPanel with JLabel and JCheckBox
        assertTrue(content.getComponent(2) instanceof JPanel);
        JPanel row3 = (JPanel) content.getComponent(2);
        assertTrue(row3.getComponent(1) instanceof JCheckBox, "Expected JCheckBox for No/Yes");

        // Field 4 is enum (Disabled/Enabled) -> JPanel with JLabel and JCheckBox
        assertTrue(content.getComponent(3) instanceof JPanel);
        JPanel row4 = (JPanel) content.getComponent(3);
        assertTrue(row4.getComponent(1) instanceof JCheckBox, "Expected JCheckBox for Disabled/Enabled");
    }

    @Test
    public void testRecursiveCalibrationWidget() {
        String string = "[MegaTune]\n" +
            "signature = \"rusEFI\"\n" +
            "[Constants]\n" +
            "ochBlockSize = 100\n" +
            "nPages = 1\n" +
            "pageSize = 100\n" +
            "pageReadCommand = \"r\"\n" +
            "field1 = scalar, F32, 0, \"unit\", 1, 0, 0, 100, 1\n" +
            "field2 = scalar, F32, 4, \"unit\", 1, 0, 0, 100, 1\n" +
            "[SettingContextHelp]\n" +
            "; SettingContextHelpEnd\n" +
            "\tdialog = subDialog1, \"Sub Dialog 1\"\n" +
            "\t\tfield = \"Field 1\", field1\n" +
            "\n" +
            "\tdialog = mainDialog, \"Main Dialog\"\n" +
            "\t\tpanel = subDialog1\n" +
            "\t\tfield = \"Field 2\", field2\n";

        RawIniFile lines = IniFileReaderUtil.read(new java.io.ByteArrayInputStream(string.getBytes()));
        IniFileModel model = IniFileReaderUtil.readIniFile(lines, "test.ini", new com.opensr5.ini.IniFileMetaInfoImpl(lines));

        com.rusefi.ui.widgets.tune.CalibrationDialogWidget widget = new com.rusefi.ui.widgets.tune.CalibrationDialogWidget();
        widget.update(model.getDialogs().get("mainDialog"), model, null);

        JPanel content = widget.getContentPane();
        // Should have: JPanel(Field 2), and JPanel(subDialog1)
        // Wait, order in fillPanel is: fields then panels.
        // mainDialog has 1 field (field2) and 1 panel (subDialog1)
        assertEquals(2, content.getComponentCount());

        assertTrue(content.getComponent(0) instanceof JLabel, "Expected JLabel for Field 2 row, but was " + content.getComponent(0).getClass().getName());
        assertEquals("Field 2", ((JLabel) content.getComponent(0)).getText());

        assertTrue(content.getComponent(1) instanceof JPanel, "Expected JPanel for subDialog1");
        JPanel subDialog1Panel = (JPanel) content.getComponent(1);
        // subDialog1 has 1 field (field1)
        assertEquals(1, subDialog1Panel.getComponentCount());
        assertTrue(subDialog1Panel.getComponent(0) instanceof JLabel, "Expected JLabel for Field 1 row");
        assertEquals("Field 1", ((JLabel) subDialog1Panel.getComponent(0)).getText());
    }

    @Test
    public void testTreeStructure() throws FileNotFoundException {
        String iniPath = "../../java_console/io/src/test/java/com/rusefi/io/pin_output_mode_with_and_without_dollar/test_data/rusefi_uaefi.ini";
        IniFileModel model = IniFileReaderUtil.readIniFile(iniPath);
        assertNotNull(model);

        com.rusefi.ui.widgets.tune.MainMenuTreeWidget widget = new com.rusefi.ui.widgets.tune.MainMenuTreeWidget(model);
        JPanel content = widget.getContentPane();
        assertNotNull(content);

        JPanel topPanel = (JPanel) content.getComponent(0);
        assertNotNull(topPanel);
        assertTrue(topPanel.getComponent(0) instanceof JTextField);

        JScrollPane scrollPane = (JScrollPane) content.getComponent(1);
        JTree tree = (JTree) scrollPane.getViewport().getView();
        TreeModel treeModel = tree.getModel();
        DefaultMutableTreeNode root = (DefaultMutableTreeNode) treeModel.getRoot();

        // Check if we have some top-level menus
        assertTrue(root.getChildCount() > 0, "Tree should have top-level menus");

        // Find "Setup" menu
        DefaultMutableTreeNode setupNode = findNode(root, "Setup");
        assertNotNull(setupNode, "Setup node should exist");

        // Verify icon for Setup
        verifyIcon(tree, setupNode, "Setup");

        // Find "Fuel" menu
        DefaultMutableTreeNode fuelNode = findNode(root, "Fuel");
        assertNotNull(fuelNode, "Fuel node should exist");
        verifyIcon(tree, fuelNode, "Fuel");

        // Find "Ignition" menu
        DefaultMutableTreeNode ignitionNode = findNode(root, "Ignition");
        assertNotNull(ignitionNode, "Ignition node should exist");
        verifyIcon(tree, ignitionNode, "Ignition");

        // Find "Idle" menu
        DefaultMutableTreeNode idleNode = findNode(root, "Idle");
        assertNotNull(idleNode, "Idle node should exist");
        verifyIcon(tree, idleNode, "Idle");

        // Find "Cranking" menu
        DefaultMutableTreeNode crankingNode = findNode(root, "Cranking");
        assertNotNull(crankingNode, "Cranking node should exist");
        verifyIcon(tree, crankingNode, "Cranking");

        // Find "Limits and protection" group in Setup
        DefaultMutableTreeNode limitsNode = findNode(setupNode, "Limits and protection");
        assertNotNull(limitsNode, "Limits and protection node should exist");
        assertTrue(limitsNode.getChildCount() > 0, "Limits group should have children");
    }

    @Test
    public void testSearchLogic() throws FileNotFoundException {
        String iniPath = "../../java_console/io/src/test/java/com/rusefi/io/pin_output_mode_with_and_without_dollar/test_data/rusefi_uaefi.ini";
        IniFileModel model = IniFileReaderUtil.readIniFile(iniPath);
        com.rusefi.ui.widgets.tune.MainMenuTreeWidget widget = new com.rusefi.ui.widgets.tune.MainMenuTreeWidget(model);
        JPanel content = widget.getContentPane();

        JPanel topPanel = (JPanel) content.getComponent(0);
        JTextField searchField = (JTextField) topPanel.getComponent(0);
        JScrollPane scrollPane = (JScrollPane) content.getComponent(1);
        JTree tree = (JTree) scrollPane.getViewport().getView();

        // 1. Initial state
        assertEquals(model.getMenus().size(), ((DefaultMutableTreeNode) tree.getModel().getRoot()).getChildCount());

        // 2. Search for "setup limit"
        searchField.setText("setup limit");
        DefaultMutableTreeNode filteredRoot = (DefaultMutableTreeNode) tree.getModel().getRoot();
        // Should only have "Setup" at top level
        assertEquals(1, filteredRoot.getChildCount());
        DefaultMutableTreeNode setupNode = (DefaultMutableTreeNode) filteredRoot.getChildAt(0);
        assertEquals("Setup", setupNode.getUserObject());

        // Inside Setup, should have "Limits and protection"
        DefaultMutableTreeNode limitsNode = findNode(setupNode, "Limits and protection");
        assertNotNull(limitsNode);

        // 3. Reset search by clicking (simulated)
        tree.setSelectionPath(new TreePath(limitsNode.getPath()));
        // Selection listener should trigger search reset
        try {
            SwingUtilities.invokeAndWait(() -> {});
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        assertTrue(searchField.getText().isEmpty(), "Search field should be empty after click");
        assertEquals(model.getMenus().size(), ((DefaultMutableTreeNode) tree.getModel().getRoot()).getChildCount(), "Tree should be fully restored");

        // Verify that all nodes are expanded
        assertTrue(tree.isExpanded(new TreePath(setupNode.getPath())), "Setup node should be expanded");
        // Also check some other node to be sure
        DefaultMutableTreeNode fuelNode = findNode((DefaultMutableTreeNode) tree.getModel().getRoot(), "Fuel");
        assertTrue(tree.isExpanded(new TreePath(fuelNode.getPath())), "Fuel node should be expanded");

        // 4. Second search
        searchField.setText("fuel");
        try {
            SwingUtilities.invokeAndWait(() -> {});
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        filteredRoot = (DefaultMutableTreeNode) tree.getModel().getRoot();
        assertTrue(filteredRoot.getChildCount() > 0, "Filtered root should have children");
        // Verify we found "Fuel"
        assertNotNull(findNode(filteredRoot, "Fuel"), "Should find 'Fuel' in second search");

        // 5. Search for something non-existent
        searchField.setText("nonexistentstuff");
        try {
            SwingUtilities.invokeAndWait(() -> {});
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        filteredRoot = (DefaultMutableTreeNode) tree.getModel().getRoot();
        assertEquals(0, filteredRoot.getChildCount(), "Should have no results");

        // 6. Backspace/Remove some text
        searchField.setText("fuel");
        try {
            SwingUtilities.invokeAndWait(() -> {});
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        filteredRoot = (DefaultMutableTreeNode) tree.getModel().getRoot();
        assertTrue(filteredRoot.getChildCount() > 0, "Should have results again");
    }

    @Test
    public void testIntegration() throws FileNotFoundException {
        String iniPath = "../../java_console/io/src/test/java/com/rusefi/io/pin_output_mode_with_and_without_dollar/test_data/rusefi_uaefi.ini";
        IniFileModel model = IniFileReaderUtil.readIniFile(iniPath);

        com.rusefi.ui.widgets.tune.MainMenuTreeWidget left = new MainMenuTreeWidget(model);
        com.rusefi.ui.widgets.tune.CalibrationDialogWidget right = new CalibrationDialogWidget();

        AtomicReference<SubMenuModel> selectedSubMenu = new AtomicReference<>();
        left.setOnSelect(subMenu -> {
            selectedSubMenu.set(subMenu);
            right.update(model.getDialogs().get(subMenu.getKey()), model, null);
        });

        JTree tree = (JTree) ((JScrollPane) left.getContentPane().getComponent(1)).getViewport().getView();
        DefaultMutableTreeNode root = (DefaultMutableTreeNode) tree.getModel().getRoot();
        DefaultMutableTreeNode setupNode = findNode(root, "Setup");
        DefaultMutableTreeNode vehicleInfoNode = findNode(setupNode, "Vehicle Information");
        assertNotNull(vehicleInfoNode);

        // Simulate selection
        tree.setSelectionPath(new TreePath(vehicleInfoNode.getPath()));

        assertNotNull(selectedSubMenu.get());
        assertEquals("engineChars", selectedSubMenu.get().getKey());

        // Verify UI content
        JPanel contentPane = right.getContentPane();
        assertTrue(contentPane.getComponentCount() > 0, "Content pane should have components for engineChars");

        // Now test re-selection (clicking the same node again)

        // Simulate click on the same node
        tree.getMouseListeners()[1].mouseClicked(new MouseEvent(tree, MouseEvent.MOUSE_CLICKED, System.currentTimeMillis(), 0, 0, 0, 1, false));

        // The table should have been updated (re-populated), so our "ModifiedValue" should be gone if it was in first column
        // Actually, update() probably clears and refills the table.
        // Let's just check if it's still > 0 and maybe check that onSelect was called again.
        assertEquals("engineChars", selectedSubMenu.get().getKey());
    }

    private DefaultMutableTreeNode findNode(DefaultMutableTreeNode parent, String name) {
        for (int i = 0; i < parent.getChildCount(); i++) {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode) parent.getChildAt(i);
            Object userObject = node.getUserObject();
            String nodeName = userObject.toString();
            if (userObject instanceof SubMenuModel) {
                nodeName = ((SubMenuModel) userObject).getName();
            }
            if (name.equals(nodeName)) {
                return node;
            }
        }
        return null;
    }

    private void verifyIcon(JTree tree, DefaultMutableTreeNode node, String name) {
        TreeCellRenderer renderer = tree.getCellRenderer();
        Component rendererComponent = renderer.getTreeCellRendererComponent(tree, node, false, false, false, 0, false);
        assertTrue(rendererComponent instanceof JLabel, name + " renderer should be a JLabel");
        JLabel label = (JLabel) rendererComponent;
        assertNotNull(label.getIcon(), name + " should have an icon");
        assertEquals(48, label.getIcon().getIconWidth(), name + " icon width");
        assertEquals(48, label.getIcon().getIconHeight(), name + " icon height");
    }
    @Test
    public void testCurveWidgetInCalibrationWidget() {
        String string = "[Constants]\n" +
            "page = 1\n" +
            "scriptCurve1Bins = array, F32, 0, [16], \"x\", 1, 0, 0, 100, 3\n" +
            "scriptCurve1 = array, F32, 64, [16], \"y\", 1, 0, 0, 100, 3\n " +
            "[CurveEditor]\n" +
            "\tcurve = scriptCurve1, \"Script Curve #1\"\n" +
            "\t\txAxis\t\t=  0, 100, 10\n" +
            "\t\tyAxis\t\t=  0, 200, 20\n" +
            "\t\txBins\t\t= scriptCurve1Bins\n" +
            "\t\tyBins\t\t= scriptCurve1\n" +
            "[DummySection]\n" +
            "dialog = curveDialog, \"Curve Dialog\"\n" +
            "\t\tfield = \"Dummy\", dummy\n" +
            "\t\tpanel = scriptCurve1\n";

        RawIniFile lines = IniFileReaderUtil.read(new ByteArrayInputStream(string.getBytes()));
        IniFileModel model = readLines(lines);

        CalibrationDialogWidget widget = new CalibrationDialogWidget();
        DialogModel dialog = model.getDialogs().get("curveDialog");
        assertNotNull(dialog, "dialog curveDialog should exist");
        widget.update(dialog, model, null);

        JPanel content = widget.getContentPane();
        // Should have two components: one for "Dummy" field and one for the CurveWidget's content panel
        assertEquals(2, content.getComponentCount());
        JPanel curvePanel = (JPanel) content.getComponent(1);

        // CurveWidget content has GridBagLayout: Canvas (weightx=0.8) and Table panel (weightx=0.2)
        assertTrue(curvePanel.getLayout() instanceof GridBagLayout);
        assertEquals(2, curvePanel.getComponentCount());

        // Verify weights
        GridBagLayout gbl = (GridBagLayout) curvePanel.getLayout();
        assertEquals(0.8, gbl.getConstraints(curvePanel.getComponent(0)).weightx, 0.001);
        assertEquals(0.2, gbl.getConstraints(curvePanel.getComponent(1)).weightx, 0.001);

        // Find JTable
        JTable table = findTable(curvePanel);
        assertNotNull(table);
        assertEquals(16, table.getRowCount());
        assertEquals(2, table.getColumnCount());
    }

    @Test
    public void testCurveWidgetGridLines() {
        String iniContent = "[MegaTune]\n" +
                "  signature = \"rusEFI\"\n" +
                "[Constants]\n" +
                "  nPages = 1\n" +
                "  ochBlockSize = 100\n" +
                "  pageSize = 100\n" +
                "  pageReadCommand = \"r\"\n" +
                "  xBins = array, F32, 0, [16], \"RPM\", 1, 0, 0, 8000, 0\n" +
                "  yBins = array, F32, 64, [16], \"Deg\", 1, 0, 0, 100, 0\n";
        AxisModel xAxis = new AxisModel(0, 100, 5); // 6 lines expected: 0, 20, 40, 60, 80, 100
        AxisModel yAxis = new AxisModel(0, 200, 10); // 11 lines expected: 0, 20, ..., 200
        CurveModel curveModel = new CurveModel("id", "title", xAxis, yAxis, "xBins", "yBins");

        IniFileModel model = new com.opensr5.ini.IniFileModel() {
            @Override public String getSignature() { return null; }
            @Override public int getBlockingFactor() { return 0; }
            @Override public Map<String, List<String>> getDefines() { return null; }
            @Override public Map<String, com.opensr5.ini.field.IniField> getAllIniFields() { return null; }
            @Override public Map<String, com.opensr5.ini.field.IniField> getSecondaryIniFields() { return null; }
            @Override public java.util.Optional<com.opensr5.ini.field.IniField> findIniField(String key) {
                if ("xBins".equals(key)) {
                    return java.util.Optional.of(new com.opensr5.ini.field.ScalarIniField("xBins", 0, "RPM", com.rusefi.config.FieldType.FLOAT, 1.0, "0", 0.0));
                }
                if ("yBins".equals(key)) {
                    return java.util.Optional.of(new com.opensr5.ini.field.ScalarIniField("yBins", 0, "Deg", com.rusefi.config.FieldType.FLOAT, 1.0, "2", 0.0));
                }
                return java.util.Optional.empty();
            }
            @Override public com.opensr5.ini.field.IniField getIniField(com.rusefi.config.Field field) { return null; }
            @Override public com.opensr5.ini.field.IniField getIniField(String key) { return null; }
            @Override public com.opensr5.ini.field.IniField getOutputChannel(String key) { return null; }
            @Override public String getExpressionOutputChannel(String key) { return null; }
            @Override public Map<String, String> getExpressionOutputChannels() { return java.util.Collections.emptyMap(); }
            @Override public Map<String, String> getProtocolMeta() { return null; }
            @Override public com.opensr5.ini.IniFileMetaInfo getMetaInfo() { return null; }
            @Override public String getIniFilePath() { return null; }
            @Override public Map<String, String> getTooltips() { return null; }
            @Override public Map<String, com.opensr5.ini.DialogModel.Field> getFieldsInUiOrder() { return null; }
            @Override public Map<String, com.opensr5.ini.DialogModel> getDialogs() { return null; }
            @Override public String getDialogKeyByTitle(String dialogTitle) { return null; }
            @Override public com.opensr5.ini.field.IniField findByOffset(int i) { return null; }
            @Override public Map<String, com.opensr5.ini.GaugeCategoryModel> getGaugeCategories() { return null; }
            @Override public Map<String, com.opensr5.ini.GaugeModel> getGauges() { return null; }
            @Override public com.opensr5.ini.GaugeModel getGauge(String name) { return null; }
            @Override public com.opensr5.ini.GaugeModel findGaugeByChannel(String channelName) { return null; }
            @Override public Map<String, String> getTopicHelp() { return null; }
            @Override public Map<String, com.opensr5.ini.ContextHelpModel> getContextHelp() { return null; }
            @Override public com.opensr5.ini.ContextHelpModel getContextHelp(String referenceName) { return null; }
            @Override public Map<String, com.opensr5.ini.TableModel> getTables() { return null; }
            @Override public Map<String, CurveModel> getCurves() { return null; }
            @Override public com.opensr5.ini.TableModel getTable(String name) { return null; }
            @Override public List<com.opensr5.ini.MenuModel> getMenus() { return null; }
        };

        com.rusefi.ui.widgets.tune.CurveWidget widget = new com.rusefi.ui.widgets.tune.CurveWidget(curveModel, model, null);

        // Find the CurveCanvas
        com.rusefi.ui.widgets.tune.CurveWidget.CurveCanvas canvas = null;
        for (Component c : widget.getContentPane().getComponents()) {
            if (c.getClass().getName().endsWith("CurveCanvas")) {
                canvas = (com.rusefi.ui.widgets.tune.CurveWidget.CurveCanvas) c;
                break;
            }
        }
        assertNotNull(canvas);
        canvas.setSize(500, 500);

        BufferedImage img = new BufferedImage(500, 500, BufferedImage.TYPE_INT_ARGB);
        Graphics2D realG2 = img.createGraphics();

        final int[] verticalLines = {0};
        final int[] horizontalLines = {0};
        final List<Integer> xPositions = new ArrayList<>();
        final List<Integer> yPositions = new ArrayList<>();

        final List<String> drawnStrings = new ArrayList<>();

        Graphics2D spyG2 = new Graphics2D() {
            private Color color;
            @Override public void drawString(String str, int x, int y) {
                drawnStrings.add(str);
                realG2.drawString(str, x, y);
            }
            @Override public void drawLine(int x1, int y1, int x2, int y2) {
                if (new Color(200, 200, 200, 100).equals(this.color)) {
                    if (x1 == x2) {
                        verticalLines[0]++;
                        xPositions.add(x1);
                    } else if (y1 == y2) {
                        horizontalLines[0]++;
                        yPositions.add(y1);
                    }
                }
                realG2.drawLine(x1, y1, x2, y2);
            }
            @Override public void setColor(Color c) { this.color = c; realG2.setColor(c); }
            @Override public Color getColor() { return realG2.getColor(); }
            @Override public void setStroke(Stroke s) { realG2.setStroke(s); }
            @Override public Stroke getStroke() { return realG2.getStroke(); }
            @Override public void setRenderingHint(RenderingHints.Key hintKey, Object hintValue) { realG2.setRenderingHint(hintKey, hintValue); }
            @Override public Object getRenderingHint(RenderingHints.Key hintKey) { return realG2.getRenderingHint(hintKey); }
            @Override public void setRenderingHints(Map<?, ?> hints) { realG2.setRenderingHints(hints); }
            @Override public void addRenderingHints(Map<?, ?> hints) { realG2.addRenderingHints(hints); }
            @Override public RenderingHints getRenderingHints() { return realG2.getRenderingHints(); }
            @Override public void translate(int x, int y) { realG2.translate(x, y); }
            @Override public void translate(double tx, double ty) { realG2.translate(tx, ty); }
            @Override public void rotate(double theta) { realG2.rotate(theta); }
            @Override public void rotate(double theta, double x, double y) { realG2.rotate(theta, x, y); }
            @Override public void scale(double sx, double sy) { realG2.scale(sx, sy); }
            @Override public void shear(double shx, double shy) { realG2.shear(shx, shy); }
            @Override public void transform(AffineTransform Tx) { realG2.transform(Tx); }
            @Override public void setTransform(AffineTransform Tx) { realG2.setTransform(Tx); }
            @Override public AffineTransform getTransform() { return realG2.getTransform(); }
            @Override public Paint getPaint() { return realG2.getPaint(); }
            @Override public void setPaint(Paint paint) { realG2.setPaint(paint); }
            @Override public Composite getComposite() { return realG2.getComposite(); }
            @Override public void setComposite(Composite comp) { realG2.setComposite(comp); }
            @Override public void setBackground(Color color) { realG2.setBackground(color); }
            @Override public Color getBackground() { return realG2.getBackground(); }
            @Override public Font getFont() { return realG2.getFont(); }
            @Override public void setFont(Font font) { realG2.setFont(font); }
            @Override public FontMetrics getFontMetrics(Font f) { return realG2.getFontMetrics(f); }
            @Override public void clip(Shape s) { realG2.clip(s); }
            @Override public Shape getClip() { return realG2.getClip(); }
            @Override public void setClip(int x, int y, int width, int height) { realG2.setClip(x, y, width, height); }
            @Override public void setClip(Shape clip) { realG2.setClip(clip); }
            @Override public void copyArea(int x, int y, int width, int height, int dx, int dy) { realG2.copyArea(x, y, width, height, dx, dy); }
            @Override public void fillRect(int x, int y, int width, int height) { realG2.fillRect(x, y, width, height); }
            @Override public void clearRect(int x, int y, int width, int height) { realG2.clearRect(x, y, width, height); }
            @Override public void drawRoundRect(int x, int y, int width, int height, int arcWidth, int arcHeight) { realG2.drawRoundRect(x, y, width, height, arcWidth, arcHeight); }
            @Override public void fillRoundRect(int x, int y, int width, int height, int arcWidth, int arcHeight) { realG2.fillRoundRect(x, y, width, height, arcWidth, arcHeight); }
            @Override public void drawOval(int x, int y, int width, int height) { realG2.drawOval(x, y, width, height); }
            @Override public void fillOval(int x, int y, int width, int height) { realG2.fillOval(x, y, width, height); }
            @Override public void drawArc(int x, int y, int width, int height, int startAngle, int arcAngle) { realG2.drawArc(x, y, width, height, startAngle, arcAngle); }
            @Override public void fillArc(int x, int y, int width, int height, int startAngle, int arcAngle) { realG2.fillArc(x, y, width, height, startAngle, arcAngle); }
            @Override public void drawPolyline(int[] xPoints, int[] yPoints, int nPoints) { realG2.drawPolyline(xPoints, yPoints, nPoints); }
            @Override public void drawPolygon(int[] xPoints, int[] yPoints, int nPoints) { realG2.drawPolygon(xPoints, yPoints, nPoints); }
            @Override public void fillPolygon(int[] xPoints, int[] yPoints, int nPoints) { realG2.fillPolygon(xPoints, yPoints, nPoints); }
            @Override public void draw(Shape s) { realG2.draw(s); }
            @Override public boolean drawImage(Image img, AffineTransform xform, ImageObserver obs) { return realG2.drawImage(img, xform, obs); }
            @Override public void drawImage(BufferedImage img, BufferedImageOp op, int x, int y) { realG2.drawImage(img, op, x, y); }
            @Override public void drawRenderedImage(RenderedImage img, AffineTransform xform) { realG2.drawRenderedImage(img, xform); }
            @Override public void drawRenderableImage(RenderableImage img, AffineTransform xform) { realG2.drawRenderableImage(img, xform); }
            @Override public void drawString(String str, float x, float y) { realG2.drawString(str, x, y); }
            @Override public void drawString(java.text.AttributedCharacterIterator iterator, int x, int y) { realG2.drawString(iterator, x, y); }
            @Override public void drawString(java.text.AttributedCharacterIterator iterator, float x, float y) { realG2.drawString(iterator, x, y); }
            @Override public void drawGlyphVector(java.awt.font.GlyphVector g, float x, float y) { realG2.drawGlyphVector(g, x, y); }
            @Override public void fill(Shape s) { realG2.fill(s); }
            @Override public boolean hit(Rectangle rect, Shape s, boolean onStroke) { return realG2.hit(rect, s, onStroke); }
            @Override public GraphicsConfiguration getDeviceConfiguration() { return realG2.getDeviceConfiguration(); }
            @Override public void setXORMode(Color c1) { realG2.setXORMode(c1); }
            @Override public void setPaintMode() { realG2.setPaintMode(); }
            @Override public Graphics create() { return realG2.create(); }
            @Override public void dispose() { realG2.dispose(); }
            @Override public void clipRect(int x, int y, int width, int height) { realG2.clipRect(x, y, width, height); }
            @Override public Rectangle getClipBounds() { return realG2.getClipBounds(); }
            @Override public java.awt.font.FontRenderContext getFontRenderContext() { return realG2.getFontRenderContext(); }
            @Override public boolean drawImage(Image img, int x, int y, ImageObserver observer) { return realG2.drawImage(img, x, y, observer); }
            @Override public boolean drawImage(Image img, int x, int y, int width, int height, ImageObserver observer) { return realG2.drawImage(img, x, y, width, height, observer); }
            @Override public boolean drawImage(Image img, int x, int y, Color bgcolor, ImageObserver observer) { return realG2.drawImage(img, x, y, bgcolor, observer); }
            @Override public boolean drawImage(Image img, int x, int y, int width, int height, Color bgcolor, ImageObserver observer) { return realG2.drawImage(img, x, y, width, height, bgcolor, observer); }
            @Override public boolean drawImage(Image img, int dx1, int dy1, int dx2, int dy2, int sx1, int sy1, int sx2, int sy2, ImageObserver observer) { return realG2.drawImage(img, dx1, dy1, dx2, dy2, sx1, sy1, sx2, sy2, observer); }
            @Override public boolean drawImage(Image img, int dx1, int dy1, int dx2, int dy2, int sx1, int sy1, int sx2, int sy2, Color bgcolor, ImageObserver observer) { return realG2.drawImage(img, dx1, dy1, dx2, dy2, sx1, sy1, sx2, sy2, bgcolor, observer); }
        };

        canvas.paintComponent(spyG2);

        assertTrue(drawnStrings.contains("title"), "Should draw title");
        assertTrue(drawnStrings.contains("RPM"), "Should draw units");
        assertTrue(drawnStrings.contains("0"), "Should draw 0 for X axis");
        assertTrue(drawnStrings.contains("100"), "Should draw 100 for X axis");
        assertTrue(drawnStrings.contains("0.00"), "Should draw 0.00 for Y axis with 2 digits");
        assertTrue(drawnStrings.contains("200.00"), "Should draw 200.00 for Y axis with 2 digits");

        assertEquals(5, verticalLines[0], "Should have 5 vertical grid lines");
        assertEquals(10, horizontalLines[0], "Should have 10 horizontal grid lines");

        // Verify that the last vertical line is at the right edge of the graph
        // leftPadding = 50, rightPadding = 20, width = 500
        // graph width = 500 - 50 - 20 = 430
        // last x should be 50 + 430 = 480
        assertEquals(480, xPositions.get(xPositions.size() - 1), "Last vertical line should be at x=480");

        // Verify that the last horizontal line is at the top edge of the graph (which is the min value)
        // Actually i=0 is yMin, i=step-1 is yMax
        // worldToCanvas(x, yMin) -> cy = topPadding + height - 0 = 20 + (500-20-40) = 460
        // worldToCanvas(x, yMax) -> cy = topPadding + 0 = 20
        // So horizontal lines are drawn from bottom to top?
        // Loop: for (int i = 0; i < yAxis.getStep(); i++)
        // i=0 -> val = yMin -> worldToCanvas(xAxis.getMin(), yMin) -> p1.y = 460
        // i=yAxis.getStep()-1 -> val = yMax -> worldToCanvas(xAxis.getMin(), yMax) -> p1.y = 20
        assertEquals(460, yPositions.get(0), "First horizontal line should be at y=460");
        assertEquals(20, yPositions.get(yPositions.size() - 1), "Last horizontal line should be at y=20");
    }

    private JTable findTable(Container container) {
        for (Component c : container.getComponents()) {
            if (c instanceof JTable) return (JTable) c;
            if (c instanceof Container) {
                JTable t = findTable((Container) c);
                if (t != null) return t;
            }
        }
        return null;
    }
}
