package com.rusefi;

import com.devexperts.logging.Logging;
import com.opensr5.Logger;
import com.rusefi.core.MessagesCentral;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.ServerHolder;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.server.ApplicationRequest;
import com.rusefi.server.rusEFISSLContext;
import com.rusefi.tools.online.HttpUtil;
import com.rusefi.tools.online.ProxyClient;

import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;

public class LocalApplicationProxy {
    private static final Logging log = getLogging(LocalApplicationProxy.class);
    public static final int SERVER_PORT_FOR_APPLICATIONS = 8002;
    private final ApplicationRequest applicationRequest;

    public LocalApplicationProxy(ApplicationRequest applicationRequest) {
        this.applicationRequest = applicationRequest;
    }

    /**
     * @param serverPortForRemoteUsers port on which rusEFI proxy accepts authenticator connections
     * @param applicationRequest       remote session we want to connect to
     * @param localApplicationPort        local port we would bind for TunerStudio to connect to
     * @param jsonHttpPort
     * @param disconnectListener
     */
    public static ServerHolder startAndRun(int serverPortForRemoteUsers, ApplicationRequest applicationRequest, int localApplicationPort, int jsonHttpPort, TcpIoStream.DisconnectListener disconnectListener) throws IOException {
        String version = HttpUtil.executeGet(ProxyClient.getHttpAddress(jsonHttpPort) + ProxyClient.VERSION_PATH);
        log.info("Server says version=" + version);
        if (!version.contains(ProxyClient.BACKEND_VERSION))
            throw new IOException("Unexpected backend version " + version + " while we want " + ProxyClient.BACKEND_VERSION);

        IoStream authenticatorToProxyStream = new TcpIoStream("authenticatorToProxyStream ", rusEFISSLContext.getSSLSocket(HttpUtil.RUSEFI_PROXY_HOSTNAME, serverPortForRemoteUsers), disconnectListener);
        LocalApplicationProxy localApplicationProxy = new LocalApplicationProxy(applicationRequest);
        log.info("Pushing " + applicationRequest);
        localApplicationProxy.run(authenticatorToProxyStream);

        return BinaryProtocolProxy.createProxy(authenticatorToProxyStream, localApplicationPort);
    }

    public void run(IoStream authenticatorToProxyStream) throws IOException {
        // right from connection push session authentication data
        new HelloCommand(applicationRequest.toJson()).handle(authenticatorToProxyStream);
    }

    public static void start(String[] strings) {
    }
}
