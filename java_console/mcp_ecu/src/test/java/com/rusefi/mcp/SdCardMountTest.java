package com.rusefi.mcp;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.FieldType;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.OutputChannelSnapshot;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.LinkManager;
import org.json.simple.JSONObject;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.Collections;
import java.util.Map;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeoutException;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.AdditionalMatchers.aryEq;
import static org.mockito.Mockito.*;

class SdCardMountTest {
    private final LinkManager link = mock(LinkManager.class);
    private final BinaryProtocol protocol = mock(BinaryProtocol.class);
    private final SensorCentral sensors = mock(SensorCentral.class);
    private final IniFileModel ini = mock(IniFileModel.class);
    private SensorCentral.SnapshotListener listener;

    @BeforeEach
    void setup() throws Exception {
        Map<String, IniField> channels = Map.of(
                "sdCardMode", new ScalarIniField("sdCardMode", 0, "", FieldType.UINT8, 1, "0", 0),
                "sd_present", new ScalarIniField("sd_present", 1, "", FieldType.UINT8, 1, "0", 0));
        when(ini.getAllOutputChannels()).thenReturn(channels);
        when(ini.getOutputChannel(anyString())).thenAnswer(call -> channels.get(call.getArgument(0)));
        when(link.getBinaryProtocol()).thenReturn(protocol);
        when(protocol.getIniFileNullable()).thenReturn(ini);
        when(link.isActive()).thenReturn(true);
        when(protocol.executeCommand(anyChar(), any(), anyString()))
                .thenReturn(new byte[]{(byte) Integration.TS_RESPONSE_OK});
        when(link.submit(any(Runnable.class))).thenAnswer(call -> {
            CompletableFuture<Void> future = new CompletableFuture<>();
            try {
                call.<Runnable>getArgument(0).run();
                future.complete(null);
            } catch (Exception e) {
                future.completeExceptionally(e);
            }
            return future;
        });
        when(sensors.addSnapshotListener(any())).thenAnswer(call -> {
            listener = call.getArgument(0);
            // Use a real removable token; no background polling or hardware required.
            return SensorCentral.getInstance().addSnapshotListener(snapshot -> { });
        });
    }

    private void report(int mode, int present) {
        listener.onSnapshot(OutputChannelSnapshot.full(new byte[]{0, (byte) mode, (byte) present}));
    }

    private void onPoll(int mode, int present) {
        when(link.isActive()).thenAnswer(call -> {
            report(mode, present);
            return true;
        });
    }

    @Test
    void ecuMountSucceedsWithoutAnyLoggerActivityChannel() throws Exception {
        onPoll(1, 1);
        JSONObject result = SdCardMount.mount(link, sensors, true, 1000);
        assertEquals(true, result.get("success"));
        assertEquals("ecu", result.get("mode"));
        assertEquals(1.0, result.get("sdCardMode"));
        verify(protocol).executeCommand(eq(Integration.TS_IO_TEST_COMMAND),
                aryEq(new byte[]{0, 0x14, 0, 0x18}), anyString());
        verify(sensors, never()).getCurrentSnapshot();
    }

    @Test
    void pcMountWaitsThroughIdleAndEcuModes() throws Exception {
        int[] polls = {0};
        when(link.isActive()).thenAnswer(call -> {
            report(Math.min(polls[0]++, 2), 1);
            return true;
        });
        JSONObject result = SdCardMount.mount(link, sensors, false, 1000);
        assertEquals(true, result.get("success"));
        assertEquals("pc", result.get("mode"));
        assertEquals(3, polls[0]);
        verify(protocol).executeCommand(eq(Integration.TS_IO_TEST_COMMAND),
                aryEq(new byte[]{0, 0x14, 0, 0x17}), anyString());
    }

    @Test
    void missingCardCannotConfirmMountEvenWithMatchingMode() throws Exception {
        onPoll(1, 0);
        JSONObject result = SdCardMount.mount(link, sensors, true, 20);
        assertEquals(false, result.get("success"));
        assertEquals(0.0, result.get("sd_present"));
    }

    @Test
    void cachedStatusCannotConfirmRequestWithoutNewPolls() throws Exception {
        when(sensors.getCurrentSnapshot()).thenReturn(OutputChannelSnapshot.full(new byte[]{0, 1, 1}));
        JSONObject result = SdCardMount.mount(link, sensors, true, 20);
        assertEquals(false, result.get("success"));
        assertFalse(result.containsKey("sdCardMode"));
    }

    @Test
    void disconnectDoesNotReportSuccessfulMount() throws Exception {
        when(link.isActive()).thenAnswer(call -> {
            report(1, 1);
            return false;
        });
        JSONObject result = SdCardMount.mount(link, sensors, true, 1000);
        assertEquals(false, result.get("success"));
        assertTrue(result.get("error").toString().contains("disconnected"));
    }

    @Test
    void unsupportedFirmwareDoesNotSendMountCommand() {
        when(ini.getAllOutputChannels()).thenReturn(Collections.emptyMap());
        assertThrows(IOException.class, () -> SdCardMount.mount(link, sensors, true, 1000));
        verify(link, never()).submit(any(Runnable.class));
    }

    @Test
    void rejectedCommandDoesNotWaitForStatus() {
        when(protocol.executeCommand(anyChar(), any(), anyString())).thenReturn(null);
        assertThrows(ExecutionException.class, () -> SdCardMount.mount(link, sensors, true, 1000));
        verify(sensors, never()).addSnapshotListener(any());
    }

    @Test
    void timedOutQueuedCommandIsCancelled() {
        CompletableFuture<Void> queued = new CompletableFuture<>();
        when(link.submit(any(Runnable.class))).thenReturn(queued);
        assertThrows(TimeoutException.class, () -> SdCardMount.mount(link, sensors, true, 20));
        assertTrue(queued.isCancelled());
    }
}
