package com.rusefi.server;

import com.devexperts.logging.Logging;
import com.rusefi.auth.AutoTokenUtil;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.core.SensorsHolder;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.GetOutputsCommand;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.shared.FileUtil;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.net.Socket;

import static com.devexperts.logging.Logging.getLogging;

public class ControllerConnectionState {
    private static final Logging log = getLogging(ControllerConnectionState.class);
    private final Socket clientSocket;
    private final UserDetailsResolver userDetailsResolver;

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
    private ControllerKey controllerKey;

    private final TwoKindSemaphore twoKindSemaphore = new TwoKindSemaphore();
    private final SensorsHolder sensorsHolder = new SensorsHolder();

    public ControllerConnectionState(Socket clientSocket, UserDetailsResolver userDetailsResolver) {
        this.clientSocket = clientSocket;
        this.userDetailsResolver = userDetailsResolver;
        try {
            stream = new TcpIoStream("[controller] ", clientSocket);
            incomingData = stream.getDataBuffer();
        } catch (IOException e) {
            close();
        }
    }

    public IoStream getStream() {
        return stream;
    }

    public ControllerKey getControllerKey() {
        return controllerKey;
    }

    public boolean isClosed() {
        return isClosed;
    }

    public void close() {
        isClosed = true;
        FileUtil.close(clientSocket);
    }

    public void requestControllerInfo() throws IOException {
        HelloCommand.send(stream);
        String jsonString = HelloCommand.getHelloResponse(incomingData);
        if (jsonString == null)
            return;
        sessionDetails = SessionDetails.valueOf(jsonString);
        if (!AutoTokenUtil.isToken(sessionDetails.getAuthToken()))
            throw new IOException("Invalid token in " + jsonString);

        log.info(sessionDetails.getAuthToken() + " New client: " + sessionDetails.getControllerInfo());
        userDetails = userDetailsResolver.apply(sessionDetails.getAuthToken());
        if (userDetails == null) {
            throw new IOException("Unable to resolve " + sessionDetails.getAuthToken());
        }
        controllerKey = new ControllerKey(userDetails.getUserId(), sessionDetails.getControllerInfo());
        log.info("User " + userDetails);
    }

    public UserDetails getUserDetails() {
        return userDetails;
    }

    public SessionDetails getSessionDetails() {
        return sessionDetails;
    }

    public void getOutputs() throws IOException {
        byte[] commandPacket = GetOutputsCommand.createRequest();

        stream.sendPacket(commandPacket);

        byte[] packet = incomingData.getPacket("msg", true);
        if (packet == null)
            throw new IOException("getOutputs: No response");
        sensorsHolder.grabSensorValues(packet);
    }

    @NotNull
    public TwoKindSemaphore getTwoKindSemaphore() {
        return twoKindSemaphore;
    }

    @NotNull
    public SensorsHolder getSensorsHolder() {
        return sensorsHolder;
    }

    public void grabOutputs(Backend backend) {
        try {
            getOutputs();
        } catch (IOException e) {
            // todo: this is currently not covered by a unit test
            backend.close(this);
        }
    }
}
