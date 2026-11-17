package com.rusefi.ui.widgets;

import javax.swing.Timer;
import java.util.function.Consumer;
import java.util.function.LongSupplier;

/** One close affordance across all detached gauges. All calls run on the EDT. */
final class DetachedGaugeHover {
    static final DetachedGaugeHover SHARED = new DetachedGaugeHover(System::nanoTime);
    private static final int VISIBLE_MS = 1000;
    private final LongSupplier clock;
    private final Timer timer;
    private Consumer<Boolean> owner;
    private long deadline;

    DetachedGaugeHover(LongSupplier clock) {
        this.clock = clock;
        timer = new Timer(VISIBLE_MS, event -> expire());
        timer.setRepeats(false);
    }

    void hover(Consumer<Boolean> target) {
        if (owner != null && owner != target) {
            owner.accept(false);
        }
        owner = target;
        deadline = clock.getAsLong() + VISIBLE_MS * 1_000_000L;
        target.accept(true);
        timer.setInitialDelay(VISIBLE_MS);
        timer.restart();
    }

    void remove(Consumer<Boolean> target) {
        if (owner == target) {
            timer.stop();
            owner = null;
            target.accept(false);
        }
    }

    // A deadline also protects a new hover from an already queued old timer event.
    void expire() {
        if (owner == null) {
            return;
        }
        long remaining = deadline - clock.getAsLong();
        if (remaining > 0) {
            timer.setInitialDelay((int) ((remaining + 999_999) / 1_000_000));
            timer.restart();
        } else {
            remove(owner);
        }
    }
}
