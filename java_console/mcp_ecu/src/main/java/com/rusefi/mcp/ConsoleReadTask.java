package com.rusefi.mcp;

import com.rusefi.io.LinkManager;

import java.io.IOException;
import java.util.concurrent.CancellationException;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.atomic.AtomicBoolean;

/** Cancellable borrowed-link work; never interrupts a shared wire transaction. */
final class ConsoleReadTask {
    interface Work<T> { T run(Runnable guard) throws Exception; }

    static <T> T run(LinkManager link, Runnable check, long timeoutMs, Work<T> work) throws Exception {
        AtomicBoolean stopped = new AtomicBoolean();
        long deadline = System.nanoTime() + TimeUnit.MILLISECONDS.toNanos(timeoutMs);
        Runnable guard = () -> {
            check.run();
            if (stopped.get() || System.nanoTime() - deadline >= 0) {
                throw new CancellationException("ECU read stopped or timed out.");
            }
        };
        FutureTask<T> task = new FutureTask<>(() -> { guard.run(); return work.run(guard); });
        try {
            guard.run();
            link.submit(task);
            while (true) {
                guard.run();
                try {
                    return task.get(50, TimeUnit.MILLISECONDS);
                } catch (TimeoutException waiting) {
                    // Check the borrowed session while queued or reading.
                } catch (ExecutionException failure) {
                    Throwable cause = failure.getCause();
                    if (cause instanceof Exception) { throw (Exception) cause; }
                    throw new IOException("ECU read failed.", cause);
                }
            }
        } finally {
            stopped.set(true);
            task.cancel(false);
        }
    }
}
