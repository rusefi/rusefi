package com.rusefi.tools.tune;

import java.io.IOException;

public class TS2CRunner {
    public static void main(String[] args) throws IOException {
        String tuneFileName = "CurrentTune.msq";



        TS2C.main(new String[]{tuneFileName, "mafDecodingBins", "mafDecoding"});
        TS2C.main(new String[]{tuneFileName, "cltIdleRpmBins", "cltIdleRpm"});
        TS2C.main(new String[]{tuneFileName, "cltIdleCorrBins", "cltIdleCorr"});

        TS2C.main(new String[]{tuneFileName, "etbBiasBins", "etbBiasValues"});

        TS2C.main(new String[]{tuneFileName, "crankingCycleBins", "crankingCycleCoef"});
        TS2C.main(new String[]{tuneFileName, "crankingFuelBins", "crankingFuelCoef"});

        TS2C.main(new String[]{tuneFileName, "iacCoastingBins", "iacCoasting"});
        TS2C.main(new String[]{tuneFileName, "cltFuelCorrBins", "cltFuelCorr"});

        TS2C.main(new String[]{tuneFileName, "veLoadBins", "veRpmBins", "veTable"});
        TS2C.main(new String[]{tuneFileName, "ignitionLoadBins", "ignitionRpmBins", "ignitionTable"});

//        TS2C.main(new String[]{tuneFileName, "afrLoadBins", "afrRpmBins", "afrTable"});
//        TS2C.main(new String[]{tuneFileName, "fuelLoadBins", "fuelRpmBins", "fuelTable"});

    }
}
