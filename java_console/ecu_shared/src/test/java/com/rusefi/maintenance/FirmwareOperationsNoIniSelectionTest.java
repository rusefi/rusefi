package com.rusefi.maintenance;

import com.rusefi.core.FindFileHelper;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

class FirmwareOperationsNoIniSelectionTest {
    private static final String SIGNATURE = "rusEFI master.2026.08.30.no_ini_branch_test.1234567890";

    @TempDir
    Path inputDir;

    private String savedInputFilesPath;

    @BeforeEach
    void redirectInputFilesPath() {
        savedInputFilesPath = FindFileHelper.INPUT_FILES_PATH;
        FindFileHelper.INPUT_FILES_PATH = inputDir.toString();
    }

    @AfterEach
    void restoreInputFilesPath() {
        FindFileHelper.INPUT_FILES_PATH = savedInputFilesPath;
    }

    @Test
    void noIniRejectsSameBoardSrecFromAnotherBranchEvenWhenBundleMetadataPasses() throws Exception {
        String board = SignatureHelper.parse(SIGNATURE).getBundleTarget();
        BundleInfo masterBundle = new BundleInfo("master", null, board);
        assertTrue(FirmwareFlashEligibility.isAllowed(SIGNATURE, masterBundle, false,
            UpdateOperationCallbacks.DUMMY));

        Path leftoverLimaSrec = inputDir.resolve(
            "rusefi_lima_2026-08-30_" + board + "_1234567890_leftover.srec");
        Files.write(leftoverLimaSrec, new byte[]{1});

        assertEquals("lima", SignatureHelper.parseSrec(leftoverLimaSrec.getFileName().toString()).getBranch());
        assertFalse(FirmwareFlashEligibility.isAllowed(
            SIGNATURE, leftoverLimaSrec.toString(), UpdateOperationCallbacks.DUMMY),
            "the SREC branch is rejected even though the master bundle metadata passes");
        assertNull(FirmwareOperations.selectNoIniFirmware(SIGNATURE,
            ConnectedEcuTarget.forManualRecovery(board), UpdateOperationCallbacks.DUMMY));
    }

    @Test
    void downloadedSelectionIsAcceptedOnlyForItsVerifiedBoardTarget() throws Exception {
        String board = SignatureHelper.parse(SIGNATURE).getBundleTarget();
        BundleInfo universalBundle = new BundleInfo("master", null, "universal");
        assertFalse(FirmwareFlashEligibility.isAllowed(SIGNATURE, universalBundle, false,
            UpdateOperationCallbacks.DUMMY));
        Path boardSrec = inputDir.resolve(
            "rusefi_master_2026-08-30_" + board + "_1234567890_downloaded.srec");
        Files.write(boardSrec, new byte[]{1});

        FirmwareSelection selected = FirmwareOperations.selectNoIniFirmware(SIGNATURE,
            ConnectedEcuTarget.forManualRecovery(board), UpdateOperationCallbacks.DUMMY);

        assertNotNull(selected);
        assertEquals(boardSrec.toFile().getAbsolutePath(), selected.getSrecPath());
        assertNull(FirmwareOperations.selectNoIniFirmware(SIGNATURE,
            ConnectedEcuTarget.forManualRecovery("other_no_ini_branch_board"), UpdateOperationCallbacks.DUMMY));
    }
}
