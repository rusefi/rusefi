package com.rusefi.tune;

import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.tools.tune.TuneTools;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.Page;
import org.junit.Test;

import static com.rusefi.tune.TuneReadWriteTest.SRC_TEST_RESOURCES;
import static com.rusefi.tune.TuneReadWriteTest.TEST_INI;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

public class LoadOlderTuneTest {
    @Test
    public void loadOlderTuneAgainstCurrentIni() throws Exception {
        Msq customOldTune = Msq.readTune(TuneReadWriteTest.TUNE_NAME);
        Page dataPage = customOldTune.findPage();
        assertFalse(dataPage.constant.isEmpty());

        Msq lessOldDefaultTune = Msq.readTune(SRC_TEST_RESOURCES + "simulator_tune-2023-06.xml");

        IniFileModel ini = new IniFileModel().readIniFile(TEST_INI);
        assertFalse(ini.fieldsInUiOrder.isEmpty());

        StringBuilder sb = new StringBuilder();
        for (DialogModel.Field f : ini.fieldsInUiOrder) {
            String name = f.getKey();
            Constant customValue = customOldTune.getConstantsAsMap().get(name);
            Constant defaultValue = lessOldDefaultTune.getConstantsAsMap().get(name);
            if (defaultValue == null) {
                // no longer present
                continue;
            }

            boolean isSameValue = simplerSpaces(defaultValue.getValue()).equals(simplerSpaces(customValue.getValue()));
            if (!isSameValue) {
                if (!Node.isNumeric(customValue.getValue())) {
                    // todo: smarter logic for enums
                    continue;
                }
                sb.append("\tengineConfiguration->" + customValue.getName() + " = " + customValue.getValue() + ";\n");
            }
        }

        assertEquals("\tengineConfiguration->cylindersCount = 4;\n" +
                "\tengineConfiguration->tps1SecondaryMin = 0.0;\n" +
                "\tengineConfiguration->tps1SecondaryMax = 1000.0;\n" +
                "\tengineConfiguration->tps2SecondaryMin = 0.0;\n" +
                "\tengineConfiguration->tps2SecondaryMax = 1000.0;\n" +
                "\tengineConfiguration->throttlePedalSecondaryUpVoltage = 0.0;\n" +
                "\tengineConfiguration->mc33_hvolt = 0.0;\n" +
                "\tengineConfiguration->mc33_i_boost = 0.0;\n" +
                "\tengineConfiguration->mc33_i_peak = 0.0;\n" +
                "\tengineConfiguration->mc33_i_hold = 0.0;\n" +
                "\tengineConfiguration->mc33_t_max_boost = 0.", sb.substring(0, 500));
    }

    private static Object simplerSpaces(String value) {
        if (value == null)
            return value;
        return value.replaceAll("\\s+", " ").trim();
    }

    @Test(expected = IllegalStateException.class)
    public void testLegacyCustomEnumOrdinal() {
        String tsCustomLine = "bits, U08, @OFFSET@, [0:1], \"Single Coil\", \"Individual Coils\", \"Wasted Spark\", \"Two Distributors\"";

        assertEquals(0, TuneTools.resolveEnumByName(tsCustomLine, "One coil"));
    }

    @Test
    public void testCustomEnumOrdinal() {
        String tsCustomLine = "bits, U08, @OFFSET@, [0:1], \"Single Coil\", \"Individual Coils\", \"Wasted Spark\", \"Two Distributors\"";

        assertEquals(0, TuneTools.resolveEnumByName(tsCustomLine, "Single coil"));
        assertEquals(3, TuneTools.resolveEnumByName(tsCustomLine, "Two Distributors"));
    }
}
