package com.rusefi.io;

import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotSame;
import static org.junit.jupiter.api.Assertions.assertTrue;

class LinkManagerThreadLifecycleTest {
    @Test
    void repeatedDiscoveryLinksDoNotStartCommandWorkers() {
        Set<Thread> before = commandWorkers();
        for (int scan = 0; scan < 8; scan++) {
            try (LinkManager link = new LinkManager().setNotifyGlobalStatusOnClose(false)) {
                // A failed discovery attempt may never send a console command.
            }
        }
        Set<Thread> remaining = commandWorkers();
        remaining.removeAll(before);
        assertEquals(0, remaining.size(), "discovery without console commands needs no command worker");
    }

    @Test
    void closedDiscoveryLinksReleaseCommunicationWorkers() throws Exception {
        List<Thread> workers = new ArrayList<>();
        List<LinkManager> links = new ArrayList<>();
        try {
            for (int scan = 0; scan < 8; scan++) {
                LinkManager link = new LinkManager().setNotifyGlobalStatusOnClose(false);
                links.add(link);
                workers.add(link.COMMUNICATION_EXECUTOR.submit(() -> {
                    link.assertCommunicationThread();
                    return Thread.currentThread();
                }).get(5, TimeUnit.SECONDS));
                link.close();
            }
            // Give all workers the same bounded opportunity to retire after the probes close.
            long deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(3);
            for (Thread worker : workers) {
                long remaining = deadline - System.nanoTime();
                if (remaining > 0) {
                    TimeUnit.NANOSECONDS.timedJoin(worker, remaining);
                }
                assertFalse(worker.isAlive(), "closed probe must release its idle communication worker");
            }
        } finally {
            for (LinkManager link : links) {
                link.close();
                link.COMMUNICATION_EXECUTOR.shutdownNow();
            }
        }
    }

    private static Set<Thread> commandWorkers() {
        return Thread.getAllStackTraces().keySet().stream()
            .filter(thread -> thread.getName().equals("ECU Commands Queue") && thread.isAlive())
            .collect(Collectors.toSet());
    }

    @Test
    void communicationWorkerRestartsAfterIdleAndClose() throws Exception {
        try (LinkManager link = new LinkManager().setNotifyGlobalStatusOnClose(false)) {
            Thread previous = null;
            for (int cycle = 0; cycle < 3; cycle++) {
                Thread worker = link.COMMUNICATION_EXECUTOR.submit(() -> {
                    link.assertCommunicationThread();
                    return Thread.currentThread();
                }).get(5, TimeUnit.SECONDS);
                assertNotSame(previous, worker);
                link.close();
                worker.join(5000);
                assertFalse(worker.isAlive(), "idle communication worker must exit");
                previous = worker;
            }
        }
    }

    @Test
    void commandWorkerRestartsAndPreservesOrderAfterIdleAndClose() throws Exception {
        List<String> sent = new ArrayList<>();
        List<String> expected = new ArrayList<>();
        AtomicReference<Thread> worker = new AtomicReference<>();
        try (LinkManager link = new LinkManager() {
            @Override
            public void send(String command, boolean fireEvent) {
                worker.set(Thread.currentThread());
                sent.add(command);
                getCommandQueue().handleConfirmationMessage(CommandQueue.CONFIRMATION_PREFIX + command);
            }
        }.setNotifyGlobalStatusOnClose(false)) {
            Thread previous = null;
            for (int cycle = 0; cycle < 3; cycle++) {
                CountDownLatch confirmed = new CountDownLatch(16);
                for (int command = 0; command < 16; command++) {
                    String text = cycle + ":" + command;
                    expected.add(text);
                    link.getCommandQueue().write(text, 1000, confirmed::countDown, false);
                }
                assertTrue(confirmed.await(5, TimeUnit.SECONDS), "every queued command must be confirmed");
                assertEquals(expected, sent, "commands must remain FIFO across worker replacement");
                assertNotSame(previous, worker.get());
                assertTrue(worker.get().isDaemon());
                link.close();
                worker.get().join(5000);
                assertFalse(worker.get().isAlive(), "idle command worker must exit");
                previous = worker.get();
            }
        }
    }

    @Test
    void addIfNotPresentStartsWorkerAndPreservesPendingCommandControls() throws Exception {
        CountDownLatch entered = new CountDownLatch(1);
        CountDownLatch release = new CountDownLatch(1);
        CountDownLatch confirmed = new CountDownLatch(2);
        List<String> sent = new ArrayList<>();
        AtomicReference<Thread> worker = new AtomicReference<>();
        try (LinkManager link = new LinkManager() {
            @Override
            public void send(String command, boolean fireEvent) throws InterruptedException {
                worker.set(Thread.currentThread());
                entered.countDown();
                release.await();
                sent.add(command);
                getCommandQueue().handleConfirmationMessage(CommandQueue.CONFIRMATION_PREFIX + command);
            }
        }.setNotifyGlobalStatusOnClose(false)) {
            CommandQueue queue = link.getCommandQueue();
            try {
                queue.addIfNotPresent(new CommandQueue.MethodInvocation(
                    "first", 1000, confirmed::countDown, false));
                assertTrue(entered.await(5, TimeUnit.SECONDS));
                queue.addIfNotPresent(new CommandQueue.MethodInvocation(
                    "discard", 1000, InvocationConfirmationListener.VOID, false));
                queue.clearPendingCommands();
                IMethodInvocation next = new CommandQueue.MethodInvocation(
                    "next", 1000, confirmed::countDown, false);
                queue.addIfNotPresent(next);
                queue.addIfNotPresent(next);
            } finally {
                release.countDown();
            }
            assertTrue(confirmed.await(5, TimeUnit.SECONDS));
            worker.get().join(5000);
            assertFalse(worker.get().isAlive());
            assertEquals(java.util.Arrays.asList("first", "next"), sent,
                "pending commands must still support clearing and duplicate suppression");
        }
    }
}
