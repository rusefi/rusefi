package com.rusefi.proxy;

import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IniFileProvider;
import com.rusefi.config.FieldType;

import java.util.Map;

import static org.mockito.Mockito.*;

public class MockIniFileProvider {

    public static IniFileProvider create() {
        IniFileMetaInfo mockMeta = mock(IniFileMetaInfo.class);

        when(mockMeta.getPageSize(0)).thenReturn(15000);
        when(mockMeta.getPageReadCommand(anyInt())).thenReturn("123456");

        IniFileModel mockModel = mock(IniFileModel.class);
        when(mockModel.getSignature()).thenReturn("rusEFI mocked-ini-file");
        when(mockModel.getBlockingFactor()).thenReturn(6666);

        final IniField mockedStringIniField = new StringIniField("mockStringIniField", 0, 12);
        // Empty string ini fields from zero-filled image are not persisted to .msq file, so we need at list scalar ini
        // field to prevent empty msq error:
        final IniField mockedScalarIniField = new ScalarIniField(
            "mockScalarIniField",
            13,
            "mocked units",
            FieldType.UINT16,
            1.0,
            "0",
            0
        );
        when(mockModel.getAllIniFields()).thenReturn(Map.of(
            mockedStringIniField.getName(), mockedStringIniField,
            mockedScalarIniField.getName(), mockedScalarIniField
        ));
        when(mockModel.getIniField(anyString())).then(invocationOnMock -> mockedStringIniField);
        when(mockModel.getIniField(mockedScalarIniField.getName())).then(invocationOnMock -> mockedScalarIniField);

        when(mockModel.getMetaInfo()).thenReturn(mockMeta);

        return signature -> mockModel;
    }

    public static void install() {
        BinaryProtocol.iniFileProvider = create();
    }
}
