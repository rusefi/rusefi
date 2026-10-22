package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.awt.Rectangle;
import java.awt.event.MouseEvent;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class SignalSectionsTest {
    /** RPM and CLT change; Knock: count stays 0; Fuel: injector lag stays at 1.2. */
    private static CsvLogReader log() throws IOException {
        return CsvLogReader.read(new StringReader("Time\tRPM\tCLT\tKnock: count\tFuel: injector lag\ns\trpm\tC\t\tms\n"
                + "0\t800\t80\t0\t1.2\n1\t900\t81\t0\t1.2\n"));
    }

    private static List<String> describe(List<SignalSections.Row> rows) {
        List<String> text = new ArrayList<>();
        for (SignalSections.Row row : rows) {
            text.add(row.isHeading() ? (row.expanded ? "v " : "> ") + row.section + " " + row.count : "  " + row.field);
        }
        return text;
    }

    private static List<SignalSections.Row> rows(CsvLogReader log, PlotLayout layout, String query,
                                                 boolean hideZero, boolean hideConstant, Set<String> collapsed) {
        return SignalSections.rows(log, layout, SignalCategories.of(log, null), query, hideZero, hideConstant,
                collapsed);
    }

    @Test
    void plottedAndCommonOpenGroupsCollapsedZeroHidden() throws IOException {
        CsvLogReader log = log();
        PlotLayout layout = new PlotLayout(log.fieldCount());
        layout.add(2);
        assertEquals(java.util.Arrays.asList(
                "v Plotted 1", "  2",
                "v Common 2", "  1", "  2",
                "> Sensors 2",
                "> Fuel 1"),
                describe(rows(log, layout, "", true, false, SignalSections.defaultCollapsed())),
                "CLT is in Plotted, Common and Sensors; the zero knock count is hidden, the constant lag is not");
    }

    @Test
    void zeroAndConstantAreHiddenSeparately() throws IOException {
        CsvLogReader log = log();
        assertEquals(1, SignalSections.zeroCount(log));
        assertEquals(1, SignalSections.constantCount(log));
        assertTrue(SignalSections.isZero(log, 3));
        assertTrue(SignalSections.isConstant(log, 4));
        assertFalse(SignalSections.isZero(log, 1) || SignalSections.isConstant(log, 1), "RPM changes");

        PlotLayout none = new PlotLayout(log.fieldCount());
        List<String> constantHidden = describe(rows(log, none, "", false, true, SignalSections.defaultCollapsed()));
        assertTrue(constantHidden.contains("> Knock 1"));
        assertFalse(constantHidden.contains("> Fuel 1"));

        List<String> bothShown = describe(rows(log, none, "", false, false, SignalSections.defaultCollapsed()));
        assertTrue(bothShown.contains("> Knock 1"));
        assertTrue(bothShown.contains("> Fuel 1"));
    }

    @Test
    void searchShowsMatchesExpandedIncludingConstant() throws IOException {
        CsvLogReader log = log();
        assertEquals(java.util.Arrays.asList("v Knock 1", "  3"),
                describe(rows(log, new PlotLayout(log.fieldCount()), "knock", true, true,
                        SignalSections.defaultCollapsed())));
    }

    @Test
    void collapsingHidesMembersButKeepsTheCount() throws IOException {
        CsvLogReader log = log();
        Set<String> collapsed = SignalSections.defaultCollapsed();
        collapsed.add(SignalSections.COMMON);
        collapsed.remove("Sensors");
        List<String> rows = describe(rows(log, new PlotLayout(log.fieldCount()), "", true, true, collapsed));
        assertTrue(rows.contains("> Common 2"));
        int sensors = rows.indexOf("v Sensors 2");
        assertEquals("  1", rows.get(sensors + 1));
        assertEquals("  2", rows.get(sensors + 2));
    }

    @Test
    void clickingAHeadingInThePanelCollapsesAndExpands() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        panel.fieldList().setSize(260, 600);
        int plottedBefore = panel.chart().plotLayout().plottedCount();
        int common = indexOf(panel, "v " + SignalSections.COMMON);
        Rectangle heading = panel.fieldList().getCellBounds(common, common);
        panel.fieldList().dispatchEvent(new MouseEvent(panel.fieldList(), MouseEvent.MOUSE_CLICKED, 0, 0,
                heading.x + 20, heading.y + heading.height / 2, 1, false, MouseEvent.BUTTON1));
        assertTrue(indexOf(panel, "> " + SignalSections.COMMON) >= 0);
        assertEquals(plottedBefore, panel.chart().plotLayout().plottedCount(), "a heading is not a signal");

        panel.toggleSection(SignalSections.COMMON);
        assertTrue(indexOf(panel, "v " + SignalSections.COMMON) >= 0);
    }

    @Test
    void filterCheckboxesShowTheirCounts() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        assertEquals("Hide zero signals (1)", panel.hideZeroBox.getText());
        assertTrue(panel.hideZeroBox.isSelected(), "zero signals hidden by default");
        assertEquals("Hide constant signals (1)", panel.hideConstantBox.getText());
        assertFalse(panel.hideConstantBox.isSelected(), "constant signals can matter: shown by default");
    }

    @Test
    void countChipShowsTheSignalsLeft() throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null);
        assertEquals("3 / 4", panel.countChip().getText(), "the zero signal is hidden");

        panel.hideConstantBox.doClick();
        assertEquals("2 / 4", panel.countChip().getText(), "and the constant one");

        panel.hideZeroBox.doClick();
        panel.hideConstantBox.doClick();
        assertEquals("4", panel.countChip().getText(), "everything listed: just the total");

        panel.searchField().setText("knock");
        assertEquals("1 / 4", panel.countChip().getText(), "search matches");
    }

    private static int indexOf(LogViewerPanel panel, String headingPrefix) {
        for (int index = 0; index < panel.fieldList().getModel().getSize(); index++) {
            SignalSections.Row row = panel.fieldList().getModel().getElementAt(index);
            if (row.isHeading() && ((row.expanded ? "v " : "> ") + row.section).equals(headingPrefix)) {
                return index;
            }
        }
        return -1;
    }
}
