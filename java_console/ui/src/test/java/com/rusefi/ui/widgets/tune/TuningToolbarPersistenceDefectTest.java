package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelMocks;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.field.ScalarIniField;
import com.opensr5.ini.field.StringIniField;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Integration;
import com.rusefi.config.FieldType;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class TuningToolbarPersistenceDefectTest {
    private static final int LUA_PAGE = 0x0400;

    @Test
    void approvingConnectedRestoreChangesRamButNeverBurns(@org.junit.jupiter.api.io.TempDir java.nio.file.Path dir)
        throws Exception {
        checkConnectedRestore(dir, false);
    }

    @Test
    void editDuringReviewInvalidatesRestoreAndResumesItsUpload(@org.junit.jupiter.api.io.TempDir java.nio.file.Path dir)
        throws Exception {
        checkConnectedRestore(dir, true);
    }

    private static void checkConnectedRestore(java.nio.file.Path dir, boolean editDuringReview) throws Exception {
        IniFileModel ini = realIni();
        com.rusefi.util.RestorePointStore store = new com.rusefi.util.RestorePointStore(dir, 50, java.time.Clock.systemUTC());
        Fixture[] fixture = new Fixture[1];
        ArrayBlockingQueue<JComponent> reviewCard = new ArrayBlockingQueue<>(1);
        CountDownLatch finished = new CountDownLatch(1);
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(ini, Runnable::run);
                fixture[0].toolbar.setRestorePointStoreForTests(store);
                fixture[0].toolbar.setTuneReviewHost(panel -> {
                    reviewCard.add(panel);
                    return () -> { };
                });
                fixture[0].toolbar.getLoadTuneAction().addPropertyChangeListener(e -> {
                    if ("enabled".equals(e.getPropertyName()) && Boolean.TRUE.equals(e.getNewValue())) {
                        finished.countDown();
                    }
                });
            });
            Fixture current = fixture[0];
            com.rusefi.util.RestorePointStore.RestorePoint point = store.saveAlways(com.rusefi.tune.xml.MsqFactory.valueOf(
                ConfigurationImageTestHelper.newLittleEndian(current.b), ini),
                com.rusefi.util.RestorePointStore.Reason.MANUAL, null);
            runOnEdt(() -> assertTrue(current.toolbar.reviewRestorePoint(point)));
            JComponent review = reviewCard.poll(10, TimeUnit.SECONDS);
            assertNotNull(review);
            runOnEdt(() -> {
                assertEquals(0, current.protocol.writeCount);
                if (editDuringReview) {
                    current.editTo(current.c);
                }
                JButton restore = findButton(review, "Restore selected as changes");
                assertNotNull(restore);
                restore.doClick(0);
            });
            assertTrue(finished.await(10, TimeUnit.SECONDS));
            if (editDuringReview) {
                runOnEdt(() -> {
                    assertArrayEquals(current.a, current.protocol.ram, "the stale approval does not write");
                    assertTrue(current.toolbar.isUploadTimerRunningForUnitTest(), "the edit still needs uploading");
                    current.toolbar.flushUploadTimerForUnitTest();
                });
            }
            assertArrayEquals(editDuringReview ? current.c : current.b, current.protocol.ram);
            assertArrayEquals(current.a, current.protocol.flash);
            assertEquals(0, current.protocol.burnCount);
            assertEquals(1, store.list(RESTORE_SIGNATURE, "").size(), "restoring does not create a load or burn point");
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void tuneLoadCheckpointsFollowConsentAndPrecedeWrites(@org.junit.jupiter.api.io.TempDir java.nio.file.Path dir)
        throws Exception {
        IniFileModel ini = realIni();
        com.rusefi.util.RestorePointStore store = new com.rusefi.util.RestorePointStore(dir.resolve("points"), 50,
            java.time.Clock.systemUTC());
        Fixture[] fixture = new Fixture[1];
        ArrayBlockingQueue<JComponent> reviews = new ArrayBlockingQueue<>(1);
        String path = dir.resolve("new-tune.msq").toString();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(ini, Runnable::run);
                fixture[0].toolbar.setRestorePointStoreForTests(store);
                fixture[0].toolbar.setTuneReviewHost(panel -> {
                    reviews.add(panel);
                    return () -> { };
                });
            });
            Fixture current = fixture[0];
            List<Integer> writesAtCheckpoint = new ArrayList<>();
            store.addListener(() -> writesAtCheckpoint.add(current.protocol.writeCount));
            com.rusefi.tune.xml.MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(current.b), ini)
                .writeXmlFile(path);
            for (String action : List.of("Cancel", "Apply and Burn")) {
                CountDownLatch finished = new CountDownLatch(1);
                runOnEdt(() -> {
                    current.toolbar.getLoadTuneAction().addPropertyChangeListener(e -> {
                        if ("enabled".equals(e.getPropertyName()) && Boolean.TRUE.equals(e.getNewValue())) {
                            finished.countDown();
                        }
                    });
                    if ("Apply and Burn".equals(action)) {
                        byte[] working = current.a.clone();
                        working[1] = 21;
                        current.editTo(working);
                    }
                    assertTrue(current.toolbar.reviewTuneFile(path));
                });
                JComponent review = reviews.poll(10, TimeUnit.SECONDS);
                assertNotNull(review);
                assertTrue(store.list(RESTORE_SIGNATURE, "").isEmpty());
                assertEquals(0, current.protocol.writeCount);
                runOnEdt(() -> {
                    JButton accept = findButton(review, action);
                    assertNotNull(accept);
                    accept.doClick(0);
                });
                assertTrue(finished.await(10, TimeUnit.SECONDS));
            }
            assertArrayEquals(current.b, current.protocol.flash);
            assertEquals(1, current.protocol.burnCount);
            Map<com.rusefi.util.RestorePointStore.Reason, Integer> fuel = new java.util.HashMap<>();
            for (com.rusefi.util.RestorePointStore.RestorePoint point : store.list(RESTORE_SIGNATURE, "")) {
                fuel.put(point.reason, Byte.toUnsignedInt(point.readTune().asImage(ini).getContent()[1]));
            }
            assertEquals(Map.of(com.rusefi.util.RestorePointStore.Reason.BEFORE_LOAD, 21,
                com.rusefi.util.RestorePointStore.Reason.BEFORE_BURN, 2), fuel);
            assertEquals(List.of(0, 0), writesAtCheckpoint, "both checkpoints precede the first RAM write");
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void loadingTuneWritesRamAndRequiresExplicitBurn() throws Exception {
        runOnEdt(() -> {
            try (Fixture current = new Fixture()) {
                TuningToolbarWidget.uploadLoadedTune(current.protocol, ConfigurationImageTestHelper.newLittleEndian(current.b));

                assertArrayEquals(current.b, current.protocol.ram);
                // Reviewed Load Tune only applies to RAM; flash stays unchanged (#10567).
                assertArrayEquals(current.a, current.protocol.flash);
                assertEquals(0, current.protocol.burnCount);
            }
        });
    }

    @Test
    void mixedPageReviewDoesNotWriteUntilAcceptedAndRequiresAnExplicitBurn() throws Exception {
        runOnEdt(() -> {
            try (Fixture current = new Fixture()) {
                IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
                when(meta.getnPages()).thenReturn(2);
                when(meta.getPageIdentifier(1)).thenReturn(LUA_PAGE);
                when(meta.getPageSize(1)).thenReturn(8);
                when(meta.getBurnCommand(1)).thenReturn("B");
                when(current.ini.getMetaInfo()).thenReturn(meta);
                StringIniField lua = new StringIniField("luaScript", 0, 8);
                lua.setPageIndex(LUA_PAGE);
                ScalarIniField scalar = new ScalarIniField("fuelSetting", 1, "", FieldType.UINT8, 1, "0", 0);
                when(current.ini.getAllIniFields()).thenReturn(Map.of(scalar.getName(), scalar));
                when(current.ini.getSecondaryIniFields()).thenReturn(Map.of(lua.getName(), lua));
                ConfigurationImage previous = ConfigurationImageTestHelper.newLittleEndian(new byte[8]);
                ConfigurationImage next = ConfigurationImageTestHelper.newLittleEndian(new byte[]{'x', 0, 0, 0, 0, 0, 0, 0});
                ConfigurationImage main = current.protocol.getControllerConfiguration().clone();
                ConfigurationImage nextMain = ConfigurationImageTestHelper.newLittleEndian(current.b);
                Map<Integer, ConfigurationImage> original = Map.of(0, main, LUA_PAGE, previous);
                Map<Integer, ConfigurationImage> incoming = Map.of(0, nextMain, LUA_PAGE, next);
                TuneLoadPlan plan = TuneLoadPlan.create(current.ini, original, incoming, List.of());
                assertEquals(2, plan.changes().size());

                TuningToolbarWidget.applyReviewedTuneToRam(current.protocol, plan, null, original,
                    () -> fail("Cancelled preview must not write."));
                TuningToolbarWidget.applyReviewedTuneToRam(current.protocol, plan, Set.of(), original,
                    () -> fail("An empty selection must not write."));
                current.protocol.luaRam[0] = 'z';
                assertThrows(IllegalStateException.class, () -> TuningToolbarWidget.applyReviewedTuneToRam(
                    current.protocol, plan, Set.copyOf(plan.changes()), original, () -> fail("Stale preview must not write.")));
                assertEquals(0, current.protocol.writeCount);
                current.protocol.luaRam[0] = 0;

                AtomicBoolean invalidated = new AtomicBoolean();
                current.protocol.onLuaRead = () -> invalidated.set(true);
                assertThrows(IllegalStateException.class, () -> TuningToolbarWidget.applyReviewedTuneToRam(
                    current.protocol, plan, Set.copyOf(plan.changes()), original, () -> {
                        if (invalidated.get()) {
                            throw new IllegalStateException("Review invalidated while reading the ECU.");
                        }
                    }));
                assertEquals(0, current.protocol.writeCount);
                current.protocol.onLuaRead = () -> {};

                TuningToolbarWidget.applyReviewedTuneToRam(current.protocol, plan, Set.copyOf(plan.changes()), original, () -> {});
                assertArrayEquals(current.b, current.protocol.ram);
                assertArrayEquals(current.a, current.protocol.flash);
                assertEquals('x', current.protocol.luaRam[0]);
                assertEquals(0, current.protocol.luaFlash[0]);
                assertEquals(0, current.protocol.burnCount);
                current.toolbar.adoptReviewedTune(current.calibration, new AtomicReference<>(), current.ini,
                    original, incoming);
                button(current.toolbar, "Undo").doClick(0);
                assertArrayEquals(current.a, current.protocol.ram);
                assertEquals(0, current.protocol.luaRam[0]);
                assertEquals(0, current.protocol.burnCount);
                button(current.toolbar, "Redo").doClick(0);
                assertArrayEquals(current.b, current.protocol.ram);
                assertEquals('x', current.protocol.luaRam[0]);
                assertEquals(0, current.protocol.burnCount);
                assertTrue(current.toolbar.hasUnsavedFileChanges(nextMain));
                button(current.toolbar, "Burn to ECU").doClick();
                assertEquals('x', current.protocol.luaFlash[0]);
                assertEquals(2, current.protocol.burnCount);
                assertArrayEquals(current.b, current.protocol.flash);
                assertTrue(current.toolbar.hasUnsavedFileChanges(nextMain));
            }
        });
    }

    @Test
    void explicitReviewBurnPersistsOnlySelectedChangesAndKeepsExistingRamBytes() throws Exception {
        runOnEdt(() -> {
            try (Fixture current = new Fixture()) {
                IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
                when(meta.getnPages()).thenReturn(2);
                when(meta.getPageIdentifier(1)).thenReturn(LUA_PAGE);
                when(meta.getPageSize(1)).thenReturn(8);
                when(meta.getBurnCommand(1)).thenReturn("B");
                when(current.ini.getMetaInfo()).thenReturn(meta);
                ScalarIniField selected = new ScalarIniField("selectedSetting", 1, "", FieldType.UINT8, 1, "0", 0);
                ScalarIniField unselected = new ScalarIniField("unselectedSetting", 4, "", FieldType.UINT8, 1, "0", 0);
                StringIniField lua = new StringIniField("luaScript", 0, 8);
                lua.setPageIndex(LUA_PAGE);
                when(current.ini.getAllIniFields()).thenReturn(Map.of(
                    selected.getName(), selected, unselected.getName(), unselected));
                when(current.ini.getSecondaryIniFields()).thenReturn(Map.of(lua.getName(), lua));

                byte[] ecuRam = current.a.clone();
                ecuRam[7] = 99; // an ECU-side RAM edit already present before this review
                current.protocol.uploadChangesWithoutBurn(ConfigurationImageTestHelper.newLittleEndian(ecuRam));
                ConfigurationImage main = current.protocol.getControllerConfiguration().clone();
                ConfigurationImage incomingMain = main.clone();
                incomingMain.getContent()[1] = 42;
                incomingMain.getContent()[4] = 77;
                ConfigurationImage oldLua = ConfigurationImageTestHelper.newLittleEndian(new byte[8]);
                ConfigurationImage incomingLua = ConfigurationImageTestHelper.newLittleEndian(
                    new byte[]{'x', 0, 0, 0, 0, 0, 0, 0});
                Map<Integer, ConfigurationImage> devicePages = Map.of(0, main, LUA_PAGE, oldLua);
                Map<Integer, ConfigurationImage> incoming = Map.of(0, incomingMain, LUA_PAGE, incomingLua);
                TuneLoadPlan plan = TuneLoadPlan.create(current.ini, devicePages, incoming, List.of());
                TuneLoadPlan.Change selectedChange = plan.changes().stream()
                    .filter(change -> change.label().equals("selectedSetting")).findFirst().orElseThrow();
                TuneLoadPlan.Change luaChange = plan.changes().stream()
                    .filter(TuneLoadPlan.Change::isLua).findFirst().orElseThrow();

                Map<Integer, ConfigurationImage> target = TuningToolbarWidget.applyReviewedTuneToRam(
                    current.protocol, plan, Set.of(selectedChange, luaChange), devicePages, () -> { });
                assertEquals(42, current.protocol.ram[1]);
                assertEquals(ecuRam[4], current.protocol.ram[4], "unselected tune values must not be applied");
                assertEquals(ecuRam[7], current.protocol.ram[7], "pre-existing ECU RAM edits must be preserved");
                assertEquals('x', current.protocol.luaRam[0]);
                assertEquals(0, current.protocol.burnCount);

                Set<Integer> burnedPages = new java.util.TreeSet<>();
                TuningToolbarWidget.burnReviewedPages(current.protocol, devicePages, target, burnedPages);

                assertEquals(Set.of(0, LUA_PAGE), burnedPages, "only changed review pages should be persisted");
                assertArrayEquals(current.protocol.ram, current.protocol.flash);
                assertEquals('x', current.protocol.luaFlash[0]);
                assertEquals(2, current.protocol.burnCount);
                assertTrue(current.toolbar.hasUnsavedFileChanges(target.get(0)));
            }
        });
    }

    @Test
    void burnDoesNotMarkMainTuneAsSaved() throws Exception {
        Fixture[] fixture = new Fixture[1];
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                fixture[0].editToB();
                button(fixture[0].toolbar, "Burn to ECU").doClick();
            });
            runOnEdt(() -> {
                Fixture current = fixture[0];
                assertFalse(current.toolbar.hasUnsavedChanges(current.session.get()));
                assertTrue(current.toolbar.hasUnsavedFileChanges(current.session.get()));
            });
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void failedSecondaryBurnOnlyAcknowledgesMainPage() throws Exception {
        runOnEdt(() -> {
            try (Fixture current = new Fixture()) {
                IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
                when(meta.getnPages()).thenReturn(2);
                when(meta.getPageIdentifier(1)).thenReturn(LUA_PAGE);
                when(meta.getBurnCommand(1)).thenReturn("B");
                when(current.ini.getMetaInfo()).thenReturn(meta);
                ConfigurationImage before = current.protocol.getControllerConfiguration().clone();
                ConfigurationImage after = ConfigurationImageTestHelper.newLittleEndian(current.b);
                ConfigurationImage oldLua = ConfigurationImageTestHelper.newLittleEndian(new byte[8]);
                ConfigurationImage newLua = ConfigurationImageTestHelper.newLittleEndian(new byte[]{'x', 0, 0, 0, 0, 0, 0, 0});
                current.protocol.uploadChangesWithoutBurn(after);
                current.protocol.luaRam[0] = 'x';
                current.protocol.failLuaBurn = true;
                Set<Integer> acknowledged = new java.util.TreeSet<>();
                assertThrows(IllegalStateException.class, () -> TuningToolbarWidget.burnReviewedPages(current.protocol,
                    Map.of(0, before, LUA_PAGE, oldLua), Map.of(0, after, LUA_PAGE, newLua), acknowledged));
                assertEquals(Set.of(0), acknowledged);
                assertArrayEquals(current.b, current.protocol.flash);
                assertEquals('x', current.protocol.luaRam[0]);
                assertEquals(0, current.protocol.luaFlash[0]);
            }
        });
    }

    @Test
    void ordinaryPartialBurnOnlyAdvancesAcknowledgedBaselines() throws Exception {
        Fixture[] fixture = new Fixture[1];
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                Fixture current = fixture[0];
                IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
                when(meta.getnPages()).thenReturn(2);
                when(meta.getPageIdentifier(1)).thenReturn(LUA_PAGE);
                when(meta.getBurnCommand(1)).thenReturn("B");
                when(current.ini.getMetaInfo()).thenReturn(meta);
                current.context.setWorkingSecondaryTunePages(Map.of(LUA_PAGE,
                    ConfigurationImageTestHelper.newLittleEndian(new byte[8])));
                current.toolbar.resetTuneFileBaseline(current.session.get());
                current.context.setWorkingSecondaryTunePages(Map.of(LUA_PAGE,
                    ConfigurationImageTestHelper.newLittleEndian(new byte[]{'x', 0, 0, 0, 0, 0, 0, 0})));
                current.protocol.luaRam[0] = 'x';
                current.protocol.failLuaBurn = true;
                current.editToB();
                current.toolbar.setErrorHandler(message -> {});
                button(current.toolbar, "Burn to ECU").doClick(0);
            });
            runOnEdt(() -> {
                Fixture current = fixture[0];
                assertArrayEquals(current.b, current.protocol.flash);
                assertEquals(0, current.protocol.luaFlash[0]);
                assertArrayEquals(current.b, current.toolbar.getBaselineImage().getContent(),
                    "main flash was acknowledged even though Lua failed");
                assertTrue(current.toolbar.hasUnsavedChanges(current.session.get()));
            });
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void explicitReviewCanBurnSelectedMainSettingsAlreadyInRam() throws Exception {
        runOnEdt(() -> {
            try (Fixture current = new Fixture()) {
                current.protocol.uploadChangesWithoutBurn(ConfigurationImageTestHelper.newLittleEndian(current.b));
                Map<Integer, ConfigurationImage> device = Map.of(0, current.protocol.getControllerConfiguration().clone());
                Set<Integer> acknowledged = new java.util.TreeSet<>();
                TuningToolbarWidget.burnReviewedPages(current.protocol, device, device, acknowledged, true, () -> {});
                assertEquals(Set.of(0), acknowledged);
                assertArrayEquals(current.b, current.protocol.flash);
            }
        });
    }

    @Test
    void burnBeforeDebouncedUpload() throws Exception {
        Fixture[] fixture = new Fixture[1];
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                Fixture current = fixture[0];
                current.editToB();
                assertTrue(current.toolbar.isUploadTimerRunningForUnitTest());

                button(current.toolbar, "Burn to ECU").doClick();
                current.toolbar.flushUploadTimerForUnitTest();
            });
            // Burn completion posts the toolbar baseline update to the EDT.
            runOnEdt(() -> { });
            assertArrayEquals(fixture[0].b, fixture[0].toolbar.getBaselineImage().getContent());
            // Burn must upload the captured edit before persisting it, even ahead of the debounce.
            assertArrayEquals(fixture[0].b, fixture[0].protocol.ram);
            assertArrayEquals(fixture[0].b, fixture[0].protocol.flash);
            assertArrayEquals(fixture[0].b,
                fixture[0].protocol.getControllerConfiguration().getContent());
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    private static final String RESTORE_SIGNATURE = "rusEFI master.2026.11.20.uaefi.123";

    @Test
    void burningDoesNotSaveTheTuneFile(@org.junit.jupiter.api.io.TempDir java.nio.file.Path dir) throws Exception {
        Fixture[] fixture = new Fixture[1];
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(realIni(), Runnable::run);
                fixture[0].toolbar.setRestorePointStoreForTests(new com.rusefi.util.RestorePointStore(
                    dir.resolve("points"), 50, java.time.Clock.systemUTC()));
                fixture[0].editToB();
                button(fixture[0].toolbar, "Burn to ECU").doClick(0);
            });

            java.util.concurrent.CountDownLatch saved = new java.util.concurrent.CountDownLatch(1);
            runOnEdt(() -> {
                Fixture current = fixture[0];
                assertFalse(current.toolbar.hasUnsavedChanges(current.session.get()));
                assertTrue(current.toolbar.hasUnsavedFileChanges(current.session.get()));
                current.toolbar.saveTuneToPathAndThen(current.calibration, dir.resolve("saved.msq").toString(), saved::countDown);
            });
            assertTrue(saved.await(10, java.util.concurrent.TimeUnit.SECONDS));
            runOnEdt(() -> assertFalse(fixture[0].toolbar.hasUnsavedFileChanges(fixture[0].session.get())));
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    /** 8 bytes like the fixture's images; "fuel" is byte 1 (2 in a, 42 in b). */
    private static IniFileModel realIni() {
        String text = "[Constants]\n"
            + "signature = \"" + RESTORE_SIGNATURE + "\"\n"
            + "blockingFactor = 64\nnPages = 1\npageSize = 8\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
            + "fuel = scalar, U08, 1, \"ms\", 1, 0, 0, 255, 0\n";
        try {
            com.opensr5.ini.RawIniFile raw = com.rusefi.ini.reader.IniFileReaderUtil.read(
                new java.io.ByteArrayInputStream(text.getBytes(java.nio.charset.StandardCharsets.UTF_8)));
            return com.rusefi.ini.reader.IniFileReaderUtil.readIniFile(raw, "restore-test.ini",
                new com.opensr5.ini.IniFileMetaInfoImpl(raw));
        } catch (Throwable e) {
            throw new AssertionError("test .ini", e);
        }
    }

    /** [tag:restore_points] */
    @Test
    void burnKeepsTheEcuTuneItReplacesAsARestorePoint(@org.junit.jupiter.api.io.TempDir java.nio.file.Path dir)
        throws Exception {
        com.rusefi.util.RestorePointStore store =
            new com.rusefi.util.RestorePointStore(dir, 50, java.time.Clock.systemUTC());
        IniFileModel ini = realIni();
        Fixture[] fixture = new Fixture[1];
        java.util.concurrent.atomic.AtomicInteger opened = new java.util.concurrent.atomic.AtomicInteger();
        JComponent[] mirror = new JComponent[1];
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(ini, Runnable::run);
                fixture[0].toolbar.setRestorePointStoreForTests(store);
                fixture[0].toolbar.setShowRestorePoints(opened::incrementAndGet);
                mirror[0] = fixture[0].toolbar.createMirrorControls();
                fixture[0].editToB();
                button(fixture[0].toolbar, "Burn to ECU").doClick();
                fixture[0].toolbar.flushUploadTimerForUnitTest();
            });
            runOnEdt(() -> {
                // step 5: the note says the previous tune is kept, its link opens the restore points
                JLabel note = noteLabel(fixture[0].toolbar);
                assertTrue(note.getText().contains(TuningToolbarWidget.BURN_NOTE), note.getText());
                assertTrue(note.getText().contains("Restore points"));
                click(note);
                assertEquals(1, opened.get());
                JLabel onGauges = com.rusefi.ui.util.UiUtils.getAllComponents(mirror[0]).stream()
                    .filter(c -> c instanceof JLabel).map(c -> (JLabel) c).findFirst()
                    .orElseThrow(AssertionError::new);
                assertEquals(note.getText(), onGauges.getText(), "the Gauges toolbar repeats the note");
                click(onGauges);
                assertEquals(2, opened.get(), "and its link");
                fixture[0].toolbar.refreshState();
                assertNull(fixture[0].toolbar.getNoteActionForTests(), "the state replaces the note and its link");
                click(note);
                assertEquals(2, opened.get());
            });
            assertArrayEquals(fixture[0].b, fixture[0].protocol.flash);
            List<com.rusefi.util.RestorePointStore.RestorePoint> points = store.list(RESTORE_SIGNATURE, "");
            assertEquals(1, points.size());
            assertEquals(com.rusefi.util.RestorePointStore.Reason.BEFORE_BURN, points.get(0).reason);
            assertEquals("2", points.get(0).readTune().getConstantsAsMap().get("fuel").getValue(),
                "the ECU's tune before the Burn, not the burned one (42)");
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void createRestorePointKeepsTheOpenTuneWithItsNote(@org.junit.jupiter.api.io.TempDir java.nio.file.Path dir)
        throws Exception {
        com.rusefi.util.RestorePointStore store =
            new com.rusefi.util.RestorePointStore(dir, 50, java.time.Clock.systemUTC());
        IniFileModel ini = realIni();
        Fixture[] fixture = new Fixture[1];
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(ini, Runnable::run);
                fixture[0].toolbar.setRestorePointStoreForTests(store);
                fixture[0].toolbar.setFileWorkForTests(Runnable::run);
                fixture[0].editToB();
                fixture[0].toolbar.createRestorePoint(fixture[0].calibration, "before idle work");
            });
            // the confirmation is posted to the EDT
            runOnEdt(() -> assertTrue(toolbarTexts(fixture[0].toolbar).contains("Restore point created")));
            List<com.rusefi.util.RestorePointStore.RestorePoint> points = store.list(RESTORE_SIGNATURE, "");
            assertEquals(1, points.size());
            assertEquals(com.rusefi.util.RestorePointStore.Reason.MANUAL, points.get(0).reason);
            assertEquals("before idle work", points.get(0).readNote());
            assertEquals("42", points.get(0).readTune().getConstantsAsMap().get("fuel").getValue(),
                "the open tune, its unburned change too");
            assertArrayEquals(fixture[0].a, fixture[0].protocol.flash, "nothing is burned");
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    private static JLabel noteLabel(TuningToolbarWidget toolbar) {
        for (Component c : com.rusefi.ui.util.UiUtils.getAllComponents(toolbar.getPanel())) {
            if (c instanceof JLabel && ((JLabel) c).getText() != null
                && ((JLabel) c).getText().contains(TuningToolbarWidget.BURN_NOTE)) {
                return (JLabel) c;
            }
        }
        throw new AssertionError("no Burn note in " + toolbarTexts(toolbar));
    }

    private static void click(JLabel label) {
        java.awt.event.MouseEvent click = new java.awt.event.MouseEvent(label, java.awt.event.MouseEvent.MOUSE_CLICKED,
            System.currentTimeMillis(), java.awt.event.InputEvent.BUTTON1_DOWN_MASK, 5, 5, 1, false,
            java.awt.event.MouseEvent.BUTTON1);
        for (java.awt.event.MouseListener listener : label.getMouseListeners()) {
            listener.mouseClicked(click);
        }
    }

    private static List<String> toolbarTexts(TuningToolbarWidget toolbar) {
        List<String> texts = new ArrayList<>();
        for (Component c : com.rusefi.ui.util.UiUtils.getAllComponents(toolbar.getPanel())) {
            if (c instanceof JLabel) {
                texts.add(((JLabel) c).getText());
            }
        }
        return texts;
    }

    @Test
    void aRestorePointThatCannotBeMadeDoesNotStopTheBurn(@org.junit.jupiter.api.io.TempDir java.nio.file.Path dir)
        throws Exception {
        com.rusefi.util.RestorePointStore store =
            new com.rusefi.util.RestorePointStore(dir, 50, java.time.Clock.systemUTC());
        Fixture[] fixture = new Fixture[1];
        try {
            runOnEdt(() -> {
                // the mocked .ini cannot make an .msq
                fixture[0] = new Fixture();
                fixture[0].toolbar.setRestorePointStoreForTests(store);
                fixture[0].editToB();
                button(fixture[0].toolbar, "Burn to ECU").doClick();
                fixture[0].toolbar.flushUploadTimerForUnitTest();
            });
            runOnEdt(() -> assertFalse(toolbarTexts(fixture[0].toolbar).stream()
                .anyMatch(text -> text != null && text.contains(TuningToolbarWidget.BURN_NOTE)),
                "no restore point was made: the note would be wrong"));
            assertArrayEquals(fixture[0].b, fixture[0].protocol.flash, "burned anyway");
            try (java.util.stream.Stream<java.nio.file.Path> files = java.nio.file.Files.list(dir)) {
                assertEquals(0, files.count());
            }
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void burnUsesAnImmutableSnapshotAndDoesNotCompleteOverNewerEdits() throws Exception {
        Fixture[] fixture = new Fixture[1];
        List<Runnable> submitted = new ArrayList<>();
        AtomicBoolean succeeded = new AtomicBoolean();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(submitted::add);
                Fixture current = fixture[0];
                current.editToB();
                current.toolbar.burnToEcuAndThen(current.calibration, () -> succeeded.set(true));

                assertFalse(button(current.toolbar, "Burn to ECU").isEnabled());
                assertFalse(button(current.toolbar, "Discard changes").isEnabled());
                current.session.set(ConfigurationImageTestHelper.newLittleEndian(current.c));
                current.calibration.update((String) null, current.ini, current.session.get());
                current.toolbar.onEdit(ConfigurationImageTestHelper.newLittleEndian(current.b));
                submitted.remove(0).run();
            });
            runOnEdt(() -> { });

            assertArrayEquals(fixture[0].b, fixture[0].protocol.ram);
            assertArrayEquals(fixture[0].b, fixture[0].protocol.flash);
            assertArrayEquals(fixture[0].b, fixture[0].toolbar.getBaselineImage().getContent());
            assertArrayEquals(fixture[0].c, fixture[0].session.get().getContent());
            assertFalse(succeeded.get(), "newer edits must suppress the completion callback");
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void undoDuringBurnSuppressesExitCallbackForReplacedTune() throws Exception {
        Fixture[] fixture = new Fixture[1];
        List<Runnable> submitted = new ArrayList<>();
        AtomicBoolean succeeded = new AtomicBoolean();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(submitted::add);
                Fixture current = fixture[0];
                current.editToB();
                current.toolbar.flushBeforeNavigate();
                current.toolbar.burnToEcuAndThen(current.calibration, () -> succeeded.set(true));
                assertFalse(button(current.toolbar, "Discard changes").isEnabled());

                button(current.toolbar, "Undo").doClick();
                assertArrayEquals(current.a, current.session.get().getContent());
                submitted.remove(0).run();
            });
            runOnEdt(() -> { });

            assertArrayEquals(fixture[0].b, fixture[0].protocol.flash);
            assertArrayEquals(fixture[0].b, fixture[0].toolbar.getBaselineImage().getContent());
            assertFalse(succeeded.get(), "Undo replaced the image while Burn was in flight");
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void failedWriteDoesNotAdvanceTheToolbarBaselineOrReportSuccess() throws Exception {
        assertBurnFailure(false);
    }

    @Test
    void failedBurnDoesNotAdvanceTheToolbarBaselineOrReportSuccess() throws Exception {
        assertBurnFailure(true);
    }

    @Test
    void acknowledgedPageZeroWritesUpdateOnlyTheirDestinationRangeAndKeepLoadedBaseline() throws Exception {
        Fixture[] fixture = new Fixture[1];
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                Fixture current = fixture[0];
                byte[] source = {90, 91, 92, 93, 94};
                current.protocol.writeInBlocks(source, 1, 3, 2, 0);

                byte[] expected = current.a.clone();
                expected[3] = 91;
                expected[4] = 92;
                assertArrayEquals(expected, current.protocol.getControllerConfiguration().getContent());
                assertArrayEquals(current.a, current.protocol.getCachedImage().getContent());

                current.protocol.writeInBlocks(new byte[]{77, 78}, 0, 1, 2, 1);
                assertArrayEquals(expected, current.protocol.getControllerConfiguration().getContent(),
                    "extra-page writes must not mutate the main-page image");
                assertArrayEquals(current.a, current.protocol.getCachedImage().getContent());
            });
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void acknowledgedRamChangesCanBeRevertedByTheNextDiff() throws Exception {
        Fixture[] fixture = new Fixture[1];
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                Fixture current = fixture[0];
                current.protocol.uploadChangesWithoutBurn(ConfigurationImageTestHelper.newLittleEndian(current.b));
                current.protocol.uploadChangesWithoutBurn(ConfigurationImageTestHelper.newLittleEndian(current.a));

                assertArrayEquals(current.a, current.protocol.ram);
                assertArrayEquals(current.a, current.protocol.getControllerConfiguration().getContent());
                assertEquals(2, current.protocol.writeCount);
                assertArrayEquals(current.a, current.protocol.getCachedImage().getContent());
            });
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void pageZeroWriteWithoutLoadedMainImageIsIgnoredByComparisonCache() throws Exception {
        UIContext context = new UIContext();
        context.getLinkManager().setNotifyGlobalStatusOnClose(false);
        FakeProtocol protocol = new FakeProtocol(context.getLinkManager(), null);
        IniFileModel ini = IniFileModelMocks.withGauges();
        when(ini.getBlockingFactor()).thenReturn(64);
        protocol.setIniFileForUnitTest(ini);
        try {
            protocol.writeInBlocks(new byte[]{9, 8}, 0, 0, 2, 0);
            assertNull(protocol.getControllerConfiguration());
        } finally {
            context.getLinkManager().close();
            protocol.stream.close();
        }
    }

    @Test
    void failedDebouncedUploadReportsErrorOnEdt() throws Exception {
        Fixture[] fixture = new Fixture[1];
        AtomicBoolean errorOnEdt = new AtomicBoolean();
        List<String> errors = new ArrayList<>();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                fixture[0].protocol.failWrite = true;
                fixture[0].toolbar.setErrorHandler(message -> {
                    errors.add(message);
                    errorOnEdt.set(SwingUtilities.isEventDispatchThread());
                });
                fixture[0].editToB();
                fixture[0].toolbar.flushUploadTimerForUnitTest();
            });
            runOnEdt(() -> { });
            assertEquals(1, errors.size());
            assertTrue(errors.get(0).contains("Failed to upload tune"));
            assertTrue(errorOnEdt.get());
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void discardStopsPendingTimerAndDoesNotUploadObsoleteEdit() throws Exception {
        Fixture[] fixture = new Fixture[1];
        List<Runnable> submitted = new ArrayList<>();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(submitted::add);
                Fixture current = fixture[0];
                current.editToB();
                assertTrue(current.toolbar.isUploadTimerRunningForUnitTest());

                button(current.toolbar, "Discard changes").doClick();
                assertFalse(current.toolbar.isUploadTimerRunningForUnitTest());
                assertEquals(1, submitted.size(), "only the restoration upload should be queued");
                submitted.remove(0).run();
                assertArrayEquals(current.a, current.protocol.ram);
                assertEquals(0, current.protocol.burnCount, "Discard must not issue a burn command");
                assertEquals(0, current.protocol.writeCount, "obsolete B must never be written");
            });
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void discardRestorationRunsAfterAnAlreadyQueuedUpload() throws Exception {
        Fixture[] fixture = new Fixture[1];
        List<Runnable> submitted = new ArrayList<>();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(submitted::add);
                Fixture current = fixture[0];
                current.editToB();
                current.toolbar.flushUploadTimerForUnitTest();
                button(current.toolbar, "Discard changes").doClick();
                assertEquals(2, submitted.size());

                submitted.remove(0).run();
                assertArrayEquals(current.b, current.protocol.ram);
                submitted.remove(0).run();
                assertArrayEquals(current.a, current.protocol.ram);
            });
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void offlineDiscardDoesNotSubmitEcuRestore() throws Exception {
        Fixture[] fixture = new Fixture[1];
        List<Runnable> submitted = new ArrayList<>();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(submitted::add);
                Fixture current = fixture[0];
                current.editToB();
                current.context.getLinkManager().setBinaryProtocolForTests(null);

                button(current.toolbar, "Discard changes").doClick();

                assertArrayEquals(current.a, current.session.get().getContent());
                assertTrue(submitted.isEmpty());
            });
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void offlineDiscardWithConnectedProtocolDoesNotUploadLocalBaseline() throws Exception {
        runOnEdt(() -> {
            List<Runnable> submitted = new ArrayList<>();
            try (Fixture current = new Fixture(submitted::add)) {
                current.editToB();
                current.context.setOfflineMode(true);

                button(current.toolbar, "Discard changes").doClick();

                assertArrayEquals(current.a, current.session.get().getContent());
                assertTrue(submitted.isEmpty(), "An unreconciled offline tune must not reach the connected ECU");
                assertEquals(0, current.protocol.writeCount);
                assertEquals(0, current.protocol.burnCount);
            }
        });
    }

    @Test
    void queuedDiscardSkipsRestorationAfterSwitchingOffline() throws Exception {
        runOnEdt(() -> {
            List<Runnable> submitted = new ArrayList<>();
            try (Fixture current = new Fixture(submitted::add)) {
                current.editToB();
                current.toolbar.flushUploadTimerForUnitTest();
                submitted.remove(0).run();
                button(current.toolbar, "Discard changes").doClick();
                assertEquals(1, submitted.size());
                current.context.setOfflineMode(true);

                submitted.remove(0).run();

                assertArrayEquals(current.a, current.session.get().getContent());
                assertArrayEquals(current.b, current.protocol.ram, "Queued restore must respect the offline transition");
                assertEquals(1, current.protocol.writeCount);
                assertEquals(0, current.protocol.burnCount);
            }
        });
    }

    @Test
    void failedDiscardRestoreReportsOnEdtWithoutBurning() throws Exception {
        Fixture[] fixture = new Fixture[1];
        AtomicBoolean errorOnEdt = new AtomicBoolean();
        List<String> errors = new ArrayList<>();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                Fixture current = fixture[0];
                current.editToB();
                current.toolbar.flushUploadTimerForUnitTest();
                current.protocol.failWrite = true;
                current.toolbar.setErrorHandler(message -> {
                    errors.add(message);
                    errorOnEdt.set(SwingUtilities.isEventDispatchThread());
                });

                button(current.toolbar, "Discard changes").doClick();
            });
            runOnEdt(() -> { });

            assertArrayEquals(fixture[0].a, fixture[0].session.get().getContent());
            assertArrayEquals(fixture[0].b, fixture[0].protocol.ram);
            assertArrayEquals(fixture[0].a, fixture[0].protocol.flash);
            assertEquals(0, fixture[0].protocol.burnCount);
            assertEquals(1, errors.size());
            assertTrue(errors.get(0).contains("Failed to restore tune to ECU"));
            assertTrue(errorOnEdt.get());
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void discardSubmissionFailureReportsOnEdtAfterLocalRestore() throws Exception {
        Fixture[] fixture = new Fixture[1];
        AtomicBoolean errorOnEdt = new AtomicBoolean();
        List<String> errors = new ArrayList<>();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(operation -> {
                    throw new IllegalStateException("queue unavailable");
                });
                Fixture current = fixture[0];
                current.toolbar.setErrorHandler(message -> {
                    errors.add(message);
                    errorOnEdt.set(SwingUtilities.isEventDispatchThread());
                });
                current.editToB();

                button(current.toolbar, "Discard changes").doClick();

                assertArrayEquals(current.a, current.session.get().getContent());
            });
            runOnEdt(() -> { });
            assertEquals(1, errors.size());
            assertTrue(errors.get(0).contains("queue unavailable"));
            assertTrue(errorOnEdt.get());
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    private static void assertBurnFailure(boolean failBurn) throws Exception {
        Fixture[] fixture = new Fixture[1];
        AtomicBoolean succeeded = new AtomicBoolean();
        List<String> errors = new ArrayList<>();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                Fixture current = fixture[0];
                current.protocol.failWrite = !failBurn;
                current.protocol.failBurn = failBurn;
                current.toolbar.setErrorHandler(errors::add);
                current.editToB();
                current.toolbar.burnToEcuAndThen(current.calibration, () -> succeeded.set(true));
            });
            runOnEdt(() -> { });
            assertArrayEquals(fixture[0].a, fixture[0].toolbar.getBaselineImage().getContent());
            assertFalse(succeeded.get());
            assertEquals(1, errors.size());
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void discardAfterDebouncedUpload() throws Exception {
        Fixture[] fixture = new Fixture[1];
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                Fixture current = fixture[0];
                current.editToB();
                current.toolbar.flushUploadTimerForUnitTest();
                assertArrayEquals(current.b, current.protocol.ram);
                assertArrayEquals(current.a, current.protocol.flash);

                button(current.toolbar, "Discard changes").doClick();

                // Discard restores the UI and ECU RAM, but does not burn the restoration.
                assertArrayEquals(current.a, current.session.get().getContent());
                assertArrayEquals(current.a, current.calibration.getWorkingImage().getContent());
                assertArrayEquals(current.a, current.protocol.ram);
                assertArrayEquals(current.a, current.protocol.flash);
                assertEquals(0, current.protocol.burnCount);
            });
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void mirrorControlsFollowAndDriveTheToolbar() throws Exception {
        Fixture[] fixture = new Fixture[1];
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                Fixture current = fixture[0];
                JComponent mirror = current.toolbar.createMirrorControls();
                JButton undo = findButton(mirror, "Undo");
                assertFalse(undo.isEnabled());

                current.editToB();
                current.toolbar.flushBeforeNavigate();
                assertTrue(undo.isEnabled(), "mirror Undo follows the toolbar's");
                assertTrue(findLabelText(mirror).startsWith("Pending changes not burned"));

                findButton(mirror, "Discard changes").doClick();
                assertArrayEquals(current.a, current.session.get().getContent());
                assertFalse(undo.isEnabled());
                assertEquals("", findLabelText(mirror));

                current.editToB();
                findButton(mirror, "Burn to ECU").doClick();
            });
            runOnEdt(() -> { });
            assertArrayEquals(fixture[0].b, fixture[0].protocol.flash);
            assertArrayEquals(fixture[0].b, fixture[0].toolbar.getBaselineImage().getContent());
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    private static String findLabelText(Container container) {
        for (Component component : container.getComponents()) {
            if (component instanceof JLabel) {
                return ((JLabel) component).getText();
            }
        }
        return null;
    }

    private static void runOnEdt(Runnable action) throws Exception {
        SwingUtilities.invokeAndWait(action);
    }

    private static JButton button(TuningToolbarWidget toolbar, String text) {
        return findButton(toolbar.getPanel(), text);
    }

    private static JButton findButton(Container container, String text) {
        for (Component component : container.getComponents()) {
            if (component instanceof JButton && text.equals(((JButton) component).getText())) {
                return (JButton) component;
            }
            if (component instanceof Container) {
                JButton found = findButton((Container) component, text);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static final class Fixture implements AutoCloseable {
        private final byte[] a = {1, 2, 3, 4, 5, 6, 7, 8};
        private final byte[] b = {1, 42, 3, 4, 5, 6, 7, 8};
        private final byte[] c = {1, 42, 3, 4, 55, 6, 7, 8};
        private final UIContext context = new UIContext();
        private final IniFileModel ini;
        private final FakeProtocol protocol;
        private final AtomicReference<ConfigurationImage> session =
            new AtomicReference<>(ConfigurationImageTestHelper.newLittleEndian(a));
        private final CalibrationDialogWidget calibration = new CalibrationDialogWidget(context);
        private final TuningToolbarWidget toolbar;

        private Fixture() {
            this(Runnable::run);
        }

        private Fixture(java.util.function.Consumer<Runnable> submit) {
            this(IniFileModelMocks.withGauges(), submit);
        }

        private Fixture(IniFileModel ini, java.util.function.Consumer<Runnable> submit) {
            this.ini = ini;
            if (org.mockito.Mockito.mockingDetails(ini).isMock()) {
                when(ini.getBlockingFactor()).thenReturn(64);
            }
            context.iniFileState.setIniFileModelForTest(ini);
            context.getLinkManager().setNotifyGlobalStatusOnClose(false);
            protocol = new FakeProtocol(context.getLinkManager(), a);
            protocol.setIniFileForUnitTest(ini);
            context.getLinkManager().setBinaryProtocolForTests(protocol);
            calibration.update((String) null, ini, session.get());
            toolbar = new TuningToolbarWidget(context, calibration, new AtomicReference<>(),
                session, ConfigurationImageTestHelper.newLittleEndian(a), UpdateOperationCallbacks.DUMMY,
                () -> { }, failed -> { }, submit);
        }

        private void editToB() {
            editTo(b);
        }

        private void editTo(byte[] image) {
            session.set(ConfigurationImageTestHelper.newLittleEndian(image));
            calibration.update((String) null, ini, session.get());
            toolbar.onEdit(ConfigurationImageTestHelper.newLittleEndian(a));
        }

        @Override
        public void close() {
            toolbar.onDisconnect();
            context.getLinkManager().close();
            protocol.stream.close();
        }
    }

    /** Real BinaryProtocol diff/write/burn/cache logic over a deterministic fake ECU. */
    private static final class FakeProtocol extends BinaryProtocol {
        private final FakeStream stream;
        private final byte[] ram;
        private byte[] flash;
        private final byte[] luaRam = new byte[8];
        private byte[] luaFlash = new byte[8];
        private boolean failWrite;
        private boolean failBurn;
        private boolean failLuaBurn;
        private Runnable onLuaRead = () -> {};
        private int writeCount;
        private int burnCount;

        private FakeProtocol(LinkManager linkManager, byte[] initial) {
            this(linkManager, new FakeStream(), initial);
        }

        private FakeProtocol(LinkManager linkManager, FakeStream stream, byte[] initial) {
            super(linkManager, stream);
            this.stream = stream;
            ram = initial == null ? new byte[8] : initial.clone();
            flash = initial == null ? new byte[8] : initial.clone();
            if (initial != null) {
                setConfigurationImage(ConfigurationImageTestHelper.newLittleEndian(initial));
                cacheCurrentImage();
            }
        }

        @Override
        public byte[] executeCommand(char opcode, byte[] packet, String description, int timeoutMs) {
            if (opcode == (char) Integration.TS_CHUNK_WRITE_COMMAND) {
                if (failWrite) {
                    stream.close();
                    return null;
                }
                ByteBuffer header = ByteBuffer.wrap(packet).order(getWireByteOrder());
                int page = Short.toUnsignedInt(header.getShort());
                int offset = Short.toUnsignedInt(header.getShort());
                int count = Short.toUnsignedInt(header.getShort());
                if (page == 0) {
                    System.arraycopy(packet, 6, ram, offset, count);
                } else if (page == LUA_PAGE) {
                    System.arraycopy(packet, 6, luaRam, offset, count);
                }
                writeCount++;
            } else if (opcode == (char) Integration.TS_BURN_COMMAND) {
                burnCount++;
                if (failBurn) {
                    stream.close();
                    return null;
                }
                int page = packet == null ? 0 : Short.toUnsignedInt(ByteBuffer.wrap(packet).order(getWireByteOrder()).getShort());
                if (page == LUA_PAGE) {
                    if (failLuaBurn) {
                        return new byte[0];
                    }
                    luaFlash = luaRam.clone();
                } else {
                    flash = ram.clone();
                }
                return new byte[]{(byte) Integration.TS_RESPONSE_BURN_OK};
            }
            return new byte[]{(byte) Integration.TS_RESPONSE_OK};
        }

        @Override
        public byte[] readFromPage(int page, int offset, int size) {
            if (page == LUA_PAGE) {
                onLuaRead.run();
                return java.util.Arrays.copyOfRange(luaRam, offset, offset + size);
            }
            return super.readFromPage(page, offset, size);
        }

        @Override
        public boolean isSinglePageController() {
            return false;
        }
    }

    private static final class FakeStream extends AbstractIoStream {
        private final IncomingDataBuffer buffer = new IncomingDataBuffer("tune test", getStreamStats());

        @Override
        public void setInputListener(DataListener listener) {
        }

        @Override
        public IncomingDataBuffer getDataBuffer() {
            return buffer;
        }
    }
}
