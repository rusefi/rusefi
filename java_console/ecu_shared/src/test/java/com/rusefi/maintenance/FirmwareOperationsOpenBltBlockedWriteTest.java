package com.rusefi.maintenance;

import com.opensr5.io.DataListener;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.io.IoStream;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.updater.OpenbltDetectorStrategy;
import com.rusefi.updater.OpenbltDetectorStrategy.OpenbltInfo;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.FutureTask;
import java.util.concurrent.TimeUnit;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class FirmwareOperationsOpenBltBlockedWriteTest {
    @Test
    void windowsUsbReenumerationInitialConnectWriteKeepsDiscoveryAndCleanupBlockedDespiteInterrupt() throws Exception {
        // Passing characterization of the bad behavior, not the desired timeout policy.
        // During Windows USB re-enumeration, an initial CONNECT can block in the
        // native serial write before banner/features/signature discovery. The fake
        // models only an interruption-resistant write, not the Windows serial driver.
        BlockedConnectStream stream = new BlockedConnectStream();
        FutureTask<List<PortResult>> discovery = new FutureTask<>(() ->
            FirmwareOperations.detectNewOpenBltPorts(
                Collections.singletonList("FAKE_REENUMERATED_PORT"), Collections.emptyList(), port -> {
                    try (IoStream ownedStream = stream) {
                        return OpenbltDetectorStrategy.detectOpenbltWithSignature(ownedStream);
                    } catch (IOException e) {
                        return new OpenbltInfo(false, null);
                    }
                }));
        Thread worker = new Thread(discovery, "fake OpenBLT discovery");

        try {
            worker.start();
            assertTrue(stream.writeEntered.await(5, TimeUnit.SECONDS), "CONNECT write was not reached");
            worker.interrupt();
            assertTrue(stream.interruptIgnored.await(5, TimeUnit.SECONDS), "fake did not observe interruption");

            assertArrayEquals(new byte[]{2, (byte) 0xFF, 0}, stream.initialWrite);
            assertFalse(discovery.isDone(), "the inline discovery cannot return while its write is blocked");
            assertFalse(stream.isClosed(), "try-with-resources has not yet reached port cleanup");
            assertEquals(0, stream.bufferRequests, "response timeouts cannot run before the initial write returns");
        } finally {
            // Always release our own simulated native write, even on assertion failure.
            // Interruption/cancellation alone is intentionally not used as cleanup.
            stream.releaseWrite.countDown();
            worker.join(5_000);
            assertFalse(worker.isAlive(), "test-owned discovery thread must terminate");
        }

        assertTrue(discovery.get().isEmpty(), "a failed CONNECT must not produce a flash candidate");
        assertTrue(stream.isClosed(), "the probe owns cleanup after its write returns");
    }

    private static class BlockedConnectStream extends AbstractIoStream {
        private final CountDownLatch writeEntered = new CountDownLatch(1);
        private final CountDownLatch interruptIgnored = new CountDownLatch(1);
        private final CountDownLatch releaseWrite = new CountDownLatch(1);
        private byte[] initialWrite;
        private int bufferRequests;

        @Override
        public void write(byte[] bytes) throws IOException {
            initialWrite = bytes.clone();
            writeEntered.countDown();
            boolean released = false;
            while (!released) {
                try {
                    releaseWrite.await();
                    released = true;
                } catch (InterruptedException e) {
                    interruptIgnored.countDown();
                }
            }
            throw new IOException("test-controlled CONNECT write failure");
        }

        @Override
        public IncomingDataBuffer getDataBuffer() {
            bufferRequests++;
            throw new AssertionError("CONNECT write failed before response acquisition");
        }

        @Override
        public void setInputListener(DataListener listener) {
        }
    }
}
