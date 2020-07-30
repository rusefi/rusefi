package com.rusefi;

import java.io.IOException;

public class TS2CRunner {
    public static void main(String[] args) throws IOException {
        String tuneFileName = "CurrentTune.msq";

        TS2C.main(new String[]{tuneFileName, "afrLoadBins", "afrRpmBins", "afrTable"});
        TS2C.main(new String[]{tuneFileName, "fuelLoadBins", "fuelRpmBins", "fuelTable"});

    }
}
