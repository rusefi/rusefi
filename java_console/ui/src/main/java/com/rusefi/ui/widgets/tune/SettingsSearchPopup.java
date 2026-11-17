package com.rusefi.ui.widgets.tune;

import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.icons.FlatSearchIcon;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.tune.SettingsPageIndex.Page;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Collections;
import java.util.List;
import java.util.function.Predicate;
import java.util.function.Supplier;

/**
 * Ctrl+K from any tab: a small floating search over the Tuning menu pages. Enter or a click opens the
 * chosen page in its own settings window over the current screen; Esc or a click elsewhere closes it.
 */
public final class SettingsSearchPopup {
    public static final KeyStroke SHORTCUT = KeyStroke.getKeyStroke(KeyEvent.VK_K, InputEvent.CTRL_DOWN_MASK);
    static final String HINT = "↑↓ choose · Enter open · Esc close";
    static final String NO_PAGES = "Connect to an ECU or open a tune to search its settings.";
    private static final int VISIBLE_ROWS = 12;
    private static final int WIDTH = 560;

    private final SettingsPageIndex index;
    private final Predicate<Page> open;
    private final Runnable close;
    private final JPanel content = new JPanel(new BorderLayout(0, 6));
    final JTextField search = new JTextField();
    final DefaultListModel<Page> results = new DefaultListModel<>();
    final JList<Page> list = new JList<>(results);
    final JLabel status = new JLabel();

    /**
     * @param open  opens a page; false when it cannot open now, which keeps the popup up with a message
     * @param close hides the popup after a page opened or on Esc
     */
    SettingsSearchPopup(SettingsPageIndex index, Predicate<Page> open, Runnable close) {
        this.index = index;
        this.open = open;
        this.close = close;
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
        // the list never takes focus: typing always goes to the search field
        list.setFocusable(false);
        list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        list.setVisibleRowCount(VISIBLE_ROWS);
        // rows are measured from a sample page, not from the empty list
        list.setPrototypeCellValue(new Page("", "Wg", Collections.singletonList("Wg")));
        list.setCellRenderer(new PageRenderer());
        list.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent event) {
                int row = list.locationToIndex(event.getPoint());
                if (row >= 0 && list.getCellBounds(row, row).contains(event.getPoint())) {
                    list.setSelectedIndex(row);
                    openSelected();
                }
            }
        });
        bind(KeyEvent.VK_DOWN, "nextPage", () -> move(1));
        bind(KeyEvent.VK_UP, "previousPage", () -> move(-1));
        bind(KeyEvent.VK_PAGE_DOWN, "nextPages", () -> move(VISIBLE_ROWS - 1));
        bind(KeyEvent.VK_PAGE_UP, "previousPages", () -> move(1 - VISIBLE_ROWS));
        bind(KeyEvent.VK_ENTER, "openPage", this::openSelected);
        bind(KeyEvent.VK_ESCAPE, "closeSearch", close);

        JScrollPane scroll = new JScrollPane(list);
        scroll.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        Dimension size = new Dimension(WIDTH, scroll.getPreferredSize().height);
        scroll.setPreferredSize(size);
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
        results.clear();
        List<Page> pages = index.search(search.getText());
        for (Page page : pages) {
            results.addElement(page);
        }
        if (!pages.isEmpty()) {
            list.setSelectedIndex(0);
            list.ensureIndexIsVisible(0);
        }
        if (index.getPages().isEmpty()) {
            status.setText(NO_PAGES);
        } else if (pages.isEmpty()) {
            status.setText("No settings page matches “" + search.getText().trim() + "”.");
        } else {
            status.setText(HINT);
        }
    }

    private void move(int delta) {
        if (results.isEmpty()) {
            return;
        }
        int row = Math.max(0, Math.min(results.size() - 1, list.getSelectedIndex() + delta));
        list.setSelectedIndex(row);
        list.ensureIndexIsVisible(row);
    }

    void openSelected() {
        Page page = list.getSelectedValue();
        if (page == null) {
            return;
        }
        if (open.test(page)) {
            close.run();
        } else {
            status.setText("“" + page.getName() + "” cannot open now: no tune is loaded.");
        }
    }

    /** Shows the search, starting from {@code text}; under {@code anchor} when given, else near the top. */
    public interface Launcher {
        void show(String text, @Nullable Component anchor);
    }

    /**
     * Binds {@link #SHORTCUT} on {@code frame} and returns the launcher for other ways in, such as the
     * tab bar's search box. {@code index} is asked at each opening, so the pages follow the INI and tune
     * of the moment; a null index (firmware being flashed) opens nothing.
     */
    public static Launcher install(JFrame frame, Supplier<SettingsPageIndex> index, Predicate<Page> open) {
        Shown shown = new Shown();
        Launcher launcher = (text, anchor) -> {
            if (shown.dialog != null) {
                // letters typed in the box before the search took focus join what is there
                shown.popup.search.replaceSelection(text);
                shown.dialog.toFront();
                return;
            }
            SettingsPageIndex pages = index.get();
            if (pages == null) {
                return;
            }
            JDialog dialog = new JDialog(frame, "Search settings", Dialog.ModalityType.MODELESS);
            SettingsSearchPopup popup = new SettingsSearchPopup(pages, open, dialog::dispose);
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

    /** The page name, then its menu path in the secondary text colour. */
    private static final class PageRenderer extends DefaultListCellRenderer {
        @Override
        public Component getListCellRendererComponent(JList<?> list, Object value, int index, boolean selected,
                                                      boolean focused) {
            super.getListCellRendererComponent(list, value, index, selected, focused);
            if (selected) {
                // the list never has focus; show the choice in the active selection colours anyway
                setBackground(UIManager.getColor("List.selectionBackground"));
                setForeground(UIManager.getColor("List.selectionForeground"));
            }
            if (value instanceof Page) {
                Page page = (Page) value;
                Color secondary = selected ? getForeground() : UIManager.getColor("Label.disabledForeground");
                String path = String.join(SettingsPageIndex.PATH_SEPARATOR, page.getMenuPath());
                setText("<html>" + escape(page.getName()) + "&nbsp;&nbsp;<span style='color:"
                    + hex(secondary != null ? secondary : Color.GRAY) + "'>" + escape(path) + "</span></html>");
            }
            setBorder(BorderFactory.createEmptyBorder(3, 6, 3, 6));
            return this;
        }

        private static String hex(Color color) {
            return String.format("#%02x%02x%02x", color.getRed(), color.getGreen(), color.getBlue());
        }

        private static String escape(String text) {
            return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;");
        }
    }
}
