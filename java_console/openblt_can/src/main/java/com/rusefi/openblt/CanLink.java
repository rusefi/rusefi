package com.rusefi.openblt;

import java.io.IOException;

/**
 * Minimal CAN abstraction so the XCP client can run against a real PCAN/MacCAN
 * adapter and against an in-memory bootloader simulation in unit tests.
 */
public interface CanLink extends AutoCloseable {

    /** Opens the channel. Throws if the adapter or driver is not available. */
    void open() throws IOException;

    /** Sends a frame. Throws if the controller reports an error (e.g. bus-off). */
    void write(CanFrame frame) throws IOException;

    /**
     * Receives one frame, waiting at most timeoutMs milliseconds.
     *
     * @return the next frame, or null when the timeout expired. Implementations
     *         must be non-blocking-friendly: MacCAN's Read returns immediately
     *         when the receive queue is empty.
     */
    CanFrame readFrame(int timeoutMs) throws IOException;

    @Override
    void close();
}
