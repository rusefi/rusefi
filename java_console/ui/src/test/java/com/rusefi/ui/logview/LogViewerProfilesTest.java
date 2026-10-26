package com.rusefi.ui.logview;

import com.opensr5.ini.IniFileModel;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.logview.CsvLogReader;
import com.rusefi.sensor_logs.LoggingProfile;
import com.rusefi.sensor_logs.LoggingProfiles;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.JList;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.SwingUtilities;
import java.io.IOException;
import java.io.StringReader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

/** Logging profiles in the Log Viewer's sidebar, and saving signals as a new profile. */
class LogViewerProfilesTest {
    static final String INI = "[MegaTune]\nsignature = test\n[Constants]\npageSize = 0\n"
            + "pageReadCommand = R\nochBlockSize = 16\n"
            + "[OutputChannels]\n"
            + "RPMValue = scalar, U16, 0, \"RPM\", 1, 0\n"
            + "totalTriggerErrorCounter = scalar, U16, 2, \"count\", 1, 0\n"
            + "currentIdlePosition = scalar, U16, 4, \"%\", 1, 0\n"
            + "MAPValue = scalar, U16, 6, \"kPa\", 0.1, 0\n"
            + "coolant = scalar, S16, 8, \"deg C\", 0.01, 0\n"
            + "VBatt = scalar, U16, 10, \"V\", 0.001, 0\n"
            + "boostDuty = scalar, U16, 12, \"%\", 0.01, 0\n"
            + "idleTarget = scalar, U16, 14, \"RPM\", 1, 0\n"
            + "[GaugeConfigurations]\n"
            + "gaugeCategory = Trigger\n"
            + "triggerErrorsGauge = totalTriggerErrorCounter, \"Trigger error count\", \"count\", 0, 100, 0, 0, 10, 50, 0, 0\n"
            + "gaugeCategory = Idle\n"
            + "idlePositionGauge = currentIdlePosition, \"Idle position\", \"%\", 0, 100, 0, 0, 100, 100, 1, 1\n"
            + "[Datalog]\n"
            + "entry = RPMValue, \"RPM\", int, \"%d\"\n"
            + "entry = totalTriggerErrorCounter, \"Trigger Error Counter\", int, \"%d\"\n"
            + "entry = currentIdlePosition, \"Idle: Position\", float, \"%.3f\"\n"
            + "entry = MAPValue, \"MAP\", float, \"%.1f\"\n"
            + "entry = coolant, \"CLT\", float, \"%.2f\"\n"
            + "entry = VBatt, \"VBatt\", float, \"%.3f\"\n"
            + "entry = boostDuty, \"Boost: duty\", float, \"%.2f\"\n"
            + "entry = idleTarget, \"Idle: Target RPM\", int, \"%d\"\n";

    @TempDir
    Path tempDir;

    private IniFileModel ini() throws IOException {
        Path path = tempDir.resolve("viewer.ini");
        Files.writeString(path, INI);
        return IniFileReaderUtil.readIniFile(path.toString());
    }

    /** An idle log recorded before profiles: no trigger counter, VBatt or idle target; a computed signal. */
    private static CsvLogReader log() throws IOException {
        StringBuilder text = new StringBuilder("Time\tRPM\tMAP\tCLT\tIdle: Position\tBoost: duty\tComputed\n"
                + "s\trpm\tkPa\tC\t%\t%\t\n");
        for (int i = 0; i < 50; i++) {
            text.append(i / 10.0).append('\t').append(800 + i).append('\t').append(30 + i % 3).append('\t')
                    .append(80 + i / 10).append('\t').append(20 + i % 4).append('\t').append(i % 2).append('\t')
                    .append(i).append('\n');
        }
        return CsvLogReader.read(new StringReader(text.toString()));
    }

    private static LogViewerPanel panel(SidebarState state, IniFileModel ini, LoggingProfiles profiles)
            throws IOException {
        LogViewerPanel panel = new LogViewerPanel(log(), "idle.msl", new ChartSettings(), new SignalLists(), state,
                ini, null);
        panel.setLoggingProfiles(profiles);
        return panel;
    }

    /** "heading" rows as "PROFILE Profile: Idle (4)", members as their label, missing ones marked. */
    private static List<String> section(LogViewerPanel panel, String title) {
        List<String> rows = new ArrayList<>();
        JList<SignalSections.Row> list = panel.fieldList();
        for (int index = 0; index < list.getModel().getSize(); index++) {
            SignalSections.Row row = list.getModel().getElementAt(index);
            if (!row.section.equals(title)) {
                continue;
            }
            if (row.isHeading()) {
                rows.add(row.kind + " " + (row.expanded ? "open" : "closed"));
            } else {
                rows.add(row.isMissing() ? row.label + " (missing)" : panel.chart().log().name(row.field));
            }
        }
        return rows;
    }

    private static SignalSections.Row heading(LogViewerPanel panel, String title) {
        JList<SignalSections.Row> list = panel.fieldList();
        for (int index = 0; index < list.getModel().getSize(); index++) {
            SignalSections.Row row = list.getModel().getElementAt(index);
            if (row.isHeading() && row.section.equals(title)) {
                return row;
            }
        }
        throw new AssertionError("no " + title + " section");
    }

    @Test
    void profilesFollowTheListsClosedAndShowWhatTheLogLacks() throws Exception {
        SidebarState state = new SidebarState();
        LoggingProfiles profiles = new LoggingProfiles(new Node());
        LogViewerPanel panel = panel(state, ini(), profiles);
        assertEquals(Arrays.asList("PROFILE closed"), section(panel, "Profile: Trigger"));
        assertEquals(Arrays.asList("PROFILE closed"), section(panel, "Profile: Idle"));
        assertTrue(section(panel, "Profile: Full").isEmpty(), "Full would only repeat the groups");
        assertEquals(1, LogViewerPanel.zone(heading(panel, "Profile: Idle")), "with the lists");

        state.setCollapsed("Profile: Idle", false);
        panel.setLoggingProfiles(profiles);
        assertEquals(Arrays.asList("PROFILE open", "RPM", "Idle: Position", "MAP", "CLT", "VBatt (missing)",
                "Idle: Target RPM (missing)"), section(panel, "Profile: Idle"));
        assertEquals(6, heading(panel, "Profile: Idle").count, "missing signals count too");
    }

    @Test
    void profileMenuPlotsTheSignalsTheLogHas() throws Exception {
        LogViewerPanel panel = panel(new SidebarState(), ini(), new LoggingProfiles(new Node()));
        JPopupMenu menu = panel.rowMenu(heading(panel, "Profile: Idle"));
        JMenuItem plotAll = (JMenuItem) menu.getComponent(0);
        assertEquals("Plot all", plotAll.getText());
        plotAll.doClick();
        for (String label : Arrays.asList("RPM", "Idle: Position", "MAP", "CLT")) {
            int field = SignalSections.fieldOfLabel(panel.chart().log()).get(label);
            assertTrue(panel.chart().plotLayout().isPlotted(field), label);
        }
    }

    @Test
    void plottedSignalsSaveAsAProfileTheRecordButtonOffers() throws Exception {
        LoggingProfiles profiles = new LoggingProfiles(new Node());
        List<Runnable> told = new ArrayList<>();
        profiles.addListener(() -> told.add(null));
        LogViewerPanel panel = panel(new SidebarState(), ini(), profiles);
        AtomicReference<String> notice = new AtomicReference<>();
        panel.notices = notice::set;
        panel.namePrompt = (title, initial) -> {
            assertEquals("New logging profile", title);
            return "  Boost check ";
        };
        CsvLogReader log = panel.chart().log();
        panel.chart().plotLayout().clearPlot(0);
        for (String label : Arrays.asList("MAP", "Boost: duty", "Computed")) {
            panel.chart().plotLayout().add(SignalSections.fieldOfLabel(log).get(label));
        }

        JMenuItem save = (JMenuItem) panel.rowMenu(heading(panel, "Plot 1")).getComponent(0);
        assertEquals("Save plotted signals as logging profile…", save.getText());
        assertTrue(save.isEnabled());
        save.doClick();

        LoggingProfile saved = profiles.list().get(profiles.list().size() - 1);
        assertEquals("Boost check", saved.getName());
        assertEquals(LoggingProfile.Kind.CUSTOM, saved.getKind());
        assertEquals(Arrays.asList("boostDuty", "MAPValue"), new ArrayList<>(saved.getAdded()));
        assertEquals(LoggingProfile.FULL_ID, profiles.selected().getId(), "saving does not change what is recorded");
        assertFalse(told.isEmpty(), "the record button's menu is told");
        assertEquals("Saved the logging profile \"Boost check\" with 3 signals; 1 can't be recorded and were "
                + "left out. Choose it beside Start Data Logging.", notice.get());
        assertEquals(Arrays.asList("PROFILE open", "RPM", "MAP", "Boost: duty"), section(panel, "Profile: Boost check"),
                "shown open, RPM always included");
    }

    @Test
    void aListSavesUnderItsNameUnlessTaken() throws Exception {
        LoggingProfiles profiles = new LoggingProfiles(new Node());
        LogViewerPanel panel = panel(new SidebarState(), ini(), profiles);
        List<String> offered = new ArrayList<>();
        List<String> warnings = new ArrayList<>();
        panel.messages = warnings::add;
        panel.notices = notice -> {
        };
        panel.namePrompt = (title, initial) -> {
            offered.add(initial);
            return initial.isEmpty() ? "idle" : initial;
        };
        JPopupMenu commonMenu = panel.listMenu(SignalSections.COMMON);
        JMenuItem save = (JMenuItem) commonMenu.getComponent(commonMenu.getComponentCount() - 1);
        assertEquals("Save as logging profile…", save.getText());
        save.doClick();
        assertEquals("Common", profiles.list().get(3).getName());

        panel.saveAsProfile(Arrays.asList("RPM"), "Common");
        assertEquals(Arrays.asList("Common", ""), offered, "a taken name is not offered");
        assertEquals(Arrays.asList("There is already a profile called \"Idle\"."), warnings);
        assertEquals(4, profiles.list().size());
    }

    @Test
    void savingNeedsTheIni() throws Exception {
        LogViewerPanel panel = panel(new SidebarState(), null, new LoggingProfiles(new Node()));
        JMenuItem save = (JMenuItem) panel.plotsMenu().getComponent(0);
        assertFalse(save.isEnabled());
        assertTrue(save.getToolTipText().startsWith("Needs the ECU's .ini"));

        LogViewerPanel withoutLogging = new LogViewerPanel(log(), "idle.msl");
        assertTrue(section(withoutLogging, "Profile: Idle").isEmpty(), "no profiles without data logging");
        JPopupMenu listMenu = withoutLogging.listMenu(SignalSections.COMMON);
        assertEquals("New list…", ((JMenuItem) listMenu.getComponent(listMenu.getComponentCount() - 1)).getText());
    }

    @Test
    void theSidebarFollowsProfilesChangedElsewhere() throws Exception {
        LoggingProfiles profiles = new LoggingProfiles(new Node());
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            try {
                LogViewerHost host = new LogViewerHost();
                host.setIniSupplier(() -> ini);
                host.setLoggingProfiles(profiles);
                host.showLog(log(), "idle.msl");
                assertTrue(section(host.currentPanel(), "Profile: Mine").isEmpty());
                profiles.create("Mine");
                assertEquals(Arrays.asList("PROFILE closed"), section(host.currentPanel(), "Profile: Mine"));
                assertNull(host.currentPanel().rowMenu(missingRow()),
                        "a missing profile signal has no menu");
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
    }

    private static SignalSections.Row missingRow() {
        return SignalSections.Row.missing("Profile: Idle", SignalSections.Kind.PROFILE, "VBatt");
    }
}
