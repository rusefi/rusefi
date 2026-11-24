package com.opensr5.ini;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.FutureTask;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.Base64;

import static org.junit.jupiter.api.Assertions.*;

class IniRootSearchTest {
    private static final String WANTED = "rusEFI fix/10540.2026.11.20.uaefi_pro.1254041395";
    private static final String OTHER = "rusEFI master.2026.07.02.other.987654321";

    @Test
    void findsExactSignatureInRootIniOrCompressedArchive(@TempDir Path directory) throws Exception {
        Path firstRoot = Files.createDirectory(directory.resolve("first"));
        Path secondRoot = Files.createDirectory(directory.resolve("second"));
        Files.write(firstRoot.resolve("rusefi_other.ini"), ini(OTHER));
        Path archive = secondRoot.resolve("bundle.7z");
        Files.write(archive, Base64.getDecoder().decode(
            "N3q8ryccAAR7vJ+oewAAAAAAAABmAAAAAAAAAO8sLnngAHwAc10ALZNJDIEm5rC+xuj76BP0nKD38G9Y1AxLSssp8FEIO5s3FvEC6jR/ASUKgX4b3jgFphEMOVhCSKQ6RSuyw0SdQGQj4q9f/2UcLnH37432agR7cn1ByGBZXf7tt8+MvfQlhUHyNGgbiojfnWS06sUPNAygAAABBAYAAQl7AAcLAQABISEBAAx9AAgKAcrqHgcAAAUBGQwAAAAAAAAAAAAAAAARKwByAHUAcwBlAGYAaQBfAHUAYQBlAGYAaQBfAHAAcgBvAC4AaQBuAGkAAAAZABUGAQAggKSBAAA="));
        byte[] archivedIni = ("[MegaTune]\nsignature= \"" + WANTED
            + "\" ; signature is expected to be 7 or more characters.\n").getBytes(StandardCharsets.UTF_8);

        ThreadPoolExecutor workers = workers();
        try {
            IniRootSearch search = new IniRootSearch(
                () -> new File[]{firstRoot.toFile(), secondRoot.toFile()},
                IniLocator::findMatchingIniInRoot, workers, 1_000);
            assertArrayEquals(archivedIni, search.find(WANTED).getIni());

            IniRootSearch noMatch = new IniRootSearch(() -> new File[]{firstRoot.toFile()},
                IniLocator::findMatchingIniInRoot, workers, 1_000);
            assertEquals(IniLocator.RootSearchResult.Status.UNAVAILABLE,
                noMatch.find(WANTED).getStatus());

            Path plain = firstRoot.resolve("customer.ini");
            Files.write(plain, ini(WANTED));
            IniRootSearch withPlainIni = new IniRootSearch(() -> new File[]{firstRoot.toFile()},
                IniLocator::findMatchingIniInRoot, workers, 1_000);
            assertArrayEquals(ini(WANTED), withPlainIni.find(WANTED).getIni());
        } finally {
            workers.shutdownNow();
        }
    }

    @Test
    void reportsMonotonicRootCompletionsAndActiveWorkerCounts(@TempDir Path directory) throws Exception {
        int rootCount = 3;
        CountDownLatch entered = new CountDownLatch(rootCount);
        CountDownLatch release = new CountDownLatch(1);
        List<IniLocator.LookupProgress> updates = new CopyOnWriteArrayList<>();
        ThreadPoolExecutor workers = workers();
        try {
            File[] roots = new File[rootCount];
            for (int i = 0; i < rootCount; i++) {
                roots[i] = directory.resolve("progress-root-" + i).toFile();
            }
            IniRootSearch search = new IniRootSearch(() -> roots, (root, signature) -> {
                entered.countDown();
                try {
                    release.await();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
                return null;
            }, workers, 2_000);

            FutureTask<IniLocator.RootSearchResult> lookup =
                new FutureTask<>(() -> search.find(WANTED, updates::add));
            daemon(lookup, "test root progress").start();
            assertTrue(entered.await(1, TimeUnit.SECONDS));
            release.countDown();
            assertEquals(IniLocator.RootSearchResult.Status.UNAVAILABLE,
                lookup.get(2, TimeUnit.SECONDS).getStatus());

            int lastCompleted = -1;
            boolean observedAllWorkersActive = false;
            for (IniLocator.LookupProgress update : updates) {
                assertEquals(IniLocator.LookupStage.DRIVE_ROOTS, update.stage);
                assertEquals(rootCount, update.totalRoots);
                assertTrue(update.completedRoots >= lastCompleted,
                    "completed-root count never moves backward");
                assertTrue(update.activeWorkers >= 0 && update.activeWorkers <= rootCount);
                lastCompleted = update.completedRoots;
                observedAllWorkersActive |= update.activeWorkers == rootCount;
            }
            IniLocator.LookupProgress terminal = updates.get(updates.size() - 1);
            assertEquals(rootCount, terminal.completedRoots);
            assertEquals(0, terminal.activeWorkers);
            assertTrue(observedAllWorkersActive, "the progress includes the active worker count");
        } finally {
            release.countDown();
            workers.shutdownNow();
        }
    }

    @Test
    void serializesConcurrentCompletionProgressPublicationMonotonically() throws Exception {
        CountDownLatch bothRootListingsStarted = new CountDownLatch(2);
        CountDownLatch firstSnapshotCaptured = new CountDownLatch(1);
        CountDownLatch releaseFirstPublication = new CountDownLatch(1);
        List<IniLocator.LookupProgress> updates = new CopyOnWriteArrayList<>();
        ThreadPoolExecutor workers = workers();
        try {
            File firstRoot = new File("first-root");
            File secondRoot = new File("second-root");
            IniRootSearch search = new IniRootSearch(
                () -> new File[]{firstRoot, secondRoot},
                (root, signature) -> {
                    bothRootListingsStarted.countDown();
                    try {
                        bothRootListingsStarted.await();
                        if (root == secondRoot) {
                            firstSnapshotCaptured.await();
                        }
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                    }
                    return null;
                }, workers, 8_000);

            IniLocator.RootSearchResult result = search.find(WANTED, update -> {
                if (update.completedRoots == 1) {
                    firstSnapshotCaptured.countDown();
                    try {
                        releaseFirstPublication.await(4, TimeUnit.SECONDS);
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                    }
                    updates.add(update);
                } else {
                    updates.add(update);
                    if (update.completedRoots == 2) {
                        releaseFirstPublication.countDown();
                    }
                }
            });

            assertEquals(IniLocator.RootSearchResult.Status.UNAVAILABLE, result.getStatus());
            List<Integer> completedUpdates = new ArrayList<>();
            for (IniLocator.LookupProgress update : updates) {
                if (update.completedRoots > 0) {
                    completedUpdates.add(update.completedRoots);
                }
            }
            assertEquals(List.of(1, 2), completedUpdates,
                "completion progress remains monotonic even when the first publication is delayed");
        } finally {
            releaseFirstPublication.countDown();
            workers.shutdownNow();
        }
    }

    @Test
    void blockedRootListingsAreBoundedByDeadlineAndLimitedToSixteenWorkers(@TempDir Path directory)
            throws Exception {
        CountDownLatch entered = new CountDownLatch(IniRootSearch.MAX_ROOTS);
        CountDownLatch release = new CountDownLatch(1);
        AtomicInteger rootEnumerations = new AtomicInteger();
        AtomicInteger active = new AtomicInteger();
        AtomicInteger maximumActive = new AtomicInteger();
        AtomicInteger rootListings = new AtomicInteger();
        ThreadPoolExecutor workers = workers();
        try {
            File[] roots = new File[IniRootSearch.MAX_ROOTS + 4];
            for (int i = 0; i < roots.length; i++) {
                roots[i] = directory.resolve("root-" + i).toFile();
            }
            IniRootSearch search = new IniRootSearch(() -> {
                rootEnumerations.incrementAndGet();
                return roots;
            }, (root, signature) -> {
                rootListings.incrementAndGet();
                int nowActive = active.incrementAndGet();
                maximumActive.accumulateAndGet(nowActive, Math::max);
                entered.countDown();
                try {
                    boolean released = false;
                    while (!released) {
                        try {
                            released = release.await(5, TimeUnit.SECONDS);
                        } catch (InterruptedException ignored) {
                            // Model a native root listing that does not respond to interruption.
                        }
                    }
                    return null;
                } finally {
                    active.decrementAndGet();
                }
            }, workers, 700);

            FutureTask<IniLocator.RootSearchResult> first = new FutureTask<>(() -> search.find(WANTED));
            Thread firstThread = daemon(first, "test INI root search");
            firstThread.start();
            assertTrue(entered.await(2, TimeUnit.SECONDS), "all 16 root workers should start concurrently");

            CountDownLatch duplicateStarted = new CountDownLatch(1);
            FutureTask<IniLocator.RootSearchResult> duplicate = new FutureTask<>(() -> {
                duplicateStarted.countDown();
                return search.find(WANTED);
            });
            daemon(duplicate, "test duplicate INI root search").start();
            assertTrue(duplicateStarted.await(1, TimeUnit.SECONDS));
            assertEquals(IniLocator.RootSearchResult.Status.UNAVAILABLE,
                first.get(2, TimeUnit.SECONDS).getStatus(), "the background caller returns at its deadline");
            assertEquals(IniLocator.RootSearchResult.Status.UNAVAILABLE,
                duplicate.get(2, TimeUnit.SECONDS).getStatus(), "a second caller also has a bounded lookup");
            assertEquals(IniRootSearch.MAX_ROOTS, active.get(),
                "caller timeout leaves native-style root I/O active");
            assertEquals(1, rootEnumerations.get());
            assertEquals(IniRootSearch.MAX_ROOTS, rootListings.get(), "only the first 16 roots are submitted");
            assertTrue(maximumActive.get() <= 16, "root listing worker count must stay capped");
            release.countDown();
        } finally {
            release.countDown();
            workers.shutdownNow();
        }
    }

    @Test
    void blockedRootEnumerationAlsoReturnsByOverallDeadline(@TempDir Path directory) throws Exception {
        CountDownLatch entered = new CountDownLatch(1);
        CountDownLatch release = new CountDownLatch(1);
        ThreadPoolExecutor workers = workers();
        try {
            IniRootSearch search = new IniRootSearch(() -> {
                entered.countDown();
                boolean released = false;
                while (!released) {
                    try {
                        released = release.await(5, TimeUnit.SECONDS);
                    } catch (InterruptedException ignored) {
                        // Model a native drive-enumeration call that does not respond to interruption.
                    }
                }
                return new File[]{directory.toFile()};
            }, IniLocator::findMatchingIniInRoot, workers, 200);
            FutureTask<IniLocator.RootSearchResult> lookup = new FutureTask<>(() -> search.find(WANTED));
            daemon(lookup, "test blocked drive enumeration").start();

            assertTrue(entered.await(1, TimeUnit.SECONDS));
            assertEquals(IniLocator.RootSearchResult.Status.UNAVAILABLE,
                lookup.get(1, TimeUnit.SECONDS).getStatus());
        } finally {
            release.countDown();
            workers.shutdownNow();
        }
    }

    @Test
    void returnsLateSuccessWhenItArrivesBeforeTheBoundedSearchDeadline(@TempDir Path directory) throws Exception {
        CountDownLatch entered = new CountDownLatch(1);
        CountDownLatch release = new CountDownLatch(1);
        AtomicInteger rootEnumerations = new AtomicInteger();
        AtomicInteger rootListings = new AtomicInteger();
        ThreadPoolExecutor workers = workers();
        try {
            IniRootSearch search = new IniRootSearch(() -> {
                rootEnumerations.incrementAndGet();
                return new File[]{directory.toFile()};
            }, (root, signature) -> {
                rootListings.incrementAndGet();
                entered.countDown();
                try {
                    release.await();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    return null;
                }
                return ini(WANTED);
            }, workers, 2_000);
            FutureTask<IniLocator.RootSearchResult> lookup = new FutureTask<>(() -> search.find(WANTED));
            daemon(lookup, "test late INI root match").start();
            assertTrue(entered.await(1, TimeUnit.SECONDS));
            release.countDown();

            assertArrayEquals(ini(WANTED), lookup.get(2, TimeUnit.SECONDS).getIni());
            assertEquals(1, rootEnumerations.get());
            assertEquals(1, rootListings.get());
        } finally {
            release.countDown();
            workers.shutdownNow();
        }
    }

    @Test
    void overallDeadlineEndsLogicalSearchWithoutWaitingForUninterruptibleRootIo(@TempDir Path directory)
            throws Exception {
        CountDownLatch entered = new CountDownLatch(1);
        CountDownLatch release = new CountDownLatch(1);
        CountDownLatch workerFinished = new CountDownLatch(1);
        AtomicInteger active = new AtomicInteger();
        List<IniLocator.LookupProgress> updates = new CopyOnWriteArrayList<>();
        ThreadPoolExecutor workers = workers();
        try {
            IniRootSearch search = new IniRootSearch(() -> new File[]{directory.toFile()},
                (root, signature) -> {
                    active.incrementAndGet();
                    entered.countDown();
                    try {
                        boolean released = false;
                        while (!released) {
                            try {
                                released = release.await(5, TimeUnit.SECONDS);
                            } catch (InterruptedException ignored) {
                                // Model native filesystem I/O that ignores cancellation.
                            }
                        }
                    } finally {
                        active.decrementAndGet();
                        workerFinished.countDown();
                    }
                    return null;
                }, workers, 300);
            FutureTask<IniLocator.RootSearchResult> lookup =
                new FutureTask<>(() -> search.find(WANTED, updates::add));
            daemon(lookup, "test overall INI root deadline").start();
            assertTrue(entered.await(1, TimeUnit.SECONDS));
            assertEquals(IniLocator.RootSearchResult.Status.UNAVAILABLE,
                lookup.get(1, TimeUnit.SECONDS).getStatus());
            assertEquals(1, active.get(), "the overall deadline is logical even if native I/O persists");
            int updatesAtTimeout = updates.size();
            release.countDown();
            assertTrue(workerFinished.await(1, TimeUnit.SECONDS));
            assertEquals(updatesAtTimeout, updates.size(),
                "a root worker finishing after timeout cannot publish stale progress");
        } finally {
            release.countDown();
            workers.shutdownNow();
        }
    }

    @Test
    void completedSearchesDoNotRetainSnapshotsOrRequireCleanupTasks(@TempDir Path directory) {
        ThreadPoolExecutor workers = workers();
        AtomicInteger rootEnumerations = new AtomicInteger();
        try {
            IniRootSearch search = new IniRootSearch(() -> {
                rootEnumerations.incrementAndGet();
                return new File[]{directory.toFile()};
            },
                (root, signature) -> null, workers, 1_000);

            for (int i = 0; i < 2; i++) {
                assertEquals(IniLocator.RootSearchResult.Status.UNAVAILABLE,
                    search.find("missing-signature").getStatus());
            }

            assertEquals(2, rootEnumerations.get(),
                "a completed lookup leaves no retained signature result or scheduled cleanup");
        } finally {
            workers.shutdownNow();
        }
    }

    private static ThreadPoolExecutor workers() {
        AtomicInteger id = new AtomicInteger();
        return new ThreadPoolExecutor(16, 16, 30, TimeUnit.SECONDS,
            new ArrayBlockingQueue<>(16), runnable -> {
                Thread thread = new Thread(runnable, "test INI root worker " + id.incrementAndGet());
                thread.setDaemon(true);
                return thread;
            });
    }

    private static Thread daemon(FutureTask<?> task, String name) {
        Thread thread = new Thread(task, name);
        thread.setDaemon(true);
        return thread;
    }

    private static byte[] ini(String signature) {
        return ("[MegaTune]\nsignature = \"" + signature + "\"\n").getBytes(StandardCharsets.UTF_8);
    }
}
