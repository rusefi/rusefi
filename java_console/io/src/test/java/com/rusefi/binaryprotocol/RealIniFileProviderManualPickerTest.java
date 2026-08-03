package com.rusefi.binaryprotocol;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.FutureTask;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Covers the manual "select .ini" last-resort fallback added for issue #9774: when download and the
 * local `.`/`..` lookups all fail, {@link RealIniFileProvider} must consult the registered
 * {@link RealIniFileProvider.ManualIniPicker}. The current probe fails immediately while the UI
 * asynchronously caches the user's selection for a later probe.
 *
 * Uses an unparseable signature so {@link com.rusefi.core.SignatureHelper#getUrl} returns null and no
 * network is touched; the io module dirs (`.`/`..` at test cwd) contain no rusefi*.ini, so the picker
 * is the only remaining source — deterministic and hardware/network-free per the connectivity test conventions.
 */
public class RealIniFileProviderManualPickerTest {
    private static final String UNPARSEABLE_SIGNATURE = "not a real signature";

    @AfterEach
    public void resetPicker() {
        RealIniFileProvider.manualPicker = null;
        RealIniFileProvider.clearMissingIniStateForTests();
    }

    /** A hand-written fake per the test conventions — records the signature it was asked about. */
    private static final class RecordingPicker implements RealIniFileProvider.ManualIniPicker {
        String askedSignature;
        @Override
        public void pick(String signature) {
            askedSignature = signature;
        }
    }

    @Test
    public void requestsManualIniWhenAllElseFails() {
        RecordingPicker picker = new RecordingPicker();
        RealIniFileProvider.manualPicker = picker;

        assertThrows(IniNotFoundException.class,
            () -> new RealIniFileProvider().provide(UNPARSEABLE_SIGNATURE));

        assertEquals(UNPARSEABLE_SIGNATURE, picker.askedSignature);
    }

    /** The port scanner calls provide() on a loop; a declined signature must not re-prompt (#9774 follow-up). */
    @Test
    public void promptsOnlyOncePerSignatureAcrossRepeatedProbes() {
        int[] promptCount = {0};
        RealIniFileProvider.manualPicker = signature -> {
            promptCount[0]++;
        };
        for (int i = 0; i < 5; i++) {
            assertThrows(IniNotFoundException.class,
                    () -> new RealIniFileProvider().provide(UNPARSEABLE_SIGNATURE));
        }
        assertEquals(1, promptCount[0], "picker must be shown at most once per signature");
    }

    @Test
    public void reportsMissingIniOnlyOnceAcrossRepeatedProbes() {
        AtomicInteger reportCount = new AtomicInteger();
        RealIniFileProvider provider = new RealIniFileProvider();
        provider.setStatusConsumer(ignored -> reportCount.incrementAndGet());

        for (int i = 0; i < 5; i++) {
            assertThrows(IniNotFoundException.class,
                () -> provider.provide(UNPARSEABLE_SIGNATURE));
        }

        assertEquals(1, reportCount.get());
    }

    @Test
    public void canRetryWhenPickerCouldNotOpen() {
        AtomicInteger promptCount = new AtomicInteger();
        RealIniFileProvider.manualPicker = ignored -> promptCount.incrementAndGet();
        RealIniFileProvider provider = new RealIniFileProvider();

        assertThrows(IniNotFoundException.class, () -> provider.provide(UNPARSEABLE_SIGNATURE));
        RealIniFileProvider.allowManualPrompt(UNPARSEABLE_SIGNATURE);
        assertThrows(IniNotFoundException.class, () -> provider.provide(UNPARSEABLE_SIGNATURE));

        assertEquals(2, promptCount.get());
    }

    @Test
    public void promptsOnlyOnceForConcurrentProbes() throws Exception {
        AtomicInteger promptCount = new AtomicInteger();
        CountDownLatch pickerEntered = new CountDownLatch(1);
        CountDownLatch releasePicker = new CountDownLatch(1);
        RealIniFileProvider.manualPicker = signature -> {
            promptCount.incrementAndGet();
            pickerEntered.countDown();
            try {
                releasePicker.await();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        };

        FutureTask<Boolean> first = new FutureTask<>(() -> {
            assertThrows(IniNotFoundException.class,
                    () -> new RealIniFileProvider().provide(UNPARSEABLE_SIGNATURE));
            return true;
        });
        FutureTask<Boolean> second = new FutureTask<>(() -> {
            assertThrows(IniNotFoundException.class,
                    () -> new RealIniFileProvider().provide(UNPARSEABLE_SIGNATURE));
            return true;
        });
        new Thread(first).start();
        boolean entered = pickerEntered.await(5, TimeUnit.SECONDS);
        try {
            assertTrue(entered);
            new Thread(second).start();
            assertTrue(second.get(5, TimeUnit.SECONDS));
        } finally {
            releasePicker.countDown();
        }
        assertTrue(first.get(5, TimeUnit.SECONDS));

        assertEquals(1, promptCount.get());
    }

    @Test
    public void throwsWhenNoPickerRegistered() {
        // manualPicker stays null (reset in @AfterEach) — the pre-#9774 behavior is preserved
        assertThrows(IniNotFoundException.class,
                () -> new RealIniFileProvider().provide(UNPARSEABLE_SIGNATURE));
    }
}
