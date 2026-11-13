package com.rusefi.maintenance.jobs;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.io.IoStream;
import com.rusefi.maintenance.FirmwareSelection;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;

class OpenbltRebooterTest {
    @TempDir
    Path temp;

    @Test
    void freshSignatureChangeBlocksCanHandoffEvenWhenCachedSignatureIsUnchanged() throws Exception {
        String cached = "rusEFI development.2026.09.09.test_board.hash";
        Path srec = Files.write(temp.resolve(
            "rusefi_development_2026-09-09_test_board_123_hash_update.srec"),
            "srec bytes".getBytes(StandardCharsets.UTF_8));
        UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
        FirmwareSelection selection = FirmwareSelection.fromSrec(cached, srec.toString(), callbacks);
        assertNotNull(selection);

        IoStream stream = mock(IoStream.class);
        assertFalse(OpenbltRebooter.rebootWithFreshSignature(null, stream,
            "rusEFI lts-26lima.2026.09.09.test_board.hash", callbacks, selection));
        verify(stream, never()).sendPacket(org.mockito.ArgumentMatchers.any(byte[].class));
        verify(callbacks).firmwareUpdateBlocked("The connected ECU or its firmware has changed. Please restart the update.");
    }
}
