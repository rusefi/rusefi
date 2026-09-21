package com.rusefi.maintenance;

import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Set;

import static com.rusefi.maintenance.CalibrationsHelper.isUiContext;
import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verifyNoInteractions;

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
    public void specialTransportDoesNotNeedAnOsSerialPort() {
        final Set<String> noSerialPorts = Collections.emptySet();

        assertTrue(CalibrationsHelper.isPortPresent(
            new PortResult(LinkManager.SOCKET_CAN, SerialPortType.Ecu), noSerialPorts));
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
}
