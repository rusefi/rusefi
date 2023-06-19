package com.rusefi.tune;

import com.opensr5.ini.IniFileModel;
import com.rusefi.*;
import com.rusefi.tools.tune.TuneCanTool;
import com.rusefi.tools.tune.TuneTools;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.Page;
import org.junit.Assert;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

public class LoadOlderTuneTest {
    @Test
    public void loadOlderTuneAgainstCurrentIni() throws Exception {
        Msq customOldTune = Msq.readTune(TuneReadWriteTest.TUNE_NAME);
        Page dataPage = customOldTune.findPage();
        assertFalse(dataPage.constant.isEmpty());

        Msq lessOldDefaultTune = Msq.readTune(TuneCanTool.SRC_TEST_RESOURCES + "simulator_tune-2023-06.xml");

        IniFileModel ini = new IniFileModel().readIniFile(TuneReadWriteTest.TEST_INI);
        assertFalse(ini.fieldsInUiOrder.isEmpty());

        RootHolder.ROOT = "../../firmware/";

        StringBuilder sb = TuneCanTool.getTunePatch(lessOldDefaultTune, customOldTune, ini);

        assertEquals("\t // default \"Single Coil\"\n" +
                "\tengineConfiguration->ignitionMode = IM_ONE_COIL;\n" +
                "\t // default 4.0\n" +
                "\tengineConfiguration->cylindersCount = 4;\n" +
                "\t // default 1000.0\n" +
                "\tengineConfiguration->tps1SecondaryMin = 0.0;\n" +
                "\t // default 0.0\n" +
                "\tengineConfiguration->tps1SecondaryMax = 1000.0;\n" +
                "\t // default 1000.0\n" +
                "\tengineConfiguration->tps2SecondaryMin = 0.0;\n" +
                "\t // default 0.0\n" +
                "\tengineConfiguration->tps2SecondaryMax = 1000.0;\n" +
                "\t // default 5.0\n" +
                "\tengineConfiguration->throttlePedalSecondaryUpVoltage = 0.0;\n" +
                "\t // default 65.0\n" +
                "\tengineConfiguration->mc33_hvolt = 0.0;\n" +
                "\t // default 13000.0\n" +
                "\tengineConfiguration->mc33_i_boost = 0.0;\n" +
                "\t // default 9400.0\n" +
                "\tengineConfiguration->mc33_i_peak = 0.0;\n" +
                "\t // default 3700.0\n" +
                "\tengineConfiguration->mc33_i_hold = 0.0;\n" +
                "\t // default 400.0\n" +
                "\tengineConfiguration->mc33_t_max_boost = 0.0;\n" +
                "\t // default 10.0\n" +
                "\tengineConfiguration->mc33_t_peak_off = 0.0;\n" +
                "\t // default 700.0\n" +
                "\tengineConfiguration->mc33_t_peak_tot = 0.0;\n" +
                "\t // default 10.0\n" +
                "\tengineConfiguration->mc33_t_bypass = 0.0;\n" +
                "\t // default 60.0\n" +
                "\tengineConfiguration->mc33_t_hold_off = 0.0;\n" +
                "\t // default 10000.0\n" +
                "\tengineConfiguration->mc33_t_hold_tot = 0.0;\n" +
                "\t // default \"PULLUP\"\n" +
                "\tengineConfiguration->clutchDownPinMode = 1.0;\n" +
                "\t // default \"PULLUP\"\n" +
                "\tengineConfiguration->clutchUpPinMode = 1.0;\n" +
                "\t // default 410.0\n" +
                "\tengineConfiguration->mapErrorDetectionTooHigh = 250.0;\n" +
                "\t // default 3.0\n" +
                "\tengineConfiguration->idleStepperReactionTime = 10.0;\n" +
                "\t // default 200.0\n" +
                "\tengineConfiguration->idleStepperTotalSteps = 150.0;\n" +
                "\t // default \"true\"\n" +
                "\tengineConfiguration->stepperForceParkingEveryRestart = false;\n" +
                "\t // default -20.0\n" +
                "\tengineConfiguration->idlerpmpid_iTermMin = -200.0;\n" +
                "\t // default 20.0\n" +
                "\tengineConfiguration->idlerpmpid_iTermMax = 200.0;\n" +
                "\t // default 300.0\n" +
                "\tengineConfiguration->idlePidRpmUpperLimit = 0.0;\n" +
                "\t // default 5.0\n" +
                "\tengineConfiguration->idlePidDeactivationTpsThreshold = 2.0;\n" +
                "\t // default 0.0\n" +
                "\tengineConfiguration->warningPeriod = 10.0;\n" +
                "\t // default \"false\"\n" +
                "\tengineConfiguration->isHip9011Enabled = true;\n" +
                "\t // default 0.0\n" +
                "\tengineConfiguration->hip9011PrescalerAndSDO = 6.0;\n" +
                "\t // default 20.0\n" +
                "\tengineConfiguration->knockDetectionWindowStart = 35.0;\n" +
                "\t // default 60.0\n" +
                "\tengineConfiguration->knockDetectionWindowEnd = 135.0;\n" +
                "\t // default \"false\"\n" +
                "\tengine", sb.substring(0, 2200));
    }

    @Test(expected = IllegalStateException.class)
    public void testLegacyCustomEnumOrdinal() {
        String tsCustomLine = "bits, U08, @OFFSET@, [0:1], \"Single Coil\", \"Individual Coils\", \"Wasted Spark\", \"Two Distributors\"";

        Assert.assertEquals(0, TuneTools.resolveEnumByName(tsCustomLine, "One coil"));
    }

    @Test
    public void testCustomEnumOrdinal() {
        String tsCustomLine = "bits, U08, @OFFSET@, [0:1], \"Single Coil\", \"Individual Coils\", \"Wasted Spark\", \"Two Distributors\"";

        Assert.assertEquals(0, TuneTools.resolveEnumByName(tsCustomLine, "Single coil"));
        Assert.assertEquals(3, TuneTools.resolveEnumByName(tsCustomLine, "Two Distributors"));
    }
}
