package com.rusefi.binaryprotocol;

import com.devexperts.logging.Logging;
import com.rusefi.Timeouts;
import com.rusefi.config.generated.Fields;
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
    private static final Logging log = getLogging(IoStream.class);

    private static final int BUFFER_SIZE = 32768;
    private static String loggingPrefix;
    /**
     * buffer for response bytes from controller
     */
    private final CircularByteBuffer cbb;
    private final AbstractIoStream.StreamStats streamStats;

    public IncomingDataBuffer(AbstractIoStream.StreamStats streamStats) {
        this.streamStats = Objects.requireNonNull(streamStats, "streamStats");
        this.cbb = new CircularByteBuffer(BUFFER_SIZE);
    }

    public static IncomingDataBuffer createDataBuffer(String loggingPrefix, IoStream stream) {
        IncomingDataBuffer.loggingPrefix = loggingPrefix;
        IncomingDataBuffer incomingData = new IncomingDataBuffer(stream.getStreamStats());
        stream.setInputListener(incomingData::addData);
        return incomingData;
    }

    public byte[] getPacket(String msg) throws EOFException {
        return getPacket(msg, false, System.currentTimeMillis());
    }

    public byte[] getPacket(String msg, boolean allowLongResponse) throws EOFException {
        return getPacket(msg, allowLongResponse, System.currentTimeMillis());
    }

    /**
     * why does this method return NULL in case of timeout?!
     * todo: there is a very similar BinaryProtocolServer#readPromisedBytes which throws exception in case of timeout
     */
    public byte[] getPacket(String msg, boolean allowLongResponse, long start) throws EOFException {
        boolean isTimeout = waitForBytes(msg + " header", start, 2);
        if (isTimeout)
            return null;

        int packetSize = swap16(getShort());
        // if (log.debugEnabled())
        //     log.debug(loggingPrefix + "Got packet size " + packetSize);
        if (packetSize < 0)
            return null;
        if (!allowLongResponse && packetSize > Math.max(Fields.BLOCKING_FACTOR, Fields.TS_OUTPUT_SIZE) + 10)
            throw new IllegalArgumentException(packetSize + " packet while not allowLongResponse");

        isTimeout = waitForBytes(loggingPrefix + msg + " body", start, packetSize + 4);
        if (isTimeout)
            return null;

        byte[] packet = new byte[packetSize];
        getData(packet);
        int packetCrc = swap32(getInt());
        int actualCrc = getCrc32(packet);

        boolean isCrcOk = actualCrc == packetCrc;
        if (!isCrcOk) {
            if (log.debugEnabled())
                log.debug(String.format("CRC mismatch %x: vs %x", actualCrc, packetCrc));
            return null;
        }
        onPacketArrived();
        // if (log.debugEnabled())
        //     log.trace("packet arrived: " + Arrays.toString(packet) + ": crc OK");

        return packet;
    }

    public void onPacketArrived() {
        streamStats.onPacketArrived();
    }

    public void addData(byte[] freshData) {
        //log.info("IncomingDataBuffer: " + freshData.length + " byte(s) arrived");
        synchronized (cbb) {
            if (cbb.size() - cbb.length() < freshData.length) {
                log.error("IncomingDataBuffer: buffer overflow not expected");
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
                    log.info(loggingMessage + ": timeout. Got only " + cbb.length());
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

    public void dropPending() {
        // todo: when exactly do we need this logic?
        synchronized (cbb) {
            int pending = cbb.length();
            if (pending > 0) {
                log.error("dropPending: Unexpected pending data: " + pending + " byte(s)");
                byte[] bytes = new byte[pending];
                cbb.get(bytes);
                log.error("data: " + Arrays.toString(bytes));
            }
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
            return cbb.getShort();
        }
    }

    public int getInt() throws EOFException {
        streamStats.onArrived(4);
        synchronized (cbb) {
            return cbb.getInt();
        }
    }

    public void getData(byte[] packet) {
        synchronized (cbb) {
            cbb.get(packet);
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
