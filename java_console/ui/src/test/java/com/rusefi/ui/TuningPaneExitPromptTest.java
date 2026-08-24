package com.rusefi.ui;

import org.junit.jupiter.api.Test;

import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertEquals;

class TuningPaneExitPromptTest {
    @Test
    void cleanTuneDoesNotPrompt() {
        assertEquals(TuningPane.ExitPrompt.NONE, TuningPane.exitPrompt(false, true, false));
        assertEquals(TuningPane.ExitPrompt.NONE, TuningPane.exitPrompt(false, false, false));
    }

    @Test
    void dirtyTuneOffersAvailablePersistenceAction() {
        assertEquals(TuningPane.ExitPrompt.BURN, TuningPane.exitPrompt(true, true, false));
        assertEquals(TuningPane.ExitPrompt.SAVE, TuningPane.exitPrompt(true, false, false));
        assertEquals(TuningPane.ExitPrompt.SAVE, TuningPane.exitPrompt(true, true, true));
    }

    @Test
    void cancelDoesNothing() {
        AtomicInteger actions = new AtomicInteger();

        TuningPane.dispatchExitChoice(TuningPane.ExitPrompt.BURN, 2,
                callback -> actions.incrementAndGet(), callback -> actions.incrementAndGet(), actions::incrementAndGet);

        assertEquals(0, actions.get());
    }

    @Test
    void exitWithoutPersistingExitsImmediately() {
        AtomicInteger exits = new AtomicInteger();

        TuningPane.dispatchExitChoice(TuningPane.ExitPrompt.SAVE, 1,
                callback -> {}, callback -> {}, exits::incrementAndGet);

        assertEquals(1, exits.get());
    }

    @Test
    void persistenceControlsWhenExitRuns() {
        AtomicReference<Runnable> burnSuccess = new AtomicReference<>();
        AtomicReference<Runnable> saveSuccess = new AtomicReference<>();
        AtomicInteger exits = new AtomicInteger();

        TuningPane.dispatchExitChoice(TuningPane.ExitPrompt.BURN, 0,
                burnSuccess::set, callback -> {}, exits::incrementAndGet);
        TuningPane.dispatchExitChoice(TuningPane.ExitPrompt.SAVE, 0,
                callback -> {}, saveSuccess::set, exits::incrementAndGet);

        assertEquals(0, exits.get());
        burnSuccess.get().run();
        saveSuccess.get().run();
        assertEquals(2, exits.get());
    }
}
