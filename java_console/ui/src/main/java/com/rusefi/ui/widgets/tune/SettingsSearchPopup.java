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
import java.awt.event.AWTEventListener;
import java.awt.event.ActionEvent;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.ArrayList;
import java.util.List;
import java.util.function.BiPredicate;
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.Supplier;

/**
 * Ctrl+K from any tab: a search over the Tuning menu, shown as the same tree as the Tuning tab. The tab
 * bar's search box is the only search field: a click, Ctrl+K or typing in it drops the tree down under it.
 * With nothing typed the tree lists the categories to browse; typing keeps the matching pages, expanded
 * under their categories, with the best match chosen. Enter or a click opens the chosen page in its own
 * settings window over the current screen (Enter on a category opens or closes it); Esc or a click
 * elsewhere closes the tree.
 */
public final class SettingsSearchPopup {
    public static final KeyStroke SHORTCUT = KeyStroke.getKeyStroke(KeyEvent.VK_K, InputEvent.CTRL_DOWN_MASK);
    static final String HINT = "↑↓ choose · Enter open · Esc close";
    static final String NO_PAGES = "Connect to an ECU or open a tune to search its settings.";
    private static final int VISIBLE_ROWS = 12;
    private static final int WIDTH = 560;
    private static final int HEIGHT = 380;
    /** How far above the chosen row its category or group may be and still be scrolled into view with it. */
    private static final int HEADING_ROWS = 3;

    private final BiPredicate<String, String> open;
    private final Runnable close;
    private final JPanel content = new JPanel(new BorderLayout(0, 6));
    /** Where the query is typed: the tab bar's box, or a field of the popup's own. */
    final JTextField search;
    final SettingsMenuTree menuTree;
    final JTree tree;
    final JLabel status = new JLabel();
    final JScrollPane scroll;
    private final DocumentListener typing = new DocumentListener() {
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
    };
    private final List<KeyStroke> boundKeys = new ArrayList<>();
    /** Pointer movement in pixels that still counts as a click on a row. */
    private static final int CLICK_SLOP = 5;
    /**
     * Opens a page on release near the press: AWT sends no click once the pointer moved at all, which
     * would drop slightly moving clicks. A click on a category opens or closes it (the tree's own toggle,
     * on press).
     */
    final MouseAdapter rowClick = new MouseAdapter() {
        private Point pressedAt;
        private TreePath pressedPath;

        @Override
        public void mousePressed(MouseEvent event) {
            pressedAt = event.getPoint();
            pressedPath = tree.getPathForLocation(event.getX(), event.getY());
        }

        @Override
        public void mouseReleased(MouseEvent event) {
            Point from = pressedAt;
            TreePath path = pressedPath;
            pressedAt = null;
            pressedPath = null;
            if (from == null || path == null || !SwingUtilities.isLeftMouseButton(event)
                || from.distance(event.getPoint()) > CLICK_SLOP
                || !path.equals(tree.getPathForLocation(event.getX(), event.getY()))) {
                return;
            }
            if (pageAt(path) != null && menuTree.isSelectable(path)) {
                tree.setSelectionPath(path);
                openSelected();
            }
        }
    };

    /**
     * @param box   the field to read the query from and take the keys in; null gives the popup its own field
     * @param ini   the menus to show; null shows none
     * @param image the tune that greys out pages, as in the Tuning tree; null greys out none
     * @param open  opens a page by its INI key, at the setting with the second key when a setting was chosen
     *              (else null); false when it cannot open now, which keeps the popup up with a message
     * @param close hides the popup after a page opened or on Esc
     */
    SettingsSearchPopup(@Nullable JTextField box, @Nullable IniFileModel ini, @Nullable ConfigurationImage image,
                        BiPredicate<String, String> open, Runnable close) {
        this.open = open;
        this.close = close;
        // only pages that can open as a window: std_ entries and pages without a panel are left out
        menuTree = new SettingsMenuTree(() -> ini, page -> new CalibrationPanelKey(page.getKey()).isSupported(ini));
        menuTree.refreshExpressions(ini, image);
        tree = menuTree.getTree();
        if (box == null) {
            search = new JTextField();
            search.putClientProperty(FlatClientProperties.PLACEHOLDER_TEXT, "Search settings");
            search.putClientProperty(FlatClientProperties.TEXT_FIELD_LEADING_ICON, new FlatSearchIcon());
            search.putClientProperty(FlatClientProperties.TEXT_FIELD_SHOW_CLEAR_BUTTON, true);
            search.getAccessibleContext().setAccessibleName("Search settings");
            content.add(search, BorderLayout.NORTH);
            content.setBorder(BorderFactory.createEmptyBorder(8, 8, 6, 8));
        } else {
            search = box;
            content.setBorder(BorderFactory.createEmptyBorder(6, 6, 6, 6));
        }
        search.getDocument().addDocumentListener(typing);
        // the tree never takes focus: typing always goes to the search field; tuningSearch still shows the
        // chosen row in the selection colours
        tree.setFocusable(false);
        tree.putClientProperty(FlatClientProperties.STYLE_CLASS, "tuningMenu tuningSearch");
        tree.addMouseListener(rowClick);
        bind(KeyEvent.VK_DOWN, "nextPage", () -> move("selectNext", 1));
        bind(KeyEvent.VK_UP, "previousPage", () -> move("selectPrevious", 1));
        bind(KeyEvent.VK_PAGE_DOWN, "nextPages", () -> move("selectNext", VISIBLE_ROWS - 1));
        bind(KeyEvent.VK_PAGE_UP, "previousPages", () -> move("selectPrevious", VISIBLE_ROWS - 1));
        bind(KeyEvent.VK_ENTER, "openPage", this::openSelected);
        bind(KeyEvent.VK_ESCAPE, "closeSearch", close);

        scroll = new JScrollPane(tree);
        scroll.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        // a fixed size: the window is packed once and does not follow the number of rows
        scroll.setPreferredSize(new Dimension(WIDTH, HEIGHT));
        status.setForeground(UIManager.getColor("Label.disabledForeground"));
        content.add(scroll, BorderLayout.CENTER);
        content.add(status, BorderLayout.SOUTH);
        update();
    }

    private void bind(int key, String name, Runnable action) {
        KeyStroke stroke = KeyStroke.getKeyStroke(key, 0);
        boundKeys.add(stroke);
        search.getInputMap(JComponent.WHEN_FOCUSED).put(stroke, name);
        search.getActionMap().put(name, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                action.run();
            }
        });
    }

    /** Lets go of the search field: a box outside the popup types and takes its keys as before. */
    void detach() {
        search.getDocument().removeDocumentListener(typing);
        InputMap keys = search.getInputMap(JComponent.WHEN_FOCUSED);
        for (KeyStroke stroke : boundKeys) {
            Object name = keys.get(stroke);
            keys.remove(stroke);
            if (name != null) {
                search.getActionMap().remove(name);
            }
        }
        boundKeys.clear();
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
            status.setText("No settings page or setting matches “" + text + "”.");
            return;
        }
        status.setText(HINT);
        TreePath choice = menuTree.bestMatch(text);
        if (choice == null) {
            // nothing typed: the categories, closed, to browse
            choice = tree.getPathForRow(0);
        }
        tree.setSelectionPath(choice);
        showAtTop(choice);
    }

    /**
     * Scrolls the chosen row near the top, under its category and group when they are just above it, so the rows
     * after it are in view too. JTree's own scrollPathToVisible scrolls only as far as needed, which
     * leaves a match below the fold as the last visible row.
     */
    private void showAtTop(TreePath choice) {
        Rectangle row = tree.getPathBounds(choice);
        if (row == null) {
            return;
        }
        // the viewport is not laid out until the window is packed: use the size it is given
        int height = scroll.getViewport().getExtentSize().height;
        if (height <= 0) {
            Insets insets = scroll.getInsets();
            height = scroll.getPreferredSize().height - insets.top - insets.bottom;
        }
        int top = row.y;
        // the root is hidden: a path of two is a category
        for (TreePath heading = choice.getParentPath(); heading != null && heading.getPathCount() >= 2;
             heading = heading.getParentPath()) {
            Rectangle bounds = tree.getPathBounds(heading);
            // a heading far above (many matches in between) would push the choice back down
            if (bounds != null && row.y - bounds.y <= HEADING_ROWS * row.height
                && row.y + row.height - bounds.y <= height) {
                top = bounds.y;
            }
        }
        int maxTop = Math.max(0, tree.getPreferredSize().height - height);
        scroll.getViewport().setViewPosition(new Point(0, Math.max(0, Math.min(top, maxTop))));
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
        return SettingsMenuTree.pageOf(((DefaultMutableTreeNode) path.getLastPathComponent()).getUserObject());
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
        Object userObject = ((DefaultMutableTreeNode) path.getLastPathComponent()).getUserObject();
        String fieldKey = userObject instanceof SettingsMenuTree.SettingRow
            ? ((SettingsMenuTree.SettingRow) userObject).getFieldKey() : null;
        if (open.test(page.getKey(), fieldKey)) {
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

    /**
     * The tab bar box's search: drops the tree down under the box while the query is typed in the box.
     * Showing and hiding the window are left to the caller, so this runs without a screen.
     */
    static final class BoxSearch {
        private final JTextField box;
        private final Function<Runnable, SettingsSearchPopup> create;
        private final Consumer<SettingsSearchPopup> showWindow;
        private final Runnable hideWindow;
        private SettingsSearchPopup popup;
        /** Set while the box is emptied by the search itself, which must not drop the tree down again. */
        private boolean clearing;

        /**
         * @param create     the popup reading {@code box}, given the action that closes it; null when there is
         *                   nothing to search now
         * @param showWindow shows the popup's content under the box
         * @param hideWindow hides it
         */
        BoxSearch(JTextField box, Function<Runnable, SettingsSearchPopup> create,
                  Consumer<SettingsSearchPopup> showWindow, Runnable hideWindow) {
            this.box = box;
            this.create = create;
            this.showWindow = showWindow;
            this.hideWindow = hideWindow;
            box.addMouseListener(new MouseAdapter() {
                @Override
                public void mousePressed(MouseEvent event) {
                    open();
                }
            });
            box.addKeyListener(new KeyAdapter() {
                @Override
                public void keyPressed(KeyEvent event) {
                    // once the tree is down the popup's own bindings take these keys
                    int key = event.getKeyCode();
                    if (popup == null && (key == KeyEvent.VK_ENTER || key == KeyEvent.VK_DOWN)) {
                        event.consume();
                        open();
                    }
                }
            });
            box.addFocusListener(new FocusAdapter() {
                @Override
                public void focusLost(FocusEvent event) {
                    // a click elsewhere, another window, or the opened settings window; what was typed stays
                    close(false);
                }
            });
            box.getDocument().addDocumentListener(new DocumentListener() {
                @Override
                public void insertUpdate(DocumentEvent e) {
                    typed();
                }

                @Override
                public void removeUpdate(DocumentEvent e) {
                    typed();
                }

                @Override
                public void changedUpdate(DocumentEvent e) {
                }
            });
        }

        private void typed() {
            if (!clearing) {
                // the popup reads what is in the box when it starts
                open();
            }
        }

        boolean isOpen() {
            return popup != null;
        }

        /**
         * A mouse press anywhere in the application: one outside the box and the tree closes the tree. A
         * click on a part of the console that takes no focus (a gauge, a label) leaves the box focused, so
         * losing focus alone would leave the tree up.
         */
        void pressed(Component target) {
            SettingsSearchPopup shown = popup;
            if (shown == null || target == null || SwingUtilities.isDescendingFrom(target, box)
                || SwingUtilities.isDescendingFrom(target, shown.getContent())) {
                return;
            }
            close(false);
        }

        @Nullable
        SettingsSearchPopup getPopup() {
            return popup;
        }

        void open() {
            if (popup != null) {
                return;
            }
            popup = create.apply(() -> close(true));
            if (popup != null) {
                showWindow.accept(popup);
            }
        }

        /** @param clear empties the box too: after Esc or a page opened, not after a click elsewhere */
        void close(boolean clear) {
            SettingsSearchPopup shown = popup;
            if (shown != null) {
                popup = null;
                shown.detach();
                hideWindow.run();
            }
            if (clear) {
                clearing = true;
                try {
                    box.setText("");
                } finally {
                    clearing = false;
                }
            }
        }
    }

    /**
     * Makes {@code box} (from {@link #createSearchBox()}) the search and binds {@link #SHORTCUT} on
     * {@code frame} to it. {@code source} is asked at each opening, so the pages follow the INI and tune of
     * the moment; a null source (firmware being flashed) opens nothing. When the box is not on screen (the
     * wizard covers the tabs), Ctrl+K opens the search with a field of its own near the top of the console.
     *
     * @param open opens a page by its INI key, at a setting when one was chosen; see the constructor
     */
    public static void install(JFrame frame, JTextField box, Supplier<Source> source, BiPredicate<String, String> open) {
        JWindow dropdown = new JWindow(frame);
        // the box keeps the focus and the typing; clicks still reach the tree
        dropdown.setFocusableWindowState(false);
        BoxSearch boxSearch = new BoxSearch(box, closeAction -> {
            Source pages = source.get();
            return pages == null ? null : new SettingsSearchPopup(box, pages.ini, pages.image, open, closeAction);
        }, popup -> {
            dropdown.setContentPane(framed(popup.getContent()));
            dropdown.pack();
            dropdown.setLocation(location(frame.getBounds(), dropdown.getSize(),
                new Rectangle(box.getLocationOnScreen(), box.getSize())));
            dropdown.setVisible(true);
        }, () -> dropdown.setVisible(false));
        AWTEventListener outsideClick = event -> {
            if (event.getID() == MouseEvent.MOUSE_PRESSED && event.getSource() instanceof Component) {
                boxSearch.pressed((Component) event.getSource());
            }
        };
        Toolkit.getDefaultToolkit().addAWTEventListener(outsideClick, AWTEvent.MOUSE_EVENT_MASK);
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosed(WindowEvent e) {
                Toolkit.getDefaultToolkit().removeAWTEventListener(outsideClick);
                dropdown.dispose();
            }
        });
        frame.addComponentListener(new ComponentAdapter() {
            @Override
            public void componentMoved(ComponentEvent e) {
                boxSearch.close(false);
            }

            @Override
            public void componentResized(ComponentEvent e) {
                boxSearch.close(false);
            }
        });

        JDialog[] ownField = new JDialog[1];
        frame.getRootPane().registerKeyboardAction(event -> {
            if (box.isShowing()) {
                box.requestFocusInWindow();
                box.selectAll();
                boxSearch.open();
            } else if (ownField[0] == null) {
                ownField[0] = showWithOwnField(frame, source, open, () -> ownField[0] = null);
            } else {
                ownField[0].toFront();
            }
        }, SHORTCUT, JComponent.WHEN_IN_FOCUSED_WINDOW);
    }

    /** The search with its own field, centred near the top of the console; null when there is nothing to search. */
    @Nullable
    private static JDialog showWithOwnField(JFrame frame, Supplier<Source> source, BiPredicate<String, String> open,
                                            Runnable closed) {
        Source pages = source.get();
        if (pages == null) {
            closed.run();
            return null;
        }
        JDialog dialog = new JDialog(frame, "Search settings", Dialog.ModalityType.MODELESS);
        SettingsSearchPopup popup = new SettingsSearchPopup(null, pages.ini, pages.image, open, dialog::dispose);
        dialog.setUndecorated(true);
        dialog.setContentPane(framed(popup.getContent()));
        dialog.pack();
        dialog.setLocation(location(frame.getBounds(), dialog.getSize(), null));
        dialog.addWindowListener(new WindowAdapter() {
            @Override
            public void windowDeactivated(WindowEvent event) {
                // a click elsewhere, or the opened settings window taking focus
                dialog.dispose();
            }

            @Override
            public void windowClosed(WindowEvent event) {
                closed.run();
            }
        });
        dialog.setVisible(true);
        popup.search.requestFocusInWindow();
        return dialog;
    }

    private static JComponent framed(JComponent body) {
        Color line = UIManager.getColor("Component.borderColor");
        Border border = BorderFactory.createLineBorder(line != null ? line : Color.GRAY);
        body.setBorder(BorderFactory.createCompoundBorder(border, body.getBorder()));
        return body;
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

    /** The tab bar's search box: the field the query is typed in, once {@link #install} has wired it. */
    public static JTextField createSearchBox() {
        JTextField box = new JTextField(16);
        box.putClientProperty(FlatClientProperties.PLACEHOLDER_TEXT, "Search settings (Ctrl+K)");
        box.putClientProperty(FlatClientProperties.TEXT_FIELD_LEADING_ICON, new FlatSearchIcon());
        box.putClientProperty(FlatClientProperties.TEXT_FIELD_SHOW_CLEAR_BUTTON, true);
        box.getAccessibleContext().setAccessibleName("Search settings");
        box.getAccessibleContext().setAccessibleDescription(
            "Find a Tuning page or setting and open it in its own window over this screen");
        Dimension size = new Dimension(box.getPreferredSize().width, JoinedButtons.HEIGHT);
        box.setPreferredSize(size);
        box.setMinimumSize(new Dimension(size.width / 2, size.height));
        return box;
    }
}
