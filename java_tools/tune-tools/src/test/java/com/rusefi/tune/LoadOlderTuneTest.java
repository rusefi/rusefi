package com.rusefi.tune;

import com.opensr5.ini.IniFileModel;
import com.rusefi.*;
import com.rusefi.tools.tune.TuneCanTool;
import com.rusefi.tools.tune.TuneCanToolConstants;
import com.rusefi.tools.tune.TuneTools;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.Page;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;

public class LoadOlderTuneTest {
    @Test
    public void loadOlderTuneAgainstCurrentIni() throws Exception {
        Msq customOldTune = Msq.readTune(TuneReadWriteTest.TUNE_NAME);
        Page dataPage = customOldTune.findPage();
        assertFalse(dataPage.constant.isEmpty());

        Msq lessOldDefaultTune = Msq.readTune(TuneCanToolConstants.SRC_TEST_RESOURCES + "simulator_tune-2023-06.xml");

        IniFileModel ini = new IniFileModel().readIniFile(TuneReadWriteTest.TEST_INI);
        assertFalse(ini.fieldsInUiOrder.isEmpty());

        RootHolder.ROOT = "../../firmware/";

        StringBuilder sb = TuneCanTool.getTunePatch(lessOldDefaultTune, customOldTune, ini, TuneReadWriteTest.TUNE_NAME, new StringBuilder(), null, "");

        assertEquals("    // default \"Single Coil\"\n" +
            "    engineConfiguration->ignitionMode = IM_ONE_COIL;\n" +
            "    // default 4.0\n" +
            "    engineConfiguration->cylindersCount = 4;\n" +
            "    // default \"60/2\"\n" +
            "    engineConfiguration->trigger.type = TT_ONE_PLUS_ONE;\n" +
            "    // default 1000.0\n" +
            "    engineConfiguration->tps1SecondaryMin = 0;\n" +
            "    // default 0.0\n" +
            "    engineConfiguration->tps1SecondaryMax = 1000;\n" +
            "    // default 1000.0\n" +
            "    engineConfiguration->tps2SecondaryMin = 0;\n" +
            "    // default 0.0\n" +
            "    engineConfiguration->tps2SecondaryMax = 1000;\n" +
            "    // default 5.0\n" +
            "    engineConfiguration->throttlePedalSecondaryUpVoltage = 0;\n" +
            "    // default 65.0\n" +
            "    engineConfiguration->mc33_hvolt = 0;\n" +
            "    // default 13000.0\n" +
            "    engineConfiguration->mc33_i_boost = 0;\n" +
            "    // default 9400.0\n" +
            "    engineConfiguration->mc33_i_peak = 0;\n" +
            "    // default 3700.0\n" +
            "    engineConfiguration->mc33_i_hold = 0;\n" +
            "    // default 400.0\n" +
            "    engineConfiguration->mc33_t_max_boost = 0;\n" +
            "    // default 10.0\n" +
            "    engineConfiguration->mc33_t_peak_off = 0;\n" +
            "    // default 700.0\n" +
            "    engineConfiguration->mc33_t_peak_tot = 0;\n" +
            "    // default 10.0\n" +
            "    engineConfiguration->mc33_t_bypass = 0;\n" +
            "    // default 60.0\n" +
            "    engineConfiguration->mc33_t_hold_off = 0;\n" +
            "    // default 10000.0\n" +
            "    engineConfiguration->mc33_t_hold_tot = 0;\n" +
            "    // default \"PULLUP\"\n" +
            "    engineConfiguration->clutchDownPinMode = 1;\n" +
            "    // default \"PULLUP\"\n" +
            "    engineConfiguration->clutchUpPinMode = 1;\n" +
            "    // default 410.0\n" +
            "    engineConfiguration->mapErrorDetectionTooHigh = 250;\n" +
            "    // default 3.0\n" +
            "    engineConfiguration->idleStepperReactionTime = 10;\n" +
            "    // default 200.0\n" +
            "    engineConfiguration->idleStepperTotalSteps = 150;\n" +
            "    // default \"true\"\n" +
            "    engineConfiguration->stepperForceParkingEveryRestart = false;\n" +
            "    // default -20.0\n" +
            "    engineConfiguration->idleRpmPid.minValue = 0;\n" +
            "    // default 20.0\n" +
            "    engineConfiguration->idleRpmPid.maxValue = 99;\n" +
            "    // default -20.0\n" +
            "    engineConfiguration->idlerpmpid_iTermMin = -200;\n" +
            "    // default 20.0\n" +
            "    engineConfiguration->idlerpmpid_iTermMax = 200;\n" +
            "    // default 0.0\n" +
            "    engineConfiguration->idleRpmPid.periodMs = 10;\n" +
            "    // default 300.0\n" +
            "    engineConfiguration->idlePidRpmUpperLimit = 0;\n" +
            "    // default 5.0\n" +
            "    engineConfiguration->idlePidDeactivationTpsThreshold = 2;\n" +
            "    // default 0.10000000149011612\n" +
            "    engineConfiguration->idleTimingPid.pFactor = 0;\n" +
            "    // default -10.0\n" +
            "    engineConfiguration->idleTimingPid.minValue = 0;\n" +
            "    // default 10.0\n" +
            "    engineConfiguration->idleTimingPid.maxValue = 0;\n" +
            "    // default \"false\"\n" +
            "    engineConfiguration->isHip9011Enabled = true;\n" +
            "    // default 20.0\n" +
            "    engineConfiguration->knockDetectionWindowStart = 35;\n" +
            "    // default 60.0\n" +
            "    engineConfiguration->knockDetectionWindowEnd = 135;\n" +
            "    // default 33.0\n" +
            "    engineConfiguration->auxPid[0].offset = 0;\n" +
            "    // default 2.0\n" +
            "    engineConfiguration->auxPid[0].pFactor = 0;\n" +
            "    // default 0.004999999888241291\n" +
            "    engineConfiguration->auxPid[0].iFactor = 0;\n" +
            "    // default 0.0\n" +
            "    engineConfiguration->alternatorControl.minValue = 10;\n" +
            "    // default \"DEFAULT\"\n" +
            "    engineConfiguration->startStopButtonMode = 0;\n" +
            "    // default 3.0\n" +
            "    engineConfiguration->startCrankingDuration = 7;\n" +
            "    // default \"Duty cycle\"\n" +
            "    engineConfiguration->tachPulseDurationAsDutyCycle = false;\n" +
            "    // default 0.5\n" +
            "    engineConfiguration->tachPulseDuractionMs = 4;\n" +
            "    // default 1.0\n" +
            "    engineConfiguration->tachPulsePerRev = 0;\n" +
            "    // default \"true\"\n" +
            "    engineConfiguration->canReadEnabled = false;\n" +
            "    // default \"None\"\n" +
            "    engineConfiguration->canNbcType = CAN_BUS_MAZDA_RX8;\n" +
            "    // default \"Sp",
            sb.substring(0, 3500));
    }

    @Test
    public void testLegacyCustomEnumOrdinal() {
        Assertions.assertThrows(IllegalStateException.class, () -> {
            String tsCustomLine = "bits, U08, @OFFSET@, [0:1], \"Single Coil\", \"Individual Coils\", \"Wasted Spark\", \"Two Distributors\"";

            assertEquals(0, TuneTools.resolveEnumByName(tsCustomLine, "One coil"));
        });
    }

    @Test
    public void testCustomEnumOrdinal() {
        String tsCustomLine = "bits, U08, @OFFSET@, [0:1], \"Single Coil\", \"Individual Coils\", \"Wasted Spark\", \"Two Distributors\"";

        assertEquals(0, TuneTools.resolveEnumByName(tsCustomLine, "Single coil"));
        assertEquals(3, TuneTools.resolveEnumByName(tsCustomLine, "Two Distributors"));
    }
}
