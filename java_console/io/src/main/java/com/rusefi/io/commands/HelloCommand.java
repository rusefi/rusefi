package com.rusefi.io.commands;

import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Integration;
import com.rusefi.io.IoStream;
import com.rusefi.io.tcp.BinaryProtocolServer;
import org.jetbrains.annotations.Nullable;

import java.io.EOFException;
import java.io.IOException;
import java.nio.charset.StandardCharsets;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;

public class HelloCommand implements Command {
    private final String tsSignature;

    public static final byte[] HELLO_COMMAND = {Integration.TS_HELLO_COMMAND};

    public HelloCommand(String tsSignature) {
        this.tsSignature = tsSignature;
    }

    public static void send(IoStream stream) throws IOException {
        stream.sendPacket(HELLO_COMMAND);
    }

    @Nullable
    public static String getHelloResponse(IncomingDataBuffer incomingData) throws EOFException {
        return getStringResponse("[hello]", incomingData);
    }

    @Nullable
    public static String getStringResponse(String msg, IncomingDataBuffer incomingData) throws EOFException {
        return getStringResponse(msg, incomingData, com.rusefi.Timeouts.BINARY_IO_TIMEOUT);
    }

    @Nullable
    public static String getStringResponse(String msg, IncomingDataBuffer incomingData, int timeoutMs) throws EOFException {
        byte[] response = incomingData.getPacket(timeoutMs, msg);
        return decodeStringResponse(response);
    }

    static String decodeStringResponse(byte[] response) {
        if (!checkResponseCode(response)) {
            return null;
        }
        int end = response.length;
        while (end > 1 && response[end - 1] == 0) {
            end--;
        }
        return new String(response, 1, end - 1, StandardCharsets.US_ASCII);
    }

    @Override
    public byte getCommand() {
        return Integration.TS_HELLO_COMMAND;
    }

    @Override
    public void handle(IoStream stream) throws IOException {
        stream.sendPacket((BinaryProtocolServer.TS_OK + tsSignature).getBytes());
    }
}
