package com.rusefi.proxy;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.rusefi.Timeouts;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.AbstractConnectionStateListener;
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
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;

/**
 * Connector between rusEFI ECU and rusEFI server
 * see NetworkConnectorStartup
 */
public class NetworkConnector {
    private final static Logging log = Logging.getLogging(NetworkConnector.class);

    public static NetworkConnectorResult runNetworkConnector(String authToken, String controllerPort, NetworkConnectorContext context) {
        LinkManager controllerConnector = new LinkManager()
                .setCompositeLogicEnabled(false)
                .setNeedPullData(false);

        CountDownLatch onConnected = new CountDownLatch(1);
        controllerConnector.startAndConnect(controllerPort, new AbstractConnectionStateListener() {
            @Override
            public void onConnectionEstablished() {
                onConnected.countDown();
            }
        });

        log.info("Connecting to controller...");
        try {
            onConnected.await(1, TimeUnit.MINUTES);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
        if (onConnected.getCount() != 0) {
            log.info("Connection to controller failed");
            return NetworkConnectorResult.ERROR;
        }

        ControllerInfo controllerInfo;
        try {
            controllerInfo = getControllerInfo(controllerConnector, controllerConnector.getConnector().getBinaryProtocol().getStream());
        } catch (IOException e) {
            return NetworkConnectorResult.ERROR;
        }

        int oneTimeToken = SessionDetails.createOneTimeCode();

        new Thread(() -> {
            Semaphore proxyReconnectSemaphore = new Semaphore(1);
            try {
                while (true) {
                    log.info("Connecting to proxy server");
                    proxyReconnectSemaphore.acquire();

                    try {
                        runNetworkConnector(context.serverPortForControllers(), controllerConnector, authToken, () -> {
                            log.error("Disconnect from proxy server detected, now sleeping " + context.reconnectDelay() + " seconds");
                            sleep(context.reconnectDelay() * Timeouts.SECOND);
                            proxyReconnectSemaphore.release();
                        }, oneTimeToken, controllerInfo);
                    } catch (IOException e) {
                        log.error("IO error", e);
                    }
                }
            } catch (InterruptedException e) {
                throw new IllegalStateException(e);
            }
        }, "Proxy Reconnect").start();

        return new NetworkConnectorResult(controllerInfo, oneTimeToken);
    }

    @NotNull
    private static SessionDetails runNetworkConnector(int serverPortForControllers, LinkManager linkManager, String authToken, final TcpIoStream.DisconnectListener disconnectListener, int oneTimeToken, ControllerInfo controllerInfo) throws IOException {
        IoStream targetEcuSocket = linkManager.getConnector().getBinaryProtocol().getStream();

        SessionDetails deviceSessionDetails = new SessionDetails(controllerInfo, authToken, oneTimeToken);

        BaseBroadcastingThread baseBroadcastingThread = new BaseBroadcastingThread(rusEFISSLContext.getSSLSocket(HttpUtil.RUSEFI_PROXY_HOSTNAME, serverPortForControllers),
                deviceSessionDetails,
                disconnectListener) {
            @Override
            protected void handleCommand(BinaryProtocolServer.Packet packet, TcpIoStream stream) throws IOException {
                super.handleCommand(packet, stream);
                log.info("Relaying request to controller " + BinaryProtocol.findCommand(packet.getPacket()[0]));
                targetEcuSocket.sendPacket(packet);

                BinaryProtocolServer.Packet response = targetEcuSocket.readPacket();
                log.info("Relaying response to proxy size=" + response.getPacket().length);
                stream.sendPacket(response);
            }
        };
        baseBroadcastingThread.start();
        return deviceSessionDetails;
    }

    @NotNull
    private static ControllerInfo getControllerInfo(LinkManager linkManager, IoStream targetEcuSocket) throws IOException {
        HelloCommand.send(targetEcuSocket);
        String helloResponse = HelloCommand.getHelloResponse(targetEcuSocket.getDataBuffer());
        if (helloResponse == null)
            throw new IOException("Error getting hello response");
        String controllerSignature = helloResponse.trim();

        ConfigurationImage image = linkManager.getConnector().getBinaryProtocol().getControllerConfiguration();
        String vehicleName = Fields.VEHICLENAME.getStringValue(image);
        String engineMake = Fields.ENGINEMAKE.getStringValue(image);
        String engineCode = Fields.ENGINECODE.getStringValue(image);
        return new ControllerInfo(vehicleName, engineMake, engineCode, controllerSignature);
    }

    public static class NetworkConnectorResult {
        static NetworkConnectorResult ERROR = new NetworkConnectorResult(null, 0);
        private final ControllerInfo controllerInfo;
        private final int oneTimeToken;

        public NetworkConnectorResult(ControllerInfo controllerInfo, int oneTimeToken) {
            this.controllerInfo = controllerInfo;
            this.oneTimeToken = oneTimeToken;
        }

        public ControllerInfo getControllerInfo() {
            return controllerInfo;
        }

        public int getOneTimeToken() {
            return oneTimeToken;
        }
    }

}
