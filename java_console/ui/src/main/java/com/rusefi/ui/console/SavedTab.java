package com.rusefi.ui.console;

import com.rusefi.core.preferences.storage.Node;

import javax.swing.JTabbedPane;

/**
 * The main tab the console reopens on startup. It is stored by title, so adding a tab in the middle of
 * the row does not reopen the console on a neighbouring tab. Configs from before titles were stored only
 * have the index in the tab order of that time, which lacked the Log Viewer tab; such an index is shifted
 * past the Log Viewer's position.
 */
public final class SavedTab {
    static final String INDEX_KEY = "main_tab";
    static final String TITLE_KEY = "main_tab_title";
    /** Where the Log Viewer tab was inserted (after Gauges); legacy indexes at or after it move by one. */
    static final int LOG_VIEWER_POSITION = 1;

    private final String title;
    private final int index;

    private SavedTab(String title, int index) {
        this.title = title;
        this.index = index;
    }

    public static SavedTab read(Node root) {
        String title = root.getProperty(TITLE_KEY, null);
        if (title != null && !title.isEmpty()) {
            return new SavedTab(title, -1);
        }
        int legacy = root.getIntProperty(INDEX_KEY, 0);
        return new SavedTab(null, legacy >= LOG_VIEWER_POSITION ? legacy + 1 : legacy);
    }

    /** Whether the tab with this title, being added at {@code index}, is the one that will be shown first. */
    public boolean is(String tabTitle, int tabIndex) {
        return title != null ? title.equals(tabTitle) : index == tabIndex;
    }

    /** Selects the saved tab if it still exists; otherwise the current selection stays. */
    public void select(JTabbedPane pane) {
        int target = title != null ? pane.indexOfTab(title) : index;
        if (target >= 0 && target < pane.getTabCount()) {
            pane.setSelectedIndex(target);
        }
    }

    /** Saves the selected tab's title, and its index for older consoles reading the same config. */
    public static void save(Node root, JTabbedPane pane) {
        int selected = pane.getSelectedIndex();
        root.setProperty(INDEX_KEY, selected);
        if (selected >= 0) {
            root.setProperty(TITLE_KEY, pane.getTitleAt(selected));
        }
    }
}
