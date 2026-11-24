package com.opensr5.ini;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.function.Consumer;
import java.util.function.Supplier;

/** Bounded, non-recursive search of drive-root files for an exact-signature INI. */
final class IniRootSearch {
    static final int MAX_ROOTS = 16;
    static final int WORKER_COUNT = 16;
    static final int QUEUED_TASK_LIMIT = 16;
    static final long SEARCH_TIMEOUT_MILLIS = 12_000;

    private static final ThreadPoolExecutor WORKERS = createWorkers();
    private static final IniRootSearch WINDOWS = new IniRootSearch(
        File::listRoots, IniLocator::findMatchingIniInRoot, WORKERS, SEARCH_TIMEOUT_MILLIS);

    interface RootLister {
        byte[] find(File root, String signature);
    }

    private final Supplier<File[]> roots;
    private final RootLister rootLister;
    private final ExecutorService executor;
    private final long searchTimeoutMillis;

    IniRootSearch(Supplier<File[]> roots, RootLister rootLister,
                  ExecutorService executor, long searchTimeoutMillis) {
        this.roots = roots;
        this.rootLister = rootLister;
        this.executor = executor;
        this.searchTimeoutMillis = searchTimeoutMillis;
    }

    static IniLocator.RootSearchResult findOnWindows(String signature) {
        return WINDOWS.find(signature);
    }

    static IniLocator.RootSearchResult findOnWindows(
            String signature, Consumer<IniLocator.LookupProgress> progress) {
        return WINDOWS.find(signature, progress);
    }

    IniLocator.RootSearchResult find(String signature) {
        return find(signature, ignored -> {});
    }

    IniLocator.RootSearchResult find(String signature, Consumer<IniLocator.LookupProgress> progress) {
        if (signature == null) {
            return IniLocator.RootSearchResult.unavailable();
        }

        long deadlineNanos = System.nanoTime() + TimeUnit.MILLISECONDS.toNanos(searchTimeoutMillis);
        CompletableFuture<IniLocator.RootSearchResult> result = new CompletableFuture<>();
        List<Future<?>> tasks = new ArrayList<>();
        AtomicBoolean reportProgress = new AtomicBoolean(true);
        AtomicInteger completedRoots = new AtomicInteger();
        AtomicInteger activeWorkers = new AtomicInteger();
        Object progressLock = new Object();
        Future<File[]> rootEnumeration;
        try {
            rootEnumeration = executor.submit(roots::get);
        } catch (RejectedExecutionException e) {
            return IniLocator.RootSearchResult.unavailable();
        }

        try {
            File[] availableRoots = rootEnumeration.get(remainingNanos(deadlineNanos), TimeUnit.NANOSECONDS);
            if (availableRoots == null) {
                return IniLocator.RootSearchResult.unavailable();
            }
            AtomicInteger remaining = new AtomicInteger(1);
            int rootCount = Math.min(availableRoots.length, MAX_ROOTS);
            report(progressLock, progress, reportProgress, completedRoots, rootCount, activeWorkers);
            for (int i = 0; i < rootCount; i++) {
                File root = availableRoots[i];
                remaining.incrementAndGet();
                try {
                    tasks.add(executor.submit(() -> {
                        synchronized (progressLock) {
                            activeWorkers.incrementAndGet();
                            report(progressLock, progress, reportProgress,
                                completedRoots, rootCount, activeWorkers);
                        }
                        try {
                            if (!result.isDone()) {
                                byte[] match = rootLister.find(root, signature);
                                if (match != null) {
                                    result.complete(IniLocator.RootSearchResult.found(match));
                                }
                            }
                        } catch (RuntimeException ignored) {
                            // An inaccessible root is equivalent to no match.
                        } finally {
                            synchronized (progressLock) {
                                activeWorkers.decrementAndGet();
                                completedRoots.incrementAndGet();
                                report(progressLock, progress, reportProgress,
                                    completedRoots, rootCount, activeWorkers);
                            }
                            if (remaining.decrementAndGet() == 0) {
                                result.complete(IniLocator.RootSearchResult.unavailable());
                            }
                        }
                    }));
                } catch (RejectedExecutionException e) {
                    synchronized (progressLock) {
                        completedRoots.incrementAndGet();
                        report(progressLock, progress, reportProgress,
                            completedRoots, rootCount, activeWorkers);
                    }
                    if (remaining.decrementAndGet() == 0) {
                        result.complete(IniLocator.RootSearchResult.unavailable());
                    }
                }
            }
            if (remaining.decrementAndGet() == 0) {
                result.complete(IniLocator.RootSearchResult.unavailable());
            }
            return result.get(remainingNanos(deadlineNanos), TimeUnit.NANOSECONDS);
        } catch (TimeoutException e) {
            result.complete(IniLocator.RootSearchResult.unavailable());
            return IniLocator.RootSearchResult.unavailable();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            result.complete(IniLocator.RootSearchResult.cancelled());
            return IniLocator.RootSearchResult.cancelled();
        } catch (ExecutionException e) {
            result.complete(IniLocator.RootSearchResult.unavailable());
            return IniLocator.RootSearchResult.unavailable();
        } finally {
            synchronized (progressLock) {
                reportProgress.set(false);
            }
            rootEnumeration.cancel(true);
            for (Future<?> task : tasks) {
                if (!task.isDone()) {
                    task.cancel(true);
                }
            }
        }
    }

    private static void report(Object progressLock, Consumer<IniLocator.LookupProgress> progress,
                               AtomicBoolean reportProgress,
                               AtomicInteger completedRoots, int totalRoots,
                               AtomicInteger activeWorkers) {
        synchronized (progressLock) {
            if (reportProgress.get()) {
                try {
                    progress.accept(new IniLocator.LookupProgress(IniLocator.LookupStage.DRIVE_ROOTS,
                        completedRoots.get(), totalRoots, activeWorkers.get(), 0));
                } catch (RuntimeException ignored) {
                    // Progress reporting must never interfere with root discovery.
                }
            }
        }
    }

    private static long remainingNanos(long deadlineNanos) throws TimeoutException {
        long remaining = deadlineNanos - System.nanoTime();
        if (remaining <= 0) {
            throw new TimeoutException();
        }
        return remaining;
    }

    private static ThreadPoolExecutor createWorkers() {
        AtomicInteger nextThread = new AtomicInteger();
        ThreadFactory factory = runnable -> {
            Thread thread = new Thread(runnable, "INI drive lookup " + nextThread.incrementAndGet());
            thread.setDaemon(true);
            return thread;
        };
        ThreadPoolExecutor workers = new ThreadPoolExecutor(
            WORKER_COUNT, WORKER_COUNT, 30, TimeUnit.SECONDS,
            new ArrayBlockingQueue<>(QUEUED_TASK_LIMIT), factory,
            new ThreadPoolExecutor.AbortPolicy());
        workers.allowCoreThreadTimeOut(true);
        return workers;
    }
}
