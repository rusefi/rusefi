package com.rusefi.ui.logview;

import com.formdev.flatlaf.FlatClientProperties;
import com.opensr5.ini.DatalogEntry;
import com.opensr5.ini.GaugeCategoryModel;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.logview.CsvLogReader;
import com.rusefi.logview.LogFiles;
import com.rusefi.ui.widgets.JoinedButtons;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.JButton;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.TransferHandler;
import java.awt.Component;
import java.awt.Container;
import java.awt.Rectangle;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class LogViewerHostTest {
    @TempDir
    Path folder;

    private final List<String> errors = new ArrayList<>();
    private final List<String> titles = new ArrayList<>();

    private File copy(String resource, String name) throws IOException {
        Path target = folder.resolve(name);
        try (InputStream stream = getClass().getResourceAsStream(resource)) {
            Files.copy(stream, target);
        }
        return target.toFile();
    }

    private LogViewerHost host() throws Exception {
        AtomicReference<LogViewerHost> host = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            LogViewerHost created = new LogViewerHost();
            created.setErrorReporter(errors::add);
            created.setTitleListener(titles::add);
            host.set(created);
        });
        return host.get();
    }

    /** Opens on the event thread and waits until the loaded log or the error is shown. */
    private static void openAndWait(LogViewerHost host, File file) throws Exception {
        SwingUtilities.invokeAndWait(() -> host.open(file));
        waitUntilLoaded(host);
    }

    /** SwingWorker applies results through a Swing timer, so poll rather than join the worker. */
    private static void waitUntilLoaded(LogViewerHost host) throws Exception {
        long deadline = System.currentTimeMillis() + 10_000;
        boolean[] loading = {true};
        while (loading[0]) {
            assertTrue(System.currentTimeMillis() < deadline, "log did not finish loading");
            Thread.sleep(10);
            SwingUtilities.invokeAndWait(() -> loading[0] = host.isLoading());
        }
    }

    @Test
    void startsEmptyAndOpensALog() throws Exception {
        LogViewerHost host = host();
        assertNull(host.currentPanel());

        openAndWait(host, copy("/logview/test-log-binary-1.mlg", "drive.mlg"));
        assertNotNull(host.currentPanel());
        assertTrue(host.currentPanel().statusBar().fileLabel.getText().startsWith("drive.mlg  ·  220 samples"));
        assertEquals(Collections.singletonList("drive.mlg"), titles);
        assertTrue(errors.isEmpty(), errors.toString());
    }

    private LogViewerHost host(Node config, File logsFolder) throws Exception {
        AtomicReference<LogViewerHost> host = new AtomicReference<>();
        SwingUtilities.invokeAndWait(() -> {
            LogViewerHost created = new LogViewerHost(config, logsFolder);
            created.setErrorReporter(errors::add);
            host.set(created);
        });
        return host.get();
    }

    @Test
    void emptyCardListsRecentLogsAndOpensOnClick(@TempDir Path logs) throws Exception {
        Files.copy(getClass().getResourceAsStream("/logview/test-log-binary-1.mlg"),
                logs.resolve("rusEFI_outputChannels_2026-10-21_19_44_54_929.mlg"));
        Files.copy(getClass().getResourceAsStream("/logview/test-log-1.msl"),
                logs.resolve("rusEFI_outputChannels_2026-10-20_08_00_00_000.msl"));
        LogViewerHost host = host(new Node(), logs.toFile());
        JList<RecentLogs.Entry> list = host.recentList();
        assertEquals(2, list.getModel().getSize());
        assertEquals("rusEFI_outputChannels_2026-10-21_19_44_54_929.mlg", list.getModel().getElementAt(0).file.getName());

        // offscreen the list is never laid out; give it the size it has in a window
        list.setSize(list.getPreferredSize());
        Rectangle row = list.getCellBounds(0, 0);
        SwingUtilities.invokeAndWait(() -> list.dispatchEvent(new MouseEvent(list, MouseEvent.MOUSE_CLICKED, 0, 0,
                row.x + 20, row.y + row.height / 2, 1, false, MouseEvent.BUTTON1)));
        waitUntilLoaded(host);
        assertTrue(host.currentPanel().statusBar().fileLabel.getText().startsWith("rusEFI_outputChannels_2026-10-21"));
    }

    @Test
    void emptyLogsFolderExplainsWhereLogsComeFrom(@TempDir Path logs) throws Exception {
        LogViewerHost host = host(new Node(), logs.toFile());
        assertNull(host.recentList());
        JLabel hint = null;
        for (JLabel label : all(host, JLabel.class)) {
            if (label.getText().startsWith("Logs you record")) {
                hint = label;
            }
        }
        assertNotNull(hint);
    }

    @Test
    void logsOpenedFromElsewhereAreRememberedAcrossRestarts(@TempDir Path logs) throws Exception {
        Node config = new Node();
        File outside = copy("/logview/test-log-1.msl", "track.msl");
        LogViewerHost first = host(config, logs.toFile());
        openAndWait(first, outside);

        LogViewerHost afterRestart = host(config, logs.toFile());
        assertEquals(outside.getAbsoluteFile(), afterRestart.recentList().getModel().getElementAt(0).file);
    }

    @Test
    void externalLogDurationIsCachedOnFirstOpen(@TempDir Path logs) throws Exception {
        Node config = new Node();
        File outside = copy("/logview/test-log-1.msl", "track.msl");
        CsvLogReader expected = LogFiles.read(outside);
        LogViewerHost first = host(config, logs.toFile());
        assertTrue(Double.isNaN(first.recentEntries().isEmpty() ? Double.NaN : first.recentEntries().get(0).duration));
        openAndWait(first, outside);

        LogViewerHost afterRestart = host(config, logs.toFile());
        RecentLogs.Entry entry = afterRestart.recentList().getModel().getElementAt(0);
        assertEquals(expected.maximum(0) - expected.minimum(0), entry.duration, 1e-9);
        assertEquals("13.2 s", RecentLogs.duration(entry.duration));
        assertTrue(((JMenuItem) afterRestart.openMenu().getComponent(0)).getText().endsWith("track.msl   (13.2 s, 14.3 MB)"),
                ((JMenuItem) afterRestart.openMenu().getComponent(0)).getText());
    }

    /** Rows lay themselves out, including the nested duration/size panel: validate() is disabled in renderers. */
    @Test
    void recentRowShowsDurationAndSize(@TempDir Path logs) throws Exception {
        Node config = new Node();
        File outside = copy("/logview/test-log-1.msl", "track.msl");
        openAndWait(host(config, logs.toFile()), outside);
        JList<RecentLogs.Entry> list = host(config, logs.toFile()).recentList();
        LogViewerHost.RecentRenderer row = (LogViewerHost.RecentRenderer) list.getCellRenderer()
                .getListCellRendererComponent(list, list.getModel().getElementAt(0), 0, false, false);
        row.setBounds(0, 0, 680, 32);
        assertEquals("13.2 s", row.duration.getText());
        assertEquals("14.3 MB", row.size.getText());
        assertTrue(row.duration.getWidth() > 0 && row.size.getWidth() > 0, "both columns laid out");
    }

    @Test
    void stoppedRecordingIsListedWithItsDurationAndSaved(@TempDir Path logs) throws Exception {
        Node config = new Node();
        int[] saves = {0};
        LogViewerHost host = host(config, logs.toFile());
        host.setConfigSaver(() -> saves[0]++);
        File recording = Files.write(logs.resolve("rusEFI_outputChannels_2026-10-22_10_53_57_557.mlg"), new byte[100]).toFile();

        SwingUtilities.invokeAndWait(() -> host.recordingStopped(recording, 12.1));
        assertEquals(1, saves[0], "written to disk at once");
        RecentLogs.Entry entry = host.recentList().getModel().getElementAt(0);
        assertEquals(recording.getAbsoluteFile(), entry.file);
        assertEquals(12.1, entry.duration, 1e-9, "the empty card lists it with its duration");
        assertEquals(12.1, host(config, logs.toFile()).recentEntries().get(0).duration, 1e-9, "after a restart");
    }

    @Test
    void openingALogSavesTheConfig() throws Exception {
        LogViewerHost host = host();
        int[] saves = {0};
        host.setConfigSaver(() -> saves[0]++);
        openAndWait(host, copy("/logview/test-log-1.msl", "a.msl"));
        assertEquals(1, saves[0]);
    }

    @Test
    void openMenuListsRecentLogsWithTheOpenOneTicked(@TempDir Path logs) throws Exception {
        Files.copy(getClass().getResourceAsStream("/logview/test-log-binary-1.mlg"),
                logs.resolve("rusEFI_outputChannels_2026-10-21_19_44_54_929.mlg"));
        File outside = copy("/logview/test-log-1.msl", "track.msl");
        LogViewerHost host = host(new Node(), logs.toFile());
        openAndWait(host, outside);

        JPopupMenu menu = host.openMenu();
        List<JMenuItem> items = new ArrayList<>();
        for (Component component : menu.getComponents()) {
            if (component instanceof JMenuItem) {
                items.add((JMenuItem) component);
            }
        }
        assertEquals(4, items.size(), "two recent logs, Browse…, Open logs folder");
        JCheckBoxMenuItem openOne = null;
        for (JMenuItem item : items.subList(0, 2)) {
            if (item.getText().contains("track.msl")) {
                openOne = (JCheckBoxMenuItem) item;
            }
        }
        assertNotNull(openOne);
        assertTrue(openOne.isSelected(), "the open log is ticked");
        assertEquals("Browse…", items.get(2).getText());
        assertEquals("Open logs folder", items.get(3).getText());
    }

    @Test
    void openMenuWithoutLogsSaysSo(@TempDir Path logs) throws Exception {
        LogViewerHost host = host(new Node(), logs.toFile());
        JMenuItem first = (JMenuItem) host.openMenu().getComponent(0);
        assertEquals("No recent logs", first.getText());
        assertFalse(first.isEnabled());
    }

    private static <T> List<T> all(Container container, Class<T> type) {
        List<T> found = new ArrayList<>();
        for (Component child : container.getComponents()) {
            if (type.isInstance(child)) {
                found.add(type.cast(child));
            }
            if (child instanceof Container) {
                found.addAll(all((Container) child, type));
            }
        }
        return found;
    }

    /** Open buttons look like the console's other toolbar buttons (Edit dashboard): outlined, not blue. */
    @Test
    void openButtonsUseTheToolbarButtonStyle() throws Exception {
        LogViewerHost host = host();
        JButton empty = find(host, JButton.class);
        assertEquals("Open log…", empty.getText());
        assertTrue(empty.getParent() instanceof JoinedButtons);
        assertNull(empty.getClientProperty(FlatClientProperties.STYLE_CLASS), "not the blue primary style");

        openAndWait(host, copy("/logview/test-log-1.msl", "a.msl"));
        JButton toolbar = find(host.currentPanel(), ChartToolbar.class).openButton;
        assertTrue(toolbar.getParent() instanceof JoinedButtons);
        assertSame(empty.getIcon(), toolbar.getIcon());
        assertEquals(empty.getIconTextGap(), toolbar.getIconTextGap());
    }

    @Test
    void failedLoadKeepsPreviousLogAndReportsError() throws Exception {
        LogViewerHost host = host();
        openAndWait(host, copy("/logview/test-log-1.msl", "good.msl"));
        LogViewerPanel good = host.currentPanel();

        File broken = folder.resolve("broken.mlg").toFile();
        Files.write(broken.toPath(), new byte[]{1, 2, 3});
        openAndWait(host, broken);

        assertSame(good, host.currentPanel());
        assertEquals(1, errors.size());
        assertTrue(errors.get(0).startsWith("Could not open broken.mlg"), errors.get(0));
        assertSame(good, host.getComponent(0), "the previous log is shown again");
    }

    @Test
    void chartSettingsCarryOverToTheNextLog() throws Exception {
        LogViewerHost host = host();
        openAndWait(host, copy("/logview/test-log-1.msl", "first.msl"));
        SwingUtilities.invokeAndWait(() -> {
            ChartToolbar toolbar = find(host.currentPanel(), ChartToolbar.class);
            toolbar.darkPlot.doClick();
        });

        openAndWait(host, copy("/logview/test-log-binary-1.mlg", "second.mlg"));
        assertEquals(PlotPalette.DARK.background, host.currentPanel().chart().palette().background);
        assertTrue(find(host.currentPanel(), ChartToolbar.class).darkPlot.isSelected());
    }

    @Test
    void signalListsCarryOverAndAreSavedAsTheyChange() throws Exception {
        LogViewerHost host = host();
        int[] saves = {0};
        host.setConfigSaver(() -> saves[0]++);
        openAndWait(host, copy("/logview/test-log-1.msl", "first.msl"));
        int afterOpen = saves[0];
        SwingUtilities.invokeAndWait(() -> {
            host.currentPanel().namePrompt = (title, initial) -> "Mine";
            host.currentPanel().newList(1);
        });
        assertTrue(saves[0] > afterOpen, "saved without waiting for the console to exit");

        openAndWait(host, copy("/logview/test-log-binary-1.mlg", "second.mlg"));
        assertEquals(Collections.singletonList("Mine"), host.currentPanel().lists().customNames());
    }

    @Test
    void sidebarStateIsKeptInTheViewerConfig() throws Exception {
        Node config = new Node();
        LogViewerHost host = host(config, new File("."));
        openAndWait(host, copy("/logview/test-log-1.msl", "first.msl"));
        SwingUtilities.invokeAndWait(() -> host.currentPanel().hideConstantBox.doClick());
        assertTrue(new SidebarState(config.getChild("sidebar")).hideConstant(), "saved with the console config");
    }

    @Test
    void theEcuIniGroupsTheSignals() throws Exception {
        LogViewerHost host = host();
        IniFileModel ini = mock(IniFileModel.class);
        GaugeModel gauge = mock(GaugeModel.class);
        when(gauge.getChannel()).thenReturn("oddChannel");
        Map<String, GaugeCategoryModel> categories = new LinkedHashMap<>();
        categories.put("Knock", new GaugeCategoryModel("Knock", Collections.singletonList(gauge)));
        when(ini.getGaugeCategories()).thenReturn(categories);
        // a name no rule places: the .ini's [Datalog] gives its channel, a Knock gauge shows that channel
        when(ini.getDatalogEntries()).thenReturn(Collections.singletonList(new DatalogEntry("oddChannel", "RPM")));
        host.setIniSupplier(() -> ini);
        openAndWait(host, copy("/logview/test-log-1.msl", "first.msl"));
        CsvLogReader log = host.currentPanel().chart().log();
        int rpm = -1;
        for (int field = 1; field < log.fieldCount(); field++) {
            if (log.name(field).equals("RPM")) {
                rpm = field;
            }
        }
        assertEquals(SignalCategories.Group.KNOCK, host.currentPanel().categories().group(rpm));
    }

    @Test
    void aFailingIniSupplierDoesNotStopTheLogOpening() throws Exception {
        LogViewerHost host = host();
        host.setIniSupplier(() -> {
            throw new IllegalStateException("no ECU");
        });
        openAndWait(host, copy("/logview/test-log-1.msl", "first.msl"));
        assertNotNull(host.currentPanel(), "grouped by name instead");
    }

    @Test
    void droppedLogFileOpens() throws Exception {
        LogViewerHost host = host();
        File log = copy("/logview/test-log-binary-1.mlg", "dropped.mlg");
        boolean[] accepted = new boolean[1];
        SwingUtilities.invokeAndWait(() -> accepted[0] = drop(host, log));
        assertTrue(accepted[0]);
        waitUntilLoaded(host);
        assertNotNull(host.currentPanel());
        assertTrue(host.currentPanel().statusBar().fileLabel.getText().startsWith("dropped.mlg"));
    }

    @Test
    void droppedUnsupportedFileIsRejected() throws Exception {
        LogViewerHost host = host();
        File tune = folder.resolve("tune.msq").toFile();
        Files.write(tune.toPath(), new byte[]{1});
        boolean[] accepted = new boolean[1];
        SwingUtilities.invokeAndWait(() -> accepted[0] = drop(host, tune));
        assertFalse(accepted[0]);
        assertFalse(host.isLoading());
        assertEquals(Collections.singletonList("Only .mlg or .msl data logs can be opened."), errors);
    }

    @Test
    void dropsAreAcceptedOnTheChartAndSignalList() throws Exception {
        LogViewerHost host = host();
        openAndWait(host, copy("/logview/test-log-1.msl", "a.msl"));
        // the chart and list keep their signal handlers, which pass log files on to the host
        SignalTransfer.Handler chartHandler = (SignalTransfer.Handler) host.currentPanel().chart().getTransferHandler();
        SignalTransfer.Handler listHandler = (SignalTransfer.Handler) host.currentPanel().fieldList().getTransferHandler();
        assertSame(host.fileDropHandler(), chartHandler.fileDrop());
        assertSame(host.fileDropHandler(), listHandler.fileDrop());
        // the search field keeps text drops
        assertFalse(host.currentPanel().searchField().getTransferHandler() == host.fileDropHandler());
    }

    @Test
    void logFileDroppedOnTheChartStillOpens() throws Exception {
        LogViewerHost host = host();
        openAndWait(host, copy("/logview/test-log-1.msl", "first.msl"));
        File second = copy("/logview/test-log-binary-1.mlg", "second.mlg");
        LogChart chart = host.currentPanel().chart();
        boolean[] accepted = new boolean[1];
        SwingUtilities.invokeAndWait(() -> accepted[0] = chart.getTransferHandler()
                .importData(new TransferHandler.TransferSupport(chart, files(second))));
        assertTrue(accepted[0]);
        waitUntilLoaded(host);
        assertTrue(host.currentPanel().statusBar().fileLabel.getText().startsWith("second.mlg"));
    }

    @Test
    void ctrlOOpensTheChooser() throws Exception {
        LogViewerHost host = host();
        KeyStroke ctrlO = KeyStroke.getKeyStroke(KeyEvent.VK_O, InputEvent.CTRL_DOWN_MASK);
        Object action = host.getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT).get(ctrlO);
        assertEquals("openLog", action);
        assertNotNull(host.getActionMap().get(action));
    }

    private static boolean drop(LogViewerHost host, File file) {
        return host.fileDropHandler().importData(new TransferHandler.TransferSupport(host, files(file)));
    }

    private static Transferable files(File file) {
        return new Transferable() {
            @Override
            public DataFlavor[] getTransferDataFlavors() {
                return new DataFlavor[]{DataFlavor.javaFileListFlavor};
            }

            @Override
            public boolean isDataFlavorSupported(DataFlavor flavor) {
                return DataFlavor.javaFileListFlavor.equals(flavor);
            }

            @Override
            public Object getTransferData(DataFlavor flavor) {
                return Collections.singletonList(file);
            }
        };
    }

    private static <T> T find(java.awt.Container container, Class<T> type) {
        for (java.awt.Component child : container.getComponents()) {
            if (type.isInstance(child)) {
                return type.cast(child);
            }
            if (child instanceof java.awt.Container) {
                T found = find((java.awt.Container) child, type);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }
}
