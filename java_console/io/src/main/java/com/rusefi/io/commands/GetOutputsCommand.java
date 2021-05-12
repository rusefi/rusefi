package com.rusefi.io.commands;

import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.tcp.BinaryProtocolServer;

import java.io.IOException;

import static com.rusefi.binaryprotocol.IoHelper.putShort;
import static com.rusefi.binaryprotocol.IoHelper.swap16;

public class GetOutputsCommand {
    public static byte[] createRequest() {
        byte[] packet = new byte[5];
        packet[0] = Fields.TS_OUTPUT_COMMAND;
        putShort(packet, 1, 0); // offset
        putShort(packet, 3, swap16(Fields.TS_OUTPUT_SIZE));
        return packet;
    }

    public static void sendOutput(IoStream stream) throws IOException {
        byte[] response = new byte[1 + Fields.TS_OUTPUT_SIZE];
        response[0] = (byte) BinaryProtocolServer.TS_OK.charAt(0);
        stream.sendPacket(response);
    }
}
