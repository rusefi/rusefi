package com.rusefi.io.commands;

import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;

import org.jetbrains.annotations.Nullable;

import java.io.EOFException;
import java.io.IOException;

import static com.rusefi.io.commands.HelloCommand.getStringResponse;

public class ErrorInfoCommand {
    public static void send(IoStream stream) throws IOException {
        stream.sendPacket(new byte[]{Fields.TS_GET_CONFIG_ERROR});
    }

    @Nullable
    public static String getResponse(IncomingDataBuffer incomingData) throws EOFException {
        return getStringResponse("[config_error]", incomingData);
    }

}
