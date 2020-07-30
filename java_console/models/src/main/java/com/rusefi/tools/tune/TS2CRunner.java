package com.rusefi.tools.tune;

import java.io.IOException;

public class TS2CRunner {
    public static void main(String[] args) throws IOException {
        String tuneFileName = "CurrentTune.msq";



        TS2C.main(new String[]{tuneFileName, "mafDecodingBins", "mafDecoding", "none"});

        TS2C.main(new String[]{tuneFileName, "crankingCycleBins", "crankingCycleCoef", "none"});
        TS2C.main(new String[]{tuneFileName, "crankingFuelBins", "crankingFuelCoef", "none"});

//        TS2C.main(new String[]{tuneFileName, "afrLoadBins", "afrRpmBins", "afrTable"});
//        TS2C.main(new String[]{tuneFileName, "fuelLoadBins", "fuelRpmBins", "fuelTable"});

    }
}
