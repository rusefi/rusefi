package com.rusefi.ui.widgets.tune;

import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.icons.FlatSearchIcon;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.GroupMenuModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.MenuItem;
import com.opensr5.ini.MenuModel;
import com.opensr5.ini.PanelModel;
import com.opensr5.ini.SeparatorMenuItem;
import com.opensr5.ini.SubMenuModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.DefaultTreeSelectionModel;
import javax.swing.tree.TreePath;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.MouseEvent;
import java.util.*;
import java.util.List;
import java.util.function.Consumer;

import static com.rusefi.core.ui.AutoupdateUtil.trueLayoutAndRepaint;

/**
 * see TopLevelMenuSandbox
 */
public class MainMenuTreeWidget {
    private final JPanel contentPane = new JPanel(new BorderLayout());
    private final JTree tree;
    private int keyboardNavigationDirection;
    private boolean keyboardChildNavigation;
    private final DefaultMutableTreeNode root = new DefaultMutableTreeNode("Menus");

    private final JTextField searchField = new JTextField();
    private boolean isUpdatingModel = false;
    private boolean isProcessingSearchSelection = false;
    private Consumer<SubMenuModel> onSelect;

    private final UIContext uiContext;
    /** Nodes whose SubMenuModel has at least one enable/visible expression. */
    private final List<ExpressionEntry> expressionEntries = new ArrayList<>();
    /** Nodes currently evaluated as disabled (expression false). Used by renderer and click guard. */
    private final Set<DefaultMutableTreeNode> disabledNodes = new HashSet<>();
    /** Keys of currently disabled submenus for a quick lookup during click handling. */
    private final Set<String> disabledKeys = new HashSet<>();

    /** Renders a "std_separator" menu entry as a horizontal line. */
    private static class SeparatorRenderer extends JComponent {
        SeparatorRenderer() {
            setPreferredSize(new Dimension(120, 9));
        }

        @Override
        protected void paintComponent(Graphics g) {
            Color color = UIManager.getColor("Separator.foreground");
            g.setColor(color != null ? color : Color.GRAY);
            int y = getHeight() / 2;
            g.drawLine(0, y, getWidth(), y);
        }
    }

    private static class ExpressionEntry {
        final DefaultMutableTreeNode node;
        final SubMenuModel subMenu;

        ExpressionEntry(DefaultMutableTreeNode node, SubMenuModel subMenu) {
            this.node = node;
            this.subMenu = subMenu;
        }
    }

    public MainMenuTreeWidget(UIContext uiContext) {
        this.uiContext = uiContext;
        IniFileModel model = uiContext.iniFileState.getIniFileModel();
        // todo: do we fail if UI is re-launchd? with tuning tab active?
        if (model != null) {
            populate(model);
        }

        tree = new JTree(root) {
            private TreePath selectionBeforeMousePress;
            private boolean sawMousePress;

            @Override
            protected void processMouseEvent(MouseEvent event) {
                if (event.getID() == MouseEvent.MOUSE_PRESSED) {
                    selectionBeforeMousePress = getSelectionPath();
                    sawMousePress = true;
                }
                super.processMouseEvent(event);
                if (event.getID() == MouseEvent.MOUSE_CLICKED) {
                    TreePath clickedPath = getPathForLocation(event.getX(), event.getY());
                    TreePath selectedBeforeClick = sawMousePress ? selectionBeforeMousePress : getSelectionPath();
                    if (clickedPath != null && clickedPath.equals(selectedBeforeClick)) {
                        // Selection events handle changed pages during mouse press. Only repeat
                        // the callback for a click on the page that was already selected.
                        handleSelection(clickedPath);
                    }
                    selectionBeforeMousePress = null;
                    sawMousePress = false;
                }
            }
        };
        tree.setSelectionModel(new DefaultTreeSelectionModel() {
            {
                setSelectionMode(SINGLE_TREE_SELECTION);
            }

            @Override
            public void setSelectionPaths(TreePath[] paths) {
                if (paths == null || paths.length == 0) {
                    super.setSelectionPaths(paths);
                    return;
                }
                for (TreePath path : paths) {
                    TreePath selectablePath = getSelectablePath(path);
                    if (selectablePath != null) {
                        super.setSelectionPaths(new TreePath[]{selectablePath});
                        return;
                    }
                }
            }

            @Override
            public void addSelectionPaths(TreePath[] paths) {
                if (paths == null) {
                    return;
                }
                for (TreePath path : paths) {
                    TreePath selectablePath = getSelectablePath(path);
                    if (selectablePath != null) {
                        super.addSelectionPaths(new TreePath[]{selectablePath});
                        return;
                    }
                }
            }
        });
        installNavigationActions(-1, false, "selectPrevious", "selectPreviousChangeLead", "selectPreviousExtendSelection",
            "selectLast", "selectLastChangeLead", "selectLastExtendSelection",
            "scrollUpChangeSelection", "scrollUpChangeLead", "scrollUpExtendSelection");
        installNavigationActions(1, false, "selectNext", "selectNextChangeLead", "selectNextExtendSelection",
            "selectFirst", "selectFirstChangeLead", "selectFirstExtendSelection",
            "scrollDownChangeSelection", "scrollDownChangeLead", "scrollDownExtendSelection");
        installNavigationActions(1, true, "selectChild", "selectChildChangeLead");
        tree.setToggleClickCount(1);
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
        tree.setFont(new Font(font.getName(), font.getStyle(), (int)(font.getSize() * 1.2)));
        tree.setRootVisible(false);

        ImageIcon setupIcon = loadTuningIcon("setup", 24);
        Map<String, ImageIcon> categoryIcons = new HashMap<>();
        categoryIcons.put("Setup", setupIcon);
        categoryIcons.put("Base Engine", setupIcon);
        categoryIcons.put("Fuel", loadTuningIcon("fuel", 24));
        categoryIcons.put("Ignition", loadTuningIcon("ignition", 24));
        categoryIcons.put("Cranking", loadTuningIcon("cranking", 24));
        categoryIcons.put("Idle", loadTuningIcon("idle", 24));
        categoryIcons.put("Advanced", loadTuningIcon("advanced", 24));
        categoryIcons.put("Sensors", loadTuningIcon("sensors", 24));
        categoryIcons.put("CAN-bus", loadTuningIcon("can", 24));
        categoryIcons.put("Controller", loadTuningIcon("controller", 24));
        categoryIcons.put("Help", loadTuningIcon("help", 24));
        categoryIcons.put("View", loadTuningIcon("view", 24));

        ImageIcon groupIcon = loadTuningIcon("group", 18);
        ImageIcon tableIcon = loadTuningIcon("table", 18);
        ImageIcon curveIcon = loadTuningIcon("curve", 18);
        ImageIcon dialogIcon = loadTuningIcon("dialog", 18);
        Map<String, ImageIcon> featureIcons = new HashMap<>();
        featureIcons.put("fanSettings", loadTuningIcon("fan", 18));
        featureIcons.put("acSettings", loadTuningIcon("snowflake", 18));
        featureIcons.put("energySystems", loadTuningIcon("battery", 18));
        featureIcons.put("ignitionInputDialog", loadTuningIcon("key", 18));
        featureIcons.put("engineChars", loadTuningIcon("file-info", 18));

        tree.setCellRenderer(new DefaultTreeCellRenderer() {
            private final SeparatorRenderer separatorRenderer = new SeparatorRenderer();

            @Override
            public Component getTreeCellRendererComponent(JTree tree, Object value, boolean selected, boolean expanded, boolean leaf, int row, boolean hasFocus) {
                super.getTreeCellRendererComponent(tree, value, selected, expanded, leaf, row, hasFocus);
                if (value instanceof DefaultMutableTreeNode) {
                    DefaultMutableTreeNode node = (DefaultMutableTreeNode) value;
                    Object userObject = node.getUserObject();
                    if (userObject instanceof SeparatorMenuItem) {
                        return separatorRenderer;
                    }
                    String name = userObject.toString();
                    if (userObject instanceof SubMenuModel) {
                        name = ((SubMenuModel) userObject).getName();
                    }
                    setText(name);

                    ImageIcon categoryIcon = categoryIcons.get(name);
                    if (categoryIcon != null) {
                        setIcon(categoryIcon);
                    } else if (userObject instanceof SubMenuModel) {
                        IniFileModel currentModel = uiContext.iniFileState.getIniFileModel();
                        String key = ((SubMenuModel) userObject).getKey();
                        ImageIcon featureIcon = featureIcons.get(key);
                        if (featureIcon != null) {
                            setIcon(featureIcon);
                        } else if (currentModel != null) {
                            if (currentModel.getDialogs().containsKey(key)) {
                                setIcon(dialogIcon);
                            } else if (currentModel.getTable(key) != null) {
                                setIcon(tableIcon);
                            } else if (currentModel.getCurves().containsKey(key)) {
                                setIcon(curveIcon);
                            }
                        }
                    } else if (!leaf) {
                        setIcon(groupIcon);
                    }

                    if (disabledNodes.contains(node)) {
                        setForeground(UIManager.getColor("Label.disabledForeground"));
                    }
                }
                return this;
            }
        });

        JPanel topPanel = new JPanel(new BorderLayout(4, 0));
        topPanel.setOpaque(false);
        topPanel.setBorder(BorderFactory.createEmptyBorder(8, 8, 6, 6));
        searchField.putClientProperty(FlatClientProperties.PLACEHOLDER_TEXT, "Search settings");
        searchField.putClientProperty(FlatClientProperties.TEXT_FIELD_LEADING_ICON, new FlatSearchIcon());
        searchField.putClientProperty(FlatClientProperties.TEXT_FIELD_SHOW_CLEAR_BUTTON, true);
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
        buttonPanel.setOpaque(false);
        JButton expandAll = new JButton(TuningStyle.FoldIcon.EXPAND_ALL);
        expandAll.setToolTipText("Expand All");
        expandAll.putClientProperty(FlatClientProperties.BUTTON_TYPE, FlatClientProperties.BUTTON_TYPE_TOOLBAR_BUTTON);
        expandAll.setPreferredSize(new Dimension(30, 30));
        expandAll.addActionListener(e -> expandAll(tree, true));

        JButton collapseAll = new JButton(TuningStyle.FoldIcon.COLLAPSE_ALL);
        collapseAll.setToolTipText("Collapse All");
        collapseAll.putClientProperty(FlatClientProperties.BUTTON_TYPE, FlatClientProperties.BUTTON_TYPE_TOOLBAR_BUTTON);
        collapseAll.setPreferredSize(new Dimension(30, 30));
        collapseAll.addActionListener(e -> expandAll(tree, false));

        buttonPanel.add(expandAll);
        buttonPanel.add(collapseAll);

        topPanel.add(searchField, BorderLayout.CENTER);
        topPanel.add(buttonPanel, BorderLayout.EAST);

        // A white sidebar like the Log Viewer's, with a rounded selection (Tree.tuningMenu style).
        Color sidebar = TuningStyle.sidebarBackground();
        contentPane.setBackground(sidebar);
        contentPane.setBorder(BorderFactory.createMatteBorder(0, 0, 0, 1, TuningStyle.separator()));
        tree.putClientProperty(FlatClientProperties.STYLE_CLASS, "tuningMenu");
        tree.setBackground(sidebar);
        JScrollPane treeScroll = new JScrollPane(tree);
        treeScroll.setBorder(BorderFactory.createEmptyBorder());
        treeScroll.getViewport().setBackground(sidebar);

        contentPane.add(topPanel, BorderLayout.NORTH);
        contentPane.add(treeScroll, BorderLayout.CENTER);

        // Refresh the disabled state as soon as the tune is read from the ECU.
        // If the tree was not populated at startup (no local ini file), populate it now.
        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> {
            if (isConnected) {
                SwingUtilities.invokeLater(() -> {
                    if (root.getChildCount() == 0) {
                        IniFileModel iniModel = uiContext.iniFileState.getIniFileModel();
                        if (iniModel != null) {
                            populate(iniModel);
                            ((DefaultTreeModel) tree.getModel()).nodeStructureChanged(root);
                        }
                    }
                    refreshExpressions();
                });
            }
        });

        refreshExpressions();
    }

    private void populate(IniFileModel model) {
        for (MenuModel menu : model.getMenus()) {
            DefaultMutableTreeNode menuNode = new DefaultMutableTreeNode(menu.getName());
            root.add(menuNode);

            for (MenuItem item : menu.getItems()) {
                addMenuItem(menuNode, item);
            }
        }
    }

    private void handleSelection(TreePath path) {
        // Prevent duplicate processing during search selection
        if (isProcessingSearchSelection) {
            return;
        }

        DefaultMutableTreeNode node = (DefaultMutableTreeNode) path.getLastPathComponent();
        SubMenuModel selectedSubMenu = null;
        if (node.getUserObject() instanceof SubMenuModel) {
            selectedSubMenu = (SubMenuModel) node.getUserObject();
        }

        if (!searchField.getText().isEmpty()) {
            // When search is active, defer everything to after tree restoration
            final SubMenuModel subMenuToSelect = selectedSubMenu;
            isProcessingSearchSelection = true;
            SwingUtilities.invokeLater(() -> {
                isUpdatingModel = true;
                searchField.setText("");
                isUpdatingModel = false;
                try {
                    expandAll(tree, true);
                    // Find the node in the restored original tree model (the old path is invalid after model change)
                    if (subMenuToSelect != null) {
                        DefaultMutableTreeNode originalNode = findSubMenuNode(root, subMenuToSelect.getKey());
                        if (originalNode != null) {
                            TreePath newPath = new TreePath(originalNode.getPath());

                            // Temporarily disable listener to prevent multiple onSelect calls
                            isUpdatingModel = true;
                            tree.setSelectionPath(newPath);
                            isUpdatingModel = false;

                            // Make sure the tree is laid out so it knows its new size after expansion
                            trueLayoutAndRepaint(tree);

                            // call onSelect only if not disabled
                            if (onSelect != null && !disabledKeys.contains(subMenuToSelect.getKey())) {
                                onSelect.accept(subMenuToSelect);
                            }

                            // Defer scroll to after all other events are processed
                            SwingUtilities.invokeLater(() -> {
                                Rectangle rect = tree.getPathBounds(newPath);
                                if (rect != null) {
                                    rect.height = tree.getVisibleRect().height;
                                    tree.scrollRectToVisible(rect);
                                }
                            });
                        }
                    }
                } finally {
                    isProcessingSearchSelection = false;
                }
            });
        } else {
            if (selectedSubMenu != null && onSelect != null) {
                // Search restoration selects the result programmatically with the listener
                // suspended. The next real selection is user navigation, not a duplicate.
                // Skip disabled items
                if (disabledKeys.contains(selectedSubMenu.getKey())) {
                    return;
                }
                onSelect.accept(selectedSubMenu);
            }
            tree.scrollPathToVisible(path);
        }
    }

    private void installNavigationActions(int direction, boolean childNavigation, String... names) {
        for (String name : names) {
            Action original = tree.getActionMap().get(name);
            if (original == null) {
                continue;
            }
            tree.getActionMap().put(name, new AbstractAction() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    keyboardNavigationDirection = direction;
                    keyboardChildNavigation = childNavigation;
                    try {
                        original.actionPerformed(e);
                    } finally {
                        keyboardNavigationDirection = 0;
                        keyboardChildNavigation = false;
                    }
                    // Swing actions can set their lead/anchor and scroll to the rejected row.
                    TreePath selected = tree.getSelectionPath();
                    tree.setLeadSelectionPath(selected);
                    tree.setAnchorSelectionPath(selected);
                    if (selected != null) {
                        tree.scrollPathToVisible(selected);
                    }
                }
            });
        }
    }

    private TreePath getSelectablePath(TreePath path) {
        if (isSelectable(path)) {
            return path;
        }
        // Only keyboard navigation skips rows. Mouse/programmatic selection still rejects them.
        if (path != null && keyboardNavigationDirection != 0) {
            int row = tree.getRowForPath(path);
            for (row += keyboardNavigationDirection; row >= 0 && row < tree.getRowCount(); row += keyboardNavigationDirection) {
                TreePath candidate = tree.getPathForRow(row);
                if (keyboardChildNavigation && !path.getParentPath().isDescendant(candidate)) {
                    break;
                }
                if (isSelectable(candidate)) {
                    return candidate;
                }
            }
        }
        return null;
    }

    private boolean isSelectable(TreePath path) {
        if (path == null) {
            return false;
        }
        Object userObject = ((DefaultMutableTreeNode) path.getLastPathComponent()).getUserObject();
        if (userObject instanceof SeparatorMenuItem) {
            return false;
        }
        return !(userObject instanceof SubMenuModel)
            || !disabledKeys.contains(((SubMenuModel) userObject).getKey());
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
        // separators are decoration, they never match a search and would show up as stray lines
        if (userObject instanceof SeparatorMenuItem) {
            return false;
        }
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
        } else if (item instanceof SeparatorMenuItem) {
            parent.add(new DefaultMutableTreeNode(item));
        } else if (item instanceof SubMenuModel) {
            SubMenuModel subMenu = (SubMenuModel) item;
            DefaultMutableTreeNode node = new DefaultMutableTreeNode(subMenu);
            parent.add(node);
            if (subMenu.getEnableExpression() != null || subMenu.getVisibleExpression() != null) {
                expressionEntries.add(new ExpressionEntry(node, subMenu));
            }
        }
    }

    /**
     * Evaluates all enable/visible expressions against the current ECU configuration and updates
     * the disabled state of menu items. Called at connection time via the ConnectionStatusLogic listener.
     */
    public void refreshExpressions() {
        if (expressionEntries.isEmpty()) return;

        IniFileModel iniFileModel = uiContext.iniFileState.getIniFileModel();
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        ConfigurationImage ci = (bp != null) ? bp.getControllerConfiguration() : null;

        if (iniFileModel == null || ci == null) return;
        refreshExpressions(iniFileModel, ci);
    }

    /**
     * Evaluates all enable/visible expressions against the provided config image and updates
     * the disabled state of menu items. Use this variant when evaluating against a working
     * (locally edited) image rather than the last-read ECU image.
     * Small progress of #9159
     */
    public void refreshExpressions(ConfigurationImage ci) {
        if (expressionEntries.isEmpty()) return;

        IniFileModel iniFileModel = uiContext.iniFileState.getIniFileModel();
        if (iniFileModel == null || ci == null) return;
        refreshExpressions(iniFileModel, ci);
    }

    private void refreshExpressions(IniFileModel iniFileModel, ConfigurationImage ci) {
        disabledNodes.clear();
        disabledKeys.clear();

        for (ExpressionEntry entry : expressionEntries) {
            if (isDisabledByExpressions(entry.subMenu, iniFileModel, ci)) {
                disabledNodes.add(entry.node);
                disabledKeys.add(entry.subMenu.getKey());
            }
        }

        if (!isSelectable(tree.getSelectionPath())) {
            tree.clearSelection();
        }

        tree.repaint();
    }

    private boolean isDisabledByExpressions(SubMenuModel subMenu, IniFileModel iniFileModel, ConfigurationImage ci) {
        if (subMenu.getEnableExpression() != null) {
            Boolean enabled = ExpressionEvaluator.evaluateBooleanExpression(subMenu.getEnableExpression(), iniFileModel, ci);
            if (Boolean.FALSE.equals(enabled)) return true;
        }
        if (subMenu.getVisibleExpression() != null) {
            Boolean visible = ExpressionEvaluator.evaluateBooleanExpression(subMenu.getVisibleExpression(), iniFileModel, ci);
            return Boolean.FALSE.equals(visible);
        }
        return false;
    }


    public void setOnSelect(Consumer<SubMenuModel> onSelect) {
        this.onSelect = onSelect;
    }

    /** Selects the first available menu page containing a field, following nested panel references. */
    public boolean selectField(String fieldKey) {
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (ini == null || fieldKey == null) {
            return false;
        }
        // Search in menu order, not dialog declaration order: inner dialogs and unused
        // pin summaries may be declared before the page that actually exposes the field.
        Enumeration<?> nodes = root.preorderEnumeration();
        while (nodes.hasMoreElements()) {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode) nodes.nextElement();
            if (!(node.getUserObject() instanceof SubMenuModel) || !isSelectable(new TreePath(node.getPath()))) {
                continue;
            }
            SubMenuModel subMenu = (SubMenuModel) node.getUserObject();
            if (containsField(ini.getDialogs().get(subMenu.getKey()), ini, fieldKey, new HashSet<>())) {
                selectSubMenu(subMenu.getKey());
                return true;
            }
        }
        return false;
    }

    private static boolean containsField(DialogModel dialog, IniFileModel ini, String fieldKey, Set<String> visited) {
        if (dialog == null || !visited.add(dialog.getKey())) {
            return false;
        }
        for (DialogModel.Field field : dialog.getFields()) {
            if (fieldKey.equals(field.getKey())) {
                return true;
            }
        }
        for (PanelModel panel : dialog.getPanels()) {
            if (containsField(panel.resolveDialog(ini), ini, fieldKey, visited)) {
                return true;
            }
        }
        return false;
    }

    public void selectSubMenu(String subMenuKey) {
        DefaultMutableTreeNode node = findSubMenuNode(root, subMenuKey);
        if (node != null) {
            TreePath path = new TreePath(node.getPath());
            if (!isSelectable(path)) {
                return;
            }
            boolean alreadySelected = path.equals(tree.getSelectionPath());
            tree.setSelectionPath(path);
            tree.scrollPathToVisible(path);
            // A changed selection is handled by the listener. Preserve the public method's
            // refresh behavior when callers explicitly select the already-current page.
            if (alreadySelected) {
                handleSelection(path);
            }
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

    private static ImageIcon scaleIcon(ImageIcon icon, int size) {
        if (icon == null) {
            return null;
        }
        return new ImageIcon(icon.getImage().getScaledInstance(size, size, java.awt.Image.SCALE_SMOOTH));
    }

    private static ImageIcon loadTuningIcon(String name, int size) {
        return scaleIcon(AutoupdateUtil.loadIcon("icons/tuning/" + name + "48.png"), size);
    }

    public JPanel getContentPane() {
        return contentPane;
    }
}
