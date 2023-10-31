package com.rusefi.io;

import com.opensr5.io.DataListener;
import com.opensr5.io.WriteStream;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.IoHelper;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.io.serial.StreamStatistics;
import com.rusefi.io.tcp.BinaryProtocolServer;
import org.jetbrains.annotations.NotNull;

import java.io.Closeable;
import java.io.EOFException;
import java.io.IOException;

/**
 * Physical bi-directional controller communication level
 * <p>
 * Andrey Belomutskiy, (c) 2013-2020
 * <p>
 * 5/11/2015.
 */
public interface IoStream extends WriteStream, Closeable, StreamStatistics {
    static String printHexBinary(byte[] data) {
        if (data == null)
            return "(null)";
        char[] hexCode = "0123456789ABCDEF".toCharArray();

        StringBuilder r = new StringBuilder(data.length * 2);
        for (byte b : data) {
            r.append(hexCode[(b >> 4) & 0xF]);
            r.append(hexCode[(b & 0xF)]);
            r.append(' ');
        }
        return r.toString();
    }

    static String printByteArray(byte[] data) {
        StringBuilder sb = new StringBuilder();
        for (byte b : data) {
            if (Character.isJavaIdentifierPart(b)) {
                sb.append((char) b);
            } else {
                sb.append(' ');
            }
        }
        return printHexBinary(data) + sb;
    }

    @NotNull
    default BinaryProtocolServer.Packet readPacket() throws IOException {
        short length = readShort();
        return BinaryProtocolServer.readPromisedBytes(getDataBuffer(), length);
    }

    default void sendPacket(BinaryProtocolServer.Packet packet) throws IOException {
        writeShort(packet.getPacket().length);
        write(packet.getPacket());
        writeInt(packet.getCrc());
        flush();
        onActivity();
    }

    long latestActivityTime();

    void onActivity();

    default void sendPacket(byte[] plainPacket) throws IOException {
        if (plainPacket.length == 0)
            throw new IllegalArgumentException("Empty packets are not valid.");
        byte[] packet;
        if (BinaryProtocol.PLAIN_PROTOCOL) {
            packet = plainPacket;
        } else {
            packet = IoHelper.makeCrc32Packet(plainPacket);
        }
        // todo: verbose mode printHexBinary(plainPacket))
        //log.debug(getLoggingPrefix() + "Sending packet " + BinaryProtocol.findCommand(plainPacket[0]) + " length=" + plainPacket.length);
        write(packet);
        flush();
    }

    /**
     * @param listener would be invoked from unknown implementation-dependent thread
     */
    void setInputListener(DataListener listener);

    boolean isClosed();

    AbstractIoStream.StreamStats getStreamStats();

    void close();

    IncomingDataBuffer getDataBuffer();

    default short readShort() throws EOFException {
        return getDataBuffer().readShort();
    }

    default byte[] sendAndGetPacket(byte[] packet, String message) throws IOException {
        // synchronization is needed for example to help SD card download to live with gauge poker
        synchronized (this) {
            sendPacket(packet);
            return getDataBuffer().getPacket(message);
        }
    }
}
