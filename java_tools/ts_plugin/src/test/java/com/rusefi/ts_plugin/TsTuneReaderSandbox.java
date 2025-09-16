package com.rusefi.ts_plugin;

import com.rusefi.tune.xml.Msq;

import static com.rusefi.TsTuneReader.readTsTune;

public class TsTuneReaderSandbox {
    public static void main(String[] args) throws Exception {
        String ecuName = "dev";

        Msq tune = readTsTune(ecuName);
        System.out.println(tune);
    }
}
