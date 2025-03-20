package com.rusefi.proxy;

import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IniFileProvider;

import static org.mockito.Mockito.*;

public class MockIniFileProvider {

    public static IniFileProvider create() {
        IniFileMetaInfo mockMeta = mock(IniFileMetaInfo.class);

        when(mockMeta.getTotalSize()).thenReturn(15000);

        IniFileModel mockModel = mock(IniFileModel.class);
        when(mockModel.getBlockingFactor()).thenReturn(6666);
        when(mockModel.getMetaInfo()).thenReturn(mockMeta);
        when(mockModel.getIniField(anyString())).then(invocationOnMock -> new StringIniField("mockStringIniField", 0, 12));

        return signature -> mockModel;
    }

    public static void install() {
        BinaryProtocol.iniFileProvider = create();
    }
}
