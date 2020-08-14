package com.rusefi.board_generator;

import com.rusefi.EnumToString;
import com.rusefi.EnumsReader;
import com.rusefi.enum_reader.Value;
import com.rusefi.util.LazyFile;
import com.rusefi.util.Output;
import com.rusefi.util.SystemOut;
import org.yaml.snakeyaml.Yaml;

import java.io.*;
import java.util.Collection;
import java.util.Map;
import java.util.Objects;

/**
 * This tool read mapping yaml file and produces a .txt file with defines in the rusefi.txt format
 *
 * This is a lazy to implement mapping as a separate phase of code generation. Technically this could be merged into
 * the primary generation to avoid the intermediate file.
 */
public class BoardReader {
    public static final String INVALID = "INVALID";

    private static final String KEY_BOARD_NAME = "-board";
    private static final String YAML_INPUT_NAME = "-yaml";
    private static final String OUTPUT_FILE_NAME = "-output_file";
    private static final String KEY_FIRMWARE_PATH = "-firmware_path";

    private static final String MAPPING_YAML = "mapping.yaml";

    public static void main(String[] args) throws IOException {
        if (args.length < 2) {
            SystemOut.println("Please specify\r\n"
                    + KEY_BOARD_NAME + " x\r\n"
                    + KEY_FIRMWARE_PATH + " x\r\n"
            );
            return;
        }
        String firmwarePath = "firmware";
        String yamlInputFile = null;
        String outputFileName = null;
        EnumsReader enumsReader = new EnumsReader();
        for (int i = 0; i < args.length - 1; i += 2) {
            String key = args[i];
            if (key.equals(KEY_BOARD_NAME)) {
                String boardName = args[i + 1];
                yamlInputFile = firmwarePath + "/config/boards/" + boardName + "/" + MAPPING_YAML;
            } else if (key.equals(OUTPUT_FILE_NAME)) {
                outputFileName = args[i + 1];
            } else if (key.equals(YAML_INPUT_NAME)) {
                yamlInputFile = args[i + 1];
            } else if (key.equals(KEY_FIRMWARE_PATH)) {
                firmwarePath = args[i + 1];
            } else if (key.equals(EnumToString.KEY_ENUM_INPUT_FILE)) {
                String inputFile = args[i + 1];
                enumsReader.process(firmwarePath, inputFile);
            }
        }

        Yaml yaml = new Yaml();
        Map<String, Object> data = yaml.load(new FileReader(yamlInputFile));
        if (data == null) {
            SystemOut.println("Null yaml for " + yamlInputFile);
        } else {
            SystemOut.println(data);

            Output bw = new LazyFile(outputFileName);

            bw.write(processSection(enumsReader, data, "brain_pin_e", "output_pin_e", "outputs", "GPIO_UNASSIGNED"));
            bw.write(processSection(enumsReader, data, "adc_channel_e", "adc_channel_e", "analog_inputs", "EFI_ADC_NONE"));

            bw.write(processSection(enumsReader, data, "brain_pin_e", "brain_input_pin_e", "event_inputs", "GPIO_UNASSIGNED"));
            bw.write(processSection(enumsReader, data, "brain_pin_e", "switch_input_pin_e", "switch_inputs", "GPIO_UNASSIGNED"));

            bw.close();
        }
        SystemOut.close();
    }

    private static String processSection(EnumsReader enumsReader, Map<String, Object> data, String headerEnumName, String outputEnumName, String sectionName, String NOTHING_NAME) {
        Objects.requireNonNull(data, "data");
        Map<String, Object> outputs = (Map<String, Object>) data.get(sectionName);
        if (outputs == null)
            return "";

        Objects.requireNonNull(data, "enums");
        Map<String, Value> enumMap = enumsReader.getEnums().get(headerEnumName);
        Objects.requireNonNull(enumMap, "enum for " + headerEnumName);
        SystemOut.println(enumMap.size());

        StringBuffer sb = new StringBuffer();

        int maxValue = getMaxValue(enumMap.values());

        for (int i = 0; i < maxValue; i++) {
            if (sb.length() > 0)
                sb.append(",");

            String code;

            Value v = findByOrdinal(i, enumMap.values());

            if (v == null) {
                code = INVALID;
            } else if (v.getName().equals(NOTHING_NAME)) {
                code = "NONE";
            } else if (outputs.containsKey(v.getName())) {
                code = (String) outputs.get(v.getName());
            } else {
                code = INVALID;
            }
            sb.append("\"" + code + "\"");
        }

        return " #define " + outputEnumName + "_enum " + sb + "\r\n";
    }

    private static int getMaxValue(Collection<Value> values) {
        int result = -1;
        for (Value v : values) {
            result = Math.max(result, v.getIntValue());
        }
        return result;
    }

    private static Value findByOrdinal(int ordinal, Collection<Value> values) {
        for (Value v : values) {
            if (v.getValue().equals(String.valueOf(ordinal))) {
                return v;
            }
        }
        return null;
    }
}
