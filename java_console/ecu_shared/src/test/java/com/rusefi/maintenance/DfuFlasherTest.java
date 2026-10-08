package com.rusefi.maintenance;

import org.junit.jupiter.api.Test;

import java.io.File;
import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class DfuFlasherTest {
    @Test
    public void linuxWriteCommandTargetsStm32FlashAndLeavesDfu() {
        File firmware = new File("directory with spaces", "rusefi.bin");

        assertEquals(Arrays.asList(
            "dfu-util",
            "--device", "0483:df11",
            "--alt", "0",
            "--dfuse-address", "0x08000000:leave",
            "--download", firmware.getAbsolutePath()
        ), DfuFlasher.getDfuUtilWriteCommand(firmware.getPath()));
    }

    @Test
    public void linuxEraseCommandRequiresExplicitMassEraseForce() {
        assertEquals(Arrays.asList(
            "dfu-util",
            "--device", "0483:df11",
            "--alt", "0",
            "--dfuse-address", "0x08000000:mass-erase:force"
        ), DfuFlasher.getDfuUtilEraseCommand());
    }

    @Test
    public void explicitManualFirmwareDoesNotResolveConnectedTarget() {
        assertEquals("custom.bin", DfuFlasher.resolveManualFirmwareFile("custom.bin", null));
    }
}
