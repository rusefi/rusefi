package com.rusefi.sensor_logs;

import com.opensr5.ini.IniFileModel;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;

class LoggingProfileTest {
    private static final String INI = "[MegaTune]\nsignature = test\n[Constants]\npageSize = 0\n"
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
    private IniFileModel ini;

    @BeforeEach
    void readIni() throws Exception {
        Path path = tempDir.resolve("profiles.ini");
        Files.writeString(path, INI);
        ini = IniFileReaderUtil.readIniFile(path.toString());
    }

    private static List<String> labels(List<CustomBinaryLogEntry> entries) {
        List<String> labels = new ArrayList<>();
        for (CustomBinaryLogEntry entry : entries) {
            labels.add(entry.getName());
        }
        return labels;
    }

    private List<String> recorded(LoggingProfile profile) {
        return labels(profile.select(SensorLogger.getOutputChannels(ini), ini));
    }

    @Test
    void recommendedSetsComeFromTheIniCategoryPlusContextInLogOrder() {
        LoggingProfiles profiles = new LoggingProfiles(new Node());
        assertEquals(Arrays.asList("RPM", "Trigger Error Counter", "MAP", "VBatt"),
                recorded(profiles.get(LoggingProfile.TRIGGER_ID)));
        assertEquals(Arrays.asList("RPM", "Idle: Position", "MAP", "CLT", "VBatt", "Idle: Target RPM"),
                recorded(profiles.get(LoggingProfile.IDLE_ID)));
        assertEquals(SensorLogger.getOutputChannels(ini).size(),
                recorded(profiles.get(LoggingProfile.FULL_ID)).size(), "Full records everything");
    }

    @Test
    void editsToARecommendedProfileAreKeptAsChanges() {
        LoggingProfile trigger = new LoggingProfiles(new Node()).get(LoggingProfile.TRIGGER_ID);
        assertFalse(trigger.isEdited());

        trigger.remove("VBatt", ini);
        trigger.add("boostDuty");
        trigger.remove("rpmvalue", ini);
        assertTrue(trigger.isEdited());
        assertEquals("Recommended, edited", trigger.describe());
        assertEquals(Arrays.asList("VBatt"), new ArrayList<>(trigger.getRemoved()));
        assertEquals(Arrays.asList("boostDuty"), new ArrayList<>(trigger.getAdded()));
        assertEquals(Arrays.asList("RPM", "Trigger Error Counter", "MAP", "Boost: duty"), recorded(trigger),
                "RPM is locked");

        trigger.add("VBATT");
        assertTrue(trigger.getRemoved().isEmpty(), "adding a removed signal back restores it");
        trigger.reset();
        assertFalse(trigger.isEdited());
        assertEquals(Arrays.asList("RPM", "Trigger Error Counter", "MAP", "VBatt"), recorded(trigger));
    }

    @Test
    void fullCannotBeEditedAndCustomStartsEmpty() {
        LoggingProfiles profiles = new LoggingProfiles(new Node());
        LoggingProfile full = profiles.get(LoggingProfile.FULL_ID);
        full.remove("MAPValue", ini);
        assertEquals(8, recorded(full).size());

        LoggingProfile custom = profiles.create("Boost run");
        assertEquals(Arrays.asList("RPM"), recorded(custom), "only the locked signals");
        custom.add("boostDuty");
        custom.add("MAPValue");
        custom.remove("MAPValue", ini);
        assertTrue(custom.getRemoved().isEmpty(), "custom profiles have nothing recommended to remember");
        assertEquals(Arrays.asList("RPM", "Boost: duty"), recorded(custom));
    }

    @Test
    void duplicatingFullStartsFromEverything() {
        LoggingProfiles profiles = new LoggingProfiles(new Node());
        LoggingProfile copy = profiles.duplicate(profiles.get(LoggingProfile.FULL_ID), "Almost all", ini);
        assertEquals(LoggingProfile.Kind.CUSTOM, copy.getKind());
        copy.remove("coolant", ini);
        assertEquals(7, recorded(copy).size());
    }

    @Test
    void channelsTheIniLacksAreSkipped() {
        LoggingProfile custom = new LoggingProfiles(new Node()).create("Old ECU");
        custom.add("noSuchChannel");
        assertEquals(Arrays.asList("RPM"), recorded(custom));
    }

    @Test
    void profilesAndTheChoiceSurviveARestart() {
        Node node = new Node();
        LoggingProfiles profiles = new LoggingProfiles(node);
        assertSame(profiles.get(LoggingProfile.FULL_ID), profiles.selected(), "Full until chosen otherwise");
        LoggingProfile trigger = profiles.get(LoggingProfile.TRIGGER_ID);
        trigger.remove("VBatt", ini);
        trigger.add("boostDuty");
        LoggingProfile custom = profiles.create("Boost run");
        custom.add("MAPValue");
        profiles.select(custom.getId());

        LoggingProfiles restarted = new LoggingProfiles(node);
        assertEquals("Boost run", restarted.selected().getName());
        assertEquals(Arrays.asList("RPM", "MAP"), recorded(restarted.selected()));
        assertEquals(Arrays.asList("RPM", "Trigger Error Counter", "MAP", "Boost: duty"),
                recorded(restarted.get(LoggingProfile.TRIGGER_ID)));
    }

    @Test
    void deletingTheChosenProfileFallsBackToFull() {
        Node node = new Node();
        LoggingProfiles profiles = new LoggingProfiles(node);
        LoggingProfile custom = profiles.create("Short");
        profiles.select(custom.getId());
        profiles.delete(LoggingProfile.TRIGGER_ID);
        assertEquals(4, profiles.list().size(), "built-in profiles can't be deleted");

        profiles.delete(custom.getId());
        assertEquals(LoggingProfile.FULL_ID, profiles.selected().getId());
        LoggingProfiles restarted = new LoggingProfiles(node);
        assertNull(restarted.get(custom.getId()));
        assertEquals(3, restarted.list().size());
    }
}
