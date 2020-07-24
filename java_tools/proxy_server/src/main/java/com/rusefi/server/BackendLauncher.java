package com.rusefi.server;

import com.rusefi.LocalApplicationProxy;
import com.rusefi.tools.online.HttpUtil;

public class BackendLauncher {
    /**
     * need this method to be not in Backend class for console to work without all backend classes
     */
    public static void start() {
        /* todo
        rusEFISSLContext.setupCertificates(new File("keystore.jks"), System.getProperty("RUSEFI_PROXY_PASSWORD"));
         */

        UserDetailsResolver userDetailsFunction = new JsonUserDetailsResolver();

        Backend backend = new Backend(userDetailsFunction, HttpUtil.PROXY_JSON_API_HTTP_PORT);
        backend.runApplicationConnector(LocalApplicationProxy.SERVER_PORT_FOR_APPLICATIONS, parameter -> {
        });
        backend.runControllerConnector(Backend.SERVER_PORT_FOR_CONTROLLERS, parameter -> {
        });
    }
}
