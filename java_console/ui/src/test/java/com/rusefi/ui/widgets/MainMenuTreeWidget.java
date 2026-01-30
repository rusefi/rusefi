package com.rusefi.ui.widgets;

import com.opensr5.ini.GroupMenuModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.MenuItem;
import com.opensr5.ini.MenuModel;
import com.opensr5.ini.SubMenuModel;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import java.awt.*;

public class MainMenuTreeWidget {
    private final JPanel contentPane = new JPanel(new BorderLayout());

    public MainMenuTreeWidget(IniFileModel model) {
        DefaultMutableTreeNode root = new DefaultMutableTreeNode("Menus");

        for (MenuModel menu : model.getMenus()) {
            DefaultMutableTreeNode menuNode = new DefaultMutableTreeNode(menu.getName());
            root.add(menuNode);

            for (MenuItem item : menu.getItems()) {
                addMenuItem(menuNode, item);
            }
        }

        JTree tree = new JTree(root);
        tree.setRootVisible(false);
        contentPane.add(new JScrollPane(tree), BorderLayout.CENTER);
    }

    private void addMenuItem(DefaultMutableTreeNode parent, MenuItem item) {
        if (item instanceof GroupMenuModel) {
            GroupMenuModel group = (GroupMenuModel) item;
            DefaultMutableTreeNode groupNode = new DefaultMutableTreeNode(group.getName());
            parent.add(groupNode);
            for (MenuItem child : group.getItems()) {
                addMenuItem(groupNode, child);
            }
        } else if (item instanceof SubMenuModel) {
            SubMenuModel subMenu = (SubMenuModel) item;
            parent.add(new DefaultMutableTreeNode(subMenu.getName()));
        }
    }

    public JPanel getContentPane() {
        return contentPane;
    }
}
