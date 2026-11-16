package com.rusefi;

import org.junit.jupiter.api.Test;

import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import javax.swing.SwingUtilities;
import java.io.File;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
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

    @Test
    void recognizesSupportedLogArgumentsWithoutTakingOverPortArguments() {
        assertEquals(new File("drive.mlg"), ConsoleUI.logFileArgument(new String[]{"drive.mlg"}));
        assertEquals(new File("DRIVE.MSL"), ConsoleUI.logFileArgument(new String[]{"DRIVE.MSL"}));
        assertNull(ConsoleUI.logFileArgument(new String[]{"COM3"}));
        assertNull(ConsoleUI.logFileArgument(new String[0]));
    }

    @Test
    void initialLogSelectsTheViewerAndOpensTheFile() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JTabbedPane tabs = new JTabbedPane();
            JPanel connect = new JPanel();
            JPanel viewer = new JPanel();
            tabs.addTab("Connect", connect);
            tabs.addTab("Log Viewer", viewer);
            File log = new File("drive.mlg");
            AtomicReference<File> opened = new AtomicReference<>();

            StartupFrame.openInitialLog(tabs, viewer, opened::set, log);

            assertSame(viewer, tabs.getSelectedComponent());
            assertEquals(log, opened.get());
        });
    }

    @Test
    void forwardedLogWaitsUntilAViewerIsReady() throws Exception {
        ConsoleUI.ForwardedLogRequests requests = new ConsoleUI.ForwardedLogRequests();
        File log = new File("forwarded.mlg");
        AtomicReference<File> opened = new AtomicReference<>();

        SwingUtilities.invokeAndWait(() -> requests.submit(log));
        assertNull(opened.get());

        SwingUtilities.invokeAndWait(() -> requests.setOpener(opened::set));
        assertEquals(log, opened.get());
    }
}
