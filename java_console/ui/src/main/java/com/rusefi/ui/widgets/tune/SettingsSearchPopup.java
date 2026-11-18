package com.rusefi.ui.widgets.tune;

import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.icons.FlatSearchIcon;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.SubMenuModel;
import com.rusefi.ui.widgets.JoinedButtons;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreePath;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Enumeration;
import java.util.function.Predicate;
import java.util.function.Supplier;

/**
 * Ctrl+K from any tab: a small floating search over the Tuning menu, shown as the same tree as the Tuning
 * tab. With nothing typed it lists the categories to browse; typing keeps the matching pages, expanded under
 * their categories, with the best match chosen. Enter or a click opens the chosen page in its own settings
 * window over the current screen (Enter on a category opens or closes it); Esc or a click elsewhere closes it.
 */
public final class SettingsSearchPopup {
    public static final KeyStroke SHORTCUT = KeyStroke.getKeyStroke(KeyEvent.VK_K, InputEvent.CTRL_DOWN_MASK);
    static final String HINT = "↑↓ choose · Enter open · Esc close";
    static final String NO_PAGES = "Connect to an ECU or open a tune to search its settings.";
    private static final int VISIBLE_ROWS = 12;
    private static final int WIDTH = 560;
    private static final int HEIGHT = 380;

    private final Predicate<String> open;
    private final Runnable close;
    private final JPanel content = new JPanel(new BorderLayout(0, 6));
    final JTextField search = new JTextField();
    final SettingsMenuTree menuTree;
    final JTree tree;
    final JLabel status = new JLabel();

    /**
     * @param ini   the menus to show; null shows none
     * @param image the tune that greys out pages, as in the Tuning tree; null greys out none
     * @param open  opens a page by its INI key; false when it cannot open now, which keeps the popup up
     *              with a message
     * @param close hides the popup after a page opened or on Esc
     */
    SettingsSearchPopup(@Nullable IniFileModel ini, @Nullable ConfigurationImage image, Predicate<String> open,
                        Runnable close) {
        this.open = open;
        this.close = close;
        // only pages that can open as a window: std_ entries and pages without a panel are left out
        menuTree = new SettingsMenuTree(() -> ini, page -> new CalibrationPanelKey(page.getKey()).isSupported(ini));
        menuTree.refreshExpressions(ini, image);
        tree = menuTree.getTree();
        search.putClientProperty(FlatClientProperties.PLACEHOLDER_TEXT, "Search settings");
        search.putClientProperty(FlatClientProperties.TEXT_FIELD_LEADING_ICON, new FlatSearchIcon());
        search.putClientProperty(FlatClientProperties.TEXT_FIELD_SHOW_CLEAR_BUTTON, true);
        search.getAccessibleContext().setAccessibleName("Search settings");
        search.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                update();
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                update();
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                update();
            }
        });
        // the tree never takes focus: typing always goes to the search field; tuningSearch still shows the
        // chosen row in the selection colours
        tree.setFocusable(false);
        tree.putClientProperty(FlatClientProperties.STYLE_CLASS, "tuningMenu tuningSearch");
        tree.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent event) {
                // a click on a category opens or closes it (the tree's single-click toggle)
                TreePath path = tree.getPathForLocation(event.getX(), event.getY());
                if (pageAt(path) != null && menuTree.isSelectable(path)) {
                    tree.setSelectionPath(path);
                    openSelected();
                }
            }
        });
        bind(KeyEvent.VK_DOWN, "nextPage", () -> move("selectNext", 1));
        bind(KeyEvent.VK_UP, "previousPage", () -> move("selectPrevious", 1));
        bind(KeyEvent.VK_PAGE_DOWN, "nextPages", () -> move("selectNext", VISIBLE_ROWS - 1));
        bind(KeyEvent.VK_PAGE_UP, "previousPages", () -> move("selectPrevious", VISIBLE_ROWS - 1));
        bind(KeyEvent.VK_ENTER, "openPage", this::openSelected);
        bind(KeyEvent.VK_ESCAPE, "closeSearch", close);

        JScrollPane scroll = new JScrollPane(tree);
        scroll.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        // a fixed size: the dialog is packed once and does not follow the number of rows
        scroll.setPreferredSize(new Dimension(WIDTH, HEIGHT));
        status.setForeground(UIManager.getColor("Label.disabledForeground"));
        content.setBorder(BorderFactory.createEmptyBorder(8, 8, 6, 8));
        content.add(search, BorderLayout.NORTH);
        content.add(scroll, BorderLayout.CENTER);
        content.add(status, BorderLayout.SOUTH);
        update();
    }

    private void bind(int key, String name, Runnable action) {
        search.getInputMap(JComponent.WHEN_FOCUSED).put(KeyStroke.getKeyStroke(key, 0), name);
        search.getActionMap().put(name, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                action.run();
            }
        });
    }

    JComponent getContent() {
        return content;
    }

    private void update() {
        String text = search.getText().trim();
        menuTree.filter(text);
        if (!menuTree.isPopulated()) {
            status.setText(NO_PAGES);
            return;
        }
        DefaultMutableTreeNode shownRoot = (DefaultMutableTreeNode) tree.getModel().getRoot();
        if (shownRoot.getChildCount() == 0) {
            status.setText("No settings page matches “" + text + "”.");
            return;
        }
        status.setText(HINT);
        TreePath choice = text.isEmpty() ? null : bestMatch(shownRoot, text);
        if (choice == null) {
            // nothing typed: the categories, closed, to browse
            choice = tree.getPathForRow(0);
        }
        tree.setSelectionPath(choice);
        tree.scrollPathToVisible(choice);
    }

    /** The page answering the query best, the first in menu order among equals; null without a page. */
    @Nullable
    private TreePath bestMatch(DefaultMutableTreeNode shownRoot, String text) {
        TreePath best = null;
        int bestRank = Integer.MAX_VALUE;
        Enumeration<?> nodes = shownRoot.preorderEnumeration();
        while (nodes.hasMoreElements()) {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode) nodes.nextElement();
            TreePath path = new TreePath(node.getPath());
            SubMenuModel page = pageAt(path);
            if (page == null || !menuTree.isSelectable(path)) {
                continue;
            }
            int rank = SettingsPageIndex.rank(page.getName(), text);
            if (rank < bestRank) {
                best = path;
                bestRank = rank;
            }
        }
        return best;
    }

    /** Runs the tree's own up/down navigation, which skips separators and greyed-out pages. */
    private void move(String treeAction, int steps) {
        Action action = tree.getActionMap().get(treeAction);
        if (action == null) {
            return;
        }
        for (int i = 0; i < steps; i++) {
            action.actionPerformed(new ActionEvent(tree, ActionEvent.ACTION_PERFORMED, treeAction));
        }
    }

    @Nullable
    private static SubMenuModel pageAt(@Nullable TreePath path) {
        if (path == null) {
            return null;
        }
        Object userObject = ((DefaultMutableTreeNode) path.getLastPathComponent()).getUserObject();
        return userObject instanceof SubMenuModel ? (SubMenuModel) userObject : null;
    }

    void openSelected() {
        TreePath path = tree.getSelectionPath();
        if (path == null) {
            return;
        }
        SubMenuModel page = pageAt(path);
        if (page == null) {
            if (tree.isExpanded(path)) {
                tree.collapsePath(path);
            } else {
                tree.expandPath(path);
            }
            return;
        }
        if (open.test(page.getKey())) {
            close.run();
        } else {
            status.setText("“" + page.getName() + "” cannot open now: no tune is loaded.");
        }
    }

    /** What the search shows: the INI menus, and the tune that greys out pages. */
    public static final class Source {
        final IniFileModel ini;
        final ConfigurationImage image;

        public Source(@Nullable IniFileModel ini, @Nullable ConfigurationImage image) {
            this.ini = ini;
            this.image = image;
        }
    }

    /** Shows the search, starting from {@code text}; under {@code anchor} when given, else near the top. */
    public interface Launcher {
        void show(String text, @Nullable Component anchor);
    }

    /**
     * Binds {@link #SHORTCUT} on {@code frame} and returns the launcher for other ways in, such as the
     * tab bar's search box. {@code source} is asked at each opening, so the pages follow the INI and tune
     * of the moment; a null source (firmware being flashed) opens nothing.
     *
     * @param open opens a page by its INI key, see the constructor
     */
    public static Launcher install(JFrame frame, Supplier<Source> source, Predicate<String> open) {
        Shown shown = new Shown();
        Launcher launcher = (text, anchor) -> {
            if (shown.dialog != null) {
                // letters typed in the box before the search took focus join what is there
                shown.popup.search.replaceSelection(text);
                shown.dialog.toFront();
                return;
            }
            Source pages = source.get();
            if (pages == null) {
                return;
            }
            JDialog dialog = new JDialog(frame, "Search settings", Dialog.ModalityType.MODELESS);
            SettingsSearchPopup popup = new SettingsSearchPopup(pages.ini, pages.image, open, dialog::dispose);
            shown.dialog = dialog;
            shown.popup = popup;
            popup.search.setText(text);
            dialog.setUndecorated(true);
            JComponent body = popup.getContent();
            Color line = UIManager.getColor("Component.borderColor");
            Border border = BorderFactory.createLineBorder(line != null ? line : Color.GRAY);
            body.setBorder(BorderFactory.createCompoundBorder(border, body.getBorder()));
            dialog.setContentPane(body);
            dialog.pack();
            dialog.setLocation(location(frame.getBounds(), dialog.getSize(),
                anchor == null || !anchor.isShowing() ? null
                    : new Rectangle(anchor.getLocationOnScreen(), anchor.getSize())));
            dialog.addWindowListener(new WindowAdapter() {
                @Override
                public void windowDeactivated(WindowEvent event) {
                    // a click elsewhere, or the opened settings window taking focus
                    dialog.dispose();
                }

                @Override
                public void windowClosed(WindowEvent event) {
                    shown.dialog = null;
                    shown.popup = null;
                }
            });
            dialog.setVisible(true);
            popup.search.requestFocusInWindow();
        };
        frame.getRootPane().registerKeyboardAction(event -> launcher.show("", null), SHORTCUT,
            JComponent.WHEN_IN_FOCUSED_WINDOW);
        return launcher;
    }

    /**
     * Below the anchor with their right edges lined up, or centred near the top of the console without
     * one; kept inside the console horizontally.
     */
    static Point location(Rectangle owner, Dimension size, @Nullable Rectangle anchor) {
        int x = anchor == null ? owner.x + (owner.width - size.width) / 2 : anchor.x + anchor.width - size.width;
        x = Math.max(owner.x, Math.min(x, owner.x + owner.width - size.width));
        int y = anchor == null ? owner.y + Math.min(120, owner.height / 6) : anchor.y + anchor.height + 4;
        return new Point(x, y);
    }

    /**
     * The tab bar's way in: looks like a search field, but a click, Enter or the first typed letter opens
     * the search (with that letter), so there is one search, the same as Ctrl+K.
     */
    public static JTextField createSearchBox(Launcher launcher) {
        JTextField box = new JTextField(16);
        box.putClientProperty(FlatClientProperties.PLACEHOLDER_TEXT, "Search settings (Ctrl+K)");
        box.putClientProperty(FlatClientProperties.TEXT_FIELD_LEADING_ICON, new FlatSearchIcon());
        box.getAccessibleContext().setAccessibleName("Search settings");
        box.setToolTipText("Find a Tuning page and open it in its own window over this screen (Ctrl+K)");
        Dimension size = new Dimension(box.getPreferredSize().width, JoinedButtons.HEIGHT);
        box.setPreferredSize(size);
        box.setMinimumSize(new Dimension(size.width / 2, size.height));
        box.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent event) {
                launcher.show("", box);
            }
        });
        box.addKeyListener(new KeyAdapter() {
            @Override
            public void keyTyped(KeyEvent event) {
                char typed = event.getKeyChar();
                if (typed != KeyEvent.CHAR_UNDEFINED && !Character.isISOControl(typed)) {
                    event.consume();
                    launcher.show(String.valueOf(typed), box);
                }
            }

            @Override
            public void keyPressed(KeyEvent event) {
                if (event.getKeyCode() == KeyEvent.VK_ENTER || event.getKeyCode() == KeyEvent.VK_DOWN) {
                    event.consume();
                    launcher.show("", box);
                }
            }
        });
        return box;
    }

    private static final class Shown {
        JDialog dialog;
        SettingsSearchPopup popup;
    }
}
