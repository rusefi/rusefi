package com.rusefi.board_generator;

import com.rusefi.EnumsReader;
import com.rusefi.enum_reader.Value;
import com.rusefi.util.SystemOut;
import org.yaml.snakeyaml.Yaml;

import java.io.*;
import java.util.Collection;
import java.util.Map;
import java.util.Objects;

public class BoardReader {
    private static final String KEY_BOARD_NAME = "-board";
    private static final String KEY_OUTFOLDER = "-out";
    private static final String KEY_FIRMWARE_PATH = "-firmware_path";
    private static final String INVALID = "INVALID";
    private final static String KEY_ENUM_INPUT_FILE = "-enumInputFile";

    public static void main(String[] args) throws IOException {
        if (args.length < 2) {
            SystemOut.println("Please specify\r\n"
                    + KEY_BOARD_NAME + " x\r\n"
                    + KEY_FIRMWARE_PATH + " x\r\n"
            );
            return;
        }

        String boardName = null;
        String firmwarePath = "firmware";
        String outputPath = ".";
        for (int i = 0; i < args.length - 1; i += 2) {
            String key = args[i];
            if (key.equals(KEY_BOARD_NAME)) {
                boardName = args[i + 1];
            } else if (key.equals(KEY_FIRMWARE_PATH)) {
                firmwarePath = args[i + 1];
            } else if (key.equals(KEY_ENUM_INPUT_FILE)) {
                String inputFile = args[i + 1];
                EnumsReader.process(new FileReader(firmwarePath + File.separator + inputFile));
            } else if (key.equals(KEY_OUTFOLDER)) {
                outputPath = args[i + 1];
            }
        }

        Yaml yaml = new Yaml();
        Map<String, Object> data = yaml.load(new FileReader(firmwarePath + "/config/boards/" + boardName + "/mapping.yaml"));
        SystemOut.println(data);


        BufferedWriter bw = new BufferedWriter(new FileWriter(outputPath + File.separator + boardName + "_prefix.txt"));

        bw.write(processSection(data, "brain_pin_e", "output_pin_e", "outputs", "GPIO_UNASSIGNED"));
        bw.write(processSection(data, "adc_channel_e", "adc_channel_e", "analog_inputs", "EFI_ADC_NONE"));

        bw.write(processSection(data, "brain_pin_e", "brain_input_pin_e", "event_inputs", "GPIO_UNASSIGNED"));
        bw.write(processSection(data, "brain_pin_e", "switch_input_pin_e", "switch_inputs", "GPIO_UNASSIGNED"));

        bw.close();
    }

    private static String processSection(Map<String, Object> data, String headerEnumName, String outputEnumName, String sectionName, String NOTHING_NAME) {
        Map<String, Object> outputs = (Map<String, Object>) data.get(sectionName);
        if (outputs == null)
            return "";

        Map<String, Value> enumMap = EnumsReader.enums.get(headerEnumName);
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
        for (Value v : values)
            result = Math.max(result, v.getIntValue());
        return result;
    }

    private static Value findByOrdinal(int ordinal, Collection<Value> values) {
        for (Value v : values)
            if (v.getValue().equals(String.valueOf(ordinal)))
                return v;
        return null;
    }
}
