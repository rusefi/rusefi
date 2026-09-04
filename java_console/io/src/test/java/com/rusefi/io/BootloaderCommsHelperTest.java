package com.rusefi.io;

import org.junit.jupiter.api.Test;

import java.io.IOException;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.mock;

class BootloaderCommsHelperTest {
    @Test
    void reportsWhetherTheRebootPacketWasSent() throws IOException {
        IoStream stream = mock(IoStream.class);
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);

        assertTrue(BootloaderCommsHelper.sendBootloaderRebootCommand(stream, callbacks, "reboot_openblt"));

        doThrow(new IOException("write failed")).when(stream).sendPacket(any(byte[].class));
        assertFalse(BootloaderCommsHelper.sendBootloaderRebootCommand(stream, callbacks, "reboot_openblt"));
    }
}
