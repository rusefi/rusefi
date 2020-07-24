package com.rusefi.proxy;

import com.opensr5.ConfigurationImage;
import com.opensr5.Logger;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
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
    public static SessionDetails runNetworkConnector(String authToken, String controllerPort, int serverPortForControllers, TcpIoStream.DisconnectListener disconnectListener) throws InterruptedException, IOException {
        LinkManager linkManager = new LinkManager()
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

        Logger.CONSOLE.info("Connecting to controller...");
        onConnected.await(1, TimeUnit.MINUTES);
        if (onConnected.getCount() != 0) {
            Logger.CONSOLE.info("Connection to controller failed");
            return null;
        }

        return runNetworkConnector(serverPortForControllers, linkManager, Logger.CONSOLE, authToken, disconnectListener);
    }

    @NotNull
    private static SessionDetails runNetworkConnector(int serverPortForControllers, LinkManager linkManager, final Logger logger, String authToken, final TcpIoStream.DisconnectListener disconnectListener) throws IOException {
        IoStream targetEcuSocket = linkManager.getConnector().getBinaryProtocol().getStream();
        HelloCommand.send(targetEcuSocket);
        String helloResponse = HelloCommand.getHelloResponse(targetEcuSocket.getDataBuffer());
        if (helloResponse == null)
            throw new IOException("Error getting hello response");
        String controllerSignature = helloResponse.trim();

        ConfigurationImage image = linkManager.getConnector().getBinaryProtocol().getControllerConfiguration();
        String vehicleName = Fields.VEHICLENAME.getStringValue(image);
        String engineMake = Fields.ENGINEMAKE.getStringValue(image);
        String engineCode = Fields.ENGINECODE.getStringValue(image);
        ControllerInfo ci = new ControllerInfo(vehicleName, engineMake, engineCode, controllerSignature);

        SessionDetails deviceSessionDetails = new SessionDetails(ci, authToken, SessionDetails.createOneTimeCode());

        BaseBroadcastingThread baseBroadcastingThread = new BaseBroadcastingThread(rusEFISSLContext.getSSLSocket(HttpUtil.RUSEFI_PROXY_HOSTNAME, serverPortForControllers),
                deviceSessionDetails,
                disconnectListener) {
            @Override
            protected void handleCommand(BinaryProtocolServer.Packet packet, TcpIoStream stream) throws IOException {
                super.handleCommand(packet, stream);
                logger.info("Relaying request to controller " + BinaryProtocol.findCommand(packet.getPacket()[0]));
                targetEcuSocket.sendPacket(packet);

                BinaryProtocolServer.Packet response = targetEcuSocket.readPacket();
                logger.info("Relaying response to proxy size=" + response.getPacket().length);
                stream.sendPacket(response);
            }
        };
        baseBroadcastingThread.start();
        return deviceSessionDetails;
    }
}
