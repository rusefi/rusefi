package com.rusefi.util;

import com.rusefi.tune.xml.Constant;
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
import java.util.OptionalLong;
import java.util.concurrent.atomic.AtomicReference;
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

        List<RestorePointStore.RestorePoint> points = store.list(SIGNATURE, "");
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
        assertEquals(3, store.list(SIGNATURE, "").size());
    }

    @Test
    void aPointMadeByHandIsSavedEvenWhenTheSame() throws Exception {
        TestClock clock = new TestClock();
        RestorePointStore store = new RestorePointStore(root, 50, clock);
        store.save(tune("10"), RestorePointStore.Reason.BEFORE_BURN, null);
        clock.now = clock.now.plusSeconds(60);
        RestorePointStore.RestorePoint manual = store.saveAlways(tune("10"), RestorePointStore.Reason.MANUAL,
                "before idle work");
        List<RestorePointStore.RestorePoint> points = store.list(SIGNATURE, "");
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
        List<RestorePointStore.RestorePoint> points = store.list(SIGNATURE, "");
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
        List<RestorePointStore.RestorePoint> points = store.list(SIGNATURE, "");
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
        assertEquals(1, store.list(SIGNATURE, "").size());
    }

    @Test
    void deleteRemovesThePoint() throws Exception {
        RestorePointStore store = new RestorePointStore(root, 50, new TestClock());
        RestorePointStore.RestorePoint point = store.save(tune("10"), RestorePointStore.Reason.MANUAL, null)
                .orElseThrow(AssertionError::new);
        store.delete(point);
        assertTrue(store.list(SIGNATURE, "").isEmpty());
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

    private static Msq tune(String fuel, String vehicleName) {
        Msq tune = tune(fuel);
        tune.findPage().constant.add(new Constant(RestorePointStore.VEHICLE_NAME, "", vehicleName, "0"));
        return tune;
    }

    @Test
    void eachCarHasItsOwnFolderAndLimit() throws Exception {
        TestClock clock = new TestClock();
        RestorePointStore store = new RestorePointStore(root, 2, clock);
        store.setSerialSourceForTests(OptionalLong::empty);

        for (int i = 0; i < 3; i++) {
            clock.now = clock.now.plusSeconds(60);
            store.save(tune("1" + i, "Orange Miata"), RestorePointStore.Reason.BEFORE_BURN, null);
        }
        RestorePointStore.RestorePoint hunchback = store.save(tune("20", "Hunchback"),
                RestorePointStore.Reason.BEFORE_BURN, null).orElseThrow(AssertionError::new);

        assertEquals(root.resolve("uaefi").resolve("Hunchback"), hunchback.file.getParent());
        assertEquals(2, store.list(SIGNATURE, "Orange Miata").size(), "the other car does not use up its limit");
        assertEquals(1, store.list(SIGNATURE, "Hunchback").size());
        assertEquals(List.of(), store.list(SIGNATURE, ""), "the board folder keeps only unnamed cars' points");
    }

    @Test
    void unnamedCarGoesByTheEcuSerial() throws Exception {
        RestorePointStore store = new RestorePointStore(root, 50, new TestClock());
        store.setSerialSourceForTests(() -> OptionalLong.of(3405491532L));
        RestorePointStore.RestorePoint point = store.save(tune("10", " "), RestorePointStore.Reason.MANUAL, null)
                .orElseThrow(AssertionError::new);
        assertEquals(root.resolve("uaefi").resolve("ecu-3405491532"), point.file.getParent());

        store.setSerialSourceForTests(OptionalLong::empty);
        assertEquals(root.resolve("uaefi"), store.folderFor(SIGNATURE, ""), "no name and no serial: the board folder");
    }

    @Test
    void beforeBurnPointGoesWithTheCarBeingBurned() throws Exception {
        RestorePointStore store = new RestorePointStore(root, 50, new TestClock());
        store.setSerialSourceForTests(() -> OptionalLong.of(7));
        // the ECU's tune has no name yet; the one being burned sets it
        RestorePointStore.RestorePoint point = store.save(tune("10"), "Orange Miata",
                RestorePointStore.Reason.BEFORE_BURN, null).orElseThrow(AssertionError::new);
        assertEquals(root.resolve("uaefi").resolve("Orange Miata"), point.file.getParent());
    }

    @Test
    void vehicleNamesBecomeSafeFolderNames() {
        OptionalLong none = OptionalLong.empty();
        assertEquals(Optional.of("Orange Miata"), RestorePointStore.carOf("  Orange Miata ", none));
        assertEquals(Optional.of("a_b_c"), RestorePointStore.carOf("a/b:c", none));
        assertEquals(Optional.of("Miata"), RestorePointStore.carOf("Miata. ", none), "no trailing dot on Windows");
        assertEquals(Optional.of("ecu-1"), RestorePointStore.carOf("..", OptionalLong.of(1)));
        assertEquals(Optional.of("ecu-1"), RestorePointStore.carOf("con", OptionalLong.of(1)), "reserved on Windows");
        assertEquals(Optional.of("ecu-4294967295"), RestorePointStore.carOf(null, OptionalLong.of(4294967295L)));
        assertEquals(Optional.empty(), RestorePointStore.carOf("", OptionalLong.of(0)), "0: no serial reported");
        assertTrue(RestorePointStore.isUnnamed(" "));
        assertFalse(RestorePointStore.isUnnamed("Hunchback"));
    }

    @Test
    void readsTheVehicleNameOfATune() {
        assertEquals("Orange Miata", RestorePointStore.vehicleNameOf(tune("1", "Orange Miata")));
        assertEquals("", RestorePointStore.vehicleNameOf(tune("1")));
    }

    @Test
    void aChosenFolderIsRememberedAndUsed() throws Exception {
        AtomicReference<Path> remembered = new AtomicReference<>(root);
        Path defaultRoot = root.resolve("default");
        RestorePointStore store = new RestorePointStore(defaultRoot, null, remembered::set, 50, new TestClock());
        assertTrue(store.isDefaultRoot());

        Path synced = root.resolve("Dropbox").resolve("rusEFI");
        store.setRoot(synced);
        assertEquals(synced.toAbsolutePath().normalize(), remembered.get());
        RestorePointStore.RestorePoint point = store.save(tune("10"), RestorePointStore.Reason.MANUAL, null)
                .orElseThrow(AssertionError::new);
        assertTrue(point.file.startsWith(synced.toAbsolutePath()), point.file.toString());

        RestorePointStore restarted = new RestorePointStore(defaultRoot, remembered.get(), remembered::set, 50,
                new TestClock());
        assertEquals(1, restarted.list(SIGNATURE, "").size(), "the next start uses the chosen folder");

        restarted.setRoot(null);
        assertNull(remembered.get(), "the default is remembered as no choice");
        assertTrue(restarted.isDefaultRoot());
    }

    @Test
    void movingKeepsBoardAndCarFoldersAndSkipsPointsAlreadyThere() throws Exception {
        TestClock clock = new TestClock();
        Path from = root.resolve("from");
        Path to = root.resolve("to");
        RestorePointStore store = new RestorePointStore(from, 50, clock);
        store.setSerialSourceForTests(OptionalLong::empty);
        RestorePointStore.RestorePoint unnamed = store.save(tune("10"), RestorePointStore.Reason.MANUAL, null)
                .orElseThrow(AssertionError::new);
        clock.now = clock.now.plusSeconds(60);
        store.save(tune("11", "Miata"), RestorePointStore.Reason.MANUAL, null);
        Files.writeString(from.resolve("uaefi").resolve("notes.txt"), "mine");
        // the same point synced from another PC
        Path already = to.resolve("uaefi").resolve(unnamed.file.getFileName().toString());
        Files.createDirectories(already.getParent());
        Files.copy(unnamed.file, already);

        assertEquals(2, RestorePointStore.countMovable(from, to));
        assertEquals(1, RestorePointStore.movePoints(from, to).moved);
        assertTrue(Files.exists(to.resolve("uaefi").resolve("Miata").resolve("2026-11-23_14-03-05_manual.msq")));
        assertTrue(Files.exists(unnamed.file), "not overwritten, left where it was");
        assertTrue(Files.exists(from.resolve("uaefi").resolve("notes.txt")), "other files stay");
    }

    @Test
    void aPointThatCannotBeMovedStaysAndTheRestAreMoved() throws Exception {
        TestClock clock = new TestClock();
        Path from = root.resolve("from");
        Path to = root.resolve("to");
        RestorePointStore store = new RestorePointStore(from, 50, clock);
        store.setSerialSourceForTests(OptionalLong::empty);
        RestorePointStore.RestorePoint blocked = store.save(tune("10"), RestorePointStore.Reason.MANUAL, null)
                .orElseThrow(AssertionError::new);
        clock.now = clock.now.plusSeconds(60);
        store.save(tune("11", "Miata"), RestorePointStore.Reason.MANUAL, null);
        // a file where the board folder should be: the unnamed point cannot go there, the car folder's can't either
        Files.createDirectories(to);
        Files.writeString(to.resolve("uaefi"), "in the way");

        RestorePointStore.MoveResult result = RestorePointStore.movePoints(from, to);
        assertEquals(0, result.moved);
        assertEquals(2, result.notMoved.size());
        assertNotNull(result.firstFailure);
        assertTrue(Files.exists(blocked.file), "left in the old folder");

        Files.delete(to.resolve("uaefi"));
        RestorePointStore.MoveResult again = RestorePointStore.movePoints(from, to);
        assertEquals(2, again.moved, "trying again moves them");
        assertEquals(List.of(), again.notMoved);
        assertNull(again.firstFailure);
    }

    @Test
    void aFolderInsideTheOldOneIsNotMovedIntoItself() throws Exception {
        RestorePointStore store = new RestorePointStore(root, 50, new TestClock());
        store.setSerialSourceForTests(OptionalLong::empty);
        store.save(tune("10"), RestorePointStore.Reason.MANUAL, null);
        Path inside = root.resolve("sync");
        assertEquals(1, RestorePointStore.movePoints(root, inside).moved);
        assertEquals(0, RestorePointStore.countMovable(root, inside), "what is in the new folder stays");
        assertEquals(0, RestorePointStore.countMovable(root, root));
    }

    @Test
    void listsTheBoardsCarsThatHavePoints() throws Exception {
        TestClock clock = new TestClock();
        RestorePointStore store = new RestorePointStore(root, 50, clock);
        store.setSerialSourceForTests(OptionalLong::empty);
        assertEquals(List.of(), store.carFolders(SIGNATURE));

        store.save(tune("10", "orange Miata"), RestorePointStore.Reason.MANUAL, null);
        store.save(tune("11", "Hunchback"), RestorePointStore.Reason.MANUAL, null);
        Files.createDirectories(root.resolve("uaefi").resolve("empty car"));
        Path board = root.resolve("uaefi");
        assertEquals(List.of(board.resolve("Hunchback"), board.resolve("orange Miata")),
                store.carFolders(SIGNATURE), "by name, ignoring case; folders without points left out");

        store.save(tune("12"), RestorePointStore.Reason.MANUAL, null);
        assertEquals(board, store.carFolders(SIGNATURE).get(0), "unnamed and older points first");
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
