package com.rusefi.binaryprotocol;

import com.opensr5.ini.IniLocator;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.UncheckedIOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicBoolean;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.FutureTask;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

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
    private static final String MISSING_SIGNATURE = "rusEFI master.2026.08.30.test-board.1234567890";

    @Test
    public void rejectsMismatchedCacheAndAllowsReplacement(@TempDir Path directory) throws Throwable {
        String actualSignature = "rusEFI master.2026.08.30.other-board.987654321";
        Path cached = directory.resolve("1234567890.ini");
        Files.writeString(cached, "[MegaTune]\nsignature = \"" + actualSignature
            + "\"\n[Constants]\nnPages = 1\npageSize = 8\npageReadCommand = \"R\"\n"
            + "page = 1\nvalue = scalar, U08, 0, \"\", 1, 0, 0, 255, 0\n"
            + "[OutputChannels]\nochBlockSize = 8\n");
        RealIniFileProvider provider = new RealIniFileProvider((signature, location, download) -> cached.toString());

        RecordingPicker picker = new RecordingPicker();
        AtomicInteger prompts = new AtomicInteger();
        RealIniFileProvider.manualPicker = signature -> {
            prompts.incrementAndGet();
            picker.pick(signature);
        };
        for (int i = 0; i < 2; i++) {
            IniNotFoundException error = assertThrows(IniNotFoundException.class,
                () -> provider.provide(MISSING_SIGNATURE));
            assertEquals("Failed to locate matching .ini file", error.getMessage());
        }
        assertEquals(1, prompts.get());
        assertEquals(MISSING_SIGNATURE, picker.askedSignature);
        assertTrue(Files.exists(cached), "Keep the original file for inspection or replacement");

        Files.writeString(cached, Files.readString(cached).replace(actualSignature, MISSING_SIGNATURE));
        assertEquals(MISSING_SIGNATURE, provider.provide(MISSING_SIGNATURE).getSignature());
    }

    @Test
    public void findsRootCompressedArchiveWhenManualPickerIsDisabled(@TempDir Path directory) throws Throwable {
        byte[] ini = matchingIni(MISSING_SIGNATURE);
        Path archive = CompressedTestArchive.create(directory.resolve("board-inis.7z"),
            directory.resolve("archive-input"), "board/rusefi_board.ini", ini);
        AtomicInteger prompts = new AtomicInteger();
        AtomicInteger imports = new AtomicInteger();
        RealIniFileProvider.manualPicker = ignored -> prompts.incrementAndGet();
        RealIniFileProvider provider = new RealIniFileProvider(
            (requestedSignature, location, allowDownload) -> null,
            false,
            (signature, progress) -> {
                try {
                    byte[] snapshot = com.rusefi.core.SignatureHelper.readMatchingIni(signature, archive.toFile());
                    return snapshot == null
                        ? com.opensr5.ini.IniLocator.RootSearchResult.unavailable()
                        : com.opensr5.ini.IniLocator.RootSearchResult.found(snapshot);
                } catch (java.io.IOException e) {
                    throw new UncheckedIOException(e);
                }
            },
            (signature, source) -> {
                imports.incrementAndGet();
                return com.rusefi.core.SignatureHelper.importIntoCache(signature, source,
                    directory.resolve("cache").toString());
            });

        assertEquals(MISSING_SIGNATURE, provider.provide(MISSING_SIGNATURE).getSignature());
        assertEquals(1, imports.get());
        assertEquals(0, prompts.get(), "automatic 7z lookup must not rely on the UI picker");
        assertArrayEquals(ini, Files.readAllBytes(directory.resolve("cache").resolve("1234567890.ini")));
    }

    @Test
    public void searchesDriveRootsBeforeDownloaderAndPublishesStageProgress(@TempDir Path directory)
            throws Throwable {
        List<String> events = new ArrayList<>();
        String signature = "rusEFI master.2026.11.20.lookup-order.768349102";
        RealIniFileProvider.manualPicker = new RealIniFileProvider.ManualIniPicker() {
            @Override
            public void pick(String ignored) {
            }

            @Override
            public void progress(String ignored, String port, long generation,
                                 com.opensr5.ini.IniLocator.LookupProgress progress) {
                events.add(progress.stage + (progress.stage == com.opensr5.ini.IniLocator.LookupStage.DRIVE_ROOTS
                    && progress.totalRoots > 0
                    ? ":" + progress.completedRoots + "/" + progress.totalRoots + ":" + progress.activeWorkers
                    : ""));
            }
        };
        RealIniFileProvider provider = new RealIniFileProvider(
            (requestedSignature, location, allowDownload) -> {
                events.add("download");
                return null;
            },
            true,
            (requestedSignature, rootProgress) -> {
                events.add("drive-roots");
                rootProgress.accept(new com.opensr5.ini.IniLocator.LookupProgress(
                    com.opensr5.ini.IniLocator.LookupStage.DRIVE_ROOTS, 5, 12, 2, 0));
                return com.opensr5.ini.IniLocator.RootSearchResult.found(matchingIni(requestedSignature));
            },
            (requestedSignature, source) -> {
                events.add("import");
                Path imported = directory.resolve("root.ini");
                Files.write(imported, source);
                return imported.toString();
            });

        assertEquals(signature, provider.provide(signature).getSignature());

        List<String> expectedEvents = new ArrayList<>(List.of("BUNDLED_INI", "CURRENT_DIRECTORY"));
        if (com.opensr5.ini.IniLocator.isWindowsRootSearchSupported()) {
            expectedEvents.add("DRIVE_ROOTS");
        }
        expectedEvents.add("drive-roots");
        if (com.opensr5.ini.IniLocator.isWindowsRootSearchSupported()) {
            expectedEvents.add("DRIVE_ROOTS:5/12:2");
        }
        expectedEvents.add("import");
        expectedEvents.add("COMPLETE");
        assertEquals(expectedEvents, events);
    }

    @Test
    public void matchingDriveRootPrecedesMismatchedDownloadedCache(@TempDir Path directory) {
        String signature = "rusEFI master.2026.11.20.cache-mismatch.768349103";
        String wrongSignature = "rusEFI master.2026.11.20.other-layout.768349104";
        Path wrongCache = directory.resolve("wrong-cache.ini");
        try {
            Files.write(wrongCache, matchingIni(wrongSignature));
        } catch (java.io.IOException e) {
            throw new UncheckedIOException(e);
        }
        AtomicInteger rootLookups = new AtomicInteger();
        AtomicInteger downloads = new AtomicInteger();
        RealIniFileProvider provider = new RealIniFileProvider(
            (requestedSignature, location, allowDownload) -> {
                downloads.incrementAndGet();
                return wrongCache.toString();
            },
            false,
            (requestedSignature, progress) -> {
                rootLookups.incrementAndGet();
                return com.opensr5.ini.IniLocator.RootSearchResult.found(matchingIni(requestedSignature));
            },
            (requestedSignature, source) -> {
                Path imported = directory.resolve("root.ini");
                try {
                    Files.write(imported, source);
                } catch (java.io.IOException e) {
                    throw new UncheckedIOException(e);
                }
                return imported.toString();
            });

        assertEquals(signature, assertDoesNotThrow(() -> provider.provide(signature)).getSignature());
        assertEquals(1, rootLookups.get(),
            "the matching root is found before an unrelated cached layout is considered");
        assertEquals(0, downloads.get());
    }

    @Test
    public void mismatchedDownloadedCacheFallsThroughToMatchingTunerStudioIni(@TempDir Path directory)
            throws Throwable {
        String signature = "rusEFI master.2026.11.20.cache-fallback.768349106";
        String wrongSignature = "rusEFI master.2026.11.20.wrong-layout.768349107";
        Path wrongCache = directory.resolve("wrong-cache.ini");
        Path tunerStudioIni = directory.resolve("tunerstudio.ini");
        Files.write(wrongCache, matchingIni(wrongSignature));
        Files.write(tunerStudioIni, matchingIni(signature));
        AtomicInteger rootLookups = new AtomicInteger();
        AtomicInteger tunerLookups = new AtomicInteger();
        RealIniFileProvider provider = new RealIniFileProvider(
            (requestedSignature, location, allowDownload) -> wrongCache.toString(),
            false,
            (requestedSignature, progress) -> {
                rootLookups.incrementAndGet();
                return com.opensr5.ini.IniLocator.RootSearchResult.unavailable();
            },
            (requestedSignature, source) -> {
                throw new AssertionError("no root INI should be imported");
            },
            requestedSignature -> {
                tunerLookups.incrementAndGet();
                return tunerStudioIni.toString();
            });

        assertEquals(signature, provider.provide(signature).getSignature());
        assertEquals(1, rootLookups.get());
        assertEquals(1, tunerLookups.get(),
            "an unrelated download/cache INI is treated as a miss, not as a valid layout");
    }

    @Test
    public void exhaustsAutomaticSourcesInOrderBeforeManualPicker() {
        String signature = "rusEFI master.2026.11.20.fallback-order.768349105";
        List<String> events = new ArrayList<>();
        RealIniFileProvider.manualPicker = new RealIniFileProvider.ManualIniPicker() {
            @Override
            public void pick(String ignored) {
                events.add("manual");
            }

            @Override
            public void progress(String ignored, String port, long generation,
                                 com.opensr5.ini.IniLocator.LookupProgress progress) {
                events.add(progress.stage.name());
            }
        };
        RealIniFileProvider provider = new RealIniFileProvider(
            (requestedSignature, location, allowDownload) -> {
                events.add("download");
                return null;
            },
            true,
            (requestedSignature, progress) -> {
                events.add("drive-roots");
                return com.opensr5.ini.IniLocator.RootSearchResult.unavailable();
            },
            (requestedSignature, source) -> {
                throw new AssertionError("there is no matching root snapshot");
            });

        assertThrows(IniNotFoundException.class, () -> provider.provide(signature));

        List<String> expected = new ArrayList<>(List.of("BUNDLED_INI", "CURRENT_DIRECTORY"));
        if (com.opensr5.ini.IniLocator.isWindowsRootSearchSupported()) {
            expected.add("DRIVE_ROOTS");
        }
        expected.add("drive-roots");
        expected.add("CONSOLE_CACHE_DOWNLOAD");
        expected.add("download");
        expected.add("TUNERSTUDIO_CACHE");
        expected.add("COMPLETE");
        expected.add("manual");
        assertEquals(expected, events);
    }

    @Test
    public void cancelledLookupPublishesTerminalProgressWithoutOpeningManualPicker() {
        String signature = "rusEFI master.2026.11.20.cancelled-lookup.768349108";
        AtomicBoolean connectionCurrent = new AtomicBoolean(true);
        AtomicInteger prompts = new AtomicInteger();
        List<com.opensr5.ini.IniLocator.LookupProgress> updates = new ArrayList<>();
        RealIniFileProvider.manualPicker = new RealIniFileProvider.ManualIniPicker() {
            @Override
            public void pick(String ignored) {
                prompts.incrementAndGet();
            }

            @Override
            public void progress(String ignored, String port, long generation,
                                 com.opensr5.ini.IniLocator.LookupProgress progress) {
                updates.add(progress);
            }
        };
        RealIniFileProvider provider = new RealIniFileProvider(
            (requestedSignature, location, allowDownload) -> null,
            true,
            (requestedSignature, progress) -> {
                connectionCurrent.set(false);
                return com.opensr5.ini.IniLocator.RootSearchResult.cancelled();
            },
            (requestedSignature, source) -> null);

        assertThrows(IniNotFoundException.class,
            () -> provider.provide(signature, "COM9", 20, connectionCurrent::get));

        assertEquals(IniLocator.LookupStage.COMPLETE, updates.get(updates.size() - 1).stage);
        assertEquals(0, prompts.get());
    }

    @Test
    public void doesNotImportRootSnapshotAfterConnectionBecomesObsolete(@TempDir Path directory) throws Exception {
        String signature = "rusEFI master.2026.11.20.connection-stale-found.768349109";
        AtomicBoolean connectionCurrent = new AtomicBoolean(true);
        AtomicInteger imports = new AtomicInteger();
        Path importedIni = directory.resolve("imported.ini");
        RealIniFileProvider provider = new RealIniFileProvider(
            (requestedSignature, location, allowDownload) -> null,
            false,
            (requestedSignature, progress) -> {
                connectionCurrent.set(false);
                return IniLocator.RootSearchResult.found(matchingIni(requestedSignature));
            },
            (requestedSignature, source) -> {
                imports.incrementAndGet();
                Files.write(importedIni, source);
                return importedIni.toString();
            });

        assertThrows(IniNotFoundException.class,
            () -> provider.provide(signature, "COM9", 42, connectionCurrent::get));

        assertEquals(0, imports.get(),
            "the stale connection is rejected before importing the root snapshot");
        assertFalse(Files.exists(importedIni),
            "an obsolete lookup leaves no imported INI side effect");
    }

    @Test
    public void parseFailureLeavesDriveRootIniUntouched(@TempDir Path directory) throws Exception {
        Path rootIni = directory.resolve("customer.ini");
        Files.write(rootIni, ("[MegaTune]\nsignature = \"" + MISSING_SIGNATURE + "\"\n"
            + "nPages = 1\nochBlockSize = 8\npageSize = 8\npageReadCommand = \"R\"\n"
            + "page = not-a-number\n").getBytes(StandardCharsets.UTF_8));
        RealIniFileProvider provider = new RealIniFileProvider(
            (requestedSignature, location, allowDownload) -> null, false,
            (signature, progress) -> {
                try {
                    byte[] snapshot = com.rusefi.core.SignatureHelper.readMatchingIni(signature, rootIni.toFile());
                    return snapshot == null
                        ? com.opensr5.ini.IniLocator.RootSearchResult.unavailable()
                        : com.opensr5.ini.IniLocator.RootSearchResult.found(snapshot);
                } catch (java.io.IOException e) {
                    throw new UncheckedIOException(e);
                }
            },
            (signature, source) -> {
                return com.rusefi.core.SignatureHelper.importIntoCache(
                    signature, source, directory.resolve("cache").toString());
            });

        IniNotFoundException error = assertThrows(IniNotFoundException.class,
            () -> provider.provide(MISSING_SIGNATURE));

        assertTrue(error.getMessage().contains("Parsing error"));
        assertTrue(Files.exists(rootIni), "provider cleanup must never delete the original root file");
    }

    @Test
    public void importsRootCompressedArchiveSnapshotAfterSourceDisappears(@TempDir Path directory) throws Throwable {
        byte[] ini = matchingIni(MISSING_SIGNATURE);
        Path archive = CompressedTestArchive.create(directory.resolve("board-inis.7z"),
            directory.resolve("archive-input"), "board/rusefi_board.ini", ini);
        AtomicInteger lookupCount = new AtomicInteger();
        AtomicInteger importCount = new AtomicInteger();
        RealIniFileProvider provider = new RealIniFileProvider(
            (requestedSignature, location, allowDownload) -> null, false,
            (signature, progress) -> {
                lookupCount.incrementAndGet();
                try {
                    byte[] snapshot = com.rusefi.core.SignatureHelper.readMatchingIni(signature, archive.toFile());
                    Files.delete(archive);
                    return com.opensr5.ini.IniLocator.RootSearchResult.found(snapshot);
                } catch (java.io.IOException e) {
                    throw new UncheckedIOException(e);
                }
            },
            (signature, source) -> {
                importCount.incrementAndGet();
                return com.rusefi.core.SignatureHelper.importIntoCache(
                    signature, source, directory.resolve("cache").toString());
            });

        assertEquals(MISSING_SIGNATURE, provider.provide(MISSING_SIGNATURE).getSignature());
        assertEquals(1, lookupCount.get());
        assertEquals(1, importCount.get(), "the owned snapshot is published after lookup returns");
        assertFalse(Files.exists(archive), "source archive remains unavailable after the snapshot is returned");
        assertArrayEquals(ini, Files.readAllBytes(directory.resolve("cache").resolve("1234567890.ini")));
    }

    @Test
    public void manualPickerIsRequestedOnlyAfterSynchronousRootLookupFinishes() throws Exception {
        CountDownLatch rootSearchEntered = new CountDownLatch(1);
        CountDownLatch releaseWorker = new CountDownLatch(1);
        AtomicInteger prompts = new AtomicInteger();
        RealIniFileProvider.manualPicker = ignored -> {
            prompts.incrementAndGet();
        };
        RealIniFileProvider provider = new RealIniFileProvider(
            (requestedSignature, location, allowDownload) -> null, true,
            (signature, progress) -> {
                rootSearchEntered.countDown();
                try {
                    releaseWorker.await();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    return com.opensr5.ini.IniLocator.RootSearchResult.cancelled();
                }
                return com.opensr5.ini.IniLocator.RootSearchResult.unavailable();
            },
            (signature, source) -> {
                throw new AssertionError("no matching root snapshot should be imported");
            });

        try {
            FutureTask<Void> lookup = new FutureTask<>(() -> {
                assertThrows(IniNotFoundException.class, () -> provider.provide(MISSING_SIGNATURE));
                return null;
            });
            Thread lookupThread = new Thread(lookup, "test bounded provider lookup");
            lookupThread.setDaemon(true);
            lookupThread.start();
            assertTrue(rootSearchEntered.await(1, TimeUnit.SECONDS));
            assertEquals(0, prompts.get(), "the fallback must not open while root lookup is running");
            releaseWorker.countDown();
            lookup.get(1, TimeUnit.SECONDS);
            assertEquals(1, prompts.get(), "the fallback opens only after terminal lookup failure");
        } finally {
            releaseWorker.countDown();
        }
    }

    @Test
    public void unrelatedMissingProbeStillSuppliesPortForUiOwnershipFiltering() {
        AtomicReference<String> fallbackPort = new AtomicReference<>();
        AtomicInteger fallbackEvents = new AtomicInteger();
        RealIniFileProvider.manualPicker = new RealIniFileProvider.ManualIniPicker() {
            @Override
            public void pick(String signature) {
            }

            @Override
            public void pick(String signature, String port, long generation) {
                fallbackEvents.incrementAndGet();
                fallbackPort.set(port);
            }
        };
        RealIniFileProvider provider = new RealIniFileProvider(
            (requestedSignature, location, allowDownload) -> null, true,
            (signature, progress) -> com.opensr5.ini.IniLocator.RootSearchResult.unavailable(),
            (signature, source) -> null);

        assertThrows(IniNotFoundException.class,
            () -> provider.provide(UNPARSEABLE_SIGNATURE, "COM-B", 12));

        assertEquals(1, fallbackEvents.get());
        assertEquals("COM-B", fallbackPort.get(),
            "the UI must filter an unrelated probe using the supplied port");
    }

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

    private static byte[] matchingIni(String signature) {
        return ("[MegaTune]\nsignature = \"" + signature + "\"\n[Constants]\n"
            + "nPages = 1\npageSize = 8\npageReadCommand = \"R\"\n"
            + "page = 1\nvalue = scalar, U08, 0, \"\", 1, 0, 0, 255, 0\n"
            + "[OutputChannels]\nochBlockSize = 8\n").getBytes(StandardCharsets.UTF_8);
    }

    @Test
    public void requestsManualIniWhenAllElseFails() {
        RecordingPicker picker = new RecordingPicker();
        RealIniFileProvider.manualPicker = picker;

        assertThrows(IniNotFoundException.class,
            () -> new RealIniFileProvider().provide(UNPARSEABLE_SIGNATURE));

        assertEquals(UNPARSEABLE_SIGNATURE, picker.askedSignature);
    }

    @Test
    public void unsupportedPlatformsDoNotReportWindowsDriveProgress() {
        if (com.opensr5.ini.IniLocator.isWindowsRootSearchSupported()) {
            return;
        }
        AtomicInteger progressEvents = new AtomicInteger();
        RealIniFileProvider.manualPicker = new RealIniFileProvider.ManualIniPicker() {
            @Override
            public void pick(String signature) {
            }

            @Override
            public void searching(String signature, String port, long generation) {
                progressEvents.incrementAndGet();
            }
        };

        assertThrows(IniNotFoundException.class,
            () -> new RealIniFileProvider().provide(UNPARSEABLE_SIGNATURE));

        assertEquals(0, progressEvents.get());
    }

    @Test
    public void providerCanSkipGlobalManualPickerForScopedLookup() {
        AtomicInteger promptCount = new AtomicInteger();
        RealIniFileProvider.manualPicker = ignored -> promptCount.incrementAndGet();
        RealIniFileProvider provider = new RealIniFileProvider((signature, location, download) -> null, false);

        assertThrows(IniNotFoundException.class, () -> provider.provide(UNPARSEABLE_SIGNATURE));

        assertEquals(0, promptCount.get());
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
    public void repeatedProbesDoNotRepeatRemoteLookupAfterManualPickerWasRequested() {
        AtomicInteger downloadCount = new AtomicInteger();
        RealIniFileProvider.manualPicker = ignored -> { };
        RealIniFileProvider provider = new RealIniFileProvider((signature, ignored, allowDownload) -> {
            if (allowDownload) {
                downloadCount.incrementAndGet();
            }
            return null;
        });

        for (int i = 0; i < 5; i++) {
            assertThrows(IniNotFoundException.class, () -> provider.provide(UNPARSEABLE_SIGNATURE));
        }

        assertEquals(1, downloadCount.get(),
            "later scanner probes must check the cache without retrying the unavailable remote INI");
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
