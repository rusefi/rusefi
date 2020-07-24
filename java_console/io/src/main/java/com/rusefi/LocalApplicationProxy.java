package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.ServerHolder;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.server.ApplicationRequest;
import com.rusefi.server.rusEFISSLContext;
import com.rusefi.tools.online.HttpUtil;
import com.rusefi.tools.online.ProxyClient;
import org.apache.http.HttpResponse;

import java.io.IOException;

public class LocalApplicationProxy {
    public static final int SERVER_PORT_FOR_APPLICATIONS = 8002;
    private final Logger logger;
    private final ApplicationRequest applicationRequest;

    public LocalApplicationProxy(Logger logger, ApplicationRequest applicationRequest) {
        this.logger = logger;
        this.applicationRequest = applicationRequest;
    }

    /**
     * @param serverPortForRemoteUsers port on which rusEFI proxy accepts authenticator connections
     * @param applicationRequest       remote session we want to connect to
     * @param localApplicationPort        local port we would bind for TunerStudio to connect to
     * @param jsonHttpPort
     */
    public static ServerHolder startAndRun(Logger logger, int serverPortForRemoteUsers, ApplicationRequest applicationRequest, int localApplicationPort, int jsonHttpPort) throws IOException {
        String version = HttpUtil.executeGet(logger,ProxyClient.getHttpAddress(jsonHttpPort) + ProxyClient.VERSION_PATH);
        logger.info("Server says version=" + version);
        if (!version.contains(ProxyClient.BACKEND_VERSION))
            throw new IOException("Unexpected backend version " + version + " while we want " + ProxyClient.BACKEND_VERSION);

        IoStream authenticatorToProxyStream = new TcpIoStream("authenticatorToProxyStream ", logger, rusEFISSLContext.getSSLSocket(HttpUtil.RUSEFI_PROXY_HOSTNAME, serverPortForRemoteUsers));
        LocalApplicationProxy localApplicationProxy = new LocalApplicationProxy(logger, applicationRequest);
        logger.info("Pushing " + applicationRequest);
        localApplicationProxy.run(authenticatorToProxyStream);

        return BinaryProtocolProxy.createProxy(logger, authenticatorToProxyStream, localApplicationPort);
    }

    public void run(IoStream authenticatorToProxyStream) throws IOException {
        // right from connection push session authentication data
        new HelloCommand(logger, applicationRequest.toJson()).handle(authenticatorToProxyStream);
    }

    public static void start(String[] strings) {
    }
}
