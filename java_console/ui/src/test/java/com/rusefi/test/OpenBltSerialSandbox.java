package com.rusefi.test;

import com.rusefi.libopenblt.XcpSettings;
import com.rusefi.maintenance.OpenBltFlasher;
import com.rusefi.maintenance.ConsoleOpenbltCallbacks;

import java.io.IOException;

/**
 * we are trying to replace OpenbltJni
 */
public class OpenBltSerialSandbox {
    public static void main(String[] args) throws IOException {
        String port = "COM31";
        // this depends on ECU already being in OpenBLT mode!
        String filename = "C:\\stuff\\bundles\\rusefi_bundle_uaefi\\rusefi.snapshot.uaefi\\rusefi_update.srec";
        OpenBltFlasher.flashSerial(filename, port, ConsoleOpenbltCallbacks.CONSOLE);
    }
}
