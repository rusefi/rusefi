package com.rusefi.binaryprotocol;

import com.opensr5.Logger;
import com.rusefi.Timeouts;
import etch.util.CircularByteBuffer;
import net.jcip.annotations.ThreadSafe;

import java.io.EOFException;
import java.util.Arrays;

/**
 * Thread-safe byte queue with blocking {@link #waitForBytes} method
 *
 * (c) Andrey Belomutskiy
 * 6/20/2015.
 */
@ThreadSafe
public class IncomingDataBuffer {
    private static final int BUFFER_SIZE = 32768;
    /**
     * buffer for response bytes from controller
     */
    private final CircularByteBuffer cbb;
    private final Logger logger;

    public IncomingDataBuffer(Logger logger) {
        this.cbb = new CircularByteBuffer(BUFFER_SIZE);
        this.logger = logger;
    }

    public void addData(byte[] freshData) {
        logger.info(freshData.length + " byte(s) arrived");
        synchronized (cbb) {
            if (cbb.size() - cbb.length() < freshData.length) {
                logger.error("IncomingDataBuffer: buffer overflow not expected");
                cbb.clear();
            }
            cbb.put(freshData);
            cbb.notifyAll();
        }
    }

    /**
     * Blocking method which would wait for specified amount of data
     *
     * @return true in case of timeout, false if everything is fine
     */
    public boolean waitForBytes(String loggingMessage, long startTimestamp, int count) throws InterruptedException {
        logger.info(loggingMessage + ": waiting for " + count + " byte(s)");
        synchronized (cbb) {
            while (cbb.length() < count) {
                int timeout = (int) (startTimestamp + Timeouts.BINARY_IO_TIMEOUT - System.currentTimeMillis());
                if (timeout <= 0) {
                    logger.info(loggingMessage + ": timeout. Got only " + cbb.length());
                    return true; // timeout. Sad face.
                }
                cbb.wait(timeout);
            }
        }
        return false; // looks good!
    }

    public void dropPending() {
        synchronized (cbb) {
            int pending = cbb.length();
            if (pending > 0) {
                logger.error("dropPending: Unexpected pending data: " + pending + " byte(s)");
                byte[] bytes = new byte[pending];
                cbb.get(bytes);
                logger.error("data: " + Arrays.toString(bytes));
            }
        }
    }

    public int getShort() throws EOFException {
        synchronized (cbb) {
            return cbb.getShort();
        }
    }

    public int getInt() throws EOFException {
        synchronized (cbb) {
            return cbb.getInt();
        }
    }

    public void getData(byte[] packet) {
        synchronized (cbb) {
            cbb.get(packet);
        }
    }
}
