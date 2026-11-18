package com.rusefi.ui.widgets.tune;

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
import com.formdev.flatlaf.FlatClientProperties;
import com.rusefi.ui.widgets.TuningIcons;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.DefaultTreeSelectionModel;
import javax.swing.tree.TreePath;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.MouseEvent;
import java.awt.font.TextAttribute;
import java.awt.font.TextLayout;
import java.text.AttributedString;
import java.util.*;
import java.util.List;
import java.util.function.Consumer;
import java.util.function.Predicate;
import java.util.function.Supplier;

/**
 * The Tuning menu as a tree: INI menus, groups and pages with their icons, separators, pages greyed out by
 * their enable/visible expressions (with unlock hints), keyboard navigation that skips unavailable rows,
 * and filtering by search text. Hosts add their own search field, styling and what selecting a page does.
 *
 * @see MainMenuTreeWidget
 */
public class SettingsMenuTree {
    private final JTree tree;
    private final DefaultMutableTreeNode root = new DefaultMutableTreeNode("Menus");
    private final Supplier<IniFileModel> iniSupplier;
    /** Pages to show, or null for every page of the INI menus. */
    private final Predicate<SubMenuModel> include;
    private int keyboardNavigationDirection;
    private boolean keyboardChildNavigation;
    private Consumer<TreePath> onSelectedRowClicked;

    /** Nodes whose SubMenuModel has at least one enable/visible expression. */
    private final List<SubMenuModel> expressionPages = new ArrayList<>();
    /**
     * Keys of currently disabled submenus (expression false). Used by renderer and selection guard.
     * Keyed by submenu rather than tree node because search results are built from copied nodes.
     */
    private final Set<String> disabledKeys = new HashSet<>();
    /** For each disabled submenu key, its conditions that are currently false: the unlock hint lists their settings. */
    private final Map<String, List<String>> failedConditions = new HashMap<>();
    /** The words of the current search, shown in bold; empty without a search. */
    private String[] queryWords = new String[0];

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

    public SettingsMenuTree(Supplier<IniFileModel> iniSupplier) {
        this(iniSupplier, null);
    }

    /**
     * @param include the pages to show; menus and groups left without a page are dropped, and so are the
     *                separators left at either end or next to each other. Null shows the menus as they are.
     */
    public SettingsMenuTree(Supplier<IniFileModel> iniSupplier, Predicate<SubMenuModel> include) {
        this.iniSupplier = iniSupplier;
        this.include = include;
        IniFileModel model = iniSupplier.get();
        if (model != null) {
            populate(model);
        }

        tree = new JTree(root) {
            private TreePath selectionBeforeMousePress;
            private boolean sawMousePress;

            @Override
            public String getToolTipText(MouseEvent event) {
                TreePath path = getPathForLocation(event.getX(), event.getY());
                if (path == null) {
                    return null;
                }
                Object userObject = ((DefaultMutableTreeNode) path.getLastPathComponent()).getUserObject();
                return userObject instanceof SubMenuModel ? getUnlockHint((SubMenuModel) userObject) : null;
            }

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
                    if (clickedPath != null && clickedPath.equals(selectedBeforeClick) && onSelectedRowClicked != null) {
                        // Selection listeners handle changed pages during mouse press. Only report
                        // a click on the row that was already selected.
                        onSelectedRowClicked.accept(clickedPath);
                    }
                    selectionBeforeMousePress = null;
                    sawMousePress = false;
                }
            }
        };
        // enables getToolTipText above: unlock hints of greyed-out pages
        ToolTipManager.sharedInstance().registerComponent(tree);
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
        Font font = tree.getFont();
        tree.setFont(new Font(font.getName(), font.getStyle(), (int) (font.getSize() * 1.2)));
        tree.setRootVisible(false);
        // rounded selection (Tree.tuningMenu style)
        tree.putClientProperty(FlatClientProperties.STYLE_CLASS, "tuningMenu");
        tree.setCellRenderer(createRenderer());
    }

    private DefaultTreeCellRenderer createRenderer() {
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

        return new DefaultTreeCellRenderer() {
            private final SeparatorRenderer separatorRenderer = new SeparatorRenderer();
            /** Letters of the text to draw in bold, or null for plain text drawn by the label itself. */
            private boolean[] bold;

            @Override
            public Component getTreeCellRendererComponent(JTree tree, Object value, boolean selected, boolean expanded, boolean leaf, int row, boolean hasFocus) {
                super.getTreeCellRendererComponent(tree, value, selected, expanded, leaf, row, hasFocus);
                bold = null;
                if (value instanceof DefaultMutableTreeNode) {
                    DefaultMutableTreeNode node = (DefaultMutableTreeNode) value;
                    Object userObject = node.getUserObject();
                    if (userObject instanceof SeparatorMenuItem) {
                        return separatorRenderer;
                    }
                    String name = nodeText(userObject);
                    setText(name);
                    bold = boldLetters(name, queryWords);

                    ImageIcon categoryIcon = categoryIcons.get(name);
                    if (categoryIcon != null) {
                        setIcon(categoryIcon);
                    } else if (userObject instanceof SubMenuModel) {
                        IniFileModel currentModel = iniSupplier.get();
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

                    if (userObject instanceof SubMenuModel && disabledKeys.contains(((SubMenuModel) userObject).getKey())) {
                        setForeground(UIManager.getColor("Label.disabledForeground"));
                    }
                }
                return this;
            }

            /*
             * Search matches are drawn here rather than as HTML <b>: Swing's HTML view spaces bold runs in a
             * tree renderer with a visible gap before each one.
             */
            @Override
            public Dimension getPreferredSize() {
                Dimension size = super.getPreferredSize();
                if (bold == null || size == null) {
                    return size;
                }
                String text = getText();
                FontMetrics metrics = getFontMetrics(getFont());
                TextLayout layout = new TextLayout(styled(text).getIterator(), metrics.getFontRenderContext());
                // a pixel to spare: the label measured its plain text with the same metrics
                return new Dimension(size.width - metrics.stringWidth(text) + (int) Math.ceil(layout.getAdvance()) + 1,
                    size.height);
            }

            @Override
            protected void paintComponent(Graphics g) {
                if (bold == null) {
                    super.paintComponent(g);
                    return;
                }
                String text = getText();
                // the label paints its icon (and the background); the text follows in runs
                setText("");
                try {
                    super.paintComponent(g);
                } finally {
                    setText(text);
                }
                Insets insets = getInsets();
                Icon icon = getIcon();
                int x = insets.left + (icon == null ? 0 : icon.getIconWidth() + getIconTextGap());
                FontMetrics plain = getFontMetrics(getFont());
                int height = getHeight() - insets.top - insets.bottom;
                int baseline = insets.top + (height - plain.getHeight()) / 2 + plain.getAscent();
                Graphics2D g2 = (Graphics2D) g.create();
                try {
                    // the desktop's text anti-aliasing, as the label uses for its own text
                    Object hints = Toolkit.getDefaultToolkit().getDesktopProperty("awt.font.desktophints");
                    if (hints instanceof Map) {
                        g2.addRenderingHints((Map<?, ?>) hints);
                    }
                    g2.setColor(getForeground());
                    // one call lays the plain and bold runs out together, so each run starts where the last ends
                    g2.drawString(styled(text).getIterator(), x, baseline);
                } finally {
                    g2.dispose();
                }
            }

            private AttributedString styled(String text) {
                AttributedString styled = new AttributedString(text);
                styled.addAttribute(TextAttribute.FONT, getFont());
                Font boldFont = getFont().deriveFont(Font.BOLD);
                for (int i = 0; i < bold.length; i++) {
                    if (bold[i]) {
                        styled.addAttribute(TextAttribute.FONT, boldFont, i, i + 1);
                    }
                }
                return styled;
            }
        };
    }

    private static String nodeText(Object userObject) {
        return userObject instanceof SubMenuModel ? ((SubMenuModel) userObject).getName() : userObject.toString();
    }

    public JTree getTree() {
        return tree;
    }

    /** The full (unfiltered) menu tree. */
    public DefaultMutableTreeNode getRoot() {
        return root;
    }

    public boolean isPopulated() {
        return root.getChildCount() > 0;
    }

    /** Fills an empty tree from the INI menus, e.g. when no INI file was available at construction. */
    public void populateIfEmpty(IniFileModel model) {
        if (isPopulated() || model == null) {
            return;
        }
        populate(model);
        ((DefaultTreeModel) tree.getModel()).nodeStructureChanged(root);
    }

    private void populate(IniFileModel model) {
        for (MenuModel menu : model.getMenus()) {
            DefaultMutableTreeNode menuNode = new DefaultMutableTreeNode(menu.getName());
            root.add(menuNode);

            for (MenuItem item : menu.getItems()) {
                addMenuItem(menuNode, item);
            }
        }
        if (include != null) {
            prune(root);
        }
    }

    /** @return whether the node still holds a page */
    private static boolean prune(DefaultMutableTreeNode node) {
        for (int i = node.getChildCount() - 1; i >= 0; i--) {
            DefaultMutableTreeNode child = (DefaultMutableTreeNode) node.getChildAt(i);
            Object userObject = child.getUserObject();
            if (!(userObject instanceof SubMenuModel) && !(userObject instanceof SeparatorMenuItem) && !prune(child)) {
                node.remove(i);
            }
        }
        boolean previousIsLine = true; // a leading separator goes
        for (int i = 0; i < node.getChildCount(); ) {
            boolean isLine = ((DefaultMutableTreeNode) node.getChildAt(i)).getUserObject() instanceof SeparatorMenuItem;
            if (isLine && (previousIsLine || i == node.getChildCount() - 1)) {
                node.remove(i);
            } else {
                previousIsLine = isLine;
                i++;
            }
        }
        // a separator that became last when the one after it was dropped
        int last = node.getChildCount() - 1;
        if (last >= 0 && ((DefaultMutableTreeNode) node.getChildAt(last)).getUserObject() instanceof SeparatorMenuItem) {
            node.remove(last);
        }
        return node.getChildCount() > 0;
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
            if (include != null && !include.test(subMenu)) {
                return;
            }
            parent.add(new DefaultMutableTreeNode(subMenu));
            if (subMenu.getEnableExpression() != null || subMenu.getVisibleExpression() != null) {
                expressionPages.add(subMenu);
            }
        }
    }

    /** Called with the path of an already-selected row the user clicks again. */
    public void setOnSelectedRowClicked(Consumer<TreePath> onSelectedRowClicked) {
        this.onSelectedRowClicked = onSelectedRowClicked;
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

    /** Separators and disabled pages cannot be selected; categories and groups can. */
    public boolean isSelectable(TreePath path) {
        if (path == null) {
            return false;
        }
        Object userObject = ((DefaultMutableTreeNode) path.getLastPathComponent()).getUserObject();
        if (userObject instanceof SeparatorMenuItem) {
            return false;
        }
        return !(userObject instanceof SubMenuModel) || !isDisabled((SubMenuModel) userObject);
    }

    public boolean isDisabled(SubMenuModel subMenu) {
        return disabledKeys.contains(subMenu.getKey());
    }

    /**
     * [search logic], shared by the Tuning tab and the Ctrl+K search: the query is split into words, ignoring
     * case. A row matches when every word appears, in any order, in its path: menu name, group names and its
     * own name. Matching rows are shown with their parents, all expanded, and the matched letters of each
     * row's own name in bold. A query without words shows the whole tree.
     *
     * Replaces the tree model, so hosts that listen to selection changes should ignore the events this fires.
     *
     * @see #bestMatch
     */
    public void filter(String text) {
        queryWords = words(text);
        if (queryWords.length == 0) {
            tree.setModel(new DefaultTreeModel(root));
            return;
        }

        DefaultMutableTreeNode filteredRoot = new DefaultMutableTreeNode("Menus");
        filterNode(root, filteredRoot, queryWords, "");

        tree.setModel(new DefaultTreeModel(filteredRoot));
        expandAll(true);
    }

    /** The words of a search query, lower case. */
    static String[] words(String query) {
        String text = query == null ? "" : query.trim().toLowerCase(Locale.ROOT);
        return text.isEmpty() ? new String[0] : text.split("\\s+");
    }

    /**
     * The page of the filtered tree answering the query best, the first in menu order among equals; null
     * when no page the user can open now matches. See {@link #rank}.
     */
    public TreePath bestMatch(String query) {
        String text = String.join(" ", words(query));
        if (text.isEmpty()) {
            return null;
        }
        TreePath best = null;
        int bestRank = Integer.MAX_VALUE;
        Enumeration<?> nodes = ((DefaultMutableTreeNode) tree.getModel().getRoot()).preorderEnumeration();
        while (nodes.hasMoreElements()) {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode) nodes.nextElement();
            TreePath path = new TreePath(node.getPath());
            if (!(node.getUserObject() instanceof SubMenuModel) || !isSelectable(path)) {
                continue;
            }
            int rank = rank(((SubMenuModel) node.getUserObject()).getName(), text);
            if (rank < bestRank) {
                best = path;
                bestRank = rank;
            }
        }
        return best;
    }

    /**
     * How well a page name answers a query its path already matches: 0 for the exact name, 1 for a name
     * starting with the query, 2 for a name containing every word, 3 for a match through the menu or group.
     */
    static int rank(String pageName, String query) {
        String text = String.join(" ", words(query));
        String name = pageName.toLowerCase(Locale.ROOT);
        if (name.equals(text)) {
            return 0;
        }
        if (name.startsWith(text)) {
            return 1;
        }
        for (String word : words(text)) {
            if (!name.contains(word)) {
                return 3;
            }
        }
        return 2;
    }

    /**
     * Which letters of the name are an occurrence of one of the words, shown in bold; null when none are.
     */
    static boolean[] boldLetters(String name, String[] words) {
        String lower = name.toLowerCase(Locale.ROOT);
        // lower case can change the length of a few letters; such a name is shown plain rather than misaligned
        if (words.length == 0 || lower.length() != name.length()) {
            return null;
        }
        boolean[] bold = new boolean[name.length()];
        boolean any = false;
        for (String word : words) {
            for (int at = lower.indexOf(word); at >= 0; at = lower.indexOf(word, at + word.length())) {
                Arrays.fill(bold, at, at + word.length(), true);
                any = true;
            }
        }
        return any ? bold : null;
    }

    private boolean filterNode(DefaultMutableTreeNode originalNode, DefaultMutableTreeNode filteredNode, String[] tokens, String path) {
        Object userObject = originalNode.getUserObject();
        // separators are decoration, they never match a search and would show up as stray lines
        if (userObject instanceof SeparatorMenuItem) {
            return false;
        }
        String nodeText = nodeText(userObject);
        String currentPath = path.isEmpty() ? nodeText : path + " " + nodeText;

        boolean matchFound = false;

        if (originalNode != root) {
            String currentPathLower = currentPath.toLowerCase(Locale.ROOT);
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

    public void expandAll(boolean expand) {
        DefaultMutableTreeNode modelRoot = (DefaultMutableTreeNode) tree.getModel().getRoot();
        expandAll(new TreePath(modelRoot), expand);
    }

    private void expandAll(TreePath parent, boolean expand) {
        DefaultMutableTreeNode node = (DefaultMutableTreeNode) parent.getLastPathComponent();
        for (Enumeration<?> e = node.children(); e.hasMoreElements(); ) {
            DefaultMutableTreeNode n = (DefaultMutableTreeNode) e.nextElement();
            expandAll(parent.pathByAddingChild(n), expand);
        }
        if (expand) {
            tree.expandPath(parent);
        } else if (parent.getPathCount() > 1) { // Don't collapse the root if we want it to stay "expanded" for user
            tree.collapsePath(parent);
        }
    }

    /**
     * Evaluates all enable/visible expressions against the given configuration image and updates
     * the disabled state of menu items. Clears a selection that is no longer selectable.
     */
    public void refreshExpressions(IniFileModel iniFileModel, ConfigurationImage ci) {
        if (expressionPages.isEmpty() || iniFileModel == null || ci == null) {
            return;
        }
        disabledKeys.clear();
        failedConditions.clear();

        for (SubMenuModel subMenu : expressionPages) {
            if (isDisabledByExpressions(subMenu, iniFileModel, ci)) {
                disabledKeys.add(subMenu.getKey());
                List<String> failed = new ArrayList<>();
                for (String expression : Arrays.asList(subMenu.getEnableExpression(), subMenu.getVisibleExpression())) {
                    if (expression != null && Boolean.FALSE.equals(
                        ExpressionEvaluator.evaluateBooleanExpression(expression, iniFileModel, ci))) {
                        failed.add(expression);
                    }
                }
                failedConditions.put(subMenu.getKey(), failed);
            }
        }

        if (!isSelectable(tree.getSelectionPath())) {
            tree.clearSelection();
        }

        tree.repaint();
    }

    /** Whether a page's enable or visible condition is false for the given tune. */
    static boolean isDisabledByExpressions(SubMenuModel subMenu, IniFileModel iniFileModel, ConfigurationImage ci) {
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

    /**
     * Explains a greyed-out page (#10625): the settings its currently false enable/visible
     * conditions depend on, by the label the user sees, with the menu page where each one is found.
     *
     * @return tooltip text, or null for a page that is not disabled
     */
    public String getUnlockHint(SubMenuModel subMenu) {
        IniFileModel ini = iniSupplier.get();
        if (ini == null || !isDisabled(subMenu)) {
            return null;
        }
        List<String> settings = new ArrayList<>();
        Set<String> visited = new HashSet<>();
        for (String expression : failedConditions.getOrDefault(subMenu.getKey(), Collections.emptyList())) {
            collectConditionSettings(expression, ini, settings, visited);
        }
        if (settings.isEmpty()) {
            return null;
        }
        StringBuilder hint = new StringBuilder("<html>Not available with the current settings. Depends on:");
        for (String key : settings) {
            DialogModel.Field uiField = ini.getFieldsInUiOrder().get(key);
            String label = uiField == null || uiField.getUiName() == null || uiField.getUiName().trim().isEmpty()
                ? key : uiField.getUiName().trim();
            hint.append("<br>&bull; ").append(escapeHtml(label));
            String page = findPageName(ini, key, subMenu.getKey());
            if (page != null) {
                hint.append(" (in ").append(escapeHtml(page)).append(")");
            }
        }
        return hint.append("</html>").toString();
    }

    /** Settings in order of appearance; calculated channels are replaced by the settings they use. */
    private static void collectConditionSettings(String expression, IniFileModel ini, List<String> settings,
                                                 Set<String> visited) {
        if (expression == null) {
            return;
        }
        List<String> variables = new ArrayList<>(ExpressionEvaluator.extractVariables(expression));
        variables.sort(Comparator.comparingInt(expression::indexOf));
        for (String variable : variables) {
            if (!visited.add(variable)) {
                continue;
            }
            String calculated = ini.getExpressionOutputChannel(variable);
            if (calculated != null && !ini.findIniField(variable).isPresent()) {
                collectConditionSettings(calculated, ini, settings, visited);
            } else if (!settings.contains(variable)) {
                settings.add(variable);
            }
        }
    }

    /** Name of the first menu page showing the setting, preferring a page the user can open now. */
    private String findPageName(IniFileModel ini, String fieldKey, String excludedPageKey) {
        String disabledPage = null;
        for (SubMenuModel subMenu : pagesInMenuOrder()) {
            if (subMenu.getKey().equals(excludedPageKey)
                || !containsField(ini.getDialogs().get(subMenu.getKey()), ini, fieldKey, new HashSet<>())) {
                continue;
            }
            if (!isDisabled(subMenu)) {
                return subMenu.getName();
            }
            if (disabledPage == null) {
                disabledPage = subMenu.getName();
            }
        }
        return disabledPage;
    }

    /** First page the user can open now that shows the field, following nested panel references. */
    public SubMenuModel findAvailablePageWithField(IniFileModel ini, String fieldKey) {
        // Search in menu order, not dialog declaration order: inner dialogs and unused
        // pin summaries may be declared before the page that actually exposes the field.
        for (SubMenuModel subMenu : pagesInMenuOrder()) {
            if (!isDisabled(subMenu)
                && containsField(ini.getDialogs().get(subMenu.getKey()), ini, fieldKey, new HashSet<>())) {
                return subMenu;
            }
        }
        return null;
    }

    private List<SubMenuModel> pagesInMenuOrder() {
        List<SubMenuModel> pages = new ArrayList<>();
        Enumeration<?> nodes = root.preorderEnumeration();
        while (nodes.hasMoreElements()) {
            Object userObject = ((DefaultMutableTreeNode) nodes.nextElement()).getUserObject();
            if (userObject instanceof SubMenuModel) {
                pages.add((SubMenuModel) userObject);
            }
        }
        return pages;
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

    /** The node of a page in the full tree, or null. */
    public DefaultMutableTreeNode findSubMenuNode(String subMenuKey) {
        Enumeration<?> nodes = root.breadthFirstEnumeration();
        while (nodes.hasMoreElements()) {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode) nodes.nextElement();
            if (node.getUserObject() instanceof SubMenuModel
                && subMenuKey.equals(((SubMenuModel) node.getUserObject()).getKey())) {
                return node;
            }
        }
        return null;
    }

    private static String escapeHtml(String text) {
        return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;");
    }

    private static ImageIcon loadTuningIcon(String name, int size) {
        ImageIcon icon = TuningIcons.load(name);
        if (icon == null) {
            return null;
        }
        return new ImageIcon(icon.getImage().getScaledInstance(size, size, java.awt.Image.SCALE_SMOOTH));
    }
}
