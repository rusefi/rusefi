package com.rusefi;

import org.junit.jupiter.api.Test;

import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import javax.swing.SwingUtilities;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertSame;

class StartupLogViewerEntryTest {
    @Test
    void openLogSelectsTheAppendedViewerBeforeOpeningTheChooser() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JTabbedPane tabs = new JTabbedPane();
            tabs.addTab("Update ECU firmware", new JPanel());
            tabs.addTab("Manage Tunes", new JPanel());
            tabs.addTab("Connect", new JPanel());
            JPanel viewer = new JPanel();
            JButton open = new JButton("Open data log…");
            AtomicInteger chooserOpenCount = new AtomicInteger();

            StartupFrame.installLogViewerEntry(tabs, viewer, open, chooserOpenCount::incrementAndGet);

            assertEquals(4, tabs.getTabCount());
            assertEquals("Log Viewer", tabs.getTitleAt(3));
            assertEquals(0, tabs.getSelectedIndex(), "adding the viewer must not change the saved startup tab");

            open.doClick();

            assertSame(viewer, tabs.getSelectedComponent());
            assertEquals(1, chooserOpenCount.get());
        });
    }
}
