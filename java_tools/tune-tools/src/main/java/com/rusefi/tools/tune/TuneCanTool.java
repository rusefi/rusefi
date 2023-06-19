package com.rusefi.tools.tune;

import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.*;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.output.ConfigStructure;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import org.jetbrains.annotations.NotNull;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;
import java.util.Objects;

import static com.rusefi.ConfigFieldImpl.unquote;
import static com.rusefi.tools.tune.WriteSimulatorConfiguration.INI_FILE_FOR_SIMULATOR;

public class TuneCanTool {
    public static final String SRC_TEST_RESOURCES = "src/test/resources/";
    private static final String FOLDER = "generated";
    public static final String DEFAULT_TUNE = FOLDER + File.separator + "simulator_tune.xml";

    public static void main(String[] args) throws Exception {
        IniFileModel ini = new IniFileModel().readIniFile(INI_FILE_FOR_SIMULATOR);
        if (ini == null)
            throw new IllegalStateException("Not found " + INI_FILE_FOR_SIMULATOR);
        Msq simulatorDefaultTune = Msq.readTune(TuneCanTool.DEFAULT_TUNE);

        RootHolder.ROOT = "../firmware/";


        String fileName = "C:\\Users\\Dell2019\\Downloads\\1258.msq";

        Msq custom = Msq.readTune(fileName);

        StringBuilder sb = TuneCanTool.getTunePatch2(simulatorDefaultTune, custom, ini);

        System.out.println(sb);

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

    @NotNull
    public static StringBuilder getTunePatch2(Msq defaultTune, Msq customTune, IniFileModel ini) throws IOException {
        List<String> options = Files.readAllLines(Paths.get(RootHolder.ROOT + "../" + ConfigDefinition.CONFIG_PATH));
        String[] totalArgs = options.toArray(new String[0]);

        ReaderStateImpl state = new ReaderStateImpl();
        ConfigDefinition.doJob(totalArgs, state);

        StringBuilder sb = new StringBuilder();
        for (DialogModel.Field f : ini.fieldsInUiOrder) {
            String name = f.getKey();
            Constant customValue = customTune.getConstantsAsMap().get(name);
            Constant defaultValue = defaultTune.getConstantsAsMap().get(name);
            if (defaultValue == null) {
                // no longer present
                continue;
            }
            Objects.requireNonNull(defaultValue.getValue(), "d value");
            if (customValue == null) {
                System.out.println("Skipping " + name + " TODO");
                continue;
            }
            Objects.requireNonNull(customValue.getValue(), "c value");

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
                    sb.append(TuneTools.getAssignmentCode(customValue.getName(), unquote(customValue.getValue())));
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
        return sb;
    }
}
