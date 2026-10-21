package com.rusefi.ui.console;

import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;

import javax.swing.JPanel;
import javax.swing.JTabbedPane;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class SavedTabTest {
    private static JTabbedPane tabs(String... titles) {
        JTabbedPane pane = new JTabbedPane();
        for (String title : titles) {
            pane.addTab(title, new JPanel());
        }
        return pane;
    }

    @Test
    void restoresByTitleWhenTabsAreInsertedBeforeIt() {
        Node root = new Node();
        JTabbedPane before = tabs("Gauges", "Messages", "Tuning");
        before.setSelectedIndex(2);
        SavedTab.save(root, before);

        JTabbedPane after = tabs("Gauges", "Log Viewer", "Messages", "Tuning");
        SavedTab saved = SavedTab.read(root);
        saved.select(after);
        assertEquals("Tuning", after.getTitleAt(after.getSelectedIndex()));
        assertTrue(saved.is("Tuning", 0), "title decides, not the index");
        assertFalse(saved.is("Messages", 3));
    }

    @Test
    void legacyIndexSkipsTheInsertedLogViewer() {
        Node root = new Node();
        // a config from before titles were saved: Tuning was index 2 of Gauges, Messages, Tuning
        root.setProperty(SavedTab.INDEX_KEY, 2);
        SavedTab saved = SavedTab.read(root);
        JTabbedPane pane = tabs("Gauges", "Log Viewer", "Messages", "Tuning");
        assertTrue(saved.is("Tuning", 3));
        saved.select(pane);
        assertEquals(3, pane.getSelectedIndex());
    }

    @Test
    void legacyGaugesStaysOnGauges() {
        Node root = new Node();
        root.setProperty(SavedTab.INDEX_KEY, 0);
        JTabbedPane pane = tabs("Gauges", "Log Viewer", "Messages");
        pane.setSelectedIndex(2);
        SavedTab.read(root).select(pane);
        assertEquals(0, pane.getSelectedIndex());
    }

    @Test
    void newConfigOpensTheFirstTab() {
        SavedTab saved = SavedTab.read(new Node());
        assertTrue(saved.is("Gauges", 0));
    }

    @Test
    void missingTabKeepsTheCurrentSelection() {
        Node root = new Node();
        root.setProperty(SavedTab.TITLE_KEY, "Removed Tab");
        JTabbedPane pane = tabs("Gauges", "Log Viewer");
        pane.setSelectedIndex(1);
        SavedTab.read(root).select(pane);
        assertEquals(1, pane.getSelectedIndex());
    }

    @Test
    void savesTheIndexTooForOlderConsoles() {
        Node root = new Node();
        JTabbedPane pane = tabs("Gauges", "Log Viewer", "Messages");
        pane.setSelectedIndex(2);
        SavedTab.save(root, pane);
        assertEquals(2, root.getIntProperty(SavedTab.INDEX_KEY, -1));
        assertEquals("Messages", root.getProperty(SavedTab.TITLE_KEY));
    }
}
