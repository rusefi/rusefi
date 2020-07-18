package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.proxy.NetworkConnector;
import com.rusefi.server.ApplicationRequest;

import javax.net.ssl.SSLSocketFactory;
import java.io.IOException;

public class LocalApplicationProxy {
    private final Logger logger;
    private final ApplicationRequest applicationRequest;

    public LocalApplicationProxy(Logger logger, ApplicationRequest applicationRequest) {
        this.logger = logger;
        this.applicationRequest = applicationRequest;
    }

    /**
     * @param serverPortForRemoteUsers port on which rusEFI proxy accepts authenticator connections
     * @param applicationRequest remote session we want to connect to
     * @param authenticatorPort local port we would bind for TunerStudio to connect to
     */
    static void startAndRun(Logger logger, int serverPortForRemoteUsers, ApplicationRequest applicationRequest, int authenticatorPort) throws IOException {
        IoStream authenticatorToProxyStream = new TcpIoStream(logger, SSLSocketFactory.getDefault().createSocket(NetworkConnector.RUSEFI_PROXY_HOSTNAME, serverPortForRemoteUsers));
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
