package com.rusefi.ui.logview;

import com.opensr5.ini.IniFileModel;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.sensor_logs.LoggingProfile;
import com.rusefi.sensor_logs.LoggingProfiles;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import javax.swing.DefaultListModel;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/** The Edit profiles window, without showing it. */
class LoggingProfilesEditorTest {
    @TempDir
    Path tempDir;

    private IniFileModel ini() throws IOException {
        Path path = tempDir.resolve("editor.ini");
        Files.writeString(path, LogViewerProfilesTest.INI);
        return IniFileReaderUtil.readIniFile(path.toString());
    }

    /** Rows as "KIND text [tag]", e.g. "SIGNAL MAP [recommended]". */
    private static List<String> rows(DefaultListModel<LoggingProfilesEditor.Row> model) {
        List<String> rows = new ArrayList<>();
        for (int index = 0; index < model.size(); index++) {
            LoggingProfilesEditor.Row row = model.get(index);
            String tag = row.kind == LoggingProfilesEditor.Kind.AVAILABLE ? (row.inProfile ? "in" : "")
                    : row.tag;
            rows.add(row.kind + " " + row.text + (tag == null || tag.isEmpty() ? "" : " [" + tag + "]"));
        }
        return rows;
    }

    private static LoggingProfilesEditor.Row row(DefaultListModel<LoggingProfilesEditor.Row> model, String text) {
        for (int index = 0; index < model.size(); index++) {
            if (model.get(index).text.equals(text) && model.get(index).kind != LoggingProfilesEditor.Kind.HEADING) {
                return model.get(index);
            }
        }
        throw new AssertionError("no " + text + " row");
    }

    private static LoggingProfilesEditor editor(LoggingProfiles profiles, IniFileModel ini) {
        LoggingProfilesEditor editor = new LoggingProfilesEditor(profiles, ini);
        editor.confirm = question -> true;
        editor.messages = message -> {
            throw new AssertionError(message);
        };
        return editor;
    }

    @Test
    void opensOnTheRecordingProfileWithItsRecommendedSignals() throws Exception {
        LoggingProfiles profiles = new LoggingProfiles(new Node());
        profiles.select(LoggingProfile.TRIGGER_ID);
        LoggingProfilesEditor editor = editor(profiles, ini());
        assertEquals("Trigger", editor.title.getText());
        assertEquals("Recommended", editor.badge.getText());
        assertFalse(editor.resetButton.isVisible());
        assertFalse(editor.renameButton.isEnabled(), "built-in profiles keep their names");
        assertFalse(editor.deleteButton.isEnabled());
        assertEquals(Arrays.asList("HEADING Always recorded", "LOCKED RPM", "HEADING Signals [3]",
                "SIGNAL Trigger Error Counter [recommended]", "SIGNAL MAP [recommended]",
                "SIGNAL VBatt [recommended]"), rows(editor.signalModel));
        assertEquals("Trigger: 4 of 8 signals · changes are saved as you make them", editor.footer.getText());
        assertEquals("4 signals · recommended", editor.subtitle(profiles.get(LoggingProfile.TRIGGER_ID)));
    }

    @Test
    void removedRecommendedSignalsCanBeRestoredOrAllChangesReset() throws Exception {
        LoggingProfiles profiles = new LoggingProfiles(new Node());
        List<String> told = new ArrayList<>();
        profiles.addListener(() -> told.add("changed"));
        LoggingProfilesEditor editor = editor(profiles, ini());
        editor.select(LoggingProfile.TRIGGER_ID);

        editor.act(row(editor.signalModel, "VBatt"));
        editor.toggleGroup("Boost");
        editor.act(row(editor.addModel, "Boost: duty"));
        assertEquals(Arrays.asList("HEADING Always recorded", "LOCKED RPM", "HEADING Signals [3]",
                "SIGNAL Trigger Error Counter [recommended]", "SIGNAL MAP [recommended]",
                "SIGNAL Boost: duty [added]", "HEADING Removed from recommended [1]", "REMOVED VBatt"),
                rows(editor.signalModel));
        assertEquals("Recommended · edited (+1 −1)", editor.badge.getText());
        assertTrue(editor.resetButton.isVisible());
        assertTrue(rows(editor.addModel).contains("AVAILABLE Boost: duty [in]"));
        assertEquals(2, told.size(), "each change reaches the profile menus");

        editor.act(row(editor.signalModel, "VBatt"));
        assertTrue(rows(editor.signalModel).contains("SIGNAL VBatt [recommended]"), "restored");
        editor.reset();
        assertEquals("Recommended", editor.badge.getText());
        assertFalse(profiles.get(LoggingProfile.TRIGGER_ID).isEdited());
    }

    @Test
    void fullIsReadOnlyAndDuplicatesIntoAnEditableProfile() throws Exception {
        LoggingProfiles profiles = new LoggingProfiles(new Node());
        IniFileModel ini = ini();
        LoggingProfilesEditor editor = editor(profiles, ini);
        assertEquals("Full", editor.title.getText());
        assertEquals("Built-in · read-only", editor.badge.getText());
        assertFalse(editor.search.isEnabled());
        assertEquals("8 signals, all recorded by Full", editor.total.getText());
        editor.toggleGroup("Sensors");
        assertTrue(rows(editor.addModel).stream().allMatch(row -> row.startsWith("GROUP") || row.endsWith("[in]")));
        editor.act(row(editor.addModel, "MAP"));
        assertEquals(8, profiles.get(LoggingProfile.FULL_ID).channels(ini).size(), "nothing changes");

        editor.namePrompt = (title, initial) -> {
            assertEquals("Full copy", initial);
            return initial;
        };
        editor.duplicateButton.doClick();
        assertEquals("Full copy", editor.title.getText());
        assertEquals("Your profile", editor.badge.getText());
        assertTrue(editor.renameButton.isEnabled());
        editor.act(row(editor.signalModel, "CLT"));
        assertEquals("Full copy: 7 of 8 signals · changes are saved as you make them", editor.footer.getText());
    }

    @Test
    void aNewProfileStartsEmptyAndCanBeRenamedAndDeleted() throws Exception {
        LoggingProfiles profiles = new LoggingProfiles(new Node());
        LoggingProfilesEditor editor = editor(profiles, ini());
        editor.namePrompt = (title, initial) -> initial;
        editor.newButton.doClick();
        assertEquals("New profile", editor.title.getText());
        assertEquals(Arrays.asList("HEADING Always recorded", "LOCKED RPM", "HEADING Signals [0]",
                "EMPTY No signals yet: add them from the list on the right."), rows(editor.signalModel));

        editor.search.setText("boost");
        assertEquals(Arrays.asList("GROUP Boost [1]", "AVAILABLE Boost: duty"), rows(editor.addModel),
                "a search opens the groups with a match");
        assertEquals("1 of 8 signals match", editor.total.getText());
        editor.search.postActionEvent();
        assertEquals("SIGNAL Boost: duty", rows(editor.signalModel).get(3), "Enter adds the only match");

        List<String> warnings = new ArrayList<>();
        editor.messages = warnings::add;
        editor.namePrompt = (title, initial) -> "trigger";
        editor.renameButton.doClick();
        assertEquals(Arrays.asList("There is already a profile called \"Trigger\"."), warnings);
        editor.namePrompt = (title, initial) -> "Boost run";
        editor.renameButton.doClick();
        assertEquals("Boost run", editor.title.getText());

        profiles.select(editor.selected().getId());
        editor.deleteButton.doClick();
        assertEquals(3, profiles.list().size());
        assertEquals(LoggingProfile.FULL_ID, profiles.selected().getId(), "recordings fall back to Full");
        assertEquals("Idle", editor.title.getText(), "the profile above takes its place");
    }

    @Test
    void groupsStartClosedAndTheTotalIsShownAboveThem() throws Exception {
        LoggingProfiles profiles = new LoggingProfiles(new Node());
        profiles.select(LoggingProfile.IDLE_ID);
        LoggingProfilesEditor editor = editor(profiles, ini());
        assertEquals(Arrays.asList("GROUP Sensors [4]", "GROUP Idle [2]", "GROUP Boost [1]",
                "GROUP Trigger & sync [1]"),
                rows(editor.addModel));
        assertEquals("8 signals \u00b7 6 in this profile", editor.total.getText());

        editor.toggleGroup("Idle");
        assertEquals(Arrays.asList("GROUP Sensors [4]", "GROUP Idle [2]", "AVAILABLE Idle: Position [in]",
                "AVAILABLE Idle: Target RPM [in]", "GROUP Boost [1]", "GROUP Trigger & sync [1]"),
                rows(editor.addModel));
        assertEquals(1, editor.addList.getSelectedIndex(), "the opened group stays selected");
        editor.toggleGroup("Idle");
        assertEquals(4, editor.addModel.size());

        editor.select(LoggingProfile.TRIGGER_ID);
        editor.toggleGroup("Sensors");
        assertTrue(rows(editor.addModel).contains("AVAILABLE CLT"), "open groups stay open for other profiles");
    }

    @Test
    void withoutTheIniNothingCanBeAdded() {
        LoggingProfiles profiles = new LoggingProfiles(new Node());
        profiles.select(LoggingProfile.IDLE_ID);
        LoggingProfilesEditor editor = editor(profiles, null);
        assertTrue(editor.addModel.isEmpty());
        assertFalse(editor.search.isEnabled());
        assertTrue(editor.footer.getText().startsWith("Connect to the ECU"));
        assertTrue(rows(editor.signalModel).contains("LOCKED RPMValue"), "channel names stand in for labels");
    }
}
