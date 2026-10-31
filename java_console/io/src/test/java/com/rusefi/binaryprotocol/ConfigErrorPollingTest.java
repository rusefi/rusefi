package com.rusefi.binaryprotocol;

import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.config.FieldType;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.OutputChannelDemand;
import com.rusefi.core.OutputChannelSnapshot;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.AfterEach;

import java.util.BitSet;
import java.util.Collections;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.Mockito.*;

class ConfigErrorPollingTest {
    private static final byte[] ACTIVE = {0, 0, 0x40, 0, 0};
    private BinaryProtocol protocol;
    private IniFileModel ini;

    @BeforeEach
    void setUp() throws Exception {
        SensorCentral.getInstance().reset();
        ini = mock(IniFileModel.class);
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(ini.getMetaInfo()).thenReturn(meta);
        when(meta.getOchBlockSize()).thenReturn(4);
        when(ini.getBlockingFactor()).thenReturn(256);
        EnumIniField indicator = new EnumIniField("hasCriticalError", 0, FieldType.INT,
                new EnumIniField.EnumKeyValueMap(Collections.emptyMap()), 14, 0);
        when(ini.getAllOutputChannels()).thenReturn(Collections.singletonMap("hasCriticalError", indicator));
        when(ini.getOutputChannel("hasCriticalError")).thenReturn(indicator);
        protocol = spy(new BinaryProtocol(new LinkManager(), mock(IoStream.class)));
        protocol.setIniFileForUnitTest(ini);
        doReturn(ACTIVE).when(protocol)
                .executeCommand(eq(Integration.TS_OUTPUT_COMMAND), any(byte[].class), anyString());
        doReturn(new byte[]{0, 'B', 'a', 'd', ' ', 'p', 'i', 'n'}).when(protocol)
                .executeCommand(eq(Integration.TS_GET_CONFIG_ERROR), anyString());
    }

    @AfterEach
    void tearDown() {
        SensorCentral.getInstance().reset();
    }

    @Test
    void issue10219ActiveIndicatorFetchesErrorText() {
        assertTrue(protocol.requestOutputChannels(OutputChannelDemand.full(0)));
        assertEquals(1, SensorCentral.getInstance().getValue("hasCriticalError"));
        verify(protocol).executeCommand(eq(Integration.TS_GET_CONFIG_ERROR), anyString());
        assertEquals("Bad pin", protocol.getConfigErrorMessage());
    }

    @Test
    void selectivePollingAlsoFetchesTheExplanation() {
        assertTrue(protocol.requestOutputChannels(
                OutputChannelDemand.selective(Collections.singleton("hasCriticalError"), 2)));
        assertEquals("Bad pin", protocol.getConfigErrorMessage());
    }

    @Test
    void pollsChangedErrorAtMostOncePerSecond() {
        protocol.updateConfigError(OutputChannelSnapshot.full(ACTIVE), 0);
        doReturn(new byte[]{0, 'N', 'e', 'w', 0, 'x'}).when(protocol)
                .executeCommand(eq(Integration.TS_GET_CONFIG_ERROR), anyString());
        protocol.updateConfigError(OutputChannelSnapshot.full(ACTIVE), 999_999_999L);
        assertEquals("Bad pin", protocol.getConfigErrorMessage());
        verify(protocol).executeCommand(eq(Integration.TS_GET_CONFIG_ERROR), anyString());

        protocol.updateConfigError(OutputChannelSnapshot.full(ACTIVE), 1_000_000_000L);
        assertEquals("New", protocol.getConfigErrorMessage());
        verify(protocol, times(2)).executeCommand(eq(Integration.TS_GET_CONFIG_ERROR), anyString());
    }

    @Test
    void clearResetsTheMessageAndAllowsImmediateRecurrence() {
        protocol.updateConfigError(OutputChannelSnapshot.full(new byte[5]), 0);
        verify(protocol, never()).executeCommand(eq(Integration.TS_GET_CONFIG_ERROR), anyString());
        protocol.updateConfigError(OutputChannelSnapshot.full(ACTIVE), 1);
        protocol.updateConfigError(OutputChannelSnapshot.full(new byte[5]), 2);
        assertNull(protocol.getConfigErrorMessage());
        protocol.updateConfigError(OutputChannelSnapshot.full(ACTIVE), 3);
        assertEquals("Bad pin", protocol.getConfigErrorMessage());
        verify(protocol, times(2)).executeCommand(eq(Integration.TS_GET_CONFIG_ERROR), anyString());
    }

    @Test
    void incompleteOrMissingIndicatorDoesNotReadOrClearError() throws Exception {
        OutputChannelSnapshot missing = new OutputChannelSnapshot(new byte[5], new BitSet(),
                Collections.singleton("rpm"), 0, false);
        protocol.updateConfigError(missing, 0);
        verify(protocol, never()).executeCommand(eq(Integration.TS_GET_CONFIG_ERROR), anyString());
        protocol.updateConfigError(OutputChannelSnapshot.full(ACTIVE), 1);
        protocol.updateConfigError(missing, 2_000_000_000L);
        assertEquals("Bad pin", protocol.getConfigErrorMessage());
        when(ini.getOutputChannel("hasCriticalError")).thenReturn(null);
        protocol.updateConfigError(OutputChannelSnapshot.full(ACTIVE), 3_000_000_000L);
        verify(protocol).executeCommand(eq(Integration.TS_GET_CONFIG_ERROR), anyString());
    }

    @Test
    void unavailableTextIsRetriedWithoutPublishingProtocolErrors() {
        byte[][] failures = {null, new byte[0], new byte[]{0}, new byte[]{(byte) 0x81, 'x'}};
        long now = 0;
        for (byte[] failure : failures) {
            doReturn(failure).when(protocol).executeCommand(eq(Integration.TS_GET_CONFIG_ERROR), anyString());
            protocol.updateConfigError(OutputChannelSnapshot.full(ACTIVE), now);
            assertNull(protocol.getConfigErrorMessage());
            now += 1_000_000_000L;
        }
        doReturn(new byte[]{0, 'O', 'k'}).when(protocol)
                .executeCommand(eq(Integration.TS_GET_CONFIG_ERROR), anyString());
        protocol.updateConfigError(OutputChannelSnapshot.full(ACTIVE), now);
        assertEquals("Ok", protocol.getConfigErrorMessage());
        verify(protocol, times(5)).executeCommand(eq(Integration.TS_GET_CONFIG_ERROR), anyString());
    }
}
