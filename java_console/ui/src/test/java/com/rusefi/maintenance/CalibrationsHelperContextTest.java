package com.rusefi.maintenance;

import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.binaryprotocol.IniFileProvider;
import com.opensr5.ConfigurationImageMeta;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.rusefi.config.generated.Integration;
import com.rusefi.io.LinkManager;
import com.rusefi.io.IoStream;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Optional;
import java.util.Set;

import static com.rusefi.maintenance.CalibrationsHelper.isUiContext;
import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoInteractions;
import static org.mockito.Mockito.when;

public class CalibrationsHelperContextTest {

    @Test
    public void testIsUiContext_withDummyCallbacks() {
        assertFalse(isUiContext(UpdateOperationCallbacks.DUMMY), "DUMMY callbacks should not be considered UI context");
    }

    @Test
    public void testIsUiContext_withLoggerCallbacks() {
        assertFalse(isUiContext(UpdateOperationCallbacks.LOGGER), "LOGGER callbacks should not be considered UI context");
    }

    @Test
    public void testIsUiContext_withConsoleCallbacks() {
        assertTrue(isUiContext(UpdateOperationCallbacks.CONSOLE), "CONSOLE callbacks should be considered UI context");
    }

    @Test
    public void testIsUiContext_withCustomCallbacks() {
        UpdateOperationCallbacks customCallbacks = new UpdateOperationCallbacks() {
            @Override
            public void log(String message, boolean breakLineOnTextArea, boolean sendToLogger) {
            }

            @Override
            public void done() {
            }

            @Override
            public void warning() {
            }

            @Override
            public void error() {
            }

            @Override
            public void clear() {
            }
        };

        assertTrue(isUiContext(customCallbacks), "Custom callbacks should be considered UI context");
    }

    @Test
    public void customHeadlessCallbacksDoNotOpenMigrationDialogs() {
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        when(callbacks.isInteractive()).thenReturn(false);
        assertFalse(isUiContext(callbacks));
    }

    @Test
    public void specialTransportDoesNotNeedAnOsSerialPort() {
        final Set<String> noSerialPorts = Collections.emptySet();

        assertTrue(CalibrationsHelper.isPortPresent(
            new PortResult(LinkManager.SOCKET_CAN, SerialPortType.Ecu), noSerialPorts));
        assertTrue(CalibrationsHelper.isPortPresent(
            new PortResult(LinkManager.PCAN, SerialPortType.Ecu), noSerialPorts));
        assertFalse(CalibrationsHelper.isPortPresent(
            new PortResult("COM5", SerialPortType.Ecu), noSerialPorts));
        assertTrue(CalibrationsHelper.isPortPresent(
            new PortResult("COM5", SerialPortType.Ecu), Collections.singleton("COM5")));
    }

    @Test
    public void firmwareHandoffHookRunsBeforeDisconnect() {
        List<String> events = new ArrayList<>();
        LinkManager linkManager = mock(LinkManager.class);
        doAnswer(invocation -> {
            events.add("disconnect");
            return null;
        }).when(linkManager).disconnect();

        assertTrue(CalibrationsHelper.prepareFirmwareHandoff(
            mock(BinaryProtocol.class), linkManager, () -> {
                events.add("handoff");
                return true;
            }));

        assertEquals(Arrays.asList("handoff", "disconnect"), events);
    }

    @Test
    public void failedFirmwareHandoffDoesNotDisconnect() {
        LinkManager linkManager = mock(LinkManager.class);

        assertFalse(CalibrationsHelper.prepareFirmwareHandoff(
            mock(BinaryProtocol.class), linkManager, () -> false));

        verifyNoInteractions(linkManager);
    }

    @Test
    public void failedMainImageReadDoesNotReturnAnExportableCalibration() throws Exception {
        final String signature = "test-signature";
        IniFileProvider previousProvider = BinaryProtocol.iniFileProvider;
        try {
            IniFileModel ini = mock(IniFileModel.class);
            IniFileMetaInfo metaInfo = mock(IniFileMetaInfo.class);
            when(ini.getMetaInfo()).thenReturn(metaInfo);
            when(metaInfo.getPageSize(0)).thenReturn(4);
            when(metaInfo.getnPages()).thenReturn(1);
            BinaryProtocol.iniFileProvider = requestedSignature -> {
                assertEquals(signature, requestedSignature);
                return ini;
            };

            IoStream stream = mock(IoStream.class);
            IncomingDataBuffer buffer = mock(IncomingDataBuffer.class);
            when(stream.getDataBuffer()).thenReturn(buffer);
            doReturn(helloResponse(signature)).when(buffer).getPacket(anyInt(), anyString());

            BinaryProtocol binaryProtocol = mock(BinaryProtocol.class);
            when(binaryProtocol.getStream()).thenReturn(stream);
            when(binaryProtocol.readFullImageFromController(any(ConfigurationImageMeta.class)))
                .thenReturn(ConfigurationImageWithMeta.VOID);

            Optional<CalibrationsInfo> result = CalibrationsHelper.readCurrentCalibrations(
                binaryProtocol, UpdateOperationCallbacks.DUMMY);

            // #10138: a failed main image must not become a tune backup.
            assertFalse(result.isPresent());
            verify(binaryProtocol).readFullImageFromController(any(ConfigurationImageMeta.class));
        } finally {
            BinaryProtocol.iniFileProvider = previousProvider;
        }
    }

    private static byte[] helloResponse(String signature) {
        byte[] response = new byte[signature.length() + 2];
        response[0] = Integration.TS_RESPONSE_OK;
        System.arraycopy(signature.getBytes(java.nio.charset.StandardCharsets.US_ASCII), 0,
            response, 1, signature.length());
        return response;
    }
}
