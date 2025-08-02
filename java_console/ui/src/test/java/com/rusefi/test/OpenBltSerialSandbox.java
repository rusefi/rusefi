package com.rusefi.test;

import com.rusefi.libopenblt.XcpSettings;
import com.rusefi.maintenance.OpenBltFlasher;
import com.rusefi.maintenance.OpenbltCallbacks;

import java.io.IOException;

public class OpenBltSerialSandbox {
    public static void main(String[] args) throws IOException {
        String port = "COM31";
        OpenBltFlasher f = OpenBltFlasher.makeSerial(port, new XcpSettings(), OpenbltCallbacks.CONSOLE);
        f.flash("C:\\stuff\\bundles\\rusefi_bundle_uaefi\\rusefi.snapshot.uaefi\\rusefi_update.srec");
    }
}
