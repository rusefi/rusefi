package com.rusefi.proxy;

import com.rusefi.binaryprotocol.IniFileProvider;
import org.mockito.Mockito;

public class MockIniFileProvider {

    public static IniFileProvider create() {
        IniFileProvider mock = Mockito.mock(IniFileProvider.class);
        return mock;
    }
}
