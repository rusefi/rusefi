package com.rusefi.ui.widgets;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;
import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Locale;

/**
 * Adds a search field to the top of a gauge popup. While the field is empty the
 * popup shows its normal items; typing replaces them with a flat list of the
 * matching gauges, labelled "Group / Gauge".
 */
public final class GaugeMenuSearch {
    /** Keeps a broad query from producing a popup taller than the screen. */
    static final int MAX_RESULTS = 25;

    private GaugeMenuSearch() {
    }

    /** Searches the gauge groups already at the top level of {@code popup}. */
    public static void addTo(JPopupMenu popup) {
        install(popup, catalogEntries(Arrays.asList(popup.getComponents())));
    }

    private static List<Entry> catalogEntries(List<Component> groups) {
        List<Entry> entries = new ArrayList<>();
        for (Component component : groups) {
            if (component instanceof JMenuItem) {
                collect((JMenuItem) component, "", entries);
            }
        }
        return entries;
    }

    private static void collect(JMenuItem item, String parentPath, List<Entry> entries) {
        String path = parentPath.isEmpty() ? item.getText() : parentPath + " / " + item.getText();
        if (item instanceof JMenu) {
            for (Component child : ((JMenu) item).getMenuComponents()) {
                if (child instanceof JMenuItem) {
                    collect((JMenuItem) child, path, entries);
                }
            }
        } else {
            entries.add(new Entry(path, item));
        }
    }

    private static void install(JPopupMenu popup, List<Entry> entries) {
        // Everything already in the popup is the normal menu, hidden while searching.
        List<Component> normalItems = new ArrayList<>(Arrays.asList(popup.getComponents()));
        List<JMenuItem> results = new ArrayList<>();

        JTextField search = new JTextField(20);
        search.putClientProperty("JTextField.placeholderText", "Group or gauge name");
        search.setToolTipText("Filter by any part of the full group / gauge name; Enter picks the first match");
        JLabel label = new JLabel("Search:");
        label.setLabelFor(search);
        JPanel searchPanel = new JPanel(new BorderLayout(6, 0));
        searchPanel.setBorder(BorderFactory.createEmptyBorder(4, 6, 4, 6));
        searchPanel.add(label, BorderLayout.WEST);
        searchPanel.add(search, BorderLayout.CENTER);
        popup.insert(searchPanel, 0);
        popup.insert(new JPopupMenu.Separator(), 1);

        Runnable focusSearch = () -> SwingUtilities.invokeLater(() -> {
            if (popup.isShowing()) {
                search.requestFocusInWindow();
            }
        });
        Runnable filter = () -> {
            String query = search.getText().trim().toLowerCase(Locale.ROOT);
            for (JMenuItem result : results) {
                popup.remove(result);
            }
            results.clear();
            boolean searching = !query.isEmpty();
            for (Component item : normalItems) {
                item.setVisible(!searching);
            }
            if (searching) {
                int matches = 0;
                for (Entry entry : entries) {
                    if (!entry.path.toLowerCase(Locale.ROOT).contains(query)) {
                        continue;
                    }
                    if (++matches <= MAX_RESULTS) {
                        results.add(resultItem(popup, entry));
                    }
                }
                if (matches == 0) {
                    results.add(disabledItem("No matching gauges"));
                } else if (matches > MAX_RESULTS) {
                    results.add(disabledItem((matches - MAX_RESULTS) + " more - keep typing to narrow"));
                }
                int index = 2;
                for (JMenuItem result : results) {
                    popup.insert(result, index++);
                }
            }
            if (popup.isVisible()) {
                boolean restoreFocus = search.isFocusOwner();
                popup.pack();
                // Some Swing themes recreate the popup window when its size changes.
                if (restoreFocus) {
                    focusSearch.run();
                }
            }
            popup.revalidate();
            popup.repaint();
        };
        search.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent event) {
                filter.run();
            }

            @Override
            public void removeUpdate(DocumentEvent event) {
                filter.run();
            }

            @Override
            public void changedUpdate(DocumentEvent event) {
                filter.run();
            }
        });
        search.addActionListener(event -> {
            if (!results.isEmpty() && results.get(0).isEnabled()) {
                results.get(0).doClick(0);
            }
        });

        popup.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent event) {
                focusSearch.run();
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent event) {
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent event) {
            }
        });
        filter.run();
    }

    private static JMenuItem resultItem(JPopupMenu popup, Entry entry) {
        JMenuItem result = new JMenuItem(entry.path);
        result.addActionListener(event -> {
            // Close first: the original action may replace the component that owns the popup.
            popup.setVisible(false);
            ActionEvent forwarded = new ActionEvent(entry.item, ActionEvent.ACTION_PERFORMED,
                entry.item.getActionCommand());
            for (ActionListener listener : entry.item.getActionListeners()) {
                listener.actionPerformed(forwarded);
            }
        });
        return result;
    }

    private static JMenuItem disabledItem(String text) {
        JMenuItem item = new JMenuItem(text);
        item.setEnabled(false);
        return item;
    }

    private static final class Entry {
        final String path;
        final JMenuItem item;

        Entry(String path, JMenuItem item) {
            this.path = path;
            this.item = item;
        }
    }
}
