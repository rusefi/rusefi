package com.rusefi.proxy.client;

import com.devexperts.logging.Logging;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.ServerSocketReference;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.server.ApplicationRequest;
import com.rusefi.server.rusEFISSLContext;
import com.rusefi.tools.online.HttpUtil;
import com.rusefi.tools.online.ProxyClient;

import java.io.Closeable;
import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;

public class LocalApplicationProxy implements Closeable {
    private static final Logging log = getLogging(LocalApplicationProxy.class);
    public static final int SERVER_PORT_FOR_APPLICATIONS = HttpUtil.getIntProperty("applications.port", 8002);
    private final ApplicationRequest applicationRequest;
    private final ServerSocketReference serverHolder;
    private final IoStream authenticatorToProxyStream;

    public LocalApplicationProxy(ApplicationRequest applicationRequest, ServerSocketReference serverHolder, IoStream authenticatorToProxyStream) {
        this.applicationRequest = applicationRequest;
        this.serverHolder = serverHolder;
        this.authenticatorToProxyStream = authenticatorToProxyStream;
    }

    public ApplicationRequest getApplicationRequest() {
        return applicationRequest;
    }

    /**
     * @param context
     * @param applicationRequest       remote session we want to connect to
     * @param jsonHttpPort
     * @param disconnectListener
     * @param connectionListener
     */
    public static ServerSocketReference startAndRun(LocalApplicationProxyContext context, ApplicationRequest applicationRequest, int jsonHttpPort, TcpIoStream.DisconnectListener disconnectListener, ConnectionListener connectionListener) throws IOException {
        String version = context.executeGet(ProxyClient.getHttpAddress(jsonHttpPort) + ProxyClient.VERSION_PATH);
        log.info("Server says version=" + version);
        if (!version.contains(ProxyClient.BACKEND_VERSION))
            throw new IOException("Unexpected backend version " + version + " while we want " + ProxyClient.BACKEND_VERSION);

        IoStream authenticatorToProxyStream = new TcpIoStream("authenticatorToProxyStream ", rusEFISSLContext.getSSLSocket(HttpUtil.RUSEFI_PROXY_HOSTNAME, context.serverPortForRemoteApplications()), disconnectListener);
        LocalApplicationProxy.sendHello(authenticatorToProxyStream, applicationRequest);

        ServerSocketReference serverHolder = BinaryProtocolProxy.createProxy(authenticatorToProxyStream, context.authenticatorPort());
        LocalApplicationProxy localApplicationProxy = new LocalApplicationProxy(applicationRequest, serverHolder, authenticatorToProxyStream);
        connectionListener.onConnected(localApplicationProxy);
        return serverHolder;
    }

    public static void sendHello(IoStream authenticatorToProxyStream, ApplicationRequest applicationRequest) throws IOException {
        log.info("Pushing " + applicationRequest);
        // right from connection push session authentication data
        new HelloCommand(applicationRequest.toJson()).handle(authenticatorToProxyStream);
    }

    public static void start() {
        throw new UnsupportedOperationException("TODO");
    }

    @Override
    public void close() {
        serverHolder.close();
        authenticatorToProxyStream.close();
    }

    public interface ConnectionListener {
        ConnectionListener VOID = localApplicationProxy -> {
        };

        void onConnected(LocalApplicationProxy localApplicationProxy);
    }
}
