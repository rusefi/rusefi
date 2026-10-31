package com.rusefi.maintenance;

import com.rusefi.core.ui.CalibrationBackupFailureAction;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import jakarta.xml.bind.JAXBException;
import java.util.Collections;
import java.util.List;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Coverage for the post-flash calibration-merge decision (#9756). This is the branch logic that
 * previously threw {@code NoSuchElementException} when a partial merge produced no calibration.
 */
public class CalibrationsHelperDecidePostMergeTest {
    private Optional<CalibrationsInfo> presentMerge;
    private final Optional<CalibrationsInfo> absentMerge = Optional.empty();
    private final List<String> noFailures = Collections.emptyList();
    private final List<String> someFailures = Collections.singletonList("someField");

    @BeforeEach
    public void setUp() throws JAXBException {
        presentMerge = Optional.of(PartialMergeTestContext.load().getUpdatedCalibrationsInfo());
    }

    private static CalibrationsHelper.MergeResult result(Optional<CalibrationsInfo> merged, List<String> failed) {
        return new CalibrationsHelper.MergeResult(merged, failed);
    }

    @Test
    public void noFailedFields_writesMerged_regardlessOfAction() {
        // strict-equivalent success: no dialog is shown, so the chosen action must not matter
        for (final CalibrationBackupFailureAction action : CalibrationBackupFailureAction.values()) {
            assertEquals(
                CalibrationsHelper.PostMergeDecision.WRITE_MERGED,
                CalibrationsHelper.decidePostMerge(result(presentMerge, noFailures), action),
                "clean merge should write for action " + action
            );
        }
    }

    @Test
    public void failed_cancel_aborts() {
        assertEquals(
            CalibrationsHelper.PostMergeDecision.CANCEL,
            CalibrationsHelper.decidePostMerge(result(presentMerge, someFailures), CalibrationBackupFailureAction.CANCEL)
        );
    }

    @Test
    public void failed_resetAll_resetsToDefaults() {
        assertEquals(
            CalibrationsHelper.PostMergeDecision.RESET_TO_DEFAULTS,
            CalibrationsHelper.decidePostMerge(result(presentMerge, someFailures), CalibrationBackupFailureAction.RESET_ALL)
        );
    }

    @Test
    public void failed_retry_retries() {
        assertEquals(
            CalibrationsHelper.PostMergeDecision.RETRY,
            CalibrationsHelper.decidePostMerge(result(presentMerge, someFailures), CalibrationBackupFailureAction.RETRY)
        );
    }

    @Test
    public void failed_skip_withUsableMerge_writesPartial() {
        assertEquals(
            CalibrationsHelper.PostMergeDecision.WRITE_MERGED,
            CalibrationsHelper.decidePostMerge(result(presentMerge, someFailures), CalibrationBackupFailureAction.SKIP_FAILED_FIELDS)
        );
    }

    @Test
    public void failed_skip_withNoUsableMerge_resetsToDefaults() {
        // partial merge produced nothing usable -> fall back to defaults (this was the crash path)
        assertEquals(
            CalibrationsHelper.PostMergeDecision.RESET_TO_DEFAULTS,
            CalibrationsHelper.decidePostMerge(result(absentMerge, someFailures), CalibrationBackupFailureAction.SKIP_FAILED_FIELDS)
        );
    }

    @Test
    public void cleanMerge_reportsNoFailedFields() throws JAXBException {
        // ties the decision above to reality: an unmodified merge must not report failed fields,
        // so the common flash path never prompts the user.
        final CalibrationsHelper.MergeResult result =
            PartialMergeTestContext.load().mergeWithSimulatedFailures(Collections.emptySet());
        assertTrue(result.failedFields.isEmpty(), "unmodified merge should not report failed fields");
    }
}
