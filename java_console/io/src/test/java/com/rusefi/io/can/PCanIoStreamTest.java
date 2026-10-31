package com.rusefi.io.can;

import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.ui.StatusConsumer;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.FutureTask;
import java.util.concurrent.LinkedBlockingQueue;
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
    void outboundMultiFrameWriteTimesOutWithoutFlowControl() throws Exception {
        BlockingRawCanPort port = new BlockingRawCanPort();
        PCanIoStream stream = newStream(port);
        try {
            IOException failure = assertThrows(IOException.class,
                () -> stream.write(new byte[]{1, 2, 3, 4, 5, 6, 7, 8}));

            assertTrue(failure.getMessage().contains("Flow Control"));
            // Only the reader may consume CAN frames; a timed-out writer must not consume the queue.
            assertEquals(0, port.receiveCalls);
            assertEquals(1, port.sent.size());
            assertArrayEquals(new byte[]{0x10, 8, 1, 2, 3, 4, 5, 6}, port.sent.get(0).getPayload());
            assertEquals(new CanAddress(VariableRegistryValues.CAN_ECU_SERIAL_RX_ID, false),
                port.sent.get(0).getAddress());
            assertTrue(stream.isClosed());
        } finally {
            stream.close();
        }
    }

    @Test
    void outboundMultiFrameWriteWaitsThroughDelayedFlowControl() throws Exception {
        ConcurrentWrite write = startConcurrentWrite(Optional.empty(), Optional.of(flowControlFrame((byte) 0x30, 0, 0)));
        try {
            write.result.get(1, TimeUnit.SECONDS);

            assertTrue(write.port.reads >= 2);
            assertEquals(2, write.port.sent.size());
            assertArrayEquals(new byte[]{0x21, 7, 8}, write.port.sent.get(1).getPayload());
            assertFalse(write.stream.isClosed());
        } finally {
            write.close();
        }
    }

    @Test
    void outboundMultiFrameWriteHonorsWaitAndThenContinuesAfterCts() throws Exception {
        ConcurrentWrite write = startConcurrentWrite(Optional.of(flowControlFrame((byte) 0x31, 0, 0)),
            Optional.of(flowControlFrame((byte) 0x30, 0, 0)));
        try {
            write.result.get(1, TimeUnit.SECONDS);

            assertTrue(write.port.reads >= 2);
            assertEquals(2, write.port.sent.size());
        } finally {
            write.close();
        }
    }

    @Test
    void closingStreamWakesWriterWaitingForFlowControl() throws Exception {
        ConcurrentWrite write = startConcurrentWrite();
        try {
            write.stream.close();
            ExecutionException failure = assertThrows(ExecutionException.class,
                () -> write.result.get(1, TimeUnit.SECONDS));
            assertTrue(failure.getCause() instanceof IOException);
        } finally {
            write.stream.close();
            for (ExecutorService executor : write.executors) {
                assertTrue(executor.awaitTermination(1, TimeUnit.SECONDS));
            }
        }
    }

    @Test
    void outboundMultiFrameWriteRejectsAbortAndUnsupportedFlowControlParameters() {
        byte[][] unsupportedFlowControls = {
            {(byte) 0x32, 0, 0},
            {0x30, 1, 0},
            {0x30, 0, 1},
        };

        for (byte[] flowControl : unsupportedFlowControls) {
            ScriptedRawCanPort port = new ScriptedRawCanPort();
            port.frames.add(Optional.of(new ClassicCanFrame(
                new CanAddress(VariableRegistryValues.CAN_ECU_SERIAL_TX_ID, false), flowControl)));
            PCanIoStream stream = newStream(port);
            try {
                IOException failure = assertThrows(IOException.class,
                    () -> stream.write(new byte[]{1, 2, 3, 4, 5, 6, 7, 8}));
                assertTrue(failure.getMessage().contains("ISO-TP"));
                assertEquals(1, port.sent.size());
                assertTrue(stream.isClosed());
            } finally {
                stream.close();
            }
        }
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
        readers.get(0).run();
        assertEquals(1, port.reads);
        assertEquals(1, received.size());
        assertArrayEquals(new byte[]{0x12, 0x34}, received.get(0));
        assertTrue(waits.isEmpty());
    }

    @Test
    void splitIsoTpResponseStreamsExpectedSixAndSevenByteChunks() {
        ScriptedRawCanPort port = new ScriptedRawCanPort();
        port.failOnExtraRead = true;
        port.frames.add(Optional.of(new ClassicCanFrame(
            new CanAddress(VariableRegistryValues.CAN_ECU_SERIAL_TX_ID, false),
            new byte[]{0x10, 0x0B, 1, 2, 3, 4, 5, 6})));
        port.frames.add(Optional.of(new ClassicCanFrame(
            new CanAddress(VariableRegistryValues.CAN_ECU_SERIAL_TX_ID, false),
            new byte[]{0x21, 7, 8, 9, 10, 11, 0, 0})));
        List<Runnable> readers = new ArrayList<>();
        PCanIoStream stream = new PCanIoStream(port, StatusConsumer.VOID, () -> readers::add,
            milliseconds -> { });
        List<byte[]> received = new ArrayList<>();
        stream.setInputListener(data -> {
            received.add(data);
            if (received.size() == 2) {
                stream.close();
            }
        });

        readers.get(0).run();

        // ISO-TP may deliver chunks separately; together they must reassemble to the response.
        assertEquals(2, received.size());
        assertArrayEquals(new byte[]{1, 2, 3, 4, 5, 6}, received.get(0));
        assertArrayEquals(new byte[]{7, 8, 9, 10, 11}, received.get(1));
        assertArrayEquals(new byte[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
            concatenate(received.get(0), received.get(1)));
        assertTrue(stream.isClosed());
    }

    @Test
    void malformedIsoTpFrameClosesReaderBeforeRetryCanMixFrames() {
        ScriptedRawCanPort port = new ScriptedRawCanPort();
        port.failOnExtraRead = true;
        CanAddress ecu = new CanAddress(VariableRegistryValues.CAN_ECU_SERIAL_TX_ID, false);
        port.frames.add(Optional.of(new ClassicCanFrame(ecu,
            new byte[]{0x10, 0x08, 1, 2, 3, 4, 5, 6})));
        port.frames.add(Optional.of(new ClassicCanFrame(ecu,
            new byte[]{0x22, 7, 8, 0, 0, 0, 0, 0})));
        port.frames.add(Optional.of(new ClassicCanFrame(ecu,
            new byte[]{0x10, 0x08, 9, 10, 11, 12, 13, 14})));
        port.frames.add(Optional.of(new ClassicCanFrame(ecu,
            new byte[]{0x21, 15, 16, 0, 0, 0, 0, 0})));
        List<Runnable> readers = new ArrayList<>();
        List<String> status = new ArrayList<>();
        PCanIoStream stream = new PCanIoStream(port, status::add, () -> readers::add,
            milliseconds -> { });
        List<byte[]> received = new ArrayList<>();
        List<Boolean> openAtCallback = new ArrayList<>();
        stream.setInputListener(data -> {
            received.add(data);
            openAtCallback.add(!stream.isClosed());
            if (received.size() == 4) {
                stream.close();
            }
        });

        assertThrows(IllegalStateException.class, () -> readers.get(0).run());

        // #10138: close on malformed ISO-TP so a retry cannot mix frames.
        assertEquals(1, received.size());
        assertArrayEquals(new byte[]{1, 2, 3, 4, 5, 6}, received.get(0));
        assertTrue(openAtCallback.get(0));
        assertTrue(stream.isClosed());
        assertTrue(status.stream().anyMatch(message -> message.contains("PCAN decode failed")));
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
            assertEquals(1, executors.size());
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

            assertEquals(3, executors.size(), "one reader per cycle owns the CAN receive queue");
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
            assertThrows(IllegalStateException.class, () -> readers.get(0).run());
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

    @SafeVarargs
    private static ConcurrentWrite startConcurrentWrite(Optional<ClassicCanFrame>... flowControls) throws Exception {
        ConcurrentRawCanPort port = new ConcurrentRawCanPort();
        port.afterFirstFrame = () -> {
            for (Optional<ClassicCanFrame> flowControl : flowControls) {
                port.incoming.add(flowControl);
            }
        };
        List<ExecutorService> executors = new ArrayList<>();
        PCanIoStream stream = new PCanIoStream(port, StatusConsumer.VOID, () -> {
            ExecutorService executor = Executors.newSingleThreadExecutor();
            executors.add(executor);
            return executor;
        }, milliseconds -> { });
        stream.setInputListener(data -> { });
        FutureTask<Void> result = new FutureTask<>(() -> {
            stream.write(new byte[]{1, 2, 3, 4, 5, 6, 7, 8});
            return null;
        });
        Thread writer = new Thread(result);
        writer.start();
        assertTrue(port.firstFrameSent.await(1, TimeUnit.SECONDS), "writer did not send First Frame");
        return new ConcurrentWrite(port, stream, result, executors);
    }

    private static byte[] concatenate(byte[] first, byte[] second) {
        byte[] result = new byte[first.length + second.length];
        System.arraycopy(first, 0, result, 0, first.length);
        System.arraycopy(second, 0, result, first.length, second.length);
        return result;
    }

    private static ClassicCanFrame flowControlFrame(byte status, int blockSize, int separationTime) {
        return new ClassicCanFrame(new CanAddress(VariableRegistryValues.CAN_ECU_SERIAL_TX_ID, false),
            new byte[]{status, (byte) blockSize, (byte) separationTime});
    }

    private static final class ConcurrentWrite {
        final ConcurrentRawCanPort port;
        final PCanIoStream stream;
        final FutureTask<Void> result;
        final List<ExecutorService> executors;

        ConcurrentWrite(ConcurrentRawCanPort port, PCanIoStream stream, FutureTask<Void> result,
                        List<ExecutorService> executors) {
            this.port = port;
            this.stream = stream;
            this.result = result;
            this.executors = executors;
        }

        void close() throws Exception {
            stream.close();
            result.get(1, TimeUnit.SECONDS);
            for (ExecutorService executor : executors) {
                assertTrue(executor.awaitTermination(1, TimeUnit.SECONDS));
            }
        }
    }

    private static class EmptyRawCanPort implements RawCanPort {
        @Override public void open(CanAddress address) throws IOException { }
        @Override public void send(ClassicCanFrame frame) throws IOException { }
        @Override public Optional<ClassicCanFrame> receive(int timeoutMs) throws IOException { return Optional.empty(); }
        @Override public void close() throws IOException { }
    }

    private static class ScriptedRawCanPort extends EmptyRawCanPort {
        final List<Optional<ClassicCanFrame>> frames = new ArrayList<>();
        final List<ClassicCanFrame> sent = new ArrayList<>();
        Runnable onReceive = () -> { };
        int reads;
        int closeCalls;
        boolean failOnExtraRead;

        @Override public void close() {
            closeCalls++;
        }

        @Override public void send(ClassicCanFrame frame) {
            sent.add(frame);
        }

        @Override public Optional<ClassicCanFrame> receive(int timeoutMs) {
            reads++;
            onReceive.run();
            if (failOnExtraRead && frames.isEmpty() && !isClosedForTest()) {
                throw new AssertionError("scripted CAN port read past end of finite frame script");
            }
            return frames.isEmpty() ? Optional.empty() : frames.remove(0);
        }

        private boolean isClosedForTest() {
            return closeCalls > 0;
        }
    }

    private static class ConcurrentRawCanPort extends EmptyRawCanPort {
        final BlockingQueue<Optional<ClassicCanFrame>> incoming = new LinkedBlockingQueue<>();
        final List<ClassicCanFrame> sent = new ArrayList<>();
        final CountDownLatch firstFrameSent = new CountDownLatch(1);
        Runnable afterFirstFrame = () -> { };
        volatile boolean closed;
        int reads;

        @Override public synchronized void send(ClassicCanFrame frame) {
            sent.add(frame);
            if (sent.size() == 1) {
                firstFrameSent.countDown();
                afterFirstFrame.run();
            }
        }

        @Override public Optional<ClassicCanFrame> receive(int timeoutMs) throws IOException {
            reads++;
            if (closed) {
                return Optional.empty();
            }
            try {
                Optional<ClassicCanFrame> frame = incoming.poll(timeoutMs, TimeUnit.MILLISECONDS);
                return frame == null ? Optional.empty() : frame;
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                throw new IOException(e);
            }
        }

        @Override public void close() {
            closed = true;
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
        int receiveCalls;

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
            receiveCalls++;
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
