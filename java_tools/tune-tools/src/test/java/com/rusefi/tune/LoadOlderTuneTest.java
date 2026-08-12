package com.rusefi.tune;

import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.opensr5.ini.IniMemberNotFound;
import com.opensr5.ini.field.ScalarIniField;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.*;
import com.rusefi.tools.tune.TuneCanTool;
import com.rusefi.tools.tune.TuneTools;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.Page;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.io.FileNotFoundException;

import static java.util.Collections.emptyMap;
import static org.junit.jupiter.api.Assertions.*;

public class LoadOlderTuneTest {

    @Test
    public void loadOlderTuneAgainstCurrentIni() throws Exception {
        Msq customOldTune = Msq.readTune(TuneReadWriteTest.TUNE_NAME);
        Page dataPage = customOldTune.findPage();
        assertFalse(dataPage.constant.isEmpty());

        Msq lessOldDefaultTune = Msq.readTune(LoadOlderTuneTest.class.getResource("/simulator_tune-2023-06.xml").getFile());

        IniFileModel ini = IniFileReaderUtil.readIniFile(TuneReadWriteTest.TEST_INI);
        assertEquals(256, ini.getBlockingFactor());
        assertFalse(ini.getFieldsInUiOrder().isEmpty());

        RootHolder.ROOT = "../../firmware/";

        // somewhere deep we have append prefix is not absolute path, so let's make path absolute
        TuneCanTool.boardPath = new File("../../firmware/" + "config/boards/hellen/uaefi/").getAbsolutePath() + File.separator;

        try {
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
                "    // default 20.0\n" +
                "    engineConfiguration->knockDetectionWindowStart = 35;\n" +
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
                "    // default \"Speed Density\"\n" +
                "    engineConfiguration->fuelAlgorithm = LM_SPEED_DENSITY;\n" +
                "    // default 300.0\n" +
                "    engineConfiguration->boostCutPressure = 0;\n" +
                "    // de", sb.substring(0, 3500));
        } catch (final Exception e) {
            System.err.print(String.format("LoadOlderTuneTest.loadOlderTuneAgainstCurrentIni: Exception: %s", e.getMessage()));
            e.printStackTrace();
            throw e;
        }
    }

    @Test
    public void testLegacyCustomEnumOrdinal() {
        Assertions.assertThrows(IllegalStateException.class, () -> {
            String tsCustomLine = "bits, U08, @OFFSET@, [0:1], \"Single Coil\", \"Individual Coils\", \"Wasted Spark\", \"Two Distributors\"";

            assertEquals(0, TuneTools.resolveEnumByName(tsCustomLine, "One coil", emptyMap()));
        });
    }

    @Test
    public void testCustomEnumOrdinal() {
        String tsCustomLine = "bits, U08, @OFFSET@, [0:1], \"Single Coil\", \"Individual Coils\", \"Wasted Spark\", \"Two Distributors\"";

        assertEquals(0, TuneTools.resolveEnumByName(tsCustomLine, "Single coil", emptyMap()));
        assertEquals(3, TuneTools.resolveEnumByName(tsCustomLine, "Two Distributors", emptyMap()));
    }

    @Test
    public void findFieldByName() throws IniMemberNotFound, FileNotFoundException {
        IniFileModel ini = IniFileReaderUtil.readIniFile(TuneReadWriteTest.TEST_INI);
        StringIniField make = (StringIniField) ini.getIniField("ENGINEMAKE");
        assertNotNull(make);
        ScalarIniField tps = (ScalarIniField) ini.getOutputChannel("RPMVALUE");
        assertNotNull(tps);
    }
}
