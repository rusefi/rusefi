package com.rusefi.proxy.client;

import java.io.IOException;

public interface LocalApplicationProxyContext {
    String executeGet(String url) throws IOException;
}
