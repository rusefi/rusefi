package com.rusefi.core.io;

import com.devexperts.logging.Logging;
import com.rusefi.core.FileUtil;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;

import static com.devexperts.logging.Logging.getLogging;

/**
 * the bundle target of the most recently connected ECU, captured from its signature at connect time.
 * <p>
 * A universal bundle's own {@code release.txt} target is not specific to the attached board, so
 * hardware-kind decisions (DFU H7 vs F4/F7 driver query, ST-Link OpenOCD config) consult this
 * instead. For a single-board bundle this equals the bundle target, so reading it is harmless.
 * <p>
 * The target is also persisted to disk (#9714) so the separate autoupdate process can pre-fetch the
 * latest firmware for the board you actually use, making a later "Update Firmware" instant.
 * <p>
 * This is a plain instance class, not a static holder: the process-wide production instance is owned
 * by {@code ConnectivityContext} (wired at {@code ProductionConnectivity}) and shared with each
 * production {@code LinkManager} so {@code BinaryProtocol} can record the target at connect time.
 * Tests construct their own instance. Only the cross-process persistence file stays static.
 */
public class ConnectedEcuTarget {
    private static final Logging log = getLogging(ConnectedEcuTarget.class);
    private static final String LAST_BOARD_FILE = FileUtil.RUSEFI_SETTINGS_FOLDER + "last_connected_board.txt";

    private volatile String connectedTarget;

    public void set(String ecuTarget) {
        connectedTarget = ecuTarget;
        persist(ecuTarget);
    }

    /**
     * Reset/override the in-memory live target WITHOUT touching the persisted file. Reflection in unit
     * tests is prohibited (see docs/java-connectivity-ui-unit-testing.md) — tests use this explicit seam
     * to simulate a live-connected board without disk side effects.
     */
    public void setConnectedTargetForUnitTest(String ecuTarget) {
        connectedTarget = ecuTarget;
    }

    /**
     * @return connected ECU target if known, otherwise the local bundle target.
     */
    public String effectiveTarget() {
        String t = connectedTarget;
        if (t != null) {
            return t;
        }
        // No live connection this session (board sitting in a bootloader, or the console was just
        // restarted): fall back to the last-connected board persisted across sessions before the generic
        // bundle target, so hardware-kind decisions and the universal-bundle flash guard have the best
        // available guess. This is only a guess — see isLiveTargetKnown(). [tag:better_ux_for_flashing]
        String persisted = readPersisted();
        return persisted != null ? persisted : BundleUtil.getBundleTarget();
    }

    /**
     * @return true when {@link #effectiveTarget()} is backed by a live connection made this session (a
     * verified ECU signature), rather than the persisted last-board guess or the bundle default. Callers
     * that flash firmware must treat an unverified target as unconfirmed (a board in a bootloader cannot
     * identify itself, and the user may have swapped hardware).
     */
    public boolean isLiveTargetKnown() {
        return connectedTarget != null;
    }

    private static void persist(String ecuTarget) {
        if (ecuTarget == null || ecuTarget.trim().isEmpty()) {
            return;
        }
        try {
            File f = new File(LAST_BOARD_FILE);
            File parent = f.getParentFile();
            if (parent != null) {
                parent.mkdirs();
            }
            Files.write(f.toPath(), ecuTarget.trim().getBytes());
        } catch (IOException e) {
            log.warn("[universal_bundle] Could not persist last connected board: " + e);
        }
    }

    /**
     * @return the target of the most recently connected ECU across sessions, or null if none recorded.
     * Static on purpose: the autoupdate process reads this file without any connection context.
     */
    public static String readPersisted() {
        try {
            Path p = Paths.get(LAST_BOARD_FILE);
            if (!Files.exists(p)) {
                return null;
            }
            List<String> lines = Files.readAllLines(p);
            if (lines.isEmpty()) {
                return null;
            }
            String s = lines.get(0).trim();
            return s.isEmpty() ? null : s;
        } catch (IOException e) {
            log.warn("[universal_bundle] Could not read last connected board: " + e);
            return null;
        }
    }
}
