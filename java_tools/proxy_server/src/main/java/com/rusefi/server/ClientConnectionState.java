package com.rusefi.server;

import com.opensr5.Logger;
import com.rusefi.auth.AutoTokenUtil;
import com.rusefi.binaryprotocol.BinaryProtocolCommands;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.GetOutputsCommand;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.TcpIoStream;

import java.io.Closeable;
import java.io.EOFException;
import java.io.IOException;
import java.net.Socket;
import java.util.function.Function;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;

public class ClientConnectionState {
    private final Socket clientSocket;
    private final Logger logger;
    private final Function<String, UserDetails> userDetailsResolver;

    private long lastActivityTimestamp;
    private boolean isClosed;
    private IoStream stream;
    private IncomingDataBuffer incomingData;
    private UserDetails userDetails;
    private String signature;

    public ClientConnectionState(Socket clientSocket, Logger logger, Function<String, UserDetails> userDetailsResolver) {
        this.clientSocket = clientSocket;
        this.logger = logger;
        this.userDetailsResolver = userDetailsResolver;
        try {
            stream = new TcpIoStream(logger, clientSocket);
            incomingData = stream.getDataBuffer();
        } catch (IOException e) {
            close();
        }
    }

//    public long getLastActivityTimestamp() {
//        return lastActivityTimestamp;
//    }


    public boolean isClosed() {
        return isClosed;
    }

    public void close() {
        isClosed = true;
        close(clientSocket);
    }

    public void sayHello() throws IOException {
        HelloCommand.send(stream, logger);
        byte[] response = incomingData.getPacket(logger, "", false);
        if (!checkResponseCode(response, BinaryProtocolCommands.RESPONSE_OK))
            return;
        String tokenAndSignature = new String(response, 1, response.length - 1);
        String authToken = tokenAndSignature.length() > AutoTokenUtil.TOKEN_LENGTH ? tokenAndSignature.substring(0, AutoTokenUtil.TOKEN_LENGTH) : null;
        if (!AutoTokenUtil.isToken(authToken))
            throw new IOException("Invalid token");
        signature = tokenAndSignature.substring(AutoTokenUtil.TOKEN_LENGTH);

        logger.info(authToken + " New client: " + signature);
        userDetails = userDetailsResolver.apply(authToken);
        logger.info("User " + userDetails);
    }

    public UserDetails getUserDetails() {
        return userDetails;
    }

    public String getSignature() {
        return signature;
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

    public void runEndlessLoop() throws IOException {

        while (true) {
            byte[] commandPacket = GetOutputsCommand.createRequest();

            stream.sendPacket(commandPacket, logger);

            byte[] packet = incomingData.getPacket(logger, "msg", true);
            if (packet == null)
                throw new IOException("No response");

        }
    }
}
