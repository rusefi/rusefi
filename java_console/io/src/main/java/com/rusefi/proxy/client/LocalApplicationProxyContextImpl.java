package com.rusefi.proxy.client;

import com.rusefi.tools.online.HttpUtil;

import java.io.IOException;

public class LocalApplicationProxyContextImpl implements LocalApplicationProxyContext {
    @Override
    public String executeGet(String url) throws IOException {
        return HttpUtil.executeGet(url);
    }
}
