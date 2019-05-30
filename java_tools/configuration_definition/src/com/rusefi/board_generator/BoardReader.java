package com.rusefi.board_generator;

import com.rusefi.EnumToString;
import com.rusefi.EnumsReader;
import com.rusefi.enum_reader.Value;
import org.yaml.snakeyaml.Yaml;

import java.io.FileReader;
import java.io.IOException;
import java.util.Collection;
import java.util.Map;

public class BoardReader {
    public static void main(String[] args) throws IOException {
        Yaml yaml = new Yaml();
        Map<String, Object> data = yaml.load(new FileReader("../../firmware/config/boards/microrusefi/mapping.yaml"));
        System.out.println(data);


        Map<String, Object> outputs = (Map<String, Object>) data.get("outputs");


        EnumsReader.process(new FileReader("../../firmware/" + EnumToString.RELATIVE_PATH));

        Map<String, Value> s = EnumsReader.enums.get("brain_pin_e");
        System.out.println(s.size());


        StringBuffer sb = new StringBuffer();

        for (int i = 0; i < 255; i++) {
            if (sb.length() > 0)
                sb.append(",");

            String code;

            Value v = findByOrdinal(i, s.values());

            if (v == null) {
                code = "INVALID";
            } else if (v.getName().equals("GPIO_UNASSIGNED")) {
                code = "NONE";
            } else if (outputs.containsKey(v.getName())) {
                code = (String) outputs.get(v.getName());
            } else {
                code = "INVALID";
            }
            sb.append("\"" + code + "\"");
        }

        System.out.println(" #define brain_pin_e_enum " + sb);

    }

    private static Value findByOrdinal(int ordinal, Collection<Value> values) {
        for (Value v : values)
            if (v.getValue().equals(String.valueOf(ordinal)))
                return v;
        return null;
    }
}
