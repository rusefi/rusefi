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
import java.io.IOException;
import java.net.Socket;
import java.util.function.Function;

import static com.rusefi.binaryprotocol.IoHelper.checkResponseCode;

public class ClientConnectionState {
    private final Socket clientSocket;
    private final Logger logger;
    private final Function<String, UserDetails> userDetailsResolver;

    private boolean isClosed;
    private IoStream stream;
    private IncomingDataBuffer incomingData;
    /**
     * Data from controller
     */
    private SessionDetails sessionDetails;
    /**
     * user info from rusEFI database based on auth token
     */
    private UserDetails userDetails;

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

    public void requestControllerInfo() throws IOException {
        HelloCommand.send(stream, logger);
        byte[] response = incomingData.getPacket(logger, "", false);
        if (!checkResponseCode(response, BinaryProtocolCommands.RESPONSE_OK))
            return;
        String jsonString = new String(response, 1, response.length - 1);
        sessionDetails = SessionDetails.valueOf(jsonString);
        if (!AutoTokenUtil.isToken(sessionDetails.getAuthToken()))
            throw new IOException("Invalid token in " + jsonString);

        logger.info(sessionDetails.getAuthToken() + " New client: " + sessionDetails.getControllerInfo());
        userDetails = userDetailsResolver.apply(sessionDetails.getAuthToken());
        logger.info("User " + userDetails);
    }

    public UserDetails getUserDetails() {
        return userDetails;
    }

    public SessionDetails getSessionDetails() {
        return sessionDetails;
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
