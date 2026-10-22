package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import javax.swing.JList;
import javax.swing.TransferHandler;
import java.awt.Component;
import java.awt.GraphicsEnvironment;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.StringReader;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class LogViewerPanelTest {
    /** Samples one second apart over 0..20 s; a log opens with nothing plotted. */
    private static CsvLogReader log() throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\tCLT\tIAT\ns\trpm\tC\tC\n");
        for (int i = 0; i <= 20; i++) {
            text.append(i).append('\t').append(i * 100).append('\t').append(80).append('\t').append(30).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    private static LogChart laidOutChart(LogViewerPanel panel) {
        LogChart chart = panel.chart();
        chart.setSize(600, 400);
        chart.paint(new BufferedImage(600, 400, BufferedImage.TYPE_INT_RGB).createGraphics());
        return chart;
    }

    @Test
    void togglingSignalsUpdatesCountAndKeepsColours() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), "test.msl");
        assertEquals("none plotted", panel.plottedLabel().getText(), "a log opens with an empty plot");
        assertEquals(1, panel.chart().plotLayout().plotCount());

        panel.toggle(1);
        assertEquals("1 plotted", panel.plottedLabel().getText());
        panel.toggle(3);
        panel.toggle(2);
        assertEquals("3 plotted", panel.plottedLabel().getText());
        panel.toggle(1);
        assertEquals("2 plotted", panel.plottedLabel().getText());
        panel.toggle(2);
        panel.toggle(3);
        assertEquals("none plotted", panel.plottedLabel().getText());
    }

    @Test
    void shiftClickPutsASignalInANewPlot() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        panel.toggle(1);
        panel.addToNewPlot(2);
        PlotLayout layout = panel.chart().plotLayout();
        assertEquals(2, layout.plotCount());
        assertEquals(1, layout.plotOf(2));
        assertEquals("2 plotted · 2 plots", panel.plottedLabel().getText());

        // a plain click goes to the active (new) plot
        panel.toggle(3);
        assertEquals(1, layout.plotOf(3));
    }

    @Test
    void listDragsTheSelectedSignalAndRemovesTracesDroppedOnIt() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        LogViewerPanel.ListTransferHandler handler = (LogViewerPanel.ListTransferHandler) panel.fieldList().getTransferHandler();
        // drag gestures need a display; CI runs headless, where the panel must still build
        assertEquals(!GraphicsEnvironment.isHeadless(), panel.fieldList().getDragEnabled());

        selectSignal(panel, 2);
        assertEquals(2, SignalTransfer.field(handler.createTransferable(panel.fieldList())));

        // dragging a plotted signal's legend item back onto the list removes it
        panel.toggle(1);
        assertTrue(handler.importData(new TransferHandler.TransferSupport(panel.fieldList(), SignalTransfer.of(1))));
        assertFalse(panel.chart().plotLayout().isPlotted(1));
        assertFalse(handler.importData(new TransferHandler.TransferSupport(panel.fieldList(), SignalTransfer.of(3))),
                "an unplotted signal dropped on the list changes nothing");
    }

    /** The centre of the first list row showing {@code field}, with the list laid out. */
    private static java.awt.Point rowCentre(LogViewerPanel panel, int field) {
        JList<SignalSections.Row> list = panel.fieldList();
        list.setSize(260, 2000);
        for (int index = 0; index < list.getModel().getSize(); index++) {
            if (list.getModel().getElementAt(index).field == field) {
                java.awt.Rectangle cell = list.getCellBounds(index, index);
                return new java.awt.Point(cell.x + cell.width / 2, cell.y + cell.height / 2);
            }
        }
        throw new AssertionError("no row for field " + field);
    }

    /**
     * Sends what AWT would deliver to the list's row handler (called directly: headless Swing's own list handler
     * throws on a mouse press).
     */
    private static void mouse(LogViewerPanel panel, int id, java.awt.Point at, int clickCount) {
        int modifiers = id == MouseEvent.MOUSE_RELEASED || id == MouseEvent.MOUSE_CLICKED ? 0 : MouseEvent.BUTTON1_DOWN_MASK;
        MouseEvent event = new MouseEvent(panel.fieldList(), id, 0, modifiers, at.x, at.y, clickCount, false,
                MouseEvent.BUTTON1);
        switch (id) {
            case MouseEvent.MOUSE_PRESSED:
                panel.rowMouse.mousePressed(event);
                break;
            case MouseEvent.MOUSE_DRAGGED:
                panel.rowMouse.mouseDragged(event);
                break;
            case MouseEvent.MOUSE_RELEASED:
                panel.rowMouse.mouseReleased(event);
                break;
            default:
                panel.rowMouse.mouseClicked(event);
        }
    }

    /**
     * BUG (reproduction): AWT sends no MOUSE_CLICKED when the pointer moved between press and release, so a click
     * that wobbles by a pixel or two adds nothing.
     */
    @Test
    void clickThatMovesAPixelDoesNotPlot() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        java.awt.Point at = rowCentre(panel, 2);
        java.awt.Point wobble = new java.awt.Point(at.x + 2, at.y + 1);
        mouse(panel, MouseEvent.MOUSE_PRESSED, at, 1);
        mouse(panel, MouseEvent.MOUSE_DRAGGED, wobble, 1);
        mouse(panel, MouseEvent.MOUSE_RELEASED, wobble, 1);
        assertEquals("none plotted", panel.plottedLabel().getText(), "BUG: the wobbly click is lost");
    }

    /**
     * BUG (reproduction): plotting a signal adds a row to Plotted above it, so the list shifts under the pointer and
     * the second click of a double-click (or a quick second click) lands on the signal that moved there.
     */
    @Test
    void secondClickLandsOnTheRowThatMovedUnderThePointer() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        java.awt.Point at = rowCentre(panel, 2);
        for (int clickCount = 1; clickCount <= 2; clickCount++) {
            mouse(panel, MouseEvent.MOUSE_PRESSED, at, clickCount);
            mouse(panel, MouseEvent.MOUSE_RELEASED, at, clickCount);
            mouse(panel, MouseEvent.MOUSE_CLICKED, at, clickCount);
        }
        PlotLayout layout = panel.chart().plotLayout();
        assertTrue(layout.isPlotted(2), "the first click plotted CLT");
        assertTrue(layout.isPlotted(1), "BUG: the second click plotted RPM, which moved under the pointer");
    }

    /** Selects the first list row showing {@code field}. */
    static void selectSignal(LogViewerPanel panel, int field) {
        for (int index = 0; index < panel.fieldList().getModel().getSize(); index++) {
            if (panel.fieldList().getModel().getElementAt(index).field == field) {
                panel.fieldList().setSelectedIndex(index);
                return;
            }
        }
        throw new AssertionError("no row for field " + field);
    }

    /** Row renderers must lay themselves out: validate() does nothing without a native peer. */
    @Test
    void signalRowPaintsWithoutAWindow() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        JList<SignalSections.Row> list = panel.fieldList();
        Component row = list.getCellRenderer().getListCellRendererComponent(list,
                SignalSections.Row.signal(SignalSections.PLOTTED, SignalSections.Kind.PLOTTED, 1), 0, false, false);
        row.setBounds(0, 0, 260, 28);
        BufferedImage image = new BufferedImage(260, 28, BufferedImage.TYPE_INT_RGB);
        row.paint(image.createGraphics());
        int background = image.getRGB(0, 0);
        int ink = 0;
        for (int y = 0; y < 28; y++) {
            for (int x = 0; x < 260; x++) {
                if (image.getRGB(x, y) != background) {
                    ink++;
                }
            }
        }
        assertTrue(ink > 50, "row should show its dot, name and unit: " + ink);
    }

    @Test
    void signalListFitsSidebarWidth() throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\tA very long signal name that is wider than the sidebar itself\ns\trpm\tkPa\n");
        text.append("0\t1\t2\n1\t2\t3\n");
        LogViewerPanel panel = new LogViewerPanel(CsvLogReader.read(new StringReader(text.toString())), null);
        assertTrue(panel.fieldList().getScrollableTracksViewportWidth());
    }

    @Test
    void statusShowsFileAndWholeLog() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), "test.msl");
        assertEquals("test.msl  ·  21 samples  ·  3 signals", panel.statusBar().fileLabel.getText());
        assertEquals("Whole log  0.00–20.00 s", panel.statusBar().viewLabel.getText());
        assertEquals("", panel.statusBar().cursorLabel.getText());
    }

    @Test
    void statusFollowsZoomAndCursor() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        LogChart chart = laidOutChart(panel);
        int middle = (54 + 578) / 2;

        chart.dispatchEvent(new MouseWheelEvent(chart, MouseEvent.MOUSE_WHEEL, 0, 0, middle, 200, 0, false,
                MouseWheelEvent.WHEEL_UNIT_SCROLL, 1, -3));
        assertTrue(panel.statusBar().viewLabel.getText().startsWith("Showing "), panel.statusBar().viewLabel.getText());
        assertTrue(panel.statusBar().viewLabel.getText().endsWith(" of 0.00–20.00 s"));
        assertTrue(panel.statusBar().cursorLabel.getText().matches("Cursor 10\\.0+ s"), panel.statusBar().cursorLabel.getText());

        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_EXITED, 0, 0, 700, 200, 0, false));
        assertEquals("", panel.statusBar().cursorLabel.getText());

        chart.dispatchEvent(new MouseEvent(chart, MouseEvent.MOUSE_CLICKED, 0, 0, middle, 200, 2, false, MouseEvent.BUTTON1));
        assertEquals("Whole log  0.00–20.00 s", panel.statusBar().viewLabel.getText());
    }

    @Test
    void statusUsesMoreDecimalsWhenZoomedIn() {
        TimeWindow window = new TimeWindow(0, 100, 0.01);
        window.show(12.3456, 13.3456);
        LogStatusBar status = new LogStatusBar(null, logUnchecked());
        status.update(window, 12.5);
        assertEquals("Showing 12.346–13.346 s of 0.000–100.000 s", status.viewLabel.getText());
        assertEquals("Cursor 12.500 s", status.cursorLabel.getText());
    }

    private static CsvLogReader logUnchecked() {
        try {
            return log();
        } catch (IOException e) {
            throw new AssertionError(e);
        }
    }
}
