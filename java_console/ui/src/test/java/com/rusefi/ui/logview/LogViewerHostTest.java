package com.rusefi.ui.logview;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.JComponent;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.TransferHandler;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;

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
