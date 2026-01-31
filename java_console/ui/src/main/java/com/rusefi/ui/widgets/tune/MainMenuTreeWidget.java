package com.rusefi.ui.widgets.tune;

import com.opensr5.ini.GroupMenuModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.MenuItem;
import com.opensr5.ini.MenuModel;
import com.opensr5.ini.SubMenuModel;
import com.rusefi.core.ui.AutoupdateUtil;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreePath;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Enumeration;
import java.util.function.Consumer;

import static com.rusefi.core.ui.AutoupdateUtil.trueLayoutAndRepaint;

public class MainMenuTreeWidget {
    private final JPanel contentPane = new JPanel(new BorderLayout());
    private final JTree tree;
    private final DefaultMutableTreeNode root = new DefaultMutableTreeNode("Menus");

    private final JTextField searchField = new JTextField();
    private boolean isUpdatingModel = false;
    private Consumer<SubMenuModel> onSelect;

    public MainMenuTreeWidget(IniFileModel model) {
        for (MenuModel menu : model.getMenus()) {
            DefaultMutableTreeNode menuNode = new DefaultMutableTreeNode(menu.getName());
            root.add(menuNode);

            for (MenuItem item : menu.getItems()) {
                addMenuItem(menuNode, item);
            }
        }

        tree = new JTree(root);
        tree.setToggleClickCount(1);
        tree.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                TreePath path = tree.getPathForLocation(e.getX(), e.getY());
                if (path != null) {
                    handleSelection(path);
                }
            }
        });
        tree.addTreeSelectionListener(e -> {
            if (isUpdatingModel) {
                return;
            }
            TreePath path = e.getPath();
            if (path != null) {
                handleSelection(path);
            }
        });
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
                    String name = userObject.toString();
                    if (userObject instanceof SubMenuModel) {
                        name = ((SubMenuModel) userObject).getName();
                    }
                    setText(name);

                    if ("Setup".equals(name)) {
                        setIcon(setupIcon);
                    } else if ("Fuel".equals(name)) {
                        setIcon(fuelIcon);
                    } else if ("Ignition".equals(name)) {
                        setIcon(ignitionIcon);
                    } else if ("Idle".equals(name)) {
                        setIcon(idleIcon);
                    } else if ("Cranking".equals(name)) {
                        setIcon(crankingIcon);
                    }
                }
                return this;
            }
        });

        JPanel topPanel = new JPanel(new BorderLayout());
        searchField.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                updateSearch(searchField.getText());
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                updateSearch(searchField.getText());
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                updateSearch(searchField.getText());
            }
        });

        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT, 0, 0));
        JButton expandAll = new JButton(AutoupdateUtil.loadIcon("expand_all16.png"));
        expandAll.setToolTipText("Expand All");
        expandAll.setPreferredSize(new Dimension(32, 32));
        expandAll.addActionListener(e -> expandAll(tree, true));

        JButton collapseAll = new JButton(AutoupdateUtil.loadIcon("collapse_all16.png"));
        collapseAll.setToolTipText("Collapse All");
        collapseAll.setPreferredSize(new Dimension(32, 32));
        collapseAll.addActionListener(e -> expandAll(tree, false));

        buttonPanel.add(expandAll);
        buttonPanel.add(collapseAll);

        topPanel.add(searchField, BorderLayout.CENTER);
        topPanel.add(buttonPanel, BorderLayout.EAST);

        contentPane.add(topPanel, BorderLayout.NORTH);
        contentPane.add(new JScrollPane(tree), BorderLayout.CENTER);
    }

    private void handleSelection(TreePath path) {
        DefaultMutableTreeNode node = (DefaultMutableTreeNode) path.getLastPathComponent();
        if (node.getUserObject() instanceof SubMenuModel) {
            if (onSelect != null) {
                onSelect.accept((SubMenuModel) node.getUserObject());
            }
        }

        if (!searchField.getText().isEmpty()) {
            SwingUtilities.invokeLater(() -> {
                searchField.setText("");
                expandAll(tree, true);
                tree.setSelectionPath(path);
                // Make sure the tree is laid out so it knows its new size after expansion
                trueLayoutAndRepaint(tree);
                tree.scrollPathToVisible(path);
            });
        } else {
            tree.scrollPathToVisible(path);
        }
    }

    /**
     * [search logic]: display all elements if text is empty
     *
     * if text is not empty, split into tokens by whitespace. match all tokens to concatenation of full
     * element path as top menu name + potential submenu name + element name
     *
     * display only matching tree elements and their parents. when user clicks on an element during search,
     * reset search text to empty, expand all elements, make sure that clicked JTree element is visible
     */
    private void updateSearch(String text) {
        isUpdatingModel = true;
        try {
            if (text == null || text.isEmpty()) {
                tree.setModel(new DefaultTreeModel(root));
                return;
            }

            String[] tokens = text.toLowerCase().split("\\s+");
            DefaultMutableTreeNode filteredRoot = new DefaultMutableTreeNode("Menus");
            filterNode(root, filteredRoot, tokens, "");

            tree.setModel(new DefaultTreeModel(filteredRoot));
            expandAll(tree, true);
        } finally {
            isUpdatingModel = false;
        }
    }

    private boolean filterNode(DefaultMutableTreeNode originalNode, DefaultMutableTreeNode filteredNode, String[] tokens, String path) {
        Object userObject = originalNode.getUserObject();
        String nodeText = userObject.toString();
        if (userObject instanceof SubMenuModel) {
            nodeText = ((SubMenuModel) userObject).getName();
        }
        String currentPath = path.isEmpty() ? nodeText : path + " " + nodeText;

        boolean matchFound = false;

        if (originalNode != root) {
            String currentPathLower = currentPath.toLowerCase();
            boolean allTokensMatch = true;
            for (String token : tokens) {
                if (!currentPathLower.contains(token)) {
                    allTokensMatch = false;
                    break;
                }
            }

            if (allTokensMatch) {
                matchFound = true;
            }
        }

        for (int i = 0; i < originalNode.getChildCount(); i++) {
            DefaultMutableTreeNode child = (DefaultMutableTreeNode) originalNode.getChildAt(i);
            DefaultMutableTreeNode filteredChild = new DefaultMutableTreeNode(child.getUserObject());
            if (filterNode(child, filteredChild, tokens, currentPath)) {
                filteredNode.add(filteredChild);
                matchFound = true;
            }
        }

        return matchFound;
    }

    private void expandAll(JTree tree, boolean expand) {
        DefaultMutableTreeNode root = (DefaultMutableTreeNode) tree.getModel().getRoot();
        expandAll(tree, new TreePath(root), expand);
    }

    private void expandAll(JTree tree, TreePath parent, boolean expand) {
        DefaultMutableTreeNode node = (DefaultMutableTreeNode) parent.getLastPathComponent();
        if (node.getChildCount() >= 0) {
            for (Enumeration<?> e = node.children(); e.hasMoreElements(); ) {
                DefaultMutableTreeNode n = (DefaultMutableTreeNode) e.nextElement();
                TreePath path = parent.pathByAddingChild(n);
                expandAll(tree, path, expand);
            }
        }
        if (expand) {
            tree.expandPath(parent);
        } else {
            if (parent.getPathCount() > 1) { // Don't collapse the root if we want it to stay "expanded" for user
                tree.collapsePath(parent);
            }
        }
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
            parent.add(new DefaultMutableTreeNode(subMenu));
        }
    }

    public void setOnSelect(Consumer<SubMenuModel> onSelect) {
        this.onSelect = onSelect;
    }

    public void selectSubMenu(String subMenuKey) {
        DefaultMutableTreeNode node = findSubMenuNode(root, subMenuKey);
        if (node != null) {
            TreePath path = new TreePath(node.getPath());
            tree.setSelectionPath(path);
            tree.scrollPathToVisible(path);
            handleSelection(path);
        }
    }

    private DefaultMutableTreeNode findSubMenuNode(DefaultMutableTreeNode parent, String subMenuKey) {
        Enumeration<javax.swing.tree.TreeNode> enumeration = parent.breadthFirstEnumeration();
        while (enumeration.hasMoreElements()) {
            javax.swing.tree.TreeNode treeNode = enumeration.nextElement();
            if (treeNode instanceof DefaultMutableTreeNode) {
                DefaultMutableTreeNode node = (DefaultMutableTreeNode) treeNode;
                if (node.getUserObject() instanceof SubMenuModel) {
                    SubMenuModel subMenu = (SubMenuModel) node.getUserObject();
                    if (subMenuKey.equals(subMenu.getKey())) {
                        return node;
                    }
                }
            }
        }
        return null;
    }

    public JPanel getContentPane() {
        return contentPane;
    }
}
