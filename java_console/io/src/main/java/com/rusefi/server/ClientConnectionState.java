package com.rusefi.server;

import com.opensr5.Logger;
import com.rusefi.auth.AutoTokenUtil;
import com.rusefi.binaryprotocol.BinaryProtocolCommands;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
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
            HelloCommand.send(stream, logger);
            byte[] response = incomingData.getPacket(logger, "", false);
            if (!checkResponseCode(response, BinaryProtocolCommands.RESPONSE_OK))
                return;
            String tokenAndSignature = new String(response, 1, response.length - 1);
            String token = tokenAndSignature.length() > AutoTokenUtil.TOKEN_LENGTH ? tokenAndSignature.substring(0, AutoTokenUtil.TOKEN_LENGTH) : null;
            if (!AutoTokenUtil.isToken(token))
                throw new IOException("Invalid token");
            String signature = tokenAndSignature.substring(AutoTokenUtil.TOKEN_LENGTH);

            logger.info(token + " New client: " + signature);

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
