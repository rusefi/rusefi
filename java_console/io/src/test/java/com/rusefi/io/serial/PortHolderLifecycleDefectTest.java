package com.rusefi.io.serial;

import com.opensr5.io.DataListener;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.rusefi.Timeouts;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.IniFileProvider;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.LinkManager;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

/** Port opens and their completion callbacks must belong to the current connection attempt. */
class PortHolderLifecycleDefectTest {
    @Test
    void disconnectDuringPortOpenClosesLateStreamWithoutPublishingProtocol() throws Exception {
        LinkManager link = new LinkManager().setNotifyGlobalStatusOnClose(false);
        CountDownLatch opening = new CountDownLatch(1);
        CountDownLatch release = new CountDownLatch(1);
        ConnectionStatusLogic.Listener listener = mock(ConnectionStatusLogic.Listener.class);
        TestStream lateStream = new TestStream();
        StreamConnector oldConnector = new StreamConnector(link, () -> {
            opening.countDown();
            try {
                assertTrue(release.await(5, TimeUnit.SECONDS));
            } catch (InterruptedException e) {
                throw new AssertionError(e);
            }
            return lateStream;
        });
        try {
            link.setConnector(oldConnector);
            oldConnector.connectAndReadConfiguration(new BinaryProtocol.Arguments(false, false), listener);
            assertTrue(opening.await(5, TimeUnit.SECONDS));
            link.close();
            assertFalse(link.isActive());
            // A subsequent connection replaces the connector reachable through LinkManager.
            link.setConnector(new StreamConnector(link, () -> null));
            release.countDown();
            drain(link);
            link.close();

            assertTrue(lateStream.isClosed(), "A cancelled open must release its late stream");
            assertEquals(0, lateStream.writes, "A cancelled open must not start a protocol handshake");
            assertNull(oldConnector.getBinaryProtocol());
            assertNull(link.getBinaryProtocol());
            verifyNoInteractions(listener);
        } finally {
            release.countDown();
            link.COMMUNICATION_EXECUTOR.shutdownNow();
            assertTrue(link.COMMUNICATION_EXECUTOR.awaitTermination(5, TimeUnit.SECONDS));
            oldConnector.stop();
            link.close();
        }
    }

    @Test
    void disconnectBeforeQueuedOpenSkipsFactoryAndAllowsReplacementConnection() throws Exception {
        LinkManager link = new LinkManager().setNotifyGlobalStatusOnClose(false);
        CountDownLatch occupied = new CountDownLatch(1);
        CountDownLatch release = new CountDownLatch(1);
        AtomicInteger oldOpens = new AtomicInteger();
        ConnectionStatusLogic.Listener oldListener = mock(ConnectionStatusLogic.Listener.class);
        ConnectionStatusLogic.Listener replacementListener = mock(ConnectionStatusLogic.Listener.class);
        TestStream replacementStream = new TestStream();
        try {
            link.execute(() -> {
                occupied.countDown();
                await(release);
            });
            assertTrue(occupied.await(5, TimeUnit.SECONDS));
            StreamConnector oldConnector = new StreamConnector(link, () -> {
                oldOpens.incrementAndGet();
                return null;
            });
            link.setConnector(oldConnector);
            oldConnector.connectAndReadConfiguration(new BinaryProtocol.Arguments(false, false), oldListener);
            link.close();

            StreamConnector replacement = new StreamConnector(link, () -> replacementStream);
            link.setConnector(replacement);
            replacement.connectAndReadConfiguration(new BinaryProtocol.Arguments(false, false), replacementListener);
            release.countDown();
            drain(link);

            assertEquals(0, oldOpens.get());
            verifyNoInteractions(oldListener);
            verify(replacementListener).onConnectionFailed(anyString());
            assertEquals(1, replacementStream.writes);
            assertTrue(replacementStream.isClosed(), "A failed handshake must release the published stream");
            assertNull(replacement.getBinaryProtocol());
        } finally {
            release.countDown();
            stop(link);
        }
    }

    @Test
    void cancelledNullFactoryResultDoesNotNotifyAReusedConnectorsListener() throws Exception {
        cancelledFactoryResultDoesNotNotifyAReusedConnectorsListener(false);
    }

    @Test
    void cancelledFactoryExceptionDoesNotNotifyAReusedConnectorsListener() throws Exception {
        cancelledFactoryResultDoesNotNotifyAReusedConnectorsListener(true);
    }

    private void cancelledFactoryResultDoesNotNotifyAReusedConnectorsListener(boolean throwFailure) throws Exception {
        LinkManager link = new LinkManager().setNotifyGlobalStatusOnClose(false);
        CountDownLatch opening = new CountDownLatch(1);
        CountDownLatch release = new CountDownLatch(1);
        AtomicInteger opens = new AtomicInteger();
        ConnectionStatusLogic.Listener oldListener = mock(ConnectionStatusLogic.Listener.class);
        ConnectionStatusLogic.Listener currentListener = mock(ConnectionStatusLogic.Listener.class);
        StreamConnector connector = new StreamConnector(link, () -> {
            if (opens.incrementAndGet() == 1) {
                opening.countDown();
                await(release);
                if (throwFailure) {
                    throw new IllegalStateException("Cancelled factory failed");
                }
            }
            return null;
        });
        try {
            link.setConnector(connector);
            connector.connectAndReadConfiguration(new BinaryProtocol.Arguments(false, false), oldListener);
            assertTrue(opening.await(5, TimeUnit.SECONDS));
            link.close();
            link.setConnector(connector);
            connector.connectAndReadConfiguration(new BinaryProtocol.Arguments(false, false), currentListener);
            release.countDown();
            drain(link);

            assertEquals(2, opens.get());
            verifyNoInteractions(oldListener);
            verify(currentListener).onConnectionFailed("Failed to open port");
            verifyNoMoreInteractions(currentListener);
        } finally {
            release.countDown();
            stop(link);
        }
    }

    @Test
    void disconnectDuringHandshakeClosesProtocolAndSuppressesCompletion() throws Exception {
        LinkManager link = new LinkManager().setNotifyGlobalStatusOnClose(false);
        CountDownLatch writing = new CountDownLatch(1);
        CountDownLatch release = new CountDownLatch(1);
        TestStream stream = new TestStream() {
            @Override
            public void write(byte[] bytes) throws IOException {
                writing.countDown();
                await(release);
                super.write(bytes);
            }
        };
        ConnectionStatusLogic.Listener listener = mock(ConnectionStatusLogic.Listener.class);
        StreamConnector connector = new StreamConnector(link, () -> stream);
        try {
            link.setConnector(connector);
            connector.connectAndReadConfiguration(new BinaryProtocol.Arguments(false, false), listener);
            assertTrue(writing.await(5, TimeUnit.SECONDS));
            assertNotNull(connector.getBinaryProtocol());
            link.close();
            assertTrue(stream.isClosed());
            assertNull(connector.getBinaryProtocol());
            release.countDown();
            drain(link);
            verifyNoInteractions(listener);
        } finally {
            release.countDown();
            stop(link);
        }
    }

    private static void drain(LinkManager link) throws Exception {
        // FIFO barrier observes completion without relying on a callback from a cancelled attempt.
        link.submit(() -> { }).get(5, TimeUnit.SECONDS);
    }

    @Test
    void closedConnectorCanBeReusedForSuccessfulConnections() throws Exception {
        LinkManager link = new LinkManager().setNotifyGlobalStatusOnClose(false).setNeedPullData(false);
        IniFileProvider previousProvider = BinaryProtocol.iniFileProvider;
        IniFileModel ini = mock(IniFileModel.class);
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(ini.getMetaInfo()).thenReturn(meta);
        when(meta.getPageReadCommand(0)).thenReturn("R");
        when(meta.getPageSize(0)).thenReturn(4);
        IncomingDataBuffer buffer = mock(IncomingDataBuffer.class);
        when(buffer.getPacket(Timeouts.BINARY_IO_TIMEOUT, "[hello]"))
            .thenReturn("\u0000lifecycle-test\u0000".getBytes(StandardCharsets.US_ASCII));
        List<AbstractIoStream> streams = new ArrayList<>();
        StreamConnector connector = new StreamConnector(link, () -> {
            AbstractIoStream stream = new AbstractIoStream() {
                @Override
                public void setInputListener(DataListener listener) { }

                @Override
                public IncomingDataBuffer getDataBuffer() {
                    return buffer;
                }
            };
            streams.add(stream);
            return stream;
        });
        int before = SensorCentral.getInstance().getSensorListenerCountForTests();
        try {
            BinaryProtocol.iniFileProvider = signature -> ini;
            for (int cycle = 0; cycle < 3; cycle++) {
                ConnectionStatusLogic.Listener listener = mock(ConnectionStatusLogic.Listener.class);
                link.setConnector(connector);
                connector.connectAndReadConfiguration(new BinaryProtocol.Arguments(false, false), listener);
                drain(link);
                verify(listener).onConnectionEstablished();
                verifyNoMoreInteractions(listener);
                assertNotNull(connector.getBinaryProtocol());
                assertFalse(connector.getBinaryProtocol().isClosed());
                assertEquals(before + 1, SensorCentral.getInstance().getSensorListenerCountForTests());
                link.close();
                assertTrue(streams.get(cycle).isClosed());
                assertNull(connector.getBinaryProtocol());
                assertEquals(before, SensorCentral.getInstance().getSensorListenerCountForTests());
            }
        } finally {
            try {
                stop(link);
            } finally {
                BinaryProtocol.iniFileProvider = previousProvider;
            }
        }
    }

    private static void await(CountDownLatch latch) {
        try {
            assertTrue(latch.await(5, TimeUnit.SECONDS));
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            throw new AssertionError(e);
        }
    }

    private static void stop(LinkManager link) throws InterruptedException {
        link.close();
        link.COMMUNICATION_EXECUTOR.shutdownNow();
        assertTrue(link.COMMUNICATION_EXECUTOR.awaitTermination(5, TimeUnit.SECONDS));
    }

    private static class TestStream extends AbstractIoStream {
        private final IncomingDataBuffer buffer = new IncomingDataBuffer("late open test", getStreamStats());
        int writes;

        @Override
        public void write(byte[] bytes) throws IOException {
            writes++;
            throw new IOException("Scripted HELLO failure; no hardware or timeout");
        }

        @Override
        public void setInputListener(DataListener listener) { }

        @Override
        public IncomingDataBuffer getDataBuffer() {
            return buffer;
        }
    }
}
