package com.rusefi.io;

import com.rusefi.core.net.PropertiesHolder;
import com.rusefi.maintenance.FirmwareSelection;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Properties;

import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;

class BootloaderHelperTest {
    @TempDir
    Path temp;

    @Test
    void selectedFirmwareSkipsLegacyActiveBundleDownload() throws Exception {
        String target = "selection_test_board";
        String signature = "rusEFI development.2026.09.09." + target + ".hash";
        Path srec = Files.write(temp.resolve(
            "rusefi_development_2026-09-09_selection_test_board_123_hash_update.srec"),
            "srec bytes".getBytes(StandardCharsets.UTF_8));
        Properties properties = PropertiesHolder.INSTANCE.getProperties();
        String previous = properties.getProperty("board_compatibility");
        properties.setProperty("board_compatibility", target);
        try {
            UpdateOperationCallbacks callbacks = mock(UpdateOperationCallbacks.class);
            FirmwareSelection selection = FirmwareSelection.fromSrec(signature, srec.toString(), callbacks);
            IoStream stream = mock(IoStream.class);

            assertTrue(BootloaderHelper.sendBootloaderRebootCommand(null, signature, stream, callbacks,
                "reboot", selection));
            verify(stream).sendPacket(org.mockito.ArgumentMatchers.any(byte[].class));
        } finally {
            if (previous == null) {
                properties.remove("board_compatibility");
            } else {
                properties.setProperty("board_compatibility", previous);
            }
        }
    }
}
