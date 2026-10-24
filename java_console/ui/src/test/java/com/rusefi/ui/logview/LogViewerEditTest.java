package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import com.rusefi.logview.LogFiles;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPopupMenu;
import javax.swing.SwingUtilities;
import java.awt.Component;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/** Exporting the visible data and dropping data before or after the cursor, with Save as… and Undo. */
class LogViewerEditTest {
    @TempDir
    Path folder;

    private final List<String> errors = new ArrayList<>();
    private final List<String> titles = new ArrayList<>();
    private final List<File> suggested = new ArrayList<>();
    private final List<String> questions = new ArrayList<>();
    /** What the save dialog answers: a file name in {@link #folder}, or null to cancel. */
    private String saveAs;
    private int promptAnswer = JOptionPane.CANCEL_OPTION;

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
            created.saveChooser = file -> {
                suggested.add(file);
                return saveAs == null ? null : folder.resolve(saveAs).toFile();
            };
            created.unsavedPrompt = question -> {
                questions.add(question);
                return promptAnswer;
            };
            host.set(created);
        });
        return host.get();
    }

    private static void openAndWait(LogViewerHost host, File file) throws Exception {
        SwingUtilities.invokeAndWait(() -> host.open(file));
        long deadline = System.currentTimeMillis() + 10_000;
        boolean[] loading = {true};
        while (loading[0]) {
            assertTrue(System.currentTimeMillis() < deadline, "log did not finish loading");
            Thread.sleep(10);
            SwingUtilities.invokeAndWait(() -> loading[0] = host.isLoading());
        }
    }

    private LogViewerHost hostWithLog() throws Exception {
        LogViewerHost host = host();
        openAndWait(host, copy("/logview/test-log-binary-1.mlg", "drive.mlg"));
        return host;
    }

    private static CsvLogReader shown(LogViewerHost host) {
        return host.currentPanel().chart().log();
    }

    private static JMenuItem item(JPopupMenu menu, String prefix) {
        for (Component component : menu.getComponents()) {
            if (component instanceof JMenuItem && ((JMenuItem) component).getText().startsWith(prefix)) {
                return (JMenuItem) component;
            }
        }
        throw new AssertionError("no " + prefix + " item");
    }

    private static void click(LogViewerHost host, int sample, String prefix) throws Exception {
        SwingUtilities.invokeAndWait(() -> item(host.currentPanel().chart().editMenu(sample), prefix).doClick());
    }

    @Test
    void menuOffersTheEditsAtTheClickedSample() throws Exception {
        LogViewerHost host = hostWithLog();
        LogChart chart = host.currentPanel().chart();
        JPopupMenu menu = chart.editMenu(0);
        assertTrue(item(menu, "Export visible data as").isEnabled());
        assertFalse(item(menu, "Drop data before cursor (0.00 s)").isEnabled(), "nothing before the first sample");
        assertTrue(item(menu, "Drop data after cursor").isEnabled());
        int last = chart.log().sampleCount() - 1;
        assertFalse(item(chart.editMenu(last), "Drop data after cursor").isEnabled(), "nothing after the last sample");
    }

    @Test
    void dropBeforeKeepsThePlotsAndUndoBringsTheDataBack() throws Exception {
        LogViewerHost host = hostWithLog();
        CsvLogReader original = shown(host);
        host.currentPanel().chart().plotLayout().addToNewPlot(3);
        assertFalse(host.editBar().isVisible());

        click(host, 100, "Drop data before cursor");
        CsvLogReader trimmed = shown(host);
        assertEquals(original.sampleCount() - 100, trimmed.sampleCount());
        assertEquals(original.value(100, 0), trimmed.value(0, 0));
        assertEquals(2, host.currentPanel().chart().plotLayout().plotCount(), "the plots stay");
        assertTrue(host.editBar().isVisible());
        assertTrue(host.editBar().detailLabel.getText().startsWith("Dropped 0.00–"), host.editBar().detailLabel.getText());
        assertTrue(host.editBar().detailLabel.getText().endsWith("(100 samples)"));
        assertEquals("drive.mlg (edited)", titles.get(titles.size() - 1));

        click(host, 10, "Drop data after cursor");
        assertEquals(11, shown(host).sampleCount());
        assertTrue(host.editBar().detailLabel.getText().endsWith("· 2 edits"));

        SwingUtilities.invokeAndWait(host::undo);
        assertEquals(trimmed.sampleCount(), shown(host).sampleCount());
        SwingUtilities.invokeAndWait(host::undo);
        assertEquals(original.sampleCount(), shown(host).sampleCount());
        assertFalse(host.editBar().isVisible());
        assertEquals("drive.mlg", titles.get(titles.size() - 1));
        assertTrue(errors.isEmpty(), errors.toString());
    }

    @Test
    void exportWritesTheVisibleSamples() throws Exception {
        LogViewerHost host = hostWithLog();
        CsvLogReader log = shown(host);
        double start = log.value(50, 0);
        double end = log.value(80, 0);
        SwingUtilities.invokeAndWait(() -> host.currentPanel().chart().timeWindow().show(start, end));
        saveAs = "part.msl";
        click(host, 60, "Export visible data as");

        assertEquals(folder.resolve("drive_" + String.format(java.util.Locale.ROOT, "%.1f-%.1fs.msl", start, end)).toFile(),
                suggested.get(0));
        CsvLogReader exported = LogFiles.read(folder.resolve("part.msl").toFile());
        assertEquals(31, exported.sampleCount());
        for (int field = 0; field < log.fieldCount(); field++) {
            assertEquals(log.name(field), exported.name(field));
            assertEquals(log.value(50, field), exported.value(0, field),
                    Math.abs(log.value(50, field)) * 1e-6 + 1e-6, log.name(field));
        }
        assertFalse(host.editBar().isVisible(), "exporting is not an edit");
    }

    @Test
    void savingTheEditedLogMakesItTheShownLog() throws Exception {
        LogViewerHost host = hostWithLog();
        click(host, 100, "Drop data before cursor");
        int samples = shown(host).sampleCount();

        saveAs = null;
        SwingUtilities.invokeAndWait(() -> host.editBar().saveButton.doClick());
        assertEquals(folder.resolve("drive_edited.msl").toFile(), suggested.get(0));
        assertTrue(host.hasUnsavedEdits(), "cancelled: still edited");

        saveAs = "drive_edited.msl";
        SwingUtilities.invokeAndWait(() -> host.editBar().saveButton.doClick());
        assertFalse(host.hasUnsavedEdits());
        assertFalse(host.editBar().isVisible());
        assertEquals("drive_edited.msl", titles.get(titles.size() - 1));
        assertEquals(samples, LogFiles.read(folder.resolve("drive_edited.msl").toFile()).sampleCount());
    }

    @Test
    void openingAnotherLogOffersToSaveTheEdits() throws Exception {
        LogViewerHost host = hostWithLog();
        File other = copy("/logview/test-log-1.msl", "other.msl");
        click(host, 100, "Drop data before cursor");
        int edited = shown(host).sampleCount();

        promptAnswer = JOptionPane.CANCEL_OPTION;
        openAndWait(host, other);
        assertEquals(1, questions.size());
        assertEquals(edited, shown(host).sampleCount(), "cancelled: the edited log stays");

        promptAnswer = JOptionPane.YES_OPTION;
        saveAs = "kept.msl";
        openAndWait(host, other);
        assertTrue(folder.resolve("kept.msl").toFile().isFile(), "saved first");
        assertEquals("other.msl", titles.get(titles.size() - 1));
        assertFalse(host.hasUnsavedEdits());

        click(host, 1, "Drop data before cursor");
        promptAnswer = JOptionPane.NO_OPTION;
        openAndWait(host, other);
        assertEquals(3, questions.size());
        assertFalse(host.hasUnsavedEdits(), "dropped without saving");
        assertFalse(host.editBar().isVisible());
    }
}
