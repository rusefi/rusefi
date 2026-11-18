package com.rusefi.ui.widgets.tune;

import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.icons.FlatSearchIcon;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.SubMenuModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreePath;
import java.awt.*;
import java.util.function.Consumer;

import static com.rusefi.core.ui.AutoupdateUtil.trueLayoutAndRepaint;

/**
 * The Tuning tab's menu sidebar: a {@link SettingsMenuTree} with its search field and expand/collapse buttons.
 * Selecting a page opens it; selecting a search result clears the search and shows the page in the full tree.
 *
 * see TopLevelMenuSandbox
 */
public class MainMenuTreeWidget {
    private final JPanel contentPane = new JPanel(new BorderLayout());
    private final SettingsMenuTree menuTree;
    private final JTree tree;

    private final JTextField searchField = new JTextField();
    private boolean isUpdatingModel = false;
    private boolean isProcessingSearchSelection = false;
    private Consumer<SubMenuModel> onSelect;

    private final UIContext uiContext;

    public MainMenuTreeWidget(UIContext uiContext) {
        this.uiContext = uiContext;
        // todo: do we fail if UI is re-launchd? with tuning tab active?
        menuTree = new SettingsMenuTree(uiContext.iniFileState::getIniFileModel);
        tree = menuTree.getTree();
        menuTree.setOnSelectedRowClicked(this::handleSelection);
        tree.addTreeSelectionListener(e -> {
            if (isUpdatingModel) {
                return;
            }
            TreePath path = e.getPath();
            if (path != null) {
                handleSelection(path);
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
        expandAll.addActionListener(e -> menuTree.expandAll(true));

        JButton collapseAll = new JButton(TuningStyle.FoldIcon.COLLAPSE_ALL);
        collapseAll.setToolTipText("Collapse All");
        collapseAll.putClientProperty(FlatClientProperties.BUTTON_TYPE, FlatClientProperties.BUTTON_TYPE_TOOLBAR_BUTTON);
        collapseAll.setPreferredSize(new Dimension(30, 30));
        collapseAll.addActionListener(e -> menuTree.expandAll(false));

        buttonPanel.add(expandAll);
        buttonPanel.add(collapseAll);

        topPanel.add(searchField, BorderLayout.CENTER);
        topPanel.add(buttonPanel, BorderLayout.EAST);

        // A white sidebar like the Log Viewer's.
        Color sidebar = TuningStyle.sidebarBackground();
        contentPane.setBackground(sidebar);
        contentPane.setBorder(BorderFactory.createMatteBorder(0, 0, 0, 1, TuningStyle.separator()));
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
                    menuTree.populateIfEmpty(uiContext.iniFileState.getIniFileModel());
                    refreshExpressions();
                });
            }
        });

        refreshExpressions();
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
                    menuTree.expandAll(true);
                    // Find the node in the restored original tree model (the old path is invalid after model change)
                    if (subMenuToSelect != null) {
                        DefaultMutableTreeNode originalNode = menuTree.findSubMenuNode(subMenuToSelect.getKey());
                        if (originalNode != null) {
                            TreePath newPath = new TreePath(originalNode.getPath());

                            // Temporarily disable listener to prevent multiple onSelect calls
                            isUpdatingModel = true;
                            tree.setSelectionPath(newPath);
                            isUpdatingModel = false;

                            // Make sure the tree is laid out so it knows its new size after expansion
                            trueLayoutAndRepaint(tree);

                            // call onSelect only if not disabled
                            if (onSelect != null && !menuTree.isDisabled(subMenuToSelect)) {
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
                if (menuTree.isDisabled(selectedSubMenu)) {
                    return;
                }
                onSelect.accept(selectedSubMenu);
            }
            tree.scrollPathToVisible(path);
        }
    }

    /** See {@link SettingsMenuTree#filter}; the model swap must not open pages. */
    private void updateSearch(String text) {
        isUpdatingModel = true;
        try {
            menuTree.filter(text);
        } finally {
            isUpdatingModel = false;
        }
    }

    /**
     * Evaluates all enable/visible expressions against the current ECU configuration and updates
     * the disabled state of menu items. Called at connection time via the ConnectionStatusLogic listener.
     */
    public void refreshExpressions() {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        ConfigurationImage ci = (bp != null) ? bp.getControllerConfiguration() : null;
        refreshExpressions(ci);
    }

    /**
     * Evaluates all enable/visible expressions against the provided config image and updates
     * the disabled state of menu items. Use this variant when evaluating against a working
     * (locally edited) image rather than the last-read ECU image.
     * Small progress of #9159
     */
    public void refreshExpressions(ConfigurationImage ci) {
        menuTree.refreshExpressions(uiContext.iniFileState.getIniFileModel(), ci);
    }

    /** @see SettingsMenuTree#getUnlockHint */
    public String getUnlockHint(SubMenuModel subMenu) {
        return menuTree.getUnlockHint(subMenu);
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
        SubMenuModel page = menuTree.findAvailablePageWithField(ini, fieldKey);
        if (page == null) {
            return false;
        }
        selectSubMenu(page.getKey());
        return true;
    }

    public void selectSubMenu(String subMenuKey) {
        DefaultMutableTreeNode node = menuTree.findSubMenuNode(subMenuKey);
        if (node != null) {
            TreePath path = new TreePath(node.getPath());
            if (!menuTree.isSelectable(path)) {
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

    public JPanel getContentPane() {
        return contentPane;
    }
}
