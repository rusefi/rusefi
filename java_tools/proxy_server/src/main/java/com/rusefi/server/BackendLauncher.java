package com.rusefi.server;

import com.rusefi.proxy.client.LocalApplicationProxy;
import com.rusefi.tools.online.HttpUtil;
import com.rusefi.tools.online.ProxyClient;

import java.io.IOException;

public class BackendLauncher {
    public static void main(String[] args) throws IOException {
        /* todo
        rusEFISSLContext.setupCertificates(new File("keystore.jks"), System.getProperty("RUSEFI_KEYSTORE_PASSWORD"));
         */

        UserDetailsResolver userDetailsFunction = new JsonUserDetailsResolver();

        Backend backend = new Backend(userDetailsFunction, HttpUtil.PROXY_JSON_API_HTTP_PORT);
        backend.runApplicationConnector(LocalApplicationProxy.SERVER_PORT_FOR_APPLICATIONS, parameter -> {
        });
        backend.runControllerConnector(ProxyClient.SERVER_PORT_FOR_CONTROLLERS, parameter -> {
        });
    }
}
