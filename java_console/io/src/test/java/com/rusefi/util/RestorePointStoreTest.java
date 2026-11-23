package com.rusefi.util;

import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.file.Files;
import java.nio.file.Path;
import java.time.Clock;
import java.time.Instant;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.ZoneOffset;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.*;

class RestorePointStoreTest {
    private static final String SIGNATURE = "rusEFI master.2026.11.20.uaefi.3405491532";
    @TempDir
    Path root;

    /** A clock the test moves on by hand. */
    private static final class TestClock extends Clock {
        Instant now = LocalDateTime.of(2026, 11, 23, 14, 2, 5).toInstant(ZoneOffset.UTC);

        @Override
        public ZoneId getZone() {
            return ZoneOffset.UTC;
        }

        @Override
        public Clock withZone(ZoneId zone) {
            return this;
        }

        @Override
        public Instant instant() {
            return now;
        }
    }

    private static Msq tune(String fuel) {
        Msq tune = TuneSnapshotTest.tune(fuel);
        tune.versionInfo.setSignature(SIGNATURE);
        return tune;
    }

    @Test
    void savesNamedPointsInTheBoardFolderNewestFirst() throws Exception {
        TestClock clock = new TestClock();
        RestorePointStore store = new RestorePointStore(root, 50, clock);

        RestorePointStore.RestorePoint first = store.save(tune("10"), RestorePointStore.Reason.BEFORE_BURN, null)
                .orElseThrow(AssertionError::new);
        assertEquals(root.resolve("uaefi").resolve("2026-11-23_14-02-05_before-burn.msq"), first.file);

        clock.now = clock.now.plusSeconds(60);
        store.save(tune("11"), RestorePointStore.Reason.MANUAL, "  before idle work ");

        List<RestorePointStore.RestorePoint> points = store.list(SIGNATURE);
        assertEquals(2, points.size());
        assertEquals(RestorePointStore.Reason.MANUAL, points.get(0).reason, "newest first");
        assertEquals(LocalDateTime.of(2026, 11, 23, 14, 3, 5), points.get(0).when);
        assertEquals("before idle work", points.get(0).readNote());
        assertEquals("", points.get(1).readNote());
        assertEquals("11", points.get(0).readTune().getConstantsAsMap().get("fuel").getValue());
    }

    @Test
    void theSameTuneAsTheNewestIsNotSavedAgain() throws Exception {
        TestClock clock = new TestClock();
        RestorePointStore store = new RestorePointStore(root, 50, clock);
        assertTrue(store.save(tune("10"), RestorePointStore.Reason.BEFORE_BURN, null).isPresent());
        clock.now = clock.now.plusSeconds(10);
        Msq same = tune("10");
        same.bibliography.setTuneComment("a comment is not a calibration change");
        assertEquals(Optional.empty(), store.save(same, RestorePointStore.Reason.BEFORE_BURN, "note"));
        assertEquals("a comment is not a calibration change", same.bibliography.getTuneComment(),
                "the caller's tune is left as it was");

        clock.now = clock.now.plusSeconds(10);
        assertTrue(store.save(tune("11"), RestorePointStore.Reason.BEFORE_BURN, null).isPresent());
        clock.now = clock.now.plusSeconds(10);
        assertTrue(store.save(tune("10"), RestorePointStore.Reason.BEFORE_BURN, null).isPresent(),
                "only the newest point is compared: going back to an older tune is a new point");
        assertEquals(3, store.list(SIGNATURE).size());
    }

    @Test
    void aPointMadeByHandIsSavedEvenWhenTheSame() throws Exception {
        TestClock clock = new TestClock();
        RestorePointStore store = new RestorePointStore(root, 50, clock);
        store.save(tune("10"), RestorePointStore.Reason.BEFORE_BURN, null);
        clock.now = clock.now.plusSeconds(60);
        RestorePointStore.RestorePoint manual = store.saveAlways(tune("10"), RestorePointStore.Reason.MANUAL,
                "before idle work");
        List<RestorePointStore.RestorePoint> points = store.list(SIGNATURE);
        assertEquals(2, points.size(), "the user asked for it, with a note");
        assertEquals(manual.file, points.get(0).file);
        assertEquals("before idle work", points.get(0).readNote());
    }

    @Test
    void twoPointsInOneSecondKeepTheirOrder() throws Exception {
        RestorePointStore store = new RestorePointStore(root, 50, new TestClock());
        store.save(tune("10"), RestorePointStore.Reason.BEFORE_LOAD, null);
        RestorePointStore.RestorePoint second = store.save(tune("11"), RestorePointStore.Reason.BEFORE_BURN, null)
                .orElseThrow(AssertionError::new);
        assertEquals("2026-11-23_14-02-05-1_before-burn.msq", second.file.getFileName().toString());
        List<RestorePointStore.RestorePoint> points = store.list(SIGNATURE);
        assertEquals(second.file, points.get(0).file, "the later one is newer");
        assertEquals(RestorePointStore.Reason.BEFORE_LOAD, points.get(1).reason);
    }

    @Test
    void onlyTheNewestAreKept() throws Exception {
        TestClock clock = new TestClock();
        RestorePointStore store = new RestorePointStore(root, 3, clock);
        for (int i = 0; i < 5; i++) {
            clock.now = clock.now.plusSeconds(60);
            store.save(tune(String.valueOf(i)), RestorePointStore.Reason.BEFORE_BURN, null);
        }
        List<RestorePointStore.RestorePoint> points = store.list(SIGNATURE);
        assertEquals(3, points.size());
        assertEquals("4", points.get(0).readTune().getConstantsAsMap().get("fuel").getValue());
        assertEquals("2", points.get(2).readTune().getConstantsAsMap().get("fuel").getValue());
        try (Stream<Path> files = Files.list(store.folderFor(SIGNATURE))) {
            assertEquals(3, files.count(), "no temporary files left behind");
        }
    }

    @Test
    void otherFilesInTheFolderAreLeftAlone() throws Exception {
        RestorePointStore store = new RestorePointStore(root, 1, new TestClock());
        Path folder = store.folderFor(SIGNATURE);
        Files.createDirectories(folder);
        Files.writeString(folder.resolve("my tune.msq"), "mine");
        Files.writeString(folder.resolve("2026-11-23_14-02-05_unknown-reason.msq"), "not ours");
        store.save(tune("10"), RestorePointStore.Reason.MANUAL, null);
        store.save(tune("11"), RestorePointStore.Reason.MANUAL, null);
        try (Stream<Path> files = Files.list(folder)) {
            assertEquals(3, files.count(), "pruning only removes restore points");
        }
        assertEquals(1, store.list(SIGNATURE).size());
    }

    @Test
    void deleteRemovesThePoint() throws Exception {
        RestorePointStore store = new RestorePointStore(root, 50, new TestClock());
        RestorePointStore.RestorePoint point = store.save(tune("10"), RestorePointStore.Reason.MANUAL, null)
                .orElseThrow(AssertionError::new);
        store.delete(point);
        assertTrue(store.list(SIGNATURE).isEmpty());
    }

    @Test
    void boardsGetTheirOwnFolders() {
        assertEquals("uaefi", RestorePointStore.boardOf(SIGNATURE));
        assertEquals("proteus_f7", RestorePointStore.boardOf("rusEFI 2026.11.20.proteus_f7.123"));
        assertEquals("custom_signature", RestorePointStore.boardOf("custom signature"));
        assertEquals("v1_2", RestorePointStore.boardOf("v1.2"), "no numeric hash: the whole signature");
        assertEquals("unknown", RestorePointStore.boardOf(null));
        assertEquals("unknown", RestorePointStore.boardOf("../.."));
    }

    @Test
    void parsesOnlyRestorePointNames() {
        assertTrue(RestorePointStore.parse(Path.of("2026-11-23_14-02-05_manual.msq")).isPresent());
        assertTrue(RestorePointStore.parse(Path.of("2026-11-23_14-02-05-2_before-update.msq")).isPresent());
        List<String> rejected = Stream.of("2026-11-23.msq", "2026-11-23_14-02-05_manual.txt",
                        "2026-13-23_14-02-05_manual.msq", "2026-11-23_14-02-05x_manual.msq", "short.msq")
                .filter(name -> RestorePointStore.parse(Path.of(name)).isPresent())
                .collect(Collectors.toList());
        assertEquals(List.of(), rejected);
    }
}
