package com.rusefi.proxy;

import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModelImpl;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IniFileProvider;

import static org.mockito.Mockito.*;

public class MockIniFileProvider {

    public static IniFileProvider create() {
        IniFileMetaInfo mockMeta = mock(IniFileMetaInfo.class);

        when(mockMeta.getTotalSize()).thenReturn(15000);

        IniFileModelImpl mockModel = mock(IniFileModelImpl.class);
        when(mockModel.getMetaInfo()).thenReturn(mockMeta);

        return signature -> mockModel;
    }

    public static void install() {
        BinaryProtocol.iniFileProvider = create();
    }
}
