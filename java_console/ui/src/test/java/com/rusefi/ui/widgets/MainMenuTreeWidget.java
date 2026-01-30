package com.rusefi.ui.widgets;

import com.opensr5.ini.GroupMenuModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.MenuItem;
import com.opensr5.ini.MenuModel;
import com.opensr5.ini.SubMenuModel;
import com.rusefi.core.ui.AutoupdateUtil;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
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
        Font font = tree.getFont();
        tree.setFont(new Font(font.getName(), font.getStyle(), font.getSize() * 2));
        tree.setRootVisible(false);

        ImageIcon setupIcon = AutoupdateUtil.loadIcon("setup48.png");
        ImageIcon fuelIcon = AutoupdateUtil.loadIcon("fuel48.png");
        ImageIcon ignitionIcon = AutoupdateUtil.loadIcon("ignition48.png");
        ImageIcon idleIcon = AutoupdateUtil.loadIcon("idle48.png");
        ImageIcon crankingIcon = AutoupdateUtil.loadIcon("cranking48.png");

        tree.setCellRenderer(new DefaultTreeCellRenderer() {
            @Override
            public Component getTreeCellRendererComponent(JTree tree, Object value, boolean selected, boolean expanded, boolean leaf, int row, boolean hasFocus) {
                super.getTreeCellRendererComponent(tree, value, selected, expanded, leaf, row, hasFocus);
                if (value instanceof DefaultMutableTreeNode) {
                    DefaultMutableTreeNode node = (DefaultMutableTreeNode) value;
                    Object userObject = node.getUserObject();
                    if ("Setup".equals(userObject)) {
                        setIcon(setupIcon);
                    } else if ("Fuel".equals(userObject)) {
                        setIcon(fuelIcon);
                    } else if ("Ignition".equals(userObject)) {
                        setIcon(ignitionIcon);
                    } else if ("Idle".equals(userObject)) {
                        setIcon(idleIcon);
                    } else if ("Cranking".equals(userObject)) {
                        setIcon(crankingIcon);
                    }
                }
                return this;
            }
        });

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
