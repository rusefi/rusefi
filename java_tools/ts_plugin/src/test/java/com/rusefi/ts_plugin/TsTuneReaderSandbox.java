package com.rusefi.ts_plugin;

import com.rusefi.tune.xml.Msq;

import static com.rusefi.TsTuneReader.readTsTune;

/**
 * Manual harness that reads a TunerStudio tune for a given ECU project.
 */
public class TsTuneReaderSandbox {
    public static void main(String[] args) throws Exception {
        String ecuName = "dev";

        Msq tune = readTsTune(ecuName);
        System.out.println(tune);
    }
}
