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
    /** Samples one second apart over 0..20 s; a log opens with RPM plotted (it has no TPS). */
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
        assertEquals("1 plotted", panel.plottedLabel().getText(), "a log opens with RPM plotted");
        assertTrue(panel.chart().plotLayout().isPlotted(1));
        assertEquals(1, panel.chart().plotLayout().plotCount());

        panel.toggle(1);
        assertEquals("none plotted", panel.plottedLabel().getText());
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
    void aLogOpensWithRpmAndTpsPlottedWhenItHasThem() throws IOException {
        String text = "Time\tCLT\tTPS\tRPM\ns\tC\t%\trpm\n0\t80\t10\t900\n1\t81\t20\t1200\n";
        LogViewerPanel panel = new LogViewerPanel(CsvLogReader.read(new StringReader(text)), null);
        PlotLayout layout = panel.chart().plotLayout();
        assertEquals("2 plotted", panel.plottedLabel().getText());
        assertTrue(layout.isPlotted(2), "TPS");
        assertTrue(layout.isPlotted(3), "RPM");
        assertFalse(layout.isPlotted(1), "CLT");
        assertEquals(1, layout.plotCount(), "both in one plot");

        String neither = "Time\tCLT\ns\tC\n0\t80\n1\t81\n";
        LogViewerPanel empty = new LogViewerPanel(CsvLogReader.read(new StringReader(neither)), null);
        assertEquals("none plotted", empty.plottedLabel().getText(), "a log without them opens with an empty plot");
    }

    @Test
    void aSignalDroppedOnAPlotSectionMovesThere() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
        panel.toggle(1);
        panel.addToNewPlot(2);
        PlotLayout layout = panel.chart().plotLayout();
        int plot1 = headingIndex(panel, SignalSections.plotTitle(0));
        assertTrue(panel.dropOnList(2, plot1));
        assertEquals(0, layout.plotOf(2), "moved to plot 1");

        assertTrue(panel.dropOnList(3, headingIndex(panel, SignalSections.plotTitle(1))));
        assertEquals(1, layout.plotOf(3), "an unplotted signal is added there");
    }

    private static int headingIndex(LogViewerPanel panel, String section) {
        for (int index = 0; index < panel.fieldList().getModel().getSize(); index++) {
            SignalSections.Row row = panel.fieldList().getModel().getElementAt(index);
            if (row.isHeading() && row.section.equals(section)) {
                return index;
            }
        }
        throw new AssertionError("no heading " + section);
    }

    @Test
    void shiftClickPutsASignalInANewPlot() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
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
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
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
        if (list.getWidth() == 0) {
            list.setSize(260, list.getPreferredSize().height);
        }
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

    /** AWT sends no MOUSE_CLICKED when the pointer moved between press and release; the click still counts. */
    @Test
    void clickThatMovesAPixelStillPlots() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
        java.awt.Point at = rowCentre(panel, 2);
        java.awt.Point wobble = new java.awt.Point(at.x + 2, at.y + 1);
        mouse(panel, MouseEvent.MOUSE_PRESSED, at, 1);
        mouse(panel, MouseEvent.MOUSE_DRAGGED, wobble, 1);
        mouse(panel, MouseEvent.MOUSE_RELEASED, wobble, 1);
        assertEquals("1 plotted", panel.plottedLabel().getText());
        assertTrue(panel.chart().plotLayout().isPlotted(2));
    }

    /**
     * The drag threshold can be 2 px: a slightly moving click then starts a drag and drop. Dropped back on the row it
     * came from, it is the click it was meant to be.
     */
    @Test
    void dragDroppedBackOnItsRowIsAClick() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        LogViewerPanel.ListTransferHandler handler = (LogViewerPanel.ListTransferHandler) panel.fieldList().getTransferHandler();
        selectSignal(panel, 2);
        int origin = panel.fieldList().getSelectedIndex();
        handler.createTransferable(panel.fieldList());
        assertTrue(panel.dropOnList(2, origin));
        assertTrue(panel.chart().plotLayout().isPlotted(2));

        selectSignal(panel, 3);
        handler.createTransferable(panel.fieldList());
        assertFalse(panel.dropOnList(3, panel.fieldList().getSelectedIndex() + 1),
                "dropped on another row, an unplotted signal changes nothing");
        assertFalse(panel.chart().plotLayout().isPlotted(3));
    }

    @Test
    void aDragIsNotAClick() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
        java.awt.Point at = rowCentre(panel, 2);
        java.awt.Point away = new java.awt.Point(at.x + 40, at.y + 30);
        mouse(panel, MouseEvent.MOUSE_PRESSED, at, 1);
        mouse(panel, MouseEvent.MOUSE_DRAGGED, away, 1);
        mouse(panel, MouseEvent.MOUSE_RELEASED, away, 1);
        assertEquals("none plotted", panel.plottedLabel().getText(), "dragging a signal is drag and drop");
    }

    /**
     * Plotting a signal adds a row to Plotted above it, so the list shifts under the pointer: the second click of a
     * double-click is ignored instead of landing on the signal that moved there (or undoing the first).
     */
    @Test
    void doubleClickPlotsOnce() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
        java.awt.Point at = rowCentre(panel, 2);
        for (int clickCount = 1; clickCount <= 2; clickCount++) {
            mouse(panel, MouseEvent.MOUSE_PRESSED, at, clickCount);
            mouse(panel, MouseEvent.MOUSE_RELEASED, at, clickCount);
            mouse(panel, MouseEvent.MOUSE_CLICKED, at, clickCount);
        }
        PlotLayout layout = panel.chart().plotLayout();
        assertTrue(layout.isPlotted(2), "the first click plotted CLT");
        assertFalse(layout.isPlotted(1), "the second click does not plot RPM, which moved under the pointer");
        assertEquals("1 plotted", panel.plottedLabel().getText());
    }

    /** Where the list can scroll, it scrolls by the row added to Plotted, keeping the clicked row in place. */
    @Test
    void clickedRowStaysUnderThePointer() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        JList<SignalSections.Row> list = panel.fieldList();
        javax.swing.JViewport viewport = (javax.swing.JViewport) list.getParent();
        // a viewport shorter than the list: the default sections and the groups of three signals
        list.setSize(260, list.getPreferredSize().height);
        viewport.setSize(260, 3 * list.getFixedCellHeight());
        viewport.setViewPosition(new java.awt.Point(0, 0));
        java.awt.Point at = rowCentre(panel, 2);
        int onScreen = at.y - viewport.getViewPosition().y;

        mouse(panel, MouseEvent.MOUSE_PRESSED, at, 1);
        mouse(panel, MouseEvent.MOUSE_RELEASED, at, 1);
        assertTrue(panel.chart().plotLayout().isPlotted(2));
        int index = -1;
        for (int i = 0; i < list.getModel().getSize(); i++) {
            SignalSections.Row row = list.getModel().getElementAt(i);
            if (row.field == 2 && row.section.equals(SignalSections.COMMON)) {
                index = i;
            }
        }
        java.awt.Rectangle cell = list.getCellBounds(index, index);
        assertEquals(onScreen, cell.y + cell.height / 2 - viewport.getViewPosition().y,
                "the clicked CLT row did not move on screen");
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

    /** A line above the first list heading and the first group heading separates them from the plots. */
    @Test
    void aLineSeparatesThePlotsTheListsAndTheGroups() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        panel.chart().plotLayout().addPlot();
        JList<SignalSections.Row> list = panel.fieldList();
        java.util.List<String> lined = new java.util.ArrayList<>();
        for (int index = 0; index < list.getModel().getSize(); index++) {
            SignalSections.Row row = list.getModel().getElementAt(index);
            Component cell = list.getCellRenderer().getListCellRendererComponent(list, row, index, false, false);
            cell.setBounds(0, 0, 260, 28);
            BufferedImage image = new BufferedImage(260, 28, BufferedImage.TYPE_INT_RGB);
            cell.paint(image.createGraphics());
            if (image.getRGB(130, 1) != image.getRGB(130, 4)) {
                lined.add(row.section);
            }
        }
        assertEquals(java.util.Arrays.asList(SignalSections.COMMON, "Sensors"), lined,
                "not between Plot 1 and Plot 2, nor between two groups");
    }

    /** Row renderers must lay themselves out: validate() does nothing without a native peer. */
    @Test
    void signalRowPaintsWithoutAWindow() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        JList<SignalSections.Row> list = panel.fieldList();
        Component row = list.getCellRenderer().getListCellRendererComponent(list,
                SignalSections.Row.signal(SignalSections.plotTitle(0), SignalSections.Kind.PLOTTED, 1), 0, false, false);
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
