package rusefi;

import com.devexperts.logging.Logging;

import java.io.*;
import java.nio.file.*;
import java.text.ParseException;
import java.util.*;

import org.jetbrains.annotations.NotNull;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import java.io.FileInputStream;
import java.io.InputStream;
import java.util.List;
import java.util.Map;

import java.io.FileReader;

import static com.devexperts.logging.Logging.getLogging;


public class GenerateEnum {
    private static final Logging log = getLogging(GenerateEnum.class);

    private static final String shebang = "!#!";
    private static final String definitionStart = "generate_enum_start";
    private static final String definitionEnd = "generate_enum_end";

    public static void main(String[] args) throws IOException {
        String definitionPath = System.getProperty("definition");
        String outputPath = System.getProperty("outputPath");

        log.info("Definition file: " + definitionPath);

        if (definitionPath == null || outputPath == null) {
            log.error("Usage: -Ddefinition=... -DoutputPath=...");
            System.exit(1);
        }

        List<String> lines = Files.readAllLines(Paths.get(definitionPath));
        List<String> enums = new ArrayList<>();

        boolean inDefinition = false;

        StringBuilder definitionBlock = null;

        for (String lineRaw : lines) {
            String line = lineRaw.trim();
            if (!line.startsWith(shebang)) {
                continue;
            }

            line = line.substring(shebang.length());

            if (line.startsWith(definitionStart)) {
                if (inDefinition) {
                    log.error("Unexpected " + definitionStart + " found at line number " + (lines.indexOf(lineRaw)+1) + " while already in definition block!" );
                    System.exit(1);
                }

                inDefinition = true;
                definitionBlock = new StringBuilder();
                continue;
            }

            if (line.startsWith(definitionEnd)) {
                if (!inDefinition) {
                    log.error("Unexpected " + definitionEnd + " found at line number " + (lines.indexOf(lineRaw) + 1) + " while not in definition block!" );
                    System.exit(1);
                }
                inDefinition = false;
                if (definitionBlock.length() == 0) {
                    log.error("Empty definition block found at line number " + (lines.indexOf(lineRaw) + 1) + "!" );
                    System.exit(1);
                }

                try {
                    enums.add(jsonToCppCode(definitionBlock.toString()));
                } catch (Exception ex) {
                    log.error("Failed to parse JSON and generate C++ code for block:\n" + definitionBlock, ex);
                    System.exit(1);
                }

                definitionBlock = null;
                continue;
            }

            if (inDefinition) {
                definitionBlock.append(line);
            }
        }

        if (inDefinition) {
            log.error("Unexpected end of file found while still in definition block!");
            System.exit(1);
        }

        try (PrintWriter out = new PrintWriter(Files.newBufferedWriter(Paths.get(outputPath)))) {
            out.println("// Generated file");
            out.println("#include <cstdint>");
            out.println("namespace rusefi::generated::enums\n{");
            out.println("template<typename T>\n" +
                "constexpr char const* Enum2String(T const e);\n" +
                "\n" +
                "template<typename T>\n" +
                "constexpr T String2Enum(char const* const s);\n");
            for (String e : enums) {
                out.println(e);
            }
            out.println("}");
        }

        log.info("Generated " + enums.size() + " enums into " + outputPath);
    }

    private static String jsonToCppCode(String jsonText) throws Exception {
        JSONParser parser = new JSONParser();
        JSONObject root = (JSONObject) parser.parse(jsonText);

        // --- Validate name and type ---
        String enumName = (String) root.get("name");
        String enumType = (String) root.get("type");

        if (enumName == null || enumName.isEmpty()) {
            throw new ParseException("Enum name is empty!", 0);
        }

        if (enumType != null && !enumType.isEmpty()) {
            Set<String> allowedTypes = Set.of(
                "char", "uint8_t", "uint16_t", "uint32_t", "uint64_t",
                "int8_t", "int16_t", "int32_t", "int64_t"
            );
            if (!allowedTypes.contains(enumType)) {
                throw new ParseException(
                    "Enum type '" + enumType + "' is not allowed. Allowed types are: " + allowedTypes, 0
                );
            }
        }

        // --- Build map of values and validate value names ---
        JSONArray valuesArray = (JSONArray) root.get("values");
        if (valuesArray == null || valuesArray.isEmpty()) {
            throw new ParseException("Values array is empty!", 0);
        }

        Map<String, Long> valuesMap = getStringObjectMap(valuesArray);

        StringBuilder cpp = new StringBuilder();

        cpp.append("enum ").append(enumName).append(": ").append(enumType).append(" {\n");
        for (Map.Entry<String, Long> entry : valuesMap.entrySet()) {
            cpp.append("    ").append(entry.getKey());
            if (entry.getValue() != null) {
                cpp.append(" = ").append(entry.getValue());
            }
            cpp.append(",\n");
        }
        cpp.append("    ").append(enumName).append("_Unsupported_Value\n");
        cpp.append("};\n\n");

        // enum2String
        cpp.append("template<> constexpr char const* Enum2String<").append(enumName)
            .append(">(").append(enumName).append(" const e){\n")
            .append("    switch(e) {\n");
        for (String valueName : valuesMap.keySet()) {
            cpp.append("        case ").append(enumName).append("::").append(valueName)
                .append(": return \"").append(valueName).append("\";\n");
        }
        cpp.append("        default: return \"").append(enumName).append("_Unsupported_Value\";\n")
            .append("    }\n};\n\n");

        // string2Enum
        cpp.append("template<> constexpr ").append(enumName)
            .append(" String2Enum<").append(enumName).append(">(char const* const s){\n");
        for (String valueName : valuesMap.keySet()) {
            int length = valueName.length();
            cpp.append("    if (strncmp(Enum2String<").append(enumName)
                .append(">(").append(enumName).append("::").append(valueName)
                .append("), s, ").append(length).append("+1) == 0) return ")
                .append(enumName).append("::").append(valueName).append(";\n");
        }
        cpp.append("    return ").append(enumName).append("::").append(enumName)
            .append("_Unsupported_Value;\n};\n\n");

        return cpp.toString();
    }

    private static Map<String, Long> getStringObjectMap(JSONArray valuesArray) throws ParseException {
        Map<String, Long> valuesMap = new LinkedHashMap<>(); // preserve order
        for (Object o : valuesArray) {
            JSONObject v = (JSONObject) o;
            String valueName = (String) v.get("v");
            Long id = (Long) v.get("i"); // can be null

            if (valueName == null || valueName.isEmpty()) {
                throw new ParseException("Value name is empty!", 0);
            }

            // Validate: only alphanumerics + underscores, no leading digit
            if (!valueName.matches("[A-Za-z_][A-Za-z0-9_]*")) {
                throw new ParseException(
                    "Invalid value name '" + valueName + "'. Must start with a letter or underscore and contain only letters, digits, or underscores.", 0
                );
            }

            valuesMap.put(valueName, id);
        }
        return valuesMap;
    }
}
