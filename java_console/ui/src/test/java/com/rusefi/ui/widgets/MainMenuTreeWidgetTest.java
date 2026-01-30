package com.rusefi.ui.widgets;

import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeCellRenderer;
import javax.swing.tree.TreeModel;
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

        JScrollPane scrollPane = (JScrollPane) content.getComponent(0);
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

        // Find "Limits and protection" group in Setup
        DefaultMutableTreeNode limitsNode = findNode(setupNode, "Limits and protection");
        assertNotNull(limitsNode, "Limits and protection node should exist");
        assertTrue(limitsNode.getChildCount() > 0, "Limits group should have children");
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
