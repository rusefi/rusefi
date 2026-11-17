package com.rusefi.ui.widgets;

import org.junit.jupiter.api.Test;

import javax.swing.SwingUtilities;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicLong;
import java.util.function.Consumer;

import static org.junit.jupiter.api.Assertions.*;

class DetachedGaugeHoverTest {
    @Test
    void expiresOneSecondAfterTheLatestMotion() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicLong now = new AtomicLong();
            DetachedGaugeHover hover = new DetachedGaugeHover(now::get);
            AtomicBoolean visible = new AtomicBoolean();
            Consumer<Boolean> target = visible::set;
            try {
                hover.hover(target);
                assertTrue(visible.get());
                now.set(999_000_000);
                hover.expire();
                assertTrue(visible.get());
                hover.hover(target);
                now.set(1_000_000_000);
                hover.expire();
                assertTrue(visible.get(), "An old queued timeout must not hide a renewed hover");
                now.set(1_999_000_000);
                hover.expire();
                assertFalse(visible.get());
            } finally {
                hover.remove(target);
            }
        });
    }

    @Test
    void onlyLatestGaugeOwnsTheCloseButtonAndClosingAnotherDoesNotHideIt() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            AtomicLong now = new AtomicLong();
            DetachedGaugeHover hover = new DetachedGaugeHover(now::get);
            AtomicBoolean first = new AtomicBoolean();
            AtomicBoolean second = new AtomicBoolean();
            Consumer<Boolean> firstTarget = first::set;
            Consumer<Boolean> secondTarget = second::set;
            try {
                hover.hover(firstTarget);
                now.set(800_000_000);
                hover.hover(secondTarget);
                assertFalse(first.get());
                assertTrue(second.get());
                hover.remove(firstTarget);
                assertTrue(second.get());
                now.set(1_000_000_000);
                hover.expire();
                assertTrue(second.get(), "The first gauge's deadline does not apply to the second");
                now.set(1_800_000_000);
                hover.expire();
                assertFalse(first.get());
                assertFalse(second.get());
                hover.hover(secondTarget);
                hover.remove(secondTarget);
                assertFalse(second.get());
                hover.expire();
                assertFalse(second.get());
            } finally {
                hover.remove(firstTarget);
                hover.remove(secondTarget);
            }
        });
    }
}
