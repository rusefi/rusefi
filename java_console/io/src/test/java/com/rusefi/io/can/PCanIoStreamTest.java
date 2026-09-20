package com.rusefi.io.can;

import org.junit.jupiter.api.Test;
import peak.can.basic.TPCANMsg;
import peak.can.basic.TPCANStatus;

import java.util.ArrayList;
import java.util.List;

import static com.rusefi.config.generated.VariableRegistryValues.CAN_ECU_SERIAL_TX_ID;
import static org.junit.jupiter.api.Assertions.*;

class PCanIoStreamTest {
    private static class FakeDriver implements PCanIoStream.Driver {
        boolean claimed = true;
        int releases;
        int reads;
        Runnable onRead = () -> {};

        @Override
        public TPCANStatus read(TPCANMsg message) {
            reads++;
            onRead.run();
            return TPCANStatus.PCAN_ERROR_QRCVEMPTY;
        }

        @Override
        public TPCANStatus write(int id, byte[] payload) {
            fail("No writes expected in this scenario");
            return TPCANStatus.PCAN_ERROR_UNKNOWN;
        }

        @Override
        public TPCANStatus uninitialize() {
            releases++;
            claimed = false;
            return TPCANStatus.PCAN_ERROR_OK;
        }

        boolean tryClaim() {
            if (claimed) {
                return false;
            }
            claimed = true;
            return true;
        }
    }

    private static class Harness {
        final List<Runnable> readers = new ArrayList<>();
        final List<Long> waits = new ArrayList<>();
        final PCanIoStream stream;

        Harness(FakeDriver driver) {
            stream = new PCanIoStream(driver, message -> {}, () -> readers::add, waits::add);
        }
    }

    @Test
    void closeReleasesChannelOnceAndReconnectSucceeds() {
        FakeDriver driver = new FakeDriver();
        Harness harness = new Harness(driver);
        List<String> notifications = new ArrayList<>();
        harness.stream.addCloseListener(() -> {
            assertFalse(driver.claimed, "Release the channel before notifying reconnect listeners");
            notifications.add("closed");
        });

        harness.stream.close();
        harness.stream.close();

        assertTrue(harness.stream.isClosed());
        assertEquals(List.of("closed"), notifications);
        assertEquals(1, driver.releases);
        assertFalse(driver.claimed);
        assertTrue(driver.tryClaim(), "Reconnect can claim the released channel");
        harness.stream.close();
        assertEquals(1, driver.releases);
        assertTrue(driver.claimed, "Closing the old stream must not release the new connection");
    }

    @Test
    void emptyQueuePollingRequestsOneMillisecondBackoff() {
        FakeDriver driver = new FakeDriver();
        Harness harness = new Harness(driver);
        driver.onRead = () -> {
            if (driver.reads == 4) {
                harness.stream.close();
            }
        };

        // Run the real loop synchronously: no native library, clocks, or worker threads.
        harness.readers.get(0).run();

        assertEquals(4, driver.reads);
        assertTrue(harness.stream.isClosed());
        assertEquals(List.of(1L, 1L, 1L, 1L), harness.waits);
    }

    @Test
    void interruptedEmptyQueuePollingClosesStream() {
        FakeDriver driver = new FakeDriver();
        List<Runnable> readers = new ArrayList<>();
        PCanIoStream stream = new PCanIoStream(driver, message -> {}, () -> readers::add,
            milliseconds -> { throw new InterruptedException(); });
        driver.onRead = () -> {
            if (driver.reads == 3) {
                stream.close();
            }
        };

        try {
            readers.get(0).run();

            assertTrue(Thread.currentThread().isInterrupted());
            assertTrue(stream.isClosed());
            assertEquals(1, driver.reads);
            assertEquals(1, driver.releases);
        } finally {
            Thread.interrupted();
        }
    }

    @Test
    void successfulReadDeliversPayloadWithoutBackoff() {
        FakeDriver driver = new FakeDriver() {
            @Override
            public TPCANStatus read(TPCANMsg message) {
                reads++;
                message.setID(CAN_ECU_SERIAL_TX_ID);
                message.setData(new byte[]{2, 0x12, 0x34, 0, 0, 0, 0, 0}, (byte) 8);
                onRead.run();
                return TPCANStatus.PCAN_ERROR_OK;
            }
        };
        Harness harness = new Harness(driver);
        List<byte[]> received = new ArrayList<>();
        harness.stream.setInputListener(received::add);
        driver.onRead = harness.stream::close;

        harness.readers.get(1).run();

        assertEquals(1, driver.reads);
        assertEquals(1, received.size());
        assertArrayEquals(new byte[]{0x12, 0x34}, received.get(0));
        assertTrue(harness.waits.isEmpty());
    }

    @Test
    void readerDoesNotTouchDriverAfterStreamIsClosed() {
        FakeDriver driver = new FakeDriver();
        Harness harness = new Harness(driver);
        harness.stream.close();
        harness.readers.get(0).run();
        assertEquals(0, driver.reads);
        assertTrue(harness.waits.isEmpty());
    }
}
