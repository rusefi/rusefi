package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.MenuElement;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

class SignalListsTest {
    /** RPM and CLT change (both default Common signals); Knock: count stays 0; Fuel: injector lag stays at 1.2. */
    private static CsvLogReader log() throws IOException {
        return CsvLogReader.read(new StringReader("Time\tRPM\tCLT\tKnock: count\tFuel: injector lag\ns\trpm\tC\t\tms\n"
                + "0\t800\t80\t0\t1.2\n1\t900\t81\t0\t1.2\n"));
    }

    private static final int RPM = 1;
    private static final int CLT = 2;
    private static final int KNOCK = 3;
    private static final int LAG = 4;

    private static LogViewerPanel panel(SignalLists lists) throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), null, new ChartSettings(), lists, null);
        panel.chart().plotLayout().clearPlot(0); // the test starts from an empty plot
        panel.messages = message -> {
            throw new AssertionError("unexpected message: " + message);
        };
        return panel;
    }

    /** Section heading and member rows as "Title count" / "  field" / "  ?label" (missing). */
    private static List<String> describe(LogViewerPanel panel) {
        List<String> text = new ArrayList<>();
        for (int index = 0; index < panel.fieldList().getModel().getSize(); index++) {
            SignalSections.Row row = panel.fieldList().getModel().getElementAt(index);
            text.add(row.isHeading() ? row.section + " " + row.count
                    : row.isMissing() ? "  ?" + row.label : "  " + row.field);
        }
        return text;
    }

    /** The rows of one section, heading first. */
    private static List<String> section(LogViewerPanel panel, String title) {
        List<String> all = describe(panel);
        List<String> rows = new ArrayList<>();
        boolean in = false;
        for (String row : all) {
            if (!row.startsWith("  ")) {
                in = row.startsWith(title + " ");
            }
            if (in) {
                rows.add(row);
            }
        }
        return rows;
    }

    private static SignalSections.Row row(LogViewerPanel panel, String section, int field) {
        for (int index = 0; index < panel.fieldList().getModel().getSize(); index++) {
            SignalSections.Row row = panel.fieldList().getModel().getElementAt(index);
            if (row.section.equals(section) && row.field == field) {
                return row;
            }
        }
        throw new AssertionError("no row " + field + " in " + section);
    }

    private static JMenuItem item(MenuElement menu, String text) {
        for (MenuElement element : menu.getSubElements()) {
            if (element instanceof JMenuItem && text.equals(((JMenuItem) element).getText())) {
                return (JMenuItem) element;
            }
            JMenuItem nested = element instanceof JMenu || element.getSubElements().length > 0 ? item(element, text) : null;
            if (nested != null) {
                return nested;
            }
        }
        return null;
    }

    @Test
    void namesMustBeNewAndNotASectionTitle() {
        SignalLists lists = new SignalLists();
        lists.create("Trigger");
        assertNull(lists.problem("Idle tuning", null));
        assertNotNull(lists.problem("  ", null));
        assertNotNull(lists.problem("trigger", null), "names are compared ignoring case");
        assertNotNull(lists.problem("common", null));
        assertNotNull(lists.problem("plot 2", null), "a plot's section");
        assertNull(lists.problem("Plotter", null));
        assertNotNull(lists.problem("Sensors", null), "a group title");
        assertNotNull(lists.problem("a|b", null), "the config separator");
        assertNull(lists.problem("Trigger", "Trigger"), "renaming a list to itself");
    }

    @Test
    void listsAreKeptInTheConfigNode() {
        Node node = new Node();
        SignalLists lists = new SignalLists(node);
        lists.create("Trigger");
        lists.create("Idle");
        lists.setMembers("Trigger", Arrays.asList("RPM", "Trigger Error Counter"));
        lists.rename("Idle", "Idle tuning");

        SignalLists reloaded = new SignalLists(node);
        assertEquals(Arrays.asList("Trigger", "Idle tuning"), reloaded.customNames());
        assertEquals(Arrays.asList("RPM", "Trigger Error Counter"), reloaded.members("Trigger"));
        assertEquals(Collections.emptyList(), reloaded.members("Idle tuning"));

        reloaded.delete("Trigger");
        assertEquals(Collections.singletonList("Idle tuning"), new SignalLists(node).customNames());
        assertNull(new SignalLists(node).members("Trigger"));
    }

    @Test
    void everyChangeIsReported() {
        SignalLists lists = new SignalLists();
        AtomicInteger changes = new AtomicInteger();
        lists.addListener(changes::incrementAndGet);
        lists.create("A");
        lists.setMembers("A", Collections.singletonList("RPM"));
        lists.rename("A", "B");
        lists.setMembers(SignalSections.COMMON, Collections.singletonList("RPM"));
        lists.resetCommon();
        lists.delete("B");
        assertEquals(6, changes.get());
    }

    @Test
    void addingToCommonCopiesTheSignalAndStartsFromTheDefaults() throws IOException {
        SignalLists lists = new SignalLists();
        LogViewerPanel panel = panel(lists);
        assertEquals(Arrays.asList("Common 2", "  " + RPM, "  " + CLT), section(panel, "Common"));

        panel.addToList(SignalSections.COMMON, LAG);
        assertEquals(Arrays.asList("Common 3", "  " + RPM, "  " + CLT, "  " + LAG), section(panel, "Common"),
                "Common lists only the signals this log has");
        assertTrue(lists.members(SignalSections.COMMON).contains("Lambda"),
                "the defaults this log lacks are kept for other logs");
        assertFalse(panel.hideConstantBox.isSelected());
        panel.toggleSection("Fuel");
        assertTrue(section(panel, "Fuel").contains("  " + LAG), "a copy: the signal stays in its group");

        panel.listMenu(SignalSections.COMMON);
        JMenuItem reset = item(panel.listMenu(SignalSections.COMMON), "Reset to defaults");
        assertTrue(reset.isEnabled());
        reset.doClick();
        assertEquals(Arrays.asList("Common 2", "  " + RPM, "  " + CLT), section(panel, "Common"));
        assertFalse(item(panel.listMenu(SignalSections.COMMON), "Reset to defaults").isEnabled());
    }

    @Test
    void newListFromTheSignalMenuHoldsTheSignalAndComesAfterCommon() throws IOException {
        SignalLists lists = new SignalLists();
        LogViewerPanel panel = panel(lists);
        panel.namePrompt = (title, initial) -> " Knock work ";
        panel.toggleSection("Sensors");

        JMenuItem newList = item(panel.rowMenu(row(panel, "Sensors", CLT)), "New list…");
        newList.doClick();

        assertEquals(Collections.singletonList("Knock work"), lists.customNames(), "the name is trimmed");
        assertEquals(Arrays.asList("Knock work 1", "  " + CLT), section(panel, "Knock work"));
        List<String> all = describe(panel);
        assertTrue(all.indexOf("Common 2") < all.indexOf("Knock work 1"));
        assertTrue(all.indexOf("Knock work 1") < all.indexOf("Sensors 2"));

        JMenuItem inList = item(panel.rowMenu(row(panel, "Sensors", CLT)), "Knock work");
        assertTrue(inList.isSelected(), "the Add to list menu ticks the lists holding the signal");
        inList.doClick();
        assertEquals(Collections.singletonList("Knock work 0"), section(panel, "Knock work"),
                "unticking removes it; an empty list stays so it can be filled or deleted");
    }

    @Test
    void listsSurviveOpeningAnotherLogAndShowMissingMembers() throws IOException {
        SignalLists lists = new SignalLists();
        lists.create("Trigger");
        lists.setMembers("Trigger", Arrays.asList("RPM", "Trigger Error Counter"));
        LogViewerPanel panel = panel(lists);
        assertEquals(Arrays.asList("Trigger 2", "  " + RPM, "  ?Trigger Error Counter"), section(panel, "Trigger"));

        SignalSections.Row missing = panel.fieldList().getModel().getElementAt(describe(panel).indexOf("  ?Trigger Error Counter"));
        JMenuItem remove = item(panel.rowMenu(missing), "Remove from Trigger");
        assertNotNull(remove);
        assertNull(item(panel.rowMenu(missing), "Add to list"), "a missing signal can only be removed");
        remove.doClick();
        assertEquals(Collections.singletonList("RPM"), lists.members("Trigger"));
    }

    @Test
    void searchFindsListMembersAndHidesEmptyLists() throws IOException {
        SignalLists lists = new SignalLists();
        lists.create("Empty");
        lists.create("Trigger");
        lists.setMembers("Trigger", Arrays.asList("RPM", "Trigger Error Counter"));
        LogViewerPanel panel = panel(lists);
        assertTrue(describe(panel).contains("Empty 0"));

        panel.searchField().setText("error");
        assertEquals(Arrays.asList("Trigger 1", "  ?Trigger Error Counter"), describe(panel));
    }

    @Test
    void plotAllFillsTheEmptyActivePlotOrAddsANewOne() throws IOException {
        SignalLists lists = new SignalLists();
        lists.create("Mixed");
        lists.setMembers("Mixed", Arrays.asList("CLT", "Knock: count", "Fuel: injector lag"));
        LogViewerPanel panel = panel(lists);
        PlotLayout layout = panel.chart().plotLayout();
        panel.toggle(RPM);
        assertEquals(1, layout.plotCount());

        panel.plotAll("Mixed");
        assertEquals(2, layout.plotCount(), "the active plot holds RPM: a new plot");
        assertEquals(Arrays.asList(CLT, KNOCK, LAG), layout.fields(1));

        assertFalse(item(panel.listMenu("Mixed"), "Plot all").isEnabled(), "everything is plotted");
    }

    @Test
    void plotAllUsesTheActivePlotWhileItIsEmpty() throws IOException {
        SignalLists lists = new SignalLists();
        lists.create("Pair");
        lists.setMembers("Pair", Arrays.asList("Knock: count", "Fuel: injector lag"));
        LogViewerPanel panel = panel(lists);
        PlotLayout layout = panel.chart().plotLayout();
        panel.plotAll("Pair");
        assertEquals(1, layout.plotCount());
        assertEquals(Arrays.asList(KNOCK, LAG), layout.fields(0));
    }

    @Test
    void renameKeepsTheMembersAndRefusesTakenNames() throws IOException {
        SignalLists lists = new SignalLists();
        lists.create("Trigger");
        lists.setMembers("Trigger", Collections.singletonList("RPM"));
        LogViewerPanel panel = panel(lists);

        panel.namePrompt = (title, initial) -> "Sync";
        item(panel.listMenu("Trigger"), "Rename…").doClick();
        assertEquals(Arrays.asList("Sync 1", "  " + RPM), section(panel, "Sync"));

        List<String> messages = new ArrayList<>();
        panel.messages = messages::add;
        panel.namePrompt = (title, initial) -> "Knock";
        panel.renameList("Sync");
        assertEquals(1, messages.size(), "a group title is refused with a message");
        assertEquals(Collections.singletonList("Sync"), lists.customNames());

        assertFalse(item(panel.listMenu(SignalSections.COMMON), "Rename…").isEnabled(), "Common keeps its name");
        assertNull(item(panel.listMenu(SignalSections.COMMON), "Delete list"));
    }

    @Test
    void deleteAsksFirst() throws IOException {
        SignalLists lists = new SignalLists();
        lists.create("Trigger");
        LogViewerPanel panel = panel(lists);

        panel.confirm = question -> false;
        item(panel.listMenu("Trigger"), "Delete list").doClick();
        assertEquals(Collections.singletonList("Trigger"), lists.customNames());

        panel.confirm = question -> true;
        panel.deleteList("Trigger");
        assertEquals(Collections.emptyList(), lists.customNames());
        assertFalse(describe(panel).contains("Trigger 0"));
    }

    @Test
    void signalsDroppedOnAListGoThere() throws IOException {
        SignalLists lists = new SignalLists();
        lists.create("Trigger");
        LogViewerPanel panel = panel(lists);
        List<String> rows = describe(panel);
        assertEquals("Trigger", panel.listAt(rows.indexOf("Trigger 0")), "a list heading");
        assertEquals(SignalSections.COMMON, panel.listAt(rows.indexOf("Common 2") + 1), "a list member");
        assertNull(panel.listAt(rows.indexOf("Plot 1 0")), "a plot section is not a list");
        assertEquals(-1, panel.plotAt(rows.indexOf("Common 2")), "elsewhere a dropped trace leaves its plot");
    }

    @Test
    void rightClickOnAGroupHeadingHasNoMenu() throws IOException {
        LogViewerPanel panel = panel(new SignalLists());
        for (int index = 0; index < panel.fieldList().getModel().getSize(); index++) {
            SignalSections.Row row = panel.fieldList().getModel().getElementAt(index);
            if (row.isHeading() && row.kind == SignalSections.Kind.GROUP) {
                assertNull(panel.rowMenu(row));
                return;
            }
        }
        throw new AssertionError("no group heading");
    }
}
