package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.BinaryProtocolProxy;
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
     * @param authenticatorPort        local port we would bind for TunerStudio to connect to
     * @param httpPort
     */
    static void startAndRun(Logger logger, int serverPortForRemoteUsers, ApplicationRequest applicationRequest, int authenticatorPort, int httpPort) throws IOException {
        HttpResponse httpResponse = HttpUtil.executeGet(ProxyClient.getHttpAddress(httpPort) + ProxyClient.VERSION_PATH);
        String version = HttpUtil.getResponse(httpResponse);
        logger.info("Version=" + version);
        if (!version.contains(ProxyClient.BACKEND_VERSION))
            throw new IOException("Unexpected backend version " + version + " while we want " + ProxyClient.BACKEND_VERSION);

        IoStream authenticatorToProxyStream = new TcpIoStream("authenticatorToProxyStream ", logger, rusEFISSLContext.getSSLSocket(HttpUtil.RUSEFI_PROXY_HOSTNAME, serverPortForRemoteUsers));
        LocalApplicationProxy localApplicationProxy = new LocalApplicationProxy(logger, applicationRequest);
        localApplicationProxy.run(authenticatorToProxyStream);

        BinaryProtocolProxy.createProxy(logger, authenticatorToProxyStream, authenticatorPort);
    }

    public void run(IoStream authenticatorToProxyStream) throws IOException {
        // right from connection push session authentication data
        new HelloCommand(logger, applicationRequest.toJson()).handle(authenticatorToProxyStream);
    }

    public static void start(String[] strings) {
    }
}
