package com.rusefi.ui.widgets;

import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeCellRenderer;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;
import java.awt.*;
import java.io.FileNotFoundException;

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
        DefaultMutableTreeNode setupNode = null;
        int setupRow = -1;
        for (int i = 0; i < root.getChildCount(); i++) {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode) root.getChildAt(i);
            if (node.getUserObject().equals("Setup")) {
                setupNode = node;
                setupRow = i; // This is NOT the row in the tree, but let's find it properly
                break;
            }
        }
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
        boolean foundFuel2 = false;
        for (int i = 0; i < filteredRoot.getChildCount(); i++) {
            if (((DefaultMutableTreeNode) filteredRoot.getChildAt(i)).getUserObject().equals("Fuel")) {
                foundFuel2 = true;
                break;
            }
        }
        assertTrue(foundFuel2, "Should find 'Fuel' in second search");

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

    private DefaultMutableTreeNode findNode(DefaultMutableTreeNode parent, String name) {
        for (int i = 0; i < parent.getChildCount(); i++) {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode) parent.getChildAt(i);
            if (node.getUserObject().equals(name)) {
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
