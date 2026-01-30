package com.rusefi.ui.widgets;

import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeModel;
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
        for (int i = 0; i < root.getChildCount(); i++) {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode) root.getChildAt(i);
            if (node.getUserObject().equals("Setup")) {
                setupNode = node;
                break;
            }
        }
        assertNotNull(setupNode, "Setup node should exist");

        // Find "Limits and protection" group in Setup
        DefaultMutableTreeNode limitsNode = null;
        for (int i = 0; i < setupNode.getChildCount(); i++) {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode) setupNode.getChildAt(i);
            if (node.getUserObject().equals("Limits and protection")) {
                limitsNode = node;
                break;
            }
        }
        assertNotNull(limitsNode, "Limits and protection node should exist");
        assertTrue(limitsNode.getChildCount() > 0, "Limits group should have children");
    }
}
