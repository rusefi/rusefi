package com.rusefi.io.can;

import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.ui.StatusConsumer;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.FutureTask;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class PCanIoStreamTest {
    @Test
    void createOpensStandardEcuResponseFilterAndCloseReleasesPort() throws Exception {
        BlockingRawCanPort port = new BlockingRawCanPort();
        PCanIoStream stream = PCanIoStream.createStream(StatusConsumer.VOID, port);
        assertEquals(new CanAddress(VariableRegistryValues.CAN_ECU_SERIAL_TX_ID, false), port.openAddress);
        assertTrue(port.receiveStarted.await(1, TimeUnit.SECONDS));
        stream.close();
        stream.close();
        assertEquals(1, port.closeCalls);
    }

    @Test
    void createFailureReturnsNullWithoutStartingReader() {
        BlockingRawCanPort port = new BlockingRawCanPort();
        port.openFailure = new IOException("not available");
        List<String> messages = new ArrayList<>();
        assertNull(PCanIoStream.createStream(messages::add, port));
        assertEquals(1, messages.size());
        assertTrue(messages.get(0).contains("not available"));
        assertEquals(0, port.closeCalls);
    }

    @Test
    void writeUsesStandardIsoTpRequestAndPropagatesFailure() throws Exception {
        BlockingRawCanPort port = new BlockingRawCanPort();
        PCanIoStream stream = PCanIoStream.createStream(StatusConsumer.VOID, port);
        assertTrue(port.receiveStarted.await(1, TimeUnit.SECONDS));
        stream.write(new byte[]{1, 2});
        assertEquals(1, port.sent.size());
        assertEquals(new CanAddress(VariableRegistryValues.CAN_ECU_SERIAL_RX_ID, false), port.sent.get(0).getAddress());
        assertArrayEquals(new byte[]{2, 1, 2}, port.sent.get(0).getPayload());
        port.sendFailure = new IOException("queue full");
        IOException failure = assertThrows(IOException.class, () -> stream.write(new byte[]{3}));
        assertEquals("queue full", failure.getMessage());
        stream.close();
    }

    @Test
    void closeReleasesPortBeforeNotifyingListenersAndIsIdempotent() {
        BlockingRawCanPort port = new BlockingRawCanPort();
        PCanIoStream stream = newStream(port);
        List<String> notifications = new ArrayList<>();
        stream.addCloseListener(() -> {
            assertTrue(port.closed, "Release the port before notifying reconnect listeners");
            notifications.add("closed");
        });
        stream.close();
        stream.close();
        assertTrue(stream.isClosed());
        assertEquals(List.of("closed"), notifications);
        assertEquals(1, port.closeCalls);
    }

    @Test
    void closeErrorStillMarksStreamClosedAndListenerCannotLeakPort() {
        BlockingRawCanPort port = new BlockingRawCanPort();
        port.closeFailure = new IOException("close failed");
        PCanIoStream stream = newStream(port);
        stream.addCloseListener(() -> {
            assertTrue(port.closed, "The port must be released even when close reports an error");
            throw new IllegalStateException("listener failed");
        });
        assertThrows(IllegalStateException.class, stream::close);
        assertTrue(stream.isClosed());
        assertEquals(1, port.closeCalls);
        stream.close();
        assertEquals(1, port.closeCalls);
    }

    @Test
    void isClosedDuringPortReleaseDoesNotBlock() throws Exception {
        BlockingRawCanPort port = new BlockingRawCanPort();
        port.closeStarted = new CountDownLatch(1);
        port.allowClose = new CountDownLatch(1);
        PCanIoStream stream = newStream(port);
        FutureTask<Void> closeResult = new FutureTask<>(() -> {
            stream.close();
            return null;
        });
        Thread closer = new Thread(closeResult);
        closer.start();
        try {
            assertTrue(port.closeStarted.await(5, TimeUnit.SECONDS));
            AtomicBoolean observedClosed = new AtomicBoolean(true);
            Thread observer = new Thread(() -> observedClosed.set(stream.isClosed()));
            observer.start();
            observer.join(5_000);
            assertFalse(observer.isAlive(), "isClosed must not acquire close()'s monitor");
            assertFalse(observedClosed.get(), "The closed flag is set after port release");
        } finally {
            port.allowClose.countDown();
            closer.join(5_000);
        }
        closeResult.get(5, TimeUnit.SECONDS);
        assertTrue(stream.isClosed());
    }

    @Test
    void emptyQueuePollingRequestsOneMillisecondBackoff() {
        ScriptedRawCanPort port = new ScriptedRawCanPort();
        List<Runnable> readers = new ArrayList<>();
        List<Long> waits = new ArrayList<>();
        PCanIoStream stream = new PCanIoStream(port, StatusConsumer.VOID, () -> readers::add, waits::add);
        port.onReceive = () -> {
            if (port.reads == 4) {
                stream.close();
            }
        };
        readers.get(0).run();
        assertEquals(4, port.reads);
        assertEquals(List.of(1L, 1L, 1L, 1L), waits);
    }

    @Test
    void interruptedEmptyQueuePollingClosesStream() {
        ScriptedRawCanPort port = new ScriptedRawCanPort();
        List<Runnable> readers = new ArrayList<>();
        PCanIoStream stream = new PCanIoStream(port, StatusConsumer.VOID, () -> readers::add,
            milliseconds -> { throw new InterruptedException(); });
        try {
            readers.get(0).run();
            assertTrue(Thread.currentThread().isInterrupted());
            assertTrue(stream.isClosed());
            assertEquals(1, port.reads);
        } finally {
            Thread.interrupted();
        }
    }

    @Test
    void successfulReadDeliversPayloadWithoutBackoff() {
        ScriptedRawCanPort port = new ScriptedRawCanPort();
        port.frames.add(Optional.of(new ClassicCanFrame(
            new CanAddress(VariableRegistryValues.CAN_ECU_SERIAL_TX_ID, false),
            new byte[]{2, 0x12, 0x34, 0, 0, 0, 0, 0})));
        List<Runnable> readers = new ArrayList<>();
        List<Long> waits = new ArrayList<>();
        PCanIoStream stream = new PCanIoStream(port, StatusConsumer.VOID, () -> readers::add, waits::add);
        List<byte[]> received = new ArrayList<>();
        stream.setInputListener(received::add);
        port.onReceive = stream::close;
        readers.get(1).run();
        assertEquals(1, port.reads);
        assertEquals(1, received.size());
        assertArrayEquals(new byte[]{0x12, 0x34}, received.get(0));
        assertTrue(waits.isEmpty());
    }

    @Test
    void readerDoesNotTouchPortAfterStreamIsClosed() {
        ScriptedRawCanPort port = new ScriptedRawCanPort();
        List<Runnable> readers = new ArrayList<>();
        PCanIoStream stream = new PCanIoStream(port, StatusConsumer.VOID, () -> readers::add, milliseconds -> { });
        stream.close();
        readers.get(0).run();
        assertEquals(0, port.reads);
    }

    @Test
    void setInputListenerExecutorsTerminateAfterStreamCloses() throws Exception {
        List<ExecutorService> executors = new ArrayList<>();
        RawCanPort port = new EmptyRawCanPort();
        PCanIoStream stream = new PCanIoStream(port, StatusConsumer.VOID, () -> {
            ExecutorService executor = Executors.newSingleThreadExecutor();
            executors.add(executor);
            return executor;
        }, milliseconds -> { });
        try {
            stream.setInputListener(data -> { });
            stream.close();
            // Every setInputListener() obtains a one-shot ExecutorService. Once the
            // stream closes, each reader task must finish and terminate its worker.
            assertEquals(2, executors.size());
            for (ExecutorService executor : executors) {
                assertTrue(executor.awaitTermination(5, TimeUnit.SECONDS),
                    "reader executor did not terminate after stream close");
                assertTrue(executor.isShutdown());
            }
        } finally {
            executors.forEach(ExecutorService::shutdownNow);
        }
    }

    @Test
    void readerExecutorsTerminateAcrossRepeatedCloseCycles() throws Exception {
        List<ExecutorService> executors = new ArrayList<>();
        try {
            for (int cycle = 0; cycle < 3; cycle++) {
                PCanIoStream stream = new PCanIoStream(new EmptyRawCanPort(), StatusConsumer.VOID, () -> {
                    ExecutorService executor = Executors.newSingleThreadExecutor();
                    executors.add(executor);
                    return executor;
                }, milliseconds -> { });
                stream.setInputListener(data -> { });
                stream.close();
            }

            assertEquals(6, executors.size(), "constructor and listener reader per cycle");
            for (ExecutorService executor : executors) {
                assertTrue(executor.awaitTermination(5, TimeUnit.SECONDS),
                    "reader executor did not terminate in repeated close cycle");
            }
        } finally {
            executors.forEach(ExecutorService::shutdownNow);
        }
    }

    @Test
    void listenerFailureReleasesPort() {
        ScriptedRawCanPort port = new ScriptedRawCanPort();
        port.frames.add(Optional.of(new ClassicCanFrame(
            new CanAddress(VariableRegistryValues.CAN_ECU_SERIAL_TX_ID, false),
            new byte[]{2, 0x12, 0x34})));
        List<Runnable> readers = new ArrayList<>();
        PCanIoStream stream = new PCanIoStream(port, StatusConsumer.VOID, () -> readers::add,
            milliseconds -> { });
        stream.setInputListener(data -> { throw new IllegalStateException("listener failed"); });
        try {
            assertThrows(IllegalStateException.class, () -> readers.get(1).run());
            // #10138: abnormal reader termination must release channel ownership.
            assertTrue(stream.isClosed());
            assertEquals(1, port.closeCalls);
        } finally {
            stream.close();
        }
    }

    @Test
    void rejectedReaderSubmissionReleasesPort() throws IOException {
        BlockingRawCanPort port = new BlockingRawCanPort();
        ExecutorService executor = Executors.newSingleThreadExecutor();
        executor.shutdown();
        try {
            assertThrows(RejectedExecutionException.class, () ->
                new PCanIoStream(port, StatusConsumer.VOID, () -> executor, milliseconds -> { }));
            // #10138: construction fails without returning a stream, so release its port here.
            assertTrue(port.closed);
            assertEquals(1, port.closeCalls);
        } finally {
            executor.shutdownNow();
            port.close();
        }
    }

    private static PCanIoStream newStream(RawCanPort port) {
        return new PCanIoStream(port, StatusConsumer.VOID, () -> command -> { }, milliseconds -> { });
    }

    private static class EmptyRawCanPort implements RawCanPort {
        @Override public void open(CanAddress address) throws IOException { }
        @Override public void send(ClassicCanFrame frame) throws IOException { }
        @Override public Optional<ClassicCanFrame> receive(int timeoutMs) throws IOException { return Optional.empty(); }
        @Override public void close() throws IOException { }
    }

    private static class ScriptedRawCanPort extends EmptyRawCanPort {
        final List<Optional<ClassicCanFrame>> frames = new ArrayList<>();
        Runnable onReceive = () -> { };
        int reads;
        int closeCalls;

        @Override public void close() {
            closeCalls++;
        }

        @Override public Optional<ClassicCanFrame> receive(int timeoutMs) {
            reads++;
            onReceive.run();
            return frames.isEmpty() ? Optional.empty() : frames.remove(0);
        }
    }

    private static class BlockingRawCanPort extends EmptyRawCanPort {
        final CountDownLatch receiveStarted = new CountDownLatch(1);
        final List<ClassicCanFrame> sent = new ArrayList<>();
        CanAddress openAddress;
        IOException openFailure;
        IOException sendFailure;
        IOException closeFailure;
        CountDownLatch closeStarted;
        CountDownLatch allowClose;
        boolean closed;
        int closeCalls;

        @Override public void open(CanAddress address) throws IOException {
            if (openFailure != null) {
                throw openFailure;
            }
            openAddress = address;
        }

        @Override public void send(ClassicCanFrame frame) throws IOException {
            sent.add(frame);
            if (sendFailure != null) {
                throw sendFailure;
            }
        }

        @Override public Optional<ClassicCanFrame> receive(int timeoutMs) throws IOException {
            receiveStarted.countDown();
            return Optional.empty();
        }

        @Override public void close() throws IOException {
            closeCalls++;
            closed = true;
            if (closeStarted != null) {
                closeStarted.countDown();
            }
            if (allowClose != null) {
                try {
                    allowClose.await();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    throw new IOException(e);
                }
            }
            if (closeFailure != null) {
                throw closeFailure;
            }
        }
    }
}
