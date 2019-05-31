package com.rusefi.board_generator;

import com.rusefi.EnumToString;
import com.rusefi.EnumsReader;
import com.rusefi.enum_reader.Value;
import org.yaml.snakeyaml.Yaml;

import java.io.*;
import java.util.Collection;
import java.util.Map;

public class BoardReader {
    private static final String KEY_BOARD_NAME = "-board";
    private static final String KEY_OUTFOLDER = "-out";
    private static final String KEY_FIRMWARE_PATH = "-firmware_path";
    private static final String INVALID = "INVALID";

    public static void main(String[] args) throws IOException {
        if (args.length < 2) {
            System.out.println("Please specify\r\n"
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
            } else if (key.equals(KEY_OUTFOLDER)) {
                outputPath = args[i + 1];
            }
        }

        Yaml yaml = new Yaml();
        Map<String, Object> data = yaml.load(new FileReader(firmwarePath + "/config/boards/" + boardName + "/mapping.yaml"));
        System.out.println(data);

        EnumsReader.process(new FileReader(firmwarePath + File.separator + EnumToString.RELATIVE_PATH));

        BufferedWriter bw = new BufferedWriter(new FileWriter(outputPath + File.separator + boardName + "_prefix.txt"));

        bw.write(processSection(data, "brain_pin_e", "outputs", "GPIO_UNASSIGNED"));
        bw.write(processSection(data, "adc_channel_e", "analog_inputs", "EFI_ADC_NONE"));

        bw.close();
    }

    private static String processSection(Map<String, Object> data, String enumName, String sectionName, String NOTHING_NAME) {
        Map<String, Object> outputs = (Map<String, Object>) data.get(sectionName);

        Map<String, Value> s = EnumsReader.enums.get(enumName);
        System.out.println(s.size());

        StringBuffer sb = new StringBuffer();

        for (int i = 0; i < 255; i++) {
            if (sb.length() > 0)
                sb.append(",");

            String code;

            Value v = findByOrdinal(i, s.values());

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

        return " #define " + enumName + "_enum " + sb + "\r\n";
    }

    private static Value findByOrdinal(int ordinal, Collection<Value> values) {
        for (Value v : values)
            if (v.getValue().equals(String.valueOf(ordinal)))
                return v;
        return null;
    }
}
