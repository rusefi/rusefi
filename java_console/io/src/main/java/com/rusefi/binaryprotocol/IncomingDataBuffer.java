package com.rusefi.binaryprotocol;

import com.devexperts.logging.Logging;
import com.rusefi.Timeouts;
import com.rusefi.binaryprotocol.test.Bug3923;
import com.rusefi.io.IoStream;
import com.rusefi.io.serial.AbstractIoStream;
import etch.util.CircularByteBuffer;
import net.jcip.annotations.ThreadSafe;

import java.io.EOFException;
import java.io.IOException;
import java.util.Arrays;
import java.util.Objects;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.binaryprotocol.IoHelper.*;

/**
 * Thread-safe byte queue with blocking {@link #waitForBytes} method
 * <p>
 * Andrey Belomutskiy, (c) 2013-2020
 * 6/20/2015.
 */
@ThreadSafe
public class IncomingDataBuffer {
    private static final Logging log = getLogging(IncomingDataBuffer.class);

    static {
        log.configureDebugEnabled(false);
    }

    private static final int BUFFER_SIZE = 32768;
    private final String loggingPrefix;

    /**
     * buffer for queued response bytes from controller
     */
    private final CircularByteBuffer cbb = new CircularByteBuffer(BUFFER_SIZE);
    private final AbstractIoStream.StreamStats streamStats;

    public IncomingDataBuffer(String loggingPrefix, AbstractIoStream.StreamStats streamStats) {
        this.loggingPrefix = loggingPrefix;
        this.streamStats = Objects.requireNonNull(streamStats, "streamStats");
    }

    public byte[] getPacket(String msg) throws EOFException {
        return getPacket(msg, System.currentTimeMillis());
    }

    /**
     * why does this method return NULL in case of timeout?!
     * todo: there is a very similar BinaryProtocolServer#readPromisedBytes which throws exception in case of timeout
     */
    public byte[] getPacket(String msg, long start) throws EOFException {
        boolean isTimeout = waitForBytes(msg + " header", start, 2);
        if (isTimeout) {
            if (Bug3923.obscene)
                log.info("Timeout waiting for header");
            return null;
        }

        int packetSize = swap16(getShort());
        // if (log.debugEnabled())
        //     log.debug(loggingPrefix + "Got packet size " + packetSize);
        if (packetSize < 0)
            return null;

        isTimeout = waitForBytes(loggingPrefix + msg + " body", start, packetSize + 4);
        if (isTimeout)
            return null;

        byte[] packet = new byte[packetSize];
        getData(packet);

        // Compare the sent and computed CRCs, make sure they match!
        int packetCrc = swap32(getInt());
        int actualCrc = getCrc32(packet);
        if (actualCrc != packetCrc) {
            String errorMessage = String.format("CRC mismatch on recv packet for %s: got %x but expected %x", msg, actualCrc, packetCrc);
            System.out.println(errorMessage);
            log.warn(errorMessage);
            return null;
        }
        if (Bug3923.obscene && packet.length < 10)
            log.info("got packet: " + Arrays.toString(packet));

        onPacketArrived();
        // if (log.debugEnabled())
        //     log.trace("packet arrived: " + Arrays.toString(packet) + ": crc OK");

        return packet;
    }

    public void onPacketArrived() {
        streamStats.onPacketArrived();
    }

    public void addData(byte[] freshData) {
        synchronized (cbb) {
            if (cbb.size() - cbb.length() < freshData.length) {
                log.error("buffer overflow not expected");
                throw new IllegalStateException("buffer overflow not expected");
            }
            cbb.put(freshData);
            cbb.notifyAll();
        }
        if (log.debugEnabled() || Bug3923.obscene)
            log.info(freshData.length + " byte(s) arrived, total " + cbb.length());
    }

    /**
     * Blocking method which would wait for specified amount of data
     *
     * @return true in case of timeout, false if everything is fine
     */
    public boolean waitForBytes(String loggingMessage, long startTimestamp, int count) {
        return waitForBytes(Timeouts.BINARY_IO_TIMEOUT, loggingMessage, startTimestamp, count);
    }

    /**
     * @return true in case of timeout, false if we have received count of bytes
     */
    public boolean waitForBytes(int timeoutMs, String loggingMessage, long startTimestamp, int count) {
        //log.info(loggingMessage + ": waiting for " + count + " byte(s)");
        synchronized (cbb) {
            while (cbb.length() < count) {
                int timeout = (int) (startTimestamp + timeoutMs - System.currentTimeMillis());
                if (timeout <= 0) {
                    log.info(loggingMessage + ": timeout " + timeoutMs + "ms. Got only " + cbb.length() + "byte(s) while expecting " + count);
                    return true; // timeout. Sad face.
                }
                try {
                    cbb.wait(timeout);
                } catch (InterruptedException e) {
                    return true; // thread thrown away, handling like a timeout
                }
            }
        }
        return false; // looks good!
    }

    public int getPendingCount() {
        synchronized (cbb) {
            return cbb.length();
        }
    }

    public int dropPending() {
        // todo: when exactly do we need this logic?
        synchronized (cbb) {
            int pending = cbb.length();
            if (pending > 0) {
                log.error("dropPending: Unexpected pending data: " + pending + " byte(s)");
                byte[] bytes = new byte[pending];
                cbb.get(bytes);
                log.error("DROPPED FROM BUFFER: " + IoStream.printByteArray(bytes));
            }
            return pending;
        }
    }

    public int getByte() throws EOFException {
        streamStats.onArrived(1);
        synchronized (cbb) {
            return cbb.getByte();
        }
    }

    public int getShort() throws EOFException {
        streamStats.onArrived(2);
        synchronized (cbb) {
            int result = cbb.getShort();
            if (log.debugEnabled() || Bug3923.obscene)
                log.info("Consumed short, " + cbb.length() + " remaining");
            return result;
        }
    }

    public int getInt() throws EOFException {
        streamStats.onArrived(4);
        synchronized (cbb) {
            int result = cbb.getInt();
            if (log.debugEnabled() || Bug3923.obscene)
                log.info("Consumed int, " + cbb.length() + " remaining");
            return result;
        }
    }

    public void getData(byte[] packet) {
        synchronized (cbb) {
            cbb.get(packet);
            if (log.debugEnabled() || Bug3923.obscene)
                log.info(packet.length + " consumed, " + cbb.length() + " remaining");
        }
        streamStats.onArrived(packet.length);
    }

    public byte readByte() throws IOException {
        return readByte(Timeouts.BINARY_IO_TIMEOUT);
    }

    public byte readByte(int timeoutMs) throws IOException {
        boolean isTimeout = waitForBytes(timeoutMs, loggingPrefix + "readByte", System.currentTimeMillis(), 1);
        if (isTimeout)
            throw new EOFException("Timeout in readByte " + timeoutMs);
        return (byte) getByte();
    }

    public int readInt() throws EOFException {
        boolean isTimeout = waitForBytes(loggingPrefix + "readInt", System.currentTimeMillis(), 4);
        if (isTimeout)
            throw new EOFException("Timeout in readInt ");
        return swap32(getInt());
    }

    public short readShort() throws EOFException {
        boolean isTimeout = waitForBytes(loggingPrefix + "readShort", System.currentTimeMillis(), 2);
        if (isTimeout)
            throw new EOFException("Timeout in readShort");
        return (short) swap16(getShort());
    }

    public void read(byte[] packet) throws EOFException {
        boolean isTimeout = waitForBytes(loggingPrefix + "read", System.currentTimeMillis(), packet.length);
        if (isTimeout)
            throw new EOFException("Timeout while waiting for " + packet.length + " byte(s)");
        getData(packet);
    }
}
