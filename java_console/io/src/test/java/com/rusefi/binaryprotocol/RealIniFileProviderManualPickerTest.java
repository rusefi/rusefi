package com.rusefi.binaryprotocol;

import com.opensr5.ini.IniFileModel;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.FutureTask;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Covers the manual "select .ini" last-resort fallback added for issue #9774: when download and the
 * local `.`/`..` lookups all fail, {@link RealIniFileProvider} must consult the registered
 * {@link RealIniFileProvider.ManualIniPicker} and use the file it returns (or fail if the user cancels).
 *
 * Uses an unparseable signature so {@link com.rusefi.core.SignatureHelper#getUrl} returns null and no
 * network is touched; the io module dirs (`.`/`..` at test cwd) contain no rusefi*.ini, so the picker
 * is the only remaining source — deterministic and hardware/network-free per the connectivity test conventions.
 */
public class RealIniFileProviderManualPickerTest {
    private static final String TEST_INI =
            "src/test/java/com/rusefi/io/pin_output_mode_with_and_without_dollar/test_data/rusefi_uaefi.ini";
    private static final String UNPARSEABLE_SIGNATURE = "not a real signature";

    @AfterEach
    public void resetPicker() {
        RealIniFileProvider.manualPicker = null;
        RealIniFileProvider.clearPromptedSignaturesForTests();
    }

    /** A hand-written fake per the test conventions — records the signature it was asked about. */
    private static final class RecordingPicker implements RealIniFileProvider.ManualIniPicker {
        String askedSignature;
        final File toReturn;
        RecordingPicker(File toReturn) {
            this.toReturn = toReturn;
        }
        @Override
        public File pick(String signature) {
            askedSignature = signature;
            return toReturn;
        }
    }

    @Test
    public void usesManuallyPickedIniWhenAllElseFails() throws IniNotFoundException {
        RecordingPicker picker = new RecordingPicker(new File(TEST_INI));
        RealIniFileProvider.manualPicker = picker;

        IniFileModel model = new RealIniFileProvider().provide(UNPARSEABLE_SIGNATURE);

        assertEquals(UNPARSEABLE_SIGNATURE, picker.askedSignature);
        assertNotNull(model);
    }

    @Test
    public void throwsWhenUserCancelsPicker() {
        RealIniFileProvider.manualPicker = signature -> null; // user declined / cancelled

        assertThrows(IniNotFoundException.class,
                () -> new RealIniFileProvider().provide(UNPARSEABLE_SIGNATURE));
    }

    /** The port scanner calls provide() on a loop; a declined signature must not re-prompt (#9774 follow-up). */
    @Test
    public void promptsOnlyOncePerSignatureAcrossRepeatedProbes() {
        int[] promptCount = {0};
        RealIniFileProvider.manualPicker = signature -> {
            promptCount[0]++;
            return null; // user declines every time
        };
        for (int i = 0; i < 5; i++) {
            assertThrows(IniNotFoundException.class,
                    () -> new RealIniFileProvider().provide(UNPARSEABLE_SIGNATURE));
        }
        assertEquals(1, promptCount[0], "picker must be shown at most once per signature");
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
            return null;
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
