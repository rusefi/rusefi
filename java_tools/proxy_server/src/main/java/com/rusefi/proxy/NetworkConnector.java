package com.rusefi.proxy;

import com.opensr5.Logger;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.server.ControllerInfo;
import com.rusefi.server.SessionDetails;
import com.rusefi.server.rusEFISSLContext;
import com.rusefi.tools.online.HttpUtil;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

/**
 * Connector between rusEFI ECU and rusEFI server
 */
public class NetworkConnector {
    public static SessionDetails runNetworkConnector(String authToken, String controllerPort, int serverPortForControllers) throws InterruptedException, IOException {
        LinkManager linkManager = new LinkManager(Logger.CONSOLE)
                .setCompositeLogicEnabled(false)
                .setNeedPullData(false);

        CountDownLatch onConnected = new CountDownLatch(1);
        linkManager.startAndConnect(controllerPort, new ConnectionStateListener() {
            @Override
            public void onConnectionEstablished() {
                onConnected.countDown();
            }

            @Override
            public void onConnectionFailed() {

            }
        });

        System.out.println("Connecting to controller...");
        onConnected.await(1, TimeUnit.MINUTES);
        if (onConnected.getCount() != 0) {
            System.out.println("Connection to controller failed");
            return null;
        }

        return runNetworkConnector(serverPortForControllers, linkManager, Logger.CONSOLE, authToken);
    }

    @NotNull
    private static SessionDetails runNetworkConnector(int serverPortForControllers, LinkManager linkManager, final Logger logger, String authToken) throws IOException {
        IoStream targetEcuSocket = linkManager.getConnector().getBinaryProtocol().getStream();
        HelloCommand.send(targetEcuSocket, logger);
        String controllerSignature = HelloCommand.getHelloResponse(targetEcuSocket.getDataBuffer(), logger);

//        Fields.VEHICLENAME.getAnyValue()
        // todo: request vehicle info from controller
        ControllerInfo ci = new ControllerInfo("vehicle", "make", "code", controllerSignature);

        SessionDetails deviceSessionDetails = new SessionDetails(ci, authToken, SessionDetails.createOneTimeCode());

        BaseBroadcastingThread baseBroadcastingThread = new BaseBroadcastingThread(rusEFISSLContext.getSSLSocket(HttpUtil.RUSEFI_PROXY_HOSTNAME, serverPortForControllers),
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
}
