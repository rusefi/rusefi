package com.rusefi.ui.basic;

import com.rusefi.autoupdate.Autoupdate;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Consumer;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class SoftwareUpdateCoordinatorTest {
    @Test
    void availabilityCheckPublishesAvailableState() {
        CompletableFuture<Autoupdate.UpdateOutcome> check = new CompletableFuture<>();
        SoftwareUpdateCoordinator coordinator = coordinator(check, ignored -> {});
        List<SoftwareUpdateCoordinator.State> states = new ArrayList<>();
        coordinator.addListener(states::add);

        check.complete(Autoupdate.UpdateOutcome.AVAILABLE);

        assertEquals(List.of(
            SoftwareUpdateCoordinator.State.CHECKING,
            SoftwareUpdateCoordinator.State.AVAILABLE), states);
    }

    @Test
    void availableUpdateIsOfferedWithoutPromptWhenAutoUpdateIsOff() {
        SoftwareUpdateCoordinator coordinator = new SoftwareUpdateCoordinator(
            CompletableFuture.completedFuture(Autoupdate.UpdateOutcome.AVAILABLE), ignored -> {},
            Runnable::run, Runnable::run, () -> false);

        assertEquals(SoftwareUpdateCoordinator.State.AVAILABLE_DEFERRED, coordinator.getState());
    }

    @Test
    void lateListenerSeesStatePublishedBeforeItsFirstDelivery() {
        CompletableFuture<Autoupdate.UpdateOutcome> check = new CompletableFuture<>();
        List<Runnable> queued = new ArrayList<>();
        SoftwareUpdateCoordinator coordinator = new SoftwareUpdateCoordinator(
            check, ignored -> {}, Runnable::run, queued::add);
        List<SoftwareUpdateCoordinator.State> states = new ArrayList<>();

        coordinator.addListener(states::add);
        check.complete(Autoupdate.UpdateOutcome.UPDATED);
        queued.forEach(Runnable::run);

        assertEquals(SoftwareUpdateCoordinator.State.RESTART_REQUIRED, states.get(states.size() - 1));
        assertFalse(states.contains(SoftwareUpdateCoordinator.State.CHECKING),
            "the listener's first delivery reads the state when it runs, not when it was added");
    }

    @Test
    void installIsSharedAndCannotStartTwice() {
        AtomicReference<Consumer<Autoupdate.UpdateOutcome>> completion = new AtomicReference<>();
        int[] installCount = {0};
        SoftwareUpdateCoordinator coordinator = coordinator(
            CompletableFuture.completedFuture(Autoupdate.UpdateOutcome.AVAILABLE), callback -> {
                installCount[0]++;
                completion.set(callback);
            });
        List<SoftwareUpdateCoordinator.State> states = new ArrayList<>();
        coordinator.addListener(states::add);

        coordinator.install();
        coordinator.install();
        completion.get().accept(Autoupdate.UpdateOutcome.UPDATED);

        assertEquals(1, installCount[0]);
        assertEquals(List.of(
            SoftwareUpdateCoordinator.State.AVAILABLE,
            SoftwareUpdateCoordinator.State.INSTALLING,
            SoftwareUpdateCoordinator.State.RESTART_REQUIRED), states);
    }

    @Test
    void lateListenerReceivesCurrentStateAcrossWindowHandoff() {
        SoftwareUpdateCoordinator coordinator = coordinator(
            CompletableFuture.completedFuture(Autoupdate.UpdateOutcome.UPDATED), ignored -> {});
        List<SoftwareUpdateCoordinator.State> mainWindowStates = new ArrayList<>();

        coordinator.addListener(mainWindowStates::add);

        assertEquals(List.of(SoftwareUpdateCoordinator.State.RESTART_REQUIRED), mainWindowStates);
    }

    @Test
    void failedInstallCanBeDismissedAndRetriedLater() {
        AtomicReference<Consumer<Autoupdate.UpdateOutcome>> completion = new AtomicReference<>();
        SoftwareUpdateCoordinator coordinator = coordinator(
            CompletableFuture.completedFuture(Autoupdate.UpdateOutcome.AVAILABLE), completion::set);

        coordinator.install();
        completion.get().accept(Autoupdate.UpdateOutcome.FAILED);
        assertEquals(SoftwareUpdateCoordinator.State.FAILED, coordinator.getState());

        coordinator.later();
        assertEquals(SoftwareUpdateCoordinator.State.IDLE, coordinator.getState());
    }

    @Test
    void laterKeepsAvailableUpdateDiscoverable() {
        SoftwareUpdateCoordinator coordinator = coordinator(
            CompletableFuture.completedFuture(Autoupdate.UpdateOutcome.AVAILABLE), ignored -> {});

        coordinator.later();

        assertEquals(SoftwareUpdateCoordinator.State.AVAILABLE_DEFERRED, coordinator.getState());
    }

    @Test
    void onlyOneWindowCanClaimTheAvailabilityPrompt() {
        SoftwareUpdateCoordinator coordinator = coordinator(
            CompletableFuture.completedFuture(Autoupdate.UpdateOutcome.AVAILABLE), ignored -> {});

        assertTrue(coordinator.claimAvailablePrompt());
        assertFalse(coordinator.claimAvailablePrompt());
    }

    private static SoftwareUpdateCoordinator coordinator(
        CompletableFuture<Autoupdate.UpdateOutcome> check,
        Consumer<Consumer<Autoupdate.UpdateOutcome>> installer) {
        return new SoftwareUpdateCoordinator(check, installer, Runnable::run, Runnable::run);
    }
}
