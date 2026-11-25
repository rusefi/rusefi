package com.rusefi.ui.basic;

import com.rusefi.autoupdate.Autoupdate;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.assertEquals;

/** [tag:update_restart_prompt] The restart question once the application update is downloaded. */
class ApplicationUpdateRestartPromptTest {
    @AfterEach
    void realDialogBack() {
        ApplicationUpdatePrompt.setRestartQuestionForTests(null);
    }

    private static SoftwareUpdateCoordinator coordinator(Autoupdate.UpdateOutcome checked) {
        return new SoftwareUpdateCoordinator(CompletableFuture.completedFuture(checked), ignored -> {},
            Runnable::run, Runnable::run);
    }

    @Test
    void restartNowRestartsOnceAndOnlyOneWindowAsks() {
        AtomicInteger asked = new AtomicInteger();
        AtomicInteger restarts = new AtomicInteger();
        ApplicationUpdatePrompt.setRestartQuestionForTests(parent -> {
            asked.incrementAndGet();
            return true;
        });
        SoftwareUpdateCoordinator coordinator = coordinator(Autoupdate.UpdateOutcome.UPDATED);

        ApplicationUpdatePrompt.showRestart(null, coordinator, restarts::incrementAndGet);
        // e.g. the main console, after the startup window asked
        ApplicationUpdatePrompt.showRestart(null, coordinator, restarts::incrementAndGet);

        assertEquals(1, asked.get());
        assertEquals(1, restarts.get());
    }

    @Test
    void laterRestartsNothing() {
        AtomicInteger restarts = new AtomicInteger();
        ApplicationUpdatePrompt.setRestartQuestionForTests(parent -> false);

        ApplicationUpdatePrompt.showRestart(null, coordinator(Autoupdate.UpdateOutcome.UPDATED),
            restarts::incrementAndGet);

        assertEquals(0, restarts.get());
    }

    @Test
    void nothingIsAskedBeforeTheUpdateIsReady() {
        AtomicInteger asked = new AtomicInteger();
        ApplicationUpdatePrompt.setRestartQuestionForTests(parent -> {
            asked.incrementAndGet();
            return true;
        });

        ApplicationUpdatePrompt.showRestart(null, coordinator(Autoupdate.UpdateOutcome.AVAILABLE), () -> { });
        ApplicationUpdatePrompt.showRestart(null, coordinator(Autoupdate.UpdateOutcome.FAILED), () -> { });

        assertEquals(0, asked.get());
    }
}
