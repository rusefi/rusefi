package com.rusefi.io.commands;

import com.rusefi.binaryprotocol.BinaryProtocol;
import org.jetbrains.annotations.NotNull;

/**
 * @see WriteChunkCommand
 */
public class WriteCommand {
    public static byte @NotNull [] getWritePacket(BinaryProtocol binaryProtocol, byte[] content, int contentOffset, int ecuOffset, int size) {
        byte[] header = binaryProtocol.smartPacketPrefix(ecuOffset, size);

        byte[] packet = new byte[header.length + size];
        // dynamic size header first
        System.arraycopy(header, 0, packet, 0, header.length);

        // followed by payload
        System.arraycopy(content, contentOffset, packet, header.length, size);
        return packet;
    }
}
