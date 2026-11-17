package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.logview.CsvLogReader;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.StringReader;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class SidebarStateTest {
    private static CsvLogReader log() throws IOException {
        return CsvLogReader.read(new StringReader("Time\tRPM\tCLT\tKnock: count\tFuel: injector lag\ns\trpm\tC\t\tms\n"
                + "0\t800\t80\t0\t1.2\n1\t900\t81\t0\t1.2\n"));
    }

    @Test
    void defaultsAreGroupsClosedListsOpenZeroHidden() {
        SidebarState state = new SidebarState();
        assertTrue(state.isCollapsed("Sensors"));
        assertFalse(state.isCollapsed(SignalSections.plotTitle(0)));
        assertFalse(state.isCollapsed(SignalSections.COMMON));
        assertFalse(state.isCollapsed("My list"));
        assertTrue(state.hideZero());
        assertFalse(state.hideConstant());
    }

    @Test
    void onlyChangesFromTheDefaultsAreStored() {
        Node node = new Node();
        SidebarState state = new SidebarState(node);
        state.setCollapsed("Sensors", false);
        state.setCollapsed(SignalSections.COMMON, true);
        assertEquals("Sensors", node.getProperty("opened"));
        assertEquals(SignalSections.COMMON, node.getProperty("closed"));

        state.setCollapsed("Sensors", true);
        assertEquals("", node.getProperty("opened"), "back at the default: nothing to remember");
    }

    @Test
    void stateSurvivesARestart() {
        Node node = new Node();
        SidebarState state = new SidebarState(node);
        state.setCollapsed("Fuel", false);
        state.setCollapsed("Trigger", true);
        state.setHideZero(false);
        state.setHideConstant(true);

        SidebarState reloaded = new SidebarState(node);
        assertFalse(reloaded.isCollapsed("Fuel"));
        assertTrue(reloaded.isCollapsed("Trigger"));
        assertFalse(reloaded.hideZero());
        assertTrue(reloaded.hideConstant());
    }

    @Test
    void renamedListsKeepTheirStateAndDeletedOnesAreForgotten() {
        SidebarState state = new SidebarState();
        state.setCollapsed("Trigger", true);
        state.rename("Trigger", "Sync");
        assertTrue(state.isCollapsed("Sync"));
        assertFalse(state.isCollapsed("Trigger"));

        state.forget("Sync");
        assertFalse(state.isCollapsed("Sync"), "a new list of that name starts open");
    }

    @Test
    void theNextLogOpensWithTheSameSectionsAndSwitches() throws IOException {
        SignalLists lists = new SignalLists();
        SidebarState state = new SidebarState();
        LogViewerPanel first = new LogViewerPanel(log(), null, new ChartSettings(), lists, state, null, null);
        first.toggleSection("Sensors");
        first.toggleSection(SignalSections.COMMON);
        first.hideZeroBox.doClick();
        first.hideConstantBox.doClick();

        LogViewerPanel next = new LogViewerPanel(log(), null, new ChartSettings(), lists, state, null, null);
        assertFalse(next.hideZeroBox.isSelected());
        assertTrue(next.hideConstantBox.isSelected());
        boolean sensorsOpen = false;
        boolean commonOpen = true;
        for (int index = 0; index < next.fieldList().getModel().getSize(); index++) {
            SignalSections.Row row = next.fieldList().getModel().getElementAt(index);
            if (row.isHeading() && row.section.equals("Sensors")) {
                sensorsOpen = row.expanded;
            }
            if (row.isHeading() && row.section.equals(SignalSections.COMMON)) {
                commonOpen = row.expanded;
            }
        }
        assertTrue(sensorsOpen);
        assertFalse(commonOpen);
    }
}
