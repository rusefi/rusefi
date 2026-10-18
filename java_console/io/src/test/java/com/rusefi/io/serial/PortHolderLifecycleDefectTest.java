package com.rusefi.io.serial;

import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.LinkManager;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static org.junit.jupiter.api.Assertions.*;

/** Passing characterization: disconnect misses an in-flight port open. Update expectations with the fix. */
class PortHolderLifecycleDefectTest {
    @Test
    void disconnectDuringPortOpenLeavesLateStreamOpenEvenAfterReplacementIsClosed() throws Exception {
        LinkManager link = new LinkManager().setNotifyGlobalStatusOnClose(false);
        CountDownLatch opening = new CountDownLatch(1);
        CountDownLatch release = new CountDownLatch(1);
        CountDownLatch failed = new CountDownLatch(1);
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
            oldConnector.connectAndReadConfiguration(new BinaryProtocol.Arguments(false, false),
                new ConnectionStatusLogic.Listener() {
                    @Override
                    public void onConnectionStatus(boolean connected) { }

                    @Override
                    public void onConnectionFailed(String message) {
                        failed.countDown();
                    }
                });
            assertTrue(opening.await(5, TimeUnit.SECONDS));
            link.close();
            assertFalse(link.isActive());
            // A subsequent connection replaces the connector reachable through LinkManager.
            link.setConnector(new StreamConnector(link, () -> null));
            release.countDown();
            assertTrue(failed.await(5, TimeUnit.SECONDS));
            link.close();

            assertFalse(lateStream.isClosed(), "Current defect: disconnect misses a stream still being opened");
            assertNotNull(oldConnector.getBinaryProtocol(), "The discarded connector publishes a late protocol");
            assertFalse(oldConnector.getBinaryProtocol().isClosed());
            assertNull(link.getBinaryProtocol(), "Closing the replacement cannot reach the abandoned stream");
        } finally {
            release.countDown();
            link.COMMUNICATION_EXECUTOR.shutdownNow();
            assertTrue(link.COMMUNICATION_EXECUTOR.awaitTermination(5, TimeUnit.SECONDS));
            oldConnector.stop(); // Explicitly clean up the defect, including its JVM shutdown hook.
            link.close();
        }
    }

    private static class TestStream extends AbstractIoStream {
        private final IncomingDataBuffer buffer = new IncomingDataBuffer("late open test", getStreamStats());

        @Override
        public void write(byte[] bytes) throws IOException {
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
