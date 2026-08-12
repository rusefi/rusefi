package com.rusefi.autoupdate;

import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.concurrent.atomic.AtomicBoolean;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class AutoupdateOutcomeTest {
    @Test
    void preservesNonDownloadOutcomes() {
        for (Autoupdate.UpdateOutcome outcome : new Autoupdate.UpdateOutcome[] {
            Autoupdate.UpdateOutcome.UP_TO_DATE,
            Autoupdate.UpdateOutcome.SKIPPED,
            Autoupdate.UpdateOutcome.FAILED
        }) {
            AtomicBoolean applied = new AtomicBoolean();
            assertEquals(outcome, Autoupdate.performUpdate(null,
                ignored -> Autoupdate.DownloadResult.of(outcome),
                ignored -> applied.set(true)));
            assertFalse(applied.get());
        }
    }

    @Test
    void reportsSuccessfulApply() {
        Autoupdate.DownloadedAutoupdateFileInfo file =
            new Autoupdate.DownloadedAutoupdateFileInfo("update.zip", 123);
        AtomicBoolean applied = new AtomicBoolean();

        assertEquals(Autoupdate.UpdateOutcome.UPDATED, Autoupdate.performUpdate(null,
            ignored -> Autoupdate.DownloadResult.updated(file),
            downloaded -> applied.set(downloaded == file)));
        assertTrue(applied.get());
    }

    @Test
    void reportsDownloadAndApplyFailures() {
        assertEquals(Autoupdate.UpdateOutcome.FAILED, Autoupdate.performUpdate(null,
            ignored -> {
                throw new IOException("download failed");
            },
            ignored -> {}));

        Autoupdate.DownloadedAutoupdateFileInfo file =
            new Autoupdate.DownloadedAutoupdateFileInfo("update.zip", 123);
        assertEquals(Autoupdate.UpdateOutcome.FAILED, Autoupdate.performUpdate(null,
            ignored -> Autoupdate.DownloadResult.updated(file),
            ignored -> {
                throw new IOException("apply failed");
            }));
    }
}
