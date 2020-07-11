package com.rusefi.server;

import com.opensr5.Logger;
import com.rusefi.binaryprotocol.BinaryProtocolCommands;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.tcp.TcpIoStream;

import java.io.Closeable;
import java.io.IOException;
import java.net.Socket;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;

public class ClientConnectionState {
    private final Socket clientSocket;
    private final Logger logger;

    private long lastActivityTimestamp;
    private boolean isClosed;
    private IoStream stream;
    private IncomingDataBuffer incomingData;

    public ClientConnectionState(Socket clientSocket, Logger logger) {
        this.clientSocket = clientSocket;
        this.logger = logger;
        try {
            stream = new TcpIoStream(logger, clientSocket);
            incomingData = stream.getDataBuffer();
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
            stream.sendPacket(new byte[]{Fields.TS_HELLO_COMMAND}, logger);
            byte[] response = incomingData.getPacket(logger, "", false);
            if (!checkResponseCode(response, BinaryProtocolCommands.RESPONSE_OK))
                return;
            String signature = new String(response, 1, response.length - 1);
            logger.info("New client: " + signature);

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
