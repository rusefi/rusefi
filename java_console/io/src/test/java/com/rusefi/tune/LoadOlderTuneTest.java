package com.rusefi.tune;

import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.*;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.tools.tune.TuneTools;
import com.rusefi.output.ConfigStructure;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.Page;
import org.junit.Test;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import static com.rusefi.VariableRegistry.unquote;
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

        List<String> options = Files.readAllLines(Paths.get("../../" + ConfigDefinition.CONFIG_PATH));
        String[] totalArgs = options.toArray(new String[0]);

        RootHolder.ROOT = "../../firmware/";
        ReaderStateImpl state = new ReaderStateImpl();
        ConfigDefinition.doJob(totalArgs, state);

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
                ConfigStructure s = state.getStructures().get("engine_configuration_s");
                System.out.println(s);
                ConfigField cf = s.getTsFieldByName(name);
                if (cf == null) {
                    System.out.println("Not found " + name);
                    continue;
                }

                if (cf.getType().equals("boolean")) {
                    // todo
                    continue;
                }


                if (!Node.isNumeric(customValue.getValue())) {
                    // todo: smarter logic for enums

                    String type = cf.getType();
                    if (isHardwareEnum(type)) {
                        continue;
                    }
                    EnumsReader.EnumState sourceCodeEnum = state.getEnumsReader().getEnums().get(type);
                    if (sourceCodeEnum == null) {
                        System.out.println("No info for " + type);
                        continue;
                    }
                    String customEnum = state.getTsCustomLine().get(type);

                    int ordinal;
                    try {
                        ordinal = TuneTools.resolveEnumByName(customEnum, unquote(customValue.getValue()));
                    } catch (IllegalStateException e) {
                        System.out.println("Looks like things were renamed: " + customValue.getValue() + " not found in " + customEnum);
                        continue;
                    }

                    System.out.println(cf + " " + sourceCodeEnum + " " + customEnum + " " + ordinal);

                    String sourceCodeValue = sourceCodeEnum.findByValue(ordinal);
                    sb.append(TuneTools.getAssignmentCode(customValue.getName(), sourceCodeValue));

                    continue;
                }
                sb.append(TuneTools.getAssignmentCode(customValue.getName(), customValue.getValue()));
            }
        }

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
                "\tengineConfiguration->mc33_i_hold ", sb.substring(0, 500));
    }

    private static boolean isHardwareEnum(String type) {
        switch (type) {
            case "output_pin_e":
            case "brain_input_pin_e":
            case "adc_channel_e":
            case "Gpio":
                return true;
        }
        return false;
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
