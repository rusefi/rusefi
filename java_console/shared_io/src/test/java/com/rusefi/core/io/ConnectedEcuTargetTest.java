package com.rusefi.core.io;

import com.rusefi.core.FileUtil;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.lang.reflect.Field;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Tests for the effectiveTarget fallback chain and live-target tracking (#9771).
 * The real persistence file lives in ~/.rusEFI/last_connected_board.txt, so tests
 * clean it up between runs and use reflection to reset the in-memory state.
 */
public class ConnectedEcuTargetTest {

    private static final String PERSISTED_FILE = FileUtil.RUSEFI_SETTINGS_FOLDER + "last_connected_board.txt";

    @BeforeEach
    @AfterEach
    void resetState() throws Exception {
        // Clear the in-memory live target
        Field f = ConnectedEcuTarget.class.getDeclaredField("connectedTarget");
        f.setAccessible(true);
        f.set(null, null);
        // Delete the persisted file so tests start from a clean slate
        Path p = Paths.get(PERSISTED_FILE);
        if (Files.exists(p)) {
            Files.delete(p);
        }
    }

    @Test
    void isLiveTargetKnown_falseInitially() {
        assertFalse(ConnectedEcuTarget.isLiveTargetKnown());
    }

    @Test
    void isLiveTargetKnown_trueAfterSet() {
        ConnectedEcuTarget.set("proteus_f7");
        assertTrue(ConnectedEcuTarget.isLiveTargetKnown());
    }

    @Test
    void effectiveTarget_returnsLiveTargetWhenSet() {
        ConnectedEcuTarget.set("proteus_f7");
        assertEquals("proteus_f7", ConnectedEcuTarget.effectiveTarget());
    }

    @Test
    void effectiveTarget_fallsBackToBundleWhenNoLiveAndNoPersisted() {
        // connectedTarget = null (reset), no persisted file
        String bundleTarget = BundleUtil.getBundleTarget();
        assertEquals(bundleTarget, ConnectedEcuTarget.effectiveTarget());
    }

    @Test
    void effectiveTarget_usesPersistedWhenNoLive() throws Exception {
        // Write a persisted value
        Files.write(Paths.get(PERSISTED_FILE), "hellen154hyundai".getBytes());
        // connectedTarget is still null
        assertFalse(ConnectedEcuTarget.isLiveTargetKnown());
        assertEquals("hellen154hyundai", ConnectedEcuTarget.effectiveTarget());
    }

    @Test
    void effectiveTarget_liveOverridesPersisted() throws Exception {
        // Write a persisted value
        Files.write(Paths.get(PERSISTED_FILE), "hellen154hyundai".getBytes());
        // Set a live target
        ConnectedEcuTarget.set("proteus_f7");
        assertEquals("proteus_f7", ConnectedEcuTarget.effectiveTarget());
        assertTrue(ConnectedEcuTarget.isLiveTargetKnown());
    }

    @Test
    void effectiveTarget_prefersPersistedOverBundle() throws Exception {
        Files.write(Paths.get(PERSISTED_FILE), "hellen121nissan".getBytes());
        String result = ConnectedEcuTarget.effectiveTarget();
        assertEquals("hellen121nissan", result);
    }
}
