package com.rusefi.io.can;

import org.junit.jupiter.api.Test;
import peak.can.MutableInteger;
import peak.can.basic.TPCANBaudrate;
import peak.can.basic.TPCANHandle;
import peak.can.basic.TPCANMessageType;
import peak.can.basic.TPCANMsg;
import peak.can.basic.TPCANParameter;
import peak.can.basic.TPCANParameterValue;
import peak.can.basic.TPCANStatus;

import java.io.IOException;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Deque;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

class PCanRawPortTest {
    private static final TPCANHandle CHANNEL = TPCANHandle.PCAN_USBBUS1;

    @Test
    void openInitializes500kChannelAndOpensNativeReceiveFilter() throws IOException {
        FakeApi api = new FakeApi();
        PCanRawPort port = port(api, new FakeClock());

        port.open(new CanAddress(0x107e1, true));

        assertEquals(1, api.initializeApiCalls);
        assertEquals(1, api.initializeCalls);
        assertEquals(CHANNEL, api.lastChannel);
        assertEquals(TPCANBaudrate.PCAN_BAUD_500K, api.lastBitrate);
        assertEquals(1, api.setValueCalls);
        assertEquals(TPCANParameter.PCAN_MESSAGE_FILTER, api.setValueParameter);
        assertEquals(TPCANParameterValue.PCAN_FILTER_OPEN.getValue(), api.setValueValue);
        assertEquals(Integer.BYTES, api.setValueBufferLength);
        assertThrows(IOException.class, () -> port.open(new CanAddress(0x123, false)));
        port.close();
    }

    @Test
    void receiveIgnoresUnrelatedFrameAndTimesOut() throws IOException {
        FakeApi api = new FakeApi();
        api.reads.add(ReadResult.frame(0x123, TPCANMessageType.PCAN_MESSAGE_STANDARD, new byte[]{9}));
        FakeClock clock = new FakeClock();
        PCanRawPort port = openPort(api, clock);

        assertEquals(Optional.empty(), port.receive(20));

        assertEquals(22, api.readCalls);
        assertEquals(20, clock.sleeps.size());
        port.close();
    }

    @Test
    void receiveIgnoresStandardFrameWithSameIdAsRequestedExtendedAddress() throws IOException {
        FakeApi api = new FakeApi();
        api.reads.add(ReadResult.frame(0x720, TPCANMessageType.PCAN_MESSAGE_STANDARD, new byte[]{7}));
        FakeClock clock = new FakeClock();
        PCanRawPort port = port(api, clock);
        port.open(new CanAddress(0x720, true));

        assertEquals(Optional.empty(), port.receive(1));

        assertEquals(3, api.readCalls);
        assertEquals(1, clock.sleeps.size());
        port.close();
    }

    @Test
    void receiveReturnsEmptyAtDeadlineUnderContinuousUnrelatedTraffic() throws IOException {
        FakeApi api = new FakeApi();
        FakeClock clock = new FakeClock();
        api.onRead = () -> clock.nanos += TimeUnit.MILLISECONDS.toNanos(1);
        api.reads.add(ReadResult.frame(0x123, TPCANMessageType.PCAN_MESSAGE_STANDARD, new byte[]{1}));
        api.reads.add(ReadResult.frame(0x124, TPCANMessageType.PCAN_MESSAGE_STANDARD, new byte[]{2}));
        api.reads.add(ReadResult.frame(0x125, TPCANMessageType.PCAN_MESSAGE_STANDARD, new byte[]{3}));
        PCanRawPort port = openPort(api, clock);

        assertEquals(Optional.empty(), port.receive(3));

        assertEquals(3, api.readCalls);
        assertEquals(TimeUnit.MILLISECONDS.toNanos(3), clock.nanos);
        assertTrue(clock.sleeps.isEmpty());
        port.close();
    }

    @Test
    void receiveSkipsNoiseAndReturnsMatchingExtendedFrame() throws IOException {
        FakeApi api = new FakeApi();
        api.reads.add(ReadResult.frame(0x123, TPCANMessageType.PCAN_MESSAGE_STANDARD, new byte[]{9}));
        api.reads.add(ReadResult.frame(0x107e1, TPCANMessageType.PCAN_MESSAGE_EXTENDED, new byte[]{1, 2}));
        PCanRawPort port = openPort(api, new FakeClock());

        ClassicCanFrame frame = port.receive(20).orElseThrow(AssertionError::new);

        assertEquals(new CanAddress(0x107e1, true), frame.getAddress());
        assertArrayEquals(new byte[]{1, 2}, frame.getPayload());
        assertEquals(2, api.readCalls);
        port.close();
    }

    @Test
    void receiveSkipsMalformedUnrelatedFrameBeforeReturningMatchingFrame() throws IOException {
        FakeApi api = new FakeApi();
        api.reads.add(ReadResult.frame(0x123, TPCANMessageType.PCAN_MESSAGE_STANDARD, new byte[9]));
        api.reads.add(ReadResult.frame(0x107e1, TPCANMessageType.PCAN_MESSAGE_EXTENDED, new byte[]{1}));
        PCanRawPort port = openPort(api, new FakeClock());

        ClassicCanFrame frame = port.receive(20).orElseThrow(AssertionError::new);

        assertEquals(new CanAddress(0x107e1, true), frame.getAddress());
        assertArrayEquals(new byte[]{1}, frame.getPayload());
        assertEquals(2, api.readCalls);
        port.close();
    }

    @Test
    void receiveIgnoresExtendedFrameWithSameIdAsRequestedStandardAddress() throws IOException {
        FakeApi api = new FakeApi();
        api.reads.add(ReadResult.frame(0x720, TPCANMessageType.PCAN_MESSAGE_EXTENDED, new byte[]{7}));
        FakeClock clock = new FakeClock();
        PCanRawPort port = port(api, clock);
        port.open(new CanAddress(0x720, false));

        assertEquals(Optional.empty(), port.receive(1));

        assertEquals(3, api.readCalls);
        assertEquals(1, clock.sleeps.size());
        port.close();
    }

    @Test
    void failedFilterReleasesInitializedChannel() throws IOException {
        FakeApi api = new FakeApi();
        api.setValueStatus = TPCANStatus.PCAN_ERROR_ILLPARAMVAL;
        PCanRawPort port = port(api, new FakeClock());

        IOException failure = assertThrows(IOException.class,
            () -> port.open(new CanAddress(0x107e1, true)));

        assertTrue(failure.getMessage().contains("configure receive filter"));
        assertEquals(1, api.uninitializeCalls);
        assertThrows(IOException.class,
            () -> port.send(new ClassicCanFrame(new CanAddress(0x10667, true), new byte[]{1})));

        FakeApi replacementApi = new FakeApi();
        PCanRawPort replacementPort = port(replacementApi, new FakeClock());
        replacementPort.open(new CanAddress(0x720, false));
        assertEquals(1, replacementApi.initializeCalls);
        assertEquals(1, replacementApi.setValueCalls);
        replacementPort.close();
    }

    @Test
    void failedChannelInitializationDoesNotUninitializeAnUnownedChannel() {
        FakeApi api = new FakeApi();
        api.initializeStatus = TPCANStatus.PCAN_ERROR_INITIALIZE;
        PCanRawPort port = port(api, new FakeClock());

        IOException failure = assertThrows(IOException.class,
            () -> port.open(new CanAddress(0x107e1, true)));

        assertTrue(failure.getMessage().contains("initialize channel"));
        assertEquals(0, api.setValueCalls);
        assertEquals(0, api.uninitializeCalls);
    }

    @Test
    void sendMapsExtendedAndStandardFramesWithoutRetry() throws IOException {
        FakeApi api = new FakeApi();
        PCanRawPort port = openPort(api, new FakeClock());

        port.send(new ClassicCanFrame(new CanAddress(0x10667, true), new byte[]{1, 2}));
        port.send(new ClassicCanFrame(new CanAddress(0x321, false), new byte[]{3}));

        assertEquals(2, api.writes.size());
        assertMessage(api.writes.get(0), 0x10667, TPCANMessageType.PCAN_MESSAGE_EXTENDED, new byte[]{1, 2});
        assertMessage(api.writes.get(1), 0x321, TPCANMessageType.PCAN_MESSAGE_STANDARD, new byte[]{3});

        api.writeStatus = TPCANStatus.PCAN_ERROR_QXMTFULL;
        assertThrows(IOException.class,
            () -> port.send(new ClassicCanFrame(new CanAddress(0x10667, true), new byte[]{4})));
        assertEquals(3, api.writes.size(), "a rejected PCAN write must not be retried");
        port.close();
    }

    @Test
    void receivePollsEmptyQueueAndMapsExtendedFrame() throws IOException {
        FakeApi api = new FakeApi();
        api.reads.add(ReadResult.empty());
        api.reads.add(ReadResult.frame(0x107e1, TPCANMessageType.PCAN_MESSAGE_EXTENDED, new byte[]{1, 2, 3}));
        FakeClock clock = new FakeClock();
        PCanRawPort port = openPort(api, clock);

        ClassicCanFrame frame = port.receive(20).orElseThrow(AssertionError::new);

        assertEquals(new CanAddress(0x107e1, true), frame.getAddress());
        assertArrayEquals(new byte[]{1, 2, 3}, frame.getPayload());
        assertEquals(Arrays.asList(1L), clock.sleeps);
        port.close();
    }

    @Test
    void receiveReturnsEmptyAtDeadline() throws IOException {
        FakeApi api = new FakeApi();
        FakeClock clock = new FakeClock();
        PCanRawPort port = openPort(api, clock);

        assertEquals(Optional.empty(), port.receive(3));
        assertEquals(Arrays.asList(1L, 1L, 1L), clock.sleeps);
        port.close();
    }

    @Test
    void receiveRejectsRemoteAndOversizedFrames() throws IOException {
        FakeApi remoteApi = new FakeApi();
        byte extendedRemoteType = (byte) (TPCANMessageType.PCAN_MESSAGE_RTR.getValue()
            | TPCANMessageType.PCAN_MESSAGE_EXTENDED.getValue());
        remoteApi.reads.add(ReadResult.frame(0x107e1, extendedRemoteType, new byte[0]));
        PCanRawPort remotePort = openPort(remoteApi, new FakeClock());
        assertThrows(IOException.class, () -> remotePort.receive(0));
        remotePort.close();

        FakeApi oversizedApi = new FakeApi();
        oversizedApi.reads.add(ReadResult.frame(0x107e1, TPCANMessageType.PCAN_MESSAGE_EXTENDED, new byte[9]));
        PCanRawPort oversizedPort = openPort(oversizedApi, new FakeClock());
        assertThrows(IOException.class, () -> oversizedPort.receive(0));
        oversizedPort.close();
    }

    @Test
    void receivePropagatesNonEmptyQueueErrorsWithoutRetry() throws IOException {
        FakeApi api = new FakeApi();
        api.reads.add(ReadResult.error(TPCANStatus.PCAN_ERROR_BUSOFF));
        PCanRawPort port = openPort(api, new FakeClock());

        IOException failure = assertThrows(IOException.class, () -> port.receive(20));

        assertTrue(failure.getMessage().contains("PCAN_ERROR_BUSOFF"));
        assertEquals(1, api.readCalls);
        port.close();
    }

    @Test
    void closeIsIdempotentAndClearsPortAfterNativeFailure() throws IOException {
        FakeApi api = new FakeApi();
        PCanRawPort port = openPort(api, new FakeClock());
        api.uninitializeStatus = TPCANStatus.PCAN_ERROR_INITIALIZE;

        assertThrows(IOException.class, port::close);
        port.close();

        assertEquals(1, api.uninitializeCalls);
        assertThrows(IOException.class, () -> port.receive(0));

        FakeApi replacementApi = new FakeApi();
        PCanRawPort replacementPort = openPort(replacementApi, new FakeClock());
        assertEquals(1, replacementApi.initializeCalls,
            "a native close failure must still release process ownership of the channel");
        replacementPort.close();
    }

    @Test
    void closeWaitsForInFlightReadBeforeUninitializingChannel() throws Exception {
        FakeApi api = new FakeApi();
        api.blockRead = true;
        PCanRawPort port = openPort(api, new FakeClock());
        AtomicReference<Throwable> readFailure = new AtomicReference<>();
        AtomicReference<Throwable> closeFailure = new AtomicReference<>();

        Thread reader = new Thread(() -> {
            try {
                port.receive(0);
            } catch (Throwable e) {
                readFailure.set(e);
            }
        });
        reader.start();
        assertTrue(api.readStarted.await(1, TimeUnit.SECONDS));

        Thread closer = new Thread(() -> {
            try {
                port.close();
            } catch (Throwable e) {
                closeFailure.set(e);
            }
        });
        closer.start();
        try {
            waitForState(closer, Thread.State.BLOCKED);
            assertEquals(0, api.uninitializeCalls);
        } finally {
            api.releaseRead.countDown();
            reader.join(1_000);
            closer.join(1_000);
        }

        assertFalse(reader.isAlive());
        assertFalse(closer.isAlive());
        assertNull(readFailure.get());
        assertNull(closeFailure.get());
        assertEquals(1, api.uninitializeCalls);
    }

    /** Prevent scanner/connection/flasher instances from invalidating each other's native channel. */
    @Test
    void separatePortInstancesCannotInitializeTheSameChannelConcurrently() throws IOException {
        FakeApi connectionApi = new FakeApi();
        FakeApi scannerApi = new FakeApi();
        PCanRawPort connectionPort = port(connectionApi, new FakeClock());
        PCanRawPort scannerPort = port(scannerApi, new FakeClock());

        connectionPort.open(new CanAddress(0x720, false));
        IOException failure = assertThrows(IOException.class,
            () -> scannerPort.open(new CanAddress(0x720, false)));

        assertTrue(failure.getMessage().contains("already in use by this process"));
        assertEquals(1, connectionApi.initializeCalls);
        assertEquals(0, scannerApi.initializeCalls);

        connectionPort.close();

        scannerPort.open(new CanAddress(0x720, false));
        assertEquals(1, scannerApi.initializeCalls);
        scannerPort.close();
    }

    private static PCanRawPort port(FakeApi api, FakeClock clock) {
        return new PCanRawPort(CHANNEL, () -> api, clock);
    }

    private static PCanRawPort openPort(FakeApi api, FakeClock clock) throws IOException {
        PCanRawPort port = port(api, clock);
        port.open(new CanAddress(0x107e1, true));
        return port;
    }

    private static void assertMessage(TPCANMsg message, int id, TPCANMessageType type, byte[] payload) {
        assertEquals(id, message.getID());
        assertEquals(type.getValue(), message.getType());
        assertEquals(payload.length, Byte.toUnsignedInt(message.getLength()));
        assertArrayEquals(payload, message.getData());
    }

    private static void waitForState(Thread thread, Thread.State expected) {
        long deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(1);
        while (thread.getState() != expected && System.nanoTime() < deadline) {
            Thread.yield();
        }
        assertEquals(expected, thread.getState());
    }

    private static class FakeClock implements PCanClock {
        long nanos;
        final List<Long> sleeps = new ArrayList<>();

        @Override
        public long nanoTime() {
            return nanos;
        }

        @Override
        public void sleep(long millis) {
            sleeps.add(millis);
            nanos += millis * 1_000_000;
        }
    }

    private static class FakeApi implements PCanApi {
        boolean apiAvailable = true;
        TPCANStatus initializeStatus = TPCANStatus.PCAN_ERROR_OK;
        TPCANStatus setValueStatus = TPCANStatus.PCAN_ERROR_OK;
        TPCANStatus writeStatus = TPCANStatus.PCAN_ERROR_OK;
        TPCANStatus uninitializeStatus = TPCANStatus.PCAN_ERROR_OK;
        final Deque<ReadResult> reads = new ArrayDeque<>();
        final List<TPCANMsg> writes = new ArrayList<>();
        int initializeApiCalls;
        int initializeCalls;
        int setValueCalls;
        int readCalls;
        int uninitializeCalls;
        TPCANHandle lastChannel;
        TPCANBaudrate lastBitrate;
        TPCANParameter setValueParameter;
        int setValueValue;
        int setValueBufferLength;
        boolean blockRead;
        Runnable onRead = () -> { };
        final CountDownLatch readStarted = new CountDownLatch(1);
        final CountDownLatch releaseRead = new CountDownLatch(1);

        @Override
        public boolean initializeApi() {
            initializeApiCalls++;
            return apiAvailable;
        }

        @Override
        public TPCANStatus initialize(TPCANHandle channel, TPCANBaudrate bitrate) {
            initializeCalls++;
            lastChannel = channel;
            lastBitrate = bitrate;
            return initializeStatus;
        }

        @Override
        public TPCANStatus setValue(TPCANHandle channel, TPCANParameter parameter,
                                    MutableInteger value, int bufferLength) {
            setValueCalls++;
            lastChannel = channel;
            setValueParameter = parameter;
            setValueValue = value.getValue();
            setValueBufferLength = bufferLength;
            return setValueStatus;
        }

        @Override
        public TPCANStatus write(TPCANHandle channel, TPCANMsg message) {
            lastChannel = channel;
            writes.add((TPCANMsg) message.clone());
            return writeStatus;
        }

        @Override
        public TPCANStatus read(TPCANHandle channel, TPCANMsg message) {
            readCalls++;
            onRead.run();
            lastChannel = channel;
            if (blockRead) {
                readStarted.countDown();
                try {
                    releaseRead.await();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    return TPCANStatus.PCAN_ERROR_UNKNOWN;
                }
            }
            ReadResult result = reads.isEmpty() ? ReadResult.empty() : reads.removeFirst();
            if (result.status == TPCANStatus.PCAN_ERROR_OK) {
                message.setID(result.id);
                message.setType(result.type);
                message.setData(result.payload, (byte) result.payload.length);
            }
            return result.status;
        }

        @Override
        public TPCANStatus uninitialize(TPCANHandle channel) {
            lastChannel = channel;
            uninitializeCalls++;
            return uninitializeStatus;
        }
    }

    private static class ReadResult {
        final TPCANStatus status;
        final int id;
        final byte type;
        final byte[] payload;

        private ReadResult(TPCANStatus status, int id, byte type, byte[] payload) {
            this.status = status;
            this.id = id;
            this.type = type;
            this.payload = payload;
        }

        static ReadResult empty() {
            return new ReadResult(TPCANStatus.PCAN_ERROR_QRCVEMPTY, 0, (byte) 0, new byte[0]);
        }

        static ReadResult error(TPCANStatus status) {
            return new ReadResult(status, 0, (byte) 0, new byte[0]);
        }

        static ReadResult frame(int id, TPCANMessageType type, byte[] payload) {
            return frame(id, type.getValue(), payload);
        }

        static ReadResult frame(int id, byte type, byte[] payload) {
            return new ReadResult(TPCANStatus.PCAN_ERROR_OK, id, type, payload);
        }
    }
}
