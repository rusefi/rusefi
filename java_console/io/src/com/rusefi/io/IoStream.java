package com.rusefi.io;

import com.opensr5.Logger;
import com.opensr5.io.DataListener;
import com.opensr5.io.WriteStream;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IoHelper;

import java.io.IOException;

/**
 * Physical bi-directional controller communication level
 * <p>
 * (c) Andrey Belomutskiy
 * <p>
 * 5/11/2015.
 */
public interface IoStream extends WriteStream {

    static String printHexBinary(byte[] data) {
        char[] hexCode = "0123456789ABCDEF".toCharArray();

        StringBuilder r = new StringBuilder(data.length * 2);
        for (byte b : data) {
            r.append(hexCode[(b >> 4) & 0xF]);
            r.append(hexCode[(b & 0xF)]);
            r.append(' ');
        }
        return r.toString();
    }

    default void sendPacket(byte[] plainPacket, Logger logger) throws IOException {
        byte[] packet;
        if (BinaryProtocol.PLAIN_PROTOCOL) {
            packet = plainPacket;
        } else {
            packet = IoHelper.makeCrc32Packet(plainPacket);
        }
        logger.info("Sending packet " + printHexBinary(plainPacket));
        write(packet);
    }

    /**
     * @param listener would be invoked from unknown implementation-dependent thread
     */
    void setInputListener(DataListener listener);

    boolean isClosed();

    void close();

    /**
     * purges pending input and output
     */
    void purge();
}
