package com.rusefi.test;

import com.rusefi.maintenance.OpenBltFlasher;
import com.rusefi.maintenance.OpenbltJni;

import java.io.IOException;

/**
 * we are trying to replace OpenbltJni
 */
public class OpenBltSerialSandbox {
    public static final OpenbltJni.OpenbltCallbacks CONSOLE = new OpenbltJni.OpenbltCallbacks() {
        @Override
        public void log(String line) {
            System.out.println(line);
        }

        @Override
        public void updateProgress(int percent) {
            System.out.println("Progress: " + percent + "%");
            System.out.flush();
        }

        @Override
        public void error(String line) {
            System.out.println("OpenbltJni Error: " + line);
        }

        @Override
        public void setPhase(String title, boolean hasProgress) {
            log("Begin phase: " + title);
        }
    };

    public static void main(String[] args) throws IOException {
        String port = "COM31";
        // this depends on ECU already being in OpenBLT mode!
        String filename = "C:\\stuff\\bundles\\rusefi_bundle_uaefi\\rusefi.snapshot.uaefi\\rusefi_update.srec";
        OpenBltFlasher.flashSerial(filename, port, CONSOLE);
    }
}
