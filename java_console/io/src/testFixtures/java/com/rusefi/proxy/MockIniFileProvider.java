package com.rusefi.proxy;

import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModelImpl;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IniFileProvider;

import static org.mockito.Mockito.*;

public class MockIniFileProvider {

    public static IniFileProvider create() {
        IniFileMetaInfo mockMeta = mock(IniFileMetaInfo.class);

        when(mockMeta.getTotalSize()).thenReturn(22972);

        IniFileModelImpl mockModel = mock(IniFileModelImpl.class);
        when(mockModel.getMetaInfo()).thenReturn(mockMeta);



        return new IniFileProvider() {
            @Override
            public IniFileModelImpl provide(String signature) {
                return mockModel;
            }
        };
    }

    public static void install() {
        BinaryProtocol.iniFileProvider = create();
    }
}
