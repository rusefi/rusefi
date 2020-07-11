package com.rusefi.server;

import com.opensr5.Logger;
import com.rusefi.binaryprotocol.BinaryProtocolCommands;
import com.rusefi.io.tcp.TcpIoStream;

import java.io.Closeable;
import java.io.IOException;
import java.net.Socket;

public class ClientConnectionState {
    private final Socket clientSocket;
    private final Logger logger;

    private long lastActivityTimestamp;
    private boolean isClosed;
    private TcpIoStream stream;

    public ClientConnectionState(Socket clientSocket, Logger logger) {
        this.clientSocket = clientSocket;
        this.logger = logger;
        try {
            stream = new TcpIoStream(logger, clientSocket);
        } catch (IOException e) {
            close();
        }
    }

    public long getLastActivityTimestamp() {
        return lastActivityTimestamp;
    }

    public void close() {
        isClosed = true;
        close(clientSocket);
    }

    public void sayHello() {
        try {
            stream.sendPacket(new byte[]{BinaryProtocolCommands.COMMAND_HELLO}, logger);
        } catch (IOException e) {
            close();
        }
    }

    private static void close(Closeable closeable) {
        if (closeable != null) {
            try {
                closeable.close();
            } catch (IOException ignored) {
                // ignored
            }
        }
    }
}
