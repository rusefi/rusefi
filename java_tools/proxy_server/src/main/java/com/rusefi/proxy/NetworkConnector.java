package com.rusefi.proxy;

import com.opensr5.Logger;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.server.ControllerInfo;
import com.rusefi.server.SessionDetails;
import com.rusefi.server.rusEFISSLContext;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;

/**
 * Connector between rusEFI ECU and rusEFI server
 */
public class NetworkConnector {
    public static String RUSEFI_PROXY_HOSTNAME = "proxy.rusefi.com";

    @NotNull
    public static SessionDetails runNetworkConnector(int serverPortForControllers, IoStream targetEcuSocket, final Logger logger, String authToken) throws IOException {
        HelloCommand.send(targetEcuSocket, logger);
        String controllerSignature = HelloCommand.getHelloResponse(targetEcuSocket.getDataBuffer(), logger);

        // todo: request vehicle info from controller
        ControllerInfo ci = new ControllerInfo("vehicle", "make", "code", controllerSignature);

        SessionDetails deviceSessionDetails = new SessionDetails(ci, authToken, SessionDetails.createOneTimeCode());

        BaseBroadcastingThread baseBroadcastingThread = new BaseBroadcastingThread(rusEFISSLContext.getSSLSocket(RUSEFI_PROXY_HOSTNAME, serverPortForControllers),
                deviceSessionDetails,
                logger) {
            @Override
            protected void handleCommand(BinaryProtocolServer.Packet packet, TcpIoStream stream) throws IOException {
                super.handleCommand(packet, stream);
                targetEcuSocket.sendPacket(packet);

                BinaryProtocolServer.Packet response = targetEcuSocket.readPacket();
                stream.sendPacket(response);
            }
        };
        baseBroadcastingThread.start();
        return deviceSessionDetails;
    }

    public static void start(String[] strings) {
    }
}
