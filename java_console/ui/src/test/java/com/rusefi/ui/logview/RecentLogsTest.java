package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

class RecentLogsTest {
    @TempDir
    Path folder;

    private File log(Path dir, String name, int bytes) throws IOException {
        Path file = dir.resolve(name);
        Files.write(file, new byte[bytes]);
        return file.toFile();
    }

    @Test
    void folderLogsAndRememberedFilesNewestFirst(@TempDir Path elsewhere) throws IOException {
        File older = log(folder, "rusEFI_outputChannels_2026-10-20_08_00_00_000.mlg", 10);
        File newer = log(folder, "rusEFI_outputChannels_2026-10-21_19_44_54_929.mlg", 10);
        log(folder, "notes.txt", 10);
        File outside = log(elsewhere, "track-2026-10-21_21_00_00.msl", 10);
        File missing = elsewhere.resolve("gone.mlg").toFile();

        List<RecentLogs.Entry> entries = RecentLogs.find(folder.toFile(), Arrays.asList(outside, missing, newer));
        assertEquals(3, entries.size(), "the text file and the missing file are left out; no duplicates");
        assertEquals(outside.getAbsoluteFile(), entries.get(0).file);
        assertEquals(newer.getAbsoluteFile(), entries.get(1).file);
        assertEquals(older.getAbsoluteFile(), entries.get(2).file);
    }

    @Test
    void keepsTheTenNewest() throws IOException {
        for (int day = 1; day <= 12; day++) {
            log(folder, String.format("rusEFI_outputChannels_2026-10-%02d_10_00_00_000.mlg", day), 1);
        }
        List<RecentLogs.Entry> entries = RecentLogs.find(folder.toFile(), Collections.emptyList());
        assertEquals(RecentLogs.LIMIT, entries.size());
        assertEquals("rusEFI_outputChannels_2026-10-12_10_00_00_000.mlg", entries.get(0).file.getName());
    }

    @Test
    void recordedTimeFromTheNameOrTheFile() throws IOException {
        File named = log(folder, "rusEFI_outputChannels_2026-10-21_19_44_54_929.mlg", 1);
        assertEquals(LocalDateTime.of(2026, 10, 21, 19, 44, 54), RecentLogs.recorded(named));

        File plain = log(folder, "drive.mlg", 1);
        LocalDateTime time = LocalDateTime.of(2026, 9, 1, 12, 30);
        plain.setLastModified(time.atZone(ZoneId.systemDefault()).toInstant().toEpochMilli());
        assertEquals(time, RecentLogs.recorded(plain));
    }

    @Test
    void friendlyDates() {
        LocalDate today = LocalDate.of(2026, 10, 22);
        assertEquals("Today 19:44", RecentLogs.when(LocalDateTime.of(2026, 10, 22, 19, 44), today));
        assertEquals("Yesterday 08:05", RecentLogs.when(LocalDateTime.of(2026, 10, 21, 8, 5), today));
        assertEquals("18 Oct 2026", RecentLogs.when(LocalDateTime.of(2026, 10, 18, 21, 15), today));
    }

    @Test
    void sizes() {
        assertEquals("512 B", RecentLogs.size(512));
        assertEquals("640 KB", RecentLogs.size(640 * 1024));
        assertEquals("1.2 MB", RecentLogs.size(1258291));
    }

    @Test
    void durations() {
        assertEquals("", RecentLogs.duration(Double.NaN), "unknown: nothing shown");
        assertEquals("17.0 s", RecentLogs.duration(17.023));
        assertEquals("2 min 15 s", RecentLogs.duration(135.2));
        assertEquals("1 h 05 min", RecentLogs.duration(3900));
    }

    @Test
    void durationIsCachedUntilTheFileChanges() throws IOException {
        Node durations = new Node();
        File file = log(folder, "track.msl", 100);
        assertTrue(Double.isNaN(RecentLogs.duration(durations, file)), "never opened");

        RecentLogs.rememberDuration(durations, file, 13.2);
        assertEquals(13.2, RecentLogs.duration(durations, file), 1e-9);
        assertEquals(13.2, RecentLogs.find(folder.toFile(), Collections.emptyList(), durations).get(0).duration, 1e-9);

        // re-recorded under the same name: the old duration no longer applies
        Files.write(file.toPath(), new byte[250]);
        assertTrue(Double.isNaN(RecentLogs.duration(durations, file)));
    }

    @Test
    void cacheKeepsOnlyListedLogs() throws IOException {
        Node durations = new Node();
        File listed = log(folder, "kept.mlg", 10);
        RecentLogs.rememberDuration(durations, listed, 5);
        RecentLogs.rememberDuration(durations, new File(folder.toFile(), "deleted.mlg"), 7);
        RecentLogs.pruneDurations(durations, RecentLogs.find(folder.toFile(), Collections.emptyList(), durations));
        assertEquals(1, durations.getConfig().size());
        assertEquals(5, RecentLogs.duration(durations, listed), 1e-9);
    }

    @Test
    void rememberPutsTheNewestFirstWithoutDuplicates() {
        Node settings = new Node();
        for (int i = 0; i < 12; i++) {
            RecentLogs.remember(settings, new File("log" + i + ".mlg"));
        }
        RecentLogs.remember(settings, new File("log5.mlg"));
        List<File> remembered = RecentLogs.remembered(settings);
        assertEquals(RecentLogs.LIMIT, remembered.size());
        assertEquals(new File("log5.mlg").getAbsoluteFile(), remembered.get(0));
        assertEquals(new File("log11.mlg").getAbsoluteFile(), remembered.get(1));
        assertEquals(1, remembered.stream().filter(f -> f.getName().equals("log5.mlg")).count());
    }
}
