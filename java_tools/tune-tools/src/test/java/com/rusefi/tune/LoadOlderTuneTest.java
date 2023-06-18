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

        StringBuilder sb = TuneCanTool.getTunePatch2(customOldTune, lessOldDefaultTune, ini);

        assertEquals("\tengineConfiguration->ignitionMode = IM_ONE_COIL;\n" +
                "\tengineConfiguration->cylindersCount = 4;\n" +
                "\tengineConfiguration->tps1SecondaryMin = 0.0;\n" +
                "\tengineConfiguration->tps1SecondaryMax = 1000.0;\n" +
                "\tengineConfiguration->tps2SecondaryMin = 0.0;\n" +
                "\tengineConfiguration->tps2SecondaryMax = 1000.0;\n" +
                "\tengineConfiguration->throttlePedalSecondaryUpVoltage = 0.0;\n" +
                "\tengineConfiguration->mc33_hvolt = 0.0;\n" +
                "\tengineConfiguration->mc33_i_boost = 0.0;\n" +
                "\tengineConfiguration->mc33_i_peak = 0.0;\n" +
                "\tengineConfiguration->mc33_i_hold = 0.0;\n" +
                "\tengineConfiguration->mc33_t_max_boost = 0.0;\n" +
                "\tengineConfiguration->mc33_t_peak_off = 0.0;\n" +
                "\tengineConfiguration->mc33_t_peak_tot = 0.0;\n" +
                "\tengineConfiguration->mc33_t_bypass = 0.0;\n" +
                "\tengineConfiguration->mc33_t_hold_off = 0.0;\n" +
                "\tengineConfiguration->mc33_t_hold_tot = 0.0;\n" +
                "\tengineConfiguration->clutchDownPinMode = 1.0;\n" +
                "\tengineConfiguration->clutchUpPinMode = 1.0;\n" +
                "\tengineConfiguration->mapErrorDetectionTooHigh = 250.0;\n" +
                "\tengineConfiguration->idleStepperReactionTime = 10.0;\n" +
                "\tengineConfiguration->idleStepperTotalSteps = 150.0;\n" +
                "\tengineConfiguration->stepperForceParkingEveryRestart = false;\n" +
                "\tengineConfiguration->idlerpmpid_iTermMin = -200.0;\n" +
                "\tengineConfiguration->idlerpmpid_iTermMax = 200.0;\n" +
                "\tengineConfiguration->idlePidRpmUpperLimit = 0.0;\n" +
                "\tengineConfiguration->idlePidDeactivationTpsThreshold = 2.0;\n" +
                "\tengineConfiguration->warningPeriod = 10.0;\n" +
                "\tengineConfiguration->isHip9011Enabled = true;\n" +
                "\tengineConfiguration->hip9011IntHoldPinMode = OM_OPENDRAIN;\n" +
                "\tengineConfiguration->hip9011CsPinMode = ", sb.substring(0, 1500));
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
