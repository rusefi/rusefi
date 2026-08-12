package com.rusefi.ui.console;

import com.rusefi.core.RusEfiSignature;
import com.rusefi.io.ConnectionStatusValue;
import org.junit.jupiter.api.Test;

import java.io.File;

import static org.junit.jupiter.api.Assertions.assertEquals;
import javax.swing.Icon;
import javax.swing.SwingUtilities;
import java.util.concurrent.atomic.AtomicBoolean;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class MainFrameUpdateCheckTest {

    @Test
    public void binaryLogExtensionIsAddedWhenMissing() {
        assertEquals(new File("capture.mlg"), MainFrame.ensureMlgExtension(new File("capture")));
        assertEquals(new File("capture.MLG"), MainFrame.ensureMlgExtension(new File("capture.MLG")));
    }

    // ECU signature: rusEFI development.2026.05.09.uaefi_pro.4226383888
    private static final RusEfiSignature ECU_SIG = new RusEfiSignature(
        "development", "2026", "05", "09", "uaefi_pro", "4226383888", false
    );

    @Test
    public void nullEcuSigReturnsFalse() {
        assertFalse(MainFrame.needsFirmwareUpdate(null, "rusefi_development_2026-05-09_uaefi_pro_4226383888_8849742d4267db6407b1400ae917a1ed39795d32_update.srec"));
    }

    @Test
    public void nullSrecNameReturnsFalse() {
        assertFalse(MainFrame.needsFirmwareUpdate(ECU_SIG, null));
    }

    @Test
    public void unparseableSrecNameReturnsFalse() {
        assertFalse(MainFrame.needsFirmwareUpdate(ECU_SIG, "garbage.srec"));
    }

    @Test
    public void checkResultDistinguishesUnavailableFromUpToDate() {
        assertEquals(MainFrame.FirmwareUpdateCheckResult.UNABLE_TO_CHECK,
            MainFrame.firmwareUpdateCheckResult(null, "firmware.srec"));
        assertEquals(MainFrame.FirmwareUpdateCheckResult.UNABLE_TO_CHECK,
            MainFrame.firmwareUpdateCheckResult(ECU_SIG, null));
        assertEquals(MainFrame.FirmwareUpdateCheckResult.UNABLE_TO_CHECK,
            MainFrame.firmwareUpdateCheckResult(ECU_SIG, "garbage.srec"));

        String matching = "rusefi_development_2026-05-09_uaefi_pro_4226383888_8849742d4267db6407b1400ae917a1ed39795d32_update.srec";
        String different = "rusefi_development_2026-05-09_uaefi_pro_9999999999_8849742d4267db6407b1400ae917a1ed39795d32_update.srec";
        assertEquals(MainFrame.FirmwareUpdateCheckResult.UP_TO_DATE,
            MainFrame.firmwareUpdateCheckResult(ECU_SIG, matching));
        assertEquals(MainFrame.FirmwareUpdateCheckResult.AVAILABLE,
            MainFrame.firmwareUpdateCheckResult(ECU_SIG, different));
    }

    @Test
    public void firmwareCheckRequiresFullyConnectedState() {
        assertFalse(MainFrame.isFirmwareUpdateConnectionReady(ConnectionStatusValue.NOT_CONNECTED));
        assertFalse(MainFrame.isFirmwareUpdateConnectionReady(ConnectionStatusValue.LOADING));
        assertTrue(MainFrame.isFirmwareUpdateConnectionReady(ConnectionStatusValue.CONNECTED));
    }

    @Test
    public void newFormatSameHashReturnsFalse() {
        // SREC numeric hash matches ECU numeric hash → no update needed
        String srec = "rusefi_development_2026-05-09_uaefi_pro_4226383888_8849742d4267db6407b1400ae917a1ed39795d32_update.srec";
        assertFalse(MainFrame.needsFirmwareUpdate(ECU_SIG, srec));
    }

    @Test
    public void newFormatDifferentHashReturnsTrue() {
        // SREC numeric hash differs from ECU numeric hash → update needed
        String srec = "rusefi_development_2026-05-09_uaefi_pro_9999999999_8849742d4267db6407b1400ae917a1ed39795d32_update.srec";
        assertTrue(MainFrame.needsFirmwareUpdate(ECU_SIG, srec));
    }

    @Test
    public void newFormatSameDateDifferentHashReturnsTrue() {
        // Even if dates match, different numeric hash means different build → update needed
        String srec = "rusefi_development_2026-05-09_uaefi_pro_1111111111_8849742d4267db6407b1400ae917a1ed39795d32_update.srec";
        assertTrue(MainFrame.needsFirmwareUpdate(ECU_SIG, srec));
    }

    @Test
    public void legacyFormatSameDateReturnsFalse() {
        // Legacy 4-part srec, same date as ECU → no update needed
        RusEfiSignature ecuSig = new RusEfiSignature(
            "development", "2026", "05", "09", "all", null, false
        );
        String srec = "rusefi_development_2026-05-09_8849742d4267db6407b1400ae917a1ed39795d32_update.srec";
        assertFalse(MainFrame.needsFirmwareUpdate(ecuSig, srec));
    }

    @Test
    public void legacyFormatDifferentDateReturnsTrue() {
        // Legacy 4-part srec, date differs from ECU → update needed
        RusEfiSignature ecuSig = new RusEfiSignature(
            "development", "2026", "05", "09", "all", null, false
        );
        String srec = "rusefi_development_2026-05-10_8849742d4267db6407b1400ae917a1ed39795d32_update.srec";
        assertTrue(MainFrame.needsFirmwareUpdate(ecuSig, srec));
    }

    @Test
    public void legacyFormatDifferentMonthReturnsTrue() {
        RusEfiSignature ecuSig = new RusEfiSignature(
            "development", "2026", "04", "09", "all", null, false
        );
        String srec = "rusefi_development_2026-05-09_8849742d4267db6407b1400ae917a1ed39795d32_update.srec";
        assertTrue(MainFrame.needsFirmwareUpdate(ecuSig, srec));
    }

    @Test
    public void newFormatWithoutUpdateSuffix() {
        // SREC name without _update suffix should still parse correctly
        String srec = "rusefi_development_2026-05-09_uaefi_pro_4226383888_8849742d4267db6407b1400ae917a1ed39795d32.srec";
        assertFalse(MainFrame.needsFirmwareUpdate(ECU_SIG, srec));
    }

    @Test
    public void menuIconsLoadAtExpectedSize() {
        String[] icons = {"folder-open", "floppy", "logout", "refresh", "controller", "player-play", "player-stop"};
        for (String name : icons) {
            Icon icon = MainFrame.loadMenuIcon(name);
            assertNotNull(icon, name + " should load");
            assertEquals(18, icon.getIconWidth(), name + " icon width");
            assertEquals(18, icon.getIconHeight(), name + " icon height");
        }
    }

    @Test
    public void overlayShowsExplicitResultsAndActions() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicBoolean updateClicked = new AtomicBoolean();
            AtomicBoolean closeClicked = new AtomicBoolean();
            MainFrame.FirmwareUpdateCheckOverlay overlay = new MainFrame.FirmwareUpdateCheckOverlay(
                () -> updateClicked.set(true), () -> closeClicked.set(true));

            assertEquals("Checking ECU firmware...", overlay.getMessageForUnitTest());
            assertFalse(overlay.isUpdateVisibleForUnitTest());
            assertTrue(overlay.isCloseVisibleForUnitTest());

            overlay.showResult(MainFrame.FirmwareUpdateCheckResult.AVAILABLE);
            assertEquals("ECU firmware update available", overlay.getMessageForUnitTest());
            assertTrue(overlay.isUpdateVisibleForUnitTest());
            assertTrue(overlay.isCloseVisibleForUnitTest());
            overlay.updateForUnitTest();
            assertTrue(updateClicked.get());

            overlay.showResult(MainFrame.FirmwareUpdateCheckResult.UP_TO_DATE);
            assertEquals("ECU already matches the local firmware image", overlay.getMessageForUnitTest());
            assertFalse(overlay.isUpdateVisibleForUnitTest());
            assertTrue(overlay.isCloseVisibleForUnitTest());

            overlay.showResult(MainFrame.FirmwareUpdateCheckResult.UNABLE_TO_CHECK);
            assertEquals("Unable to check ECU firmware", overlay.getMessageForUnitTest());
            assertFalse(overlay.isUpdateVisibleForUnitTest());
            assertTrue(overlay.isCloseVisibleForUnitTest());
            overlay.closeForUnitTest();
            assertTrue(closeClicked.get());
        });
    }

    @Test
    public void connectionFailureOverlayShowsDownloadOnlyWhenAvailable() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicBoolean downloadClicked = new AtomicBoolean();
            AtomicBoolean closeClicked = new AtomicBoolean();
            MainFrame.ConnectionFailureOverlay overlay = new MainFrame.ConnectionFailureOverlay(
                "Connection failed", () -> downloadClicked.set(true), () -> closeClicked.set(true));

            assertEquals("Connection failed", overlay.getMessageForUnitTest());
            assertTrue(overlay.isDownloadVisibleForUnitTest());
            overlay.downloadForUnitTest();
            assertTrue(downloadClicked.get());
            overlay.closeForUnitTest();
            assertTrue(closeClicked.get());

            MainFrame.ConnectionFailureOverlay noDownloadOverlay = new MainFrame.ConnectionFailureOverlay(
                "Connection failed", null, () -> { });
            assertFalse(noDownloadOverlay.isDownloadVisibleForUnitTest());
        });
    }

    @Test
    public void unsavedTuneOverlayOffersSaveDiscardAndCancel() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicBoolean saveClicked = new AtomicBoolean();
            AtomicBoolean discardClicked = new AtomicBoolean();
            AtomicBoolean cancelClicked = new AtomicBoolean();
            MainFrame.UnsavedTuneChangesOverlay overlay = new MainFrame.UnsavedTuneChangesOverlay(
                "The tune has unsaved changes.", "Save and Exit",
                () -> saveClicked.set(true), () -> discardClicked.set(true), () -> cancelClicked.set(true));

            assertEquals("The tune has unsaved changes.", overlay.getMessageForUnitTest());
            overlay.saveForUnitTest();
            overlay.discardForUnitTest();
            overlay.cancelForUnitTest();
            assertTrue(saveClicked.get());
            assertTrue(discardClicked.get());
            assertTrue(cancelClicked.get());
        });
    }
}
