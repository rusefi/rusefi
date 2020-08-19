package com.rusefi.server;

import com.devexperts.logging.Logging;
import com.rusefi.auth.AuthTokenUtil;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.SensorsHolder;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.GetOutputsCommand;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.proxy.NetworkConnector;
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
    private final Birthday birthday = new Birthday();
    private int outputRoundAroundDuration;

    public ControllerConnectionState(Socket clientSocket, UserDetailsResolver userDetailsResolver) {
        this.clientSocket = clientSocket;
        this.userDetailsResolver = userDetailsResolver;
        try {
            stream = new TcpIoStream("[backend-controller connector] ", clientSocket);
            incomingData = stream.getDataBuffer();
        } catch (IOException e) {
            close();
        }
    }

    public Birthday getBirthday() {
        return birthday;
    }

    public int getOutputRoundAroundDuration() {
        return outputRoundAroundDuration;
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

    @Override
    public String toString() {
        return "ControllerConnectionState{" +
                "userDetails=" + userDetails +
                ", sessionDetails=" + sessionDetails +
                ", controllerKey=" + controllerKey +
                ", isClosed=" + isClosed +
                ", twoKindSemaphore=" + twoKindSemaphore +
                '}';
    }

    public void requestControllerInfo() throws IOException {
        HelloCommand.send(stream);
        String jsonString = HelloCommand.getHelloResponse(incomingData);
        if (jsonString == null)
            return;
        sessionDetails = SessionDetails.valueOf(jsonString);
        if (!AuthTokenUtil.isToken(sessionDetails.getAuthToken()))
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
        long start = System.currentTimeMillis();
        stream.sendPacket(commandPacket);

        byte[] packet = incomingData.getPacket("msg", true);
        outputRoundAroundDuration = (int) (System.currentTimeMillis() - start);
        if (packet == null)
            throw new IOException("getOutputs: No response");
        if (packet.length != 1 + Fields.TS_OUTPUT_SIZE)
            throw new IOException("getOutputs: unexpected package length " + packet.length);
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
            log.error("grabOutputs " + this, e);
            backend.close(this);
        }
    }

    public void requestConnectorSoftwareUpdate() throws IOException {
        byte[] packet = new byte[2];
        packet[0] = Fields.TS_ONLINE_PROTOCOL;
        packet[1] = NetworkConnector.UPDATE_CONNECTOR_SOFTWARE;
        stream.sendPacket(packet);
    }
}
