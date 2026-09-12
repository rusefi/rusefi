package com.rusefi.binaryprotocol;

import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.config.FieldType;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.OutputChannelDemand;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import org.junit.jupiter.api.Test;

import java.util.Collections;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.Mockito.*;

class ConfigErrorPollingTest {
    @Test
    void issue10219ActiveIndicatorDoesNotFetchErrorText() throws Exception {
        IniFileModel ini = mock(IniFileModel.class);
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(ini.getMetaInfo()).thenReturn(meta);
        when(meta.getOchBlockSize()).thenReturn(4);
        when(ini.getBlockingFactor()).thenReturn(256);
        EnumIniField indicator = new EnumIniField("hasCriticalError", 0, FieldType.INT,
                new EnumIniField.EnumKeyValueMap(Collections.emptyMap()), 14, 0);
        when(ini.getAllOutputChannels()).thenReturn(Collections.singletonMap("hasCriticalError", indicator));
        when(ini.getOutputChannel("hasCriticalError")).thenReturn(indicator);
        BinaryProtocol protocol = spy(new BinaryProtocol(new LinkManager(), mock(IoStream.class)));
        protocol.setIniFileForUnitTest(ini);
        doReturn(new byte[]{0, 0, 0x40, 0, 0}).when(protocol)
                .executeCommand(eq(Integration.TS_OUTPUT_COMMAND), any(byte[].class), anyString());
        doReturn(new byte[]{0, 'B', 'a', 'd', ' ', 'p', 'i', 'n'}).when(protocol)
                .executeCommand(eq(Integration.TS_GET_CONFIG_ERROR), anyString());

        assertTrue(protocol.requestOutputChannels(OutputChannelDemand.full(0)));
        assertEquals(1, SensorCentral.getInstance().getValue("hasCriticalError"));
        // Reproduction: the indicator is red, but the ECU's explanation is never requested.
        verify(protocol, never()).executeCommand(eq(Integration.TS_GET_CONFIG_ERROR), anyString());
    }
}
