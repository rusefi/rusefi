package com.rusefi.tools.tune;

import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.*;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.output.ConfigStructure;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import org.jetbrains.annotations.NotNull;

import javax.xml.bind.JAXBException;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.List;
import java.util.Objects;

import static com.rusefi.ConfigFieldImpl.unquote;
import static com.rusefi.config.Field.niceToString;
import static com.rusefi.tools.tune.WriteSimulatorConfiguration.INI_FILE_FOR_SIMULATOR;

public class TuneCanTool {
    public static final String SRC_TEST_RESOURCES = "src/test/resources/";
    private static final String FOLDER = "generated";
    public static final String SIMULATED_PREFIX = FOLDER + File.separator + "simulator_tune";
    public static final String SIMULATED_SUFFIX = ".xml";
    public static final String DEFAULT_TUNE = SIMULATED_PREFIX + SIMULATED_SUFFIX;
    private static final String workingFolder = "downloaded_tunes";

    private static Msq simulatorDefaultTune;
    private static IniFileModel ini;


    public static void main(String[] args) throws Exception {
        simulatorDefaultTune = Msq.readTune(TuneCanTool.DEFAULT_TUNE);
        ini = new IniFileModel().readIniFile(INI_FILE_FOR_SIMULATOR);
        if (ini == null)
            throw new IllegalStateException("Not found " + INI_FILE_FOR_SIMULATOR);

        RootHolder.ROOT = "../firmware/";

        handle("PB", 1502);
        handle("Mitsubicha", 1258);
        handle("Scion-1NZ-FE", 1448);
        handle("4g93", 1425);
        handle("BMW-mtmotorsport", 1479);
        handle("m111-alex", 1490);
    }

    private static void handle(String vehicleName, int tuneId) throws JAXBException, IOException {
        String localFileName = workingFolder + File.separator + tuneId + ".msq";
        String url = "https://rusefi.com/online/view.php?msq=" + tuneId;

        downloadTune(tuneId, localFileName);

        String reportsOutputFolder = "tune_reports";
        new File(reportsOutputFolder).mkdir();

        Msq custom = Msq.readTune(localFileName);
        StringBuilder sb = TuneCanTool.getTunePatch(simulatorDefaultTune, custom, ini);

        FileWriter w = new FileWriter(reportsOutputFolder + "/" + vehicleName + ".md");
        w.append("# " + vehicleName + "\n\n");
        w.append("Tune " + url + "\n\n");

        w.append("```\n");
        w.append(sb);
        w.append("```\n");
        w.close();
    }

    private static void downloadTune(int tuneId, String localFileName) throws IOException {
        new File(workingFolder).mkdirs();
        String downloadUrl = "https://rusefi.com/online/download.php?msq=" + tuneId;
        InputStream in = new URL(downloadUrl).openStream();
        Files.copy(in, Paths.get(localFileName), StandardCopyOption.REPLACE_EXISTING);
    }

    private static boolean isHardwareEnum(String type) {
        switch (type) {
            case "output_pin_e":
            case "brain_input_pin_e":
            case "adc_channel_e":
            case "Gpio":
            case "spi_device_e":
            case "pin_input_mode_e":
            case "pin_output_mode_e":
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
    public static StringBuilder getTunePatch(Msq defaultTune, Msq customTune, IniFileModel ini) throws IOException {
        List<String> options = Files.readAllLines(Paths.get(RootHolder.ROOT + "../" + ConfigDefinition.CONFIG_PATH));
        String[] totalArgs = options.toArray(new String[0]);

        ReaderStateImpl state = new ReaderStateImpl();
        ConfigDefinition.doJob(totalArgs, state);

        StringBuilder sb = new StringBuilder();
        for (DialogModel.Field f : ini.fieldsInUiOrder.values()) {
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
                String cName = cf.getOriginalArrayName();

                if (cf.getType().equals("boolean")) {
                    sb.append(TuneTools.getAssignmentCode(defaultValue, customValue.getName(), unquote(customValue.getValue())));
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
                    sb.append(TuneTools.getAssignmentCode(defaultValue, customValue.getName(), sourceCodeValue));

                    continue;
                }
                double doubleValue = Double.valueOf(customValue.getValue());
                int intValue = (int) doubleValue;
                boolean isInteger = intValue == doubleValue;
                if (isInteger) {
                    sb.append(TuneTools.getAssignmentCode(defaultValue, cName, Integer.toString(intValue)));
                } else {
                    sb.append(TuneTools.getAssignmentCode(defaultValue, cName, niceToString(doubleValue)));
                }
            }
        }
        return sb;
    }
}
