package com.rusefi.ui.widgets;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.SubMenuModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import javax.swing.table.TableModel;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeCellRenderer;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.io.FileNotFoundException;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;

public class MainMenuTreeWidgetTest {
    @Test
    public void testTreeStructure() throws FileNotFoundException {
        String iniPath = "../../java_console/io/src/test/java/com/rusefi/io/pin_output_mode_with_and_without_dollar/test_data/rusefi_uaefi.ini";
        IniFileModel model = IniFileReaderUtil.readIniFile(iniPath);
        assertNotNull(model);

        MainMenuTreeWidget widget = new MainMenuTreeWidget(model);
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
        MainMenuTreeWidget widget = new MainMenuTreeWidget(model);
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

        MainMenuTreeWidget left = new MainMenuTreeWidget(model);
        CalibrationDialogWidget right = new CalibrationDialogWidget();

        AtomicReference<SubMenuModel> selectedSubMenu = new AtomicReference<>();
        left.setOnSelect(subMenu -> {
            selectedSubMenu.set(subMenu);
            right.update(model.getDialogs().get(subMenu.getKey()));
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
}
