package com.rusefi.ui.widgets;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;
import java.awt.BorderLayout;
import java.awt.Component;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

/** Adds search to the gauge entries already in a popup. Add other actions afterward. */
public final class GaugeMenuSearch {
    private GaugeMenuSearch() {
    }

    public static void addTo(JPopupMenu popup) {
        List<JMenuItem> gaugeItems = new ArrayList<>();
        for (Component component : popup.getComponents()) {
            if (component instanceof JMenuItem) {
                gaugeItems.add((JMenuItem) component);
            }
        }

        JTextField search = new JTextField(20);
        search.putClientProperty("JTextField.placeholderText", "Group or gauge name");
        search.setToolTipText("Filter by any part of the full group / gauge name");
        JLabel label = new JLabel("Search:");
        label.setLabelFor(search);
        JPanel searchPanel = new JPanel(new BorderLayout(6, 0));
        searchPanel.setBorder(BorderFactory.createEmptyBorder(4, 6, 4, 6));
        searchPanel.add(label, BorderLayout.WEST);
        searchPanel.add(search, BorderLayout.CENTER);
        popup.insert(searchPanel, 0);
        popup.insert(new JPopupMenu.Separator(), 1);

        JMenuItem noMatches = new JMenuItem("No matching gauges");
        noMatches.setEnabled(false);
        noMatches.setVisible(false);
        popup.add(noMatches);

        Runnable focusSearch = () -> SwingUtilities.invokeLater(() -> {
            if (popup.isShowing()) {
                search.requestFocusInWindow();
            }
        });
        Runnable filter = () -> {
            String query = search.getText().trim().toLowerCase(Locale.ROOT);
            boolean anyMatch = false;
            for (JMenuItem item : gaugeItems) {
                anyMatch |= filterItem(item, "", query);
            }
            noMatches.setVisible(!anyMatch);
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

    private static boolean filterItem(JMenuItem item, String parentPath, String query) {
        String path = parentPath.isEmpty() ? item.getText() : parentPath + " / " + item.getText();
        boolean matches;
        if (item instanceof JMenu) {
            matches = query.isEmpty();
            for (Component child : ((JMenu) item).getMenuComponents()) {
                if (child instanceof JMenuItem) {
                    matches |= filterItem((JMenuItem) child, path, query);
                }
            }
        } else {
            matches = path.toLowerCase(Locale.ROOT).contains(query);
        }
        item.setVisible(matches);
        return matches;
    }
}
