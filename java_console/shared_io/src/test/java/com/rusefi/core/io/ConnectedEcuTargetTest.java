package com.rusefi.core.io;

import com.rusefi.core.FileUtil;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Tests for the effectiveTarget fallback chain and live-target tracking [tag:better_ux_for_flashing].
 * ConnectedEcuTarget is a plain instance class, so each test constructs a fresh one; only the real
 * persistence file in ~/.rusEFI/last_connected_board.txt is shared state and is cleaned up between runs.
 */
public class ConnectedEcuTargetTest {

    private static final String PERSISTED_FILE = FileUtil.RUSEFI_SETTINGS_FOLDER + "last_connected_board.txt";

    private ConnectedEcuTarget target;

    @BeforeEach
    void setUp() throws Exception {
        target = new ConnectedEcuTarget();
        cleanUpPersistedFile();
    }

    @AfterEach
    void cleanUpPersistedFile() throws Exception {
        // Ensure the settings dir exists so the tests can write the persisted file (Files.write does
        // not create parent dirs, and ~/.rusEFI may not exist yet in a clean test env).
        Path p = Paths.get(PERSISTED_FILE);
        if (p.getParent() != null) {
            Files.createDirectories(p.getParent());
        }
        if (Files.exists(p)) {
            Files.delete(p);
        }
    }

    @Test
    void isLiveTargetKnown_falseInitially() {
        assertFalse(target.isLiveTargetKnown());
    }

    @Test
    void isLiveTargetKnown_trueAfterSet() {
        target.set("proteus_f7");
        assertTrue(target.isLiveTargetKnown());
    }

    @Test
    void effectiveTarget_returnsLiveTargetWhenSet() {
        target.set("proteus_f7");
        assertEquals("proteus_f7", target.effectiveTarget());
    }

    @Test
    void unitTestSeamSetsLiveTargetWithoutPersisting() {
        target.setConnectedTargetForUnitTest("proteus_f7");
        assertTrue(target.isLiveTargetKnown());
        assertEquals("proteus_f7", target.effectiveTarget());
        assertFalse(Files.exists(Paths.get(PERSISTED_FILE)), "the test seam must not touch the persisted file");
    }

    @Test
    void effectiveTarget_fallsBackToBundleWhenNoLiveAndNoPersisted() {
        // no live target (fresh instance), no persisted file
        String bundleTarget = BundleUtil.getBundleTarget();
        assertEquals(bundleTarget, target.effectiveTarget());
    }

    @Test
    void effectiveTarget_rejectsIncompatiblePersistedGuess() throws Exception {
        // #9814: a single-board bundle must not offer a stale, incompatible board persisted from a
        // previous session. In this test env the bundle target is "unknown" with no board_compatibility,
        // so an unrelated persisted board is not servable and the bundle target wins.
        Files.write(Paths.get(PERSISTED_FILE), "hellen154hyundai".getBytes());
        assertFalse(target.isLiveTargetKnown());
        assertEquals(BundleUtil.getBundleTarget(), target.effectiveTarget());
    }

    @Test
    void effectiveTarget_liveOverridesPersisted() throws Exception {
        // Write a persisted value
        Files.write(Paths.get(PERSISTED_FILE), "hellen154hyundai".getBytes());
        // Set a live target
        target.set("proteus_f7");
        assertEquals("proteus_f7", target.effectiveTarget());
        assertTrue(target.isLiveTargetKnown());
    }

    @Test
    void setPersistsAcrossInstances() {
        target.set("proteus_f7");
        // a different instance (e.g. the autoupdate process after restart) sees the persisted value
        assertEquals("proteus_f7", ConnectedEcuTarget.readPersisted());
        assertFalse(new ConnectedEcuTarget().isLiveTargetKnown());
        // effectiveTarget only trusts the persisted guess when this bundle can serve it; with the test
        // env's "unknown" bundle target and no compatibility list, it falls back to the bundle target.
        assertEquals(BundleUtil.getBundleTarget(), new ConnectedEcuTarget().effectiveTarget());
    }

    // --- resolveOfflineTarget: the #9814 gate, pure and independent of release.txt / properties ---

    @Test
    void resolveOfflineTarget_keepsPersistedMatchingBundle() {
        assertEquals("proteus_f7", ConnectedEcuTarget.resolveOfflineTarget("proteus_f7", "proteus_f7", null));
    }

    @Test
    void resolveOfflineTarget_rejectsForeignGuessOnSingleBoardBundle() {
        // #9814: board B persisted, single-board bundle for board A, no compatibility list → board A wins
        assertEquals("boardA", ConnectedEcuTarget.resolveOfflineTarget("boardB", "boardA", null));
    }

    @Test
    void resolveOfflineTarget_universalBundleKeepsForeignGuess() {
        // universal bundle (board_compatibility=*) serves any board → keep the guess
        assertEquals("boardB", ConnectedEcuTarget.resolveOfflineTarget("boardB", "universal", "*"));
    }

    @Test
    void resolveOfflineTarget_allowlistKeepsListedGuess() {
        assertEquals("uaefi_pro", ConnectedEcuTarget.resolveOfflineTarget("uaefi_pro", "uaefi", "uaefi,uaefi_pro"));
    }

    @Test
    void resolveOfflineTarget_allowlistRejectsUnlistedGuess() {
        assertEquals("uaefi", ConnectedEcuTarget.resolveOfflineTarget("hellen121nissan", "uaefi", "uaefi,uaefi_pro"));
    }

    @Test
    void resolveOfflineTarget_nullPersistedReturnsBundle() {
        assertEquals("boardA", ConnectedEcuTarget.resolveOfflineTarget(null, "boardA", "*"));
    }
}
