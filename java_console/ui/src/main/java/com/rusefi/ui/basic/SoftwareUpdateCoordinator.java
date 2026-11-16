package com.rusefi.ui.basic;

import com.rusefi.autoupdate.Autoupdate;

import javax.swing.SwingUtilities;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.Executor;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.function.BooleanSupplier;
import java.util.function.Consumer;

/** Owns one application-update state across the startup screen and main console. */
public final class SoftwareUpdateCoordinator {
    public enum State {
        CHECKING,
        IDLE,
        AVAILABLE,
        AVAILABLE_DEFERRED,
        INSTALLING,
        RESTART_REQUIRED,
        FAILED
    }

    private final List<Consumer<State>> listeners = new CopyOnWriteArrayList<>();
    private final Consumer<Consumer<Autoupdate.UpdateOutcome>> installer;
    private final Executor backgroundExecutor;
    private final Executor eventExecutor;
    private final BooleanSupplier promptForAvailableUpdate;
    private final AtomicBoolean availablePromptClaimed = new AtomicBoolean();
    private volatile State state = State.CHECKING;

    public SoftwareUpdateCoordinator(CompletableFuture<Autoupdate.UpdateOutcome> checkOutcome) {
        this(checkOutcome, Autoupdate::runManualUpdate, command -> {
            Thread thread = new Thread(command, "manual-update");
            thread.setDaemon(true);
            thread.start();
        }, SwingUtilities::invokeLater, Autoupdate::isAutoUpdateEnabled);
    }

    SoftwareUpdateCoordinator(CompletableFuture<Autoupdate.UpdateOutcome> checkOutcome,
                              Consumer<Consumer<Autoupdate.UpdateOutcome>> installer,
                              Executor backgroundExecutor, Executor eventExecutor) {
        this(checkOutcome, installer, backgroundExecutor, eventExecutor, () -> true);
    }

    /**
     * @param promptForAvailableUpdate false when the user or bundle turned auto-update off: an
     *                                 available update is then offered without the session pop-up
     */
    SoftwareUpdateCoordinator(CompletableFuture<Autoupdate.UpdateOutcome> checkOutcome,
                              Consumer<Consumer<Autoupdate.UpdateOutcome>> installer,
                              Executor backgroundExecutor, Executor eventExecutor,
                              BooleanSupplier promptForAvailableUpdate) {
        this.installer = installer;
        this.backgroundExecutor = backgroundExecutor;
        this.eventExecutor = eventExecutor;
        this.promptForAvailableUpdate = promptForAvailableUpdate;
        checkOutcome.thenAccept(outcome -> publish(map(outcome)));
    }

    public void addListener(Consumer<State> listener) {
        listeners.add(listener);
        // Read the state when the task runs, not now: a publish() racing this call could otherwise
        // be delivered first and then be overwritten by the older state captured here.
        eventExecutor.execute(() -> listener.accept(state));
    }

    public State getState() {
        return state;
    }

    /** Ensures that only the first active window shows the session's availability popup. */
    public boolean claimAvailablePrompt() {
        return availablePromptClaimed.compareAndSet(false, true);
    }

    public void install() {
        if (state == State.INSTALLING || state == State.RESTART_REQUIRED) {
            return;
        }
        publish(State.INSTALLING);
        backgroundExecutor.execute(() -> installer.accept(outcome -> publish(mapInstall(outcome))));
    }

    public void later() {
        if (state == State.AVAILABLE) {
            publish(State.AVAILABLE_DEFERRED);
        } else if (state == State.FAILED) {
            publish(State.IDLE);
        }
    }

    private void publish(State newState) {
        state = newState;
        eventExecutor.execute(() -> listeners.forEach(listener -> listener.accept(newState)));
    }

    private State map(Autoupdate.UpdateOutcome outcome) {
        switch (outcome) {
            case AVAILABLE:
                return promptForAvailableUpdate.getAsBoolean() ? State.AVAILABLE : State.AVAILABLE_DEFERRED;
            case UPDATED:
                return State.RESTART_REQUIRED;
            case FAILED:
                return State.FAILED;
            default:
                return State.IDLE;
        }
    }

    private static State mapInstall(Autoupdate.UpdateOutcome outcome) {
        switch (outcome) {
            case UPDATED:
                return State.RESTART_REQUIRED;
            case FAILED:
                return State.FAILED;
            default:
                return State.IDLE;
        }
    }
}
