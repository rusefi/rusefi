package com.rusefi.proxy.client;

import java.io.IOException;

public interface LocalApplicationProxyContext {
    String executeGet(String url) throws IOException;

    /**
     * port on which rusEFI proxy accepts authenticator connections
     */
    int serverPortForRemoteApplications();

    /**
     * local port on which authenticator accepts connections from Tuner Studio
     */
    int authenticatorPort();
}
