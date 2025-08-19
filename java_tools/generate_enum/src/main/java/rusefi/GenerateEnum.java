package rusefi;

import com.devexperts.logging.Logging;

import java.io.*;
import java.nio.file.*;
import java.util.*;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import java.util.List;

import java.io.FileReader;

import static com.devexperts.logging.Logging.getLogging;

public class GenerateEnum {
    private static final Logging log = getLogging(GenerateEnum.class);

    public static JSONObject getRootObjFromJsonFile(String jsonFilePath) {
        try{
            FileReader reader = new FileReader(jsonFilePath);
            JSONParser parser = new JSONParser();
            return (JSONObject) parser.parse(reader);
        } catch (Exception e){
            log.error("Error parsing JSON file: " + jsonFilePath, e);
        }

        System.exit(1);

        return null;
    }

    /*
    // Example of enum definition
    {
      "enum":{
        "name":"engine_load_mode_e",
        "type":"uint8_t",
        "comment":"This enum is used to select your desired Engine Load calculation algorithm",
        "entries":[
          {"i":0,"v":"LM_SPEED_DENSITY","n":"Speed Density", "c":"Speed Density algorithm - Engine Load is a function of MAP, VE and target AFR\nhttp://articles.sae.org/8539/"},
          {"i":1,"v":"LM_REAL_MAF","n":"MAF Air Charge", "c":"MAF with a known kg/hour function"},
          {"i":2,"v":"LM_ALPHA_N","n":"Alpha-N"},
          {"i":3,"v":"LM_LUA","n":"Lua"},
          {"no_ts":"true","i":100,"v":"LM_MOCK","n":"Mock for test", "c":"This mode is for unit testing only, so that tests don't have to rely on a particular real airmass mode"}
        ]
      }
    }
    * */

    public static class EnumDefinition{
        public static class EnumEntry{
            long index;
            String valueCpp;
            String valueTs;
            String comment;
            boolean noTs;

            EnumEntry(JSONObject entryObj) {
                Object indexObj = entryObj.get("i");
                Object valueCppObj = entryObj.get("v");

                if (indexObj == null) {
                    throw new RuntimeException("Enum entry has no index!");
                }

                if (valueCppObj == null) {
                    throw new RuntimeException("Enum entry has no value!");
                }

                index = (long) indexObj;
                valueCpp = (String) valueCppObj;

                Object valueTsObj = entryObj.get("n");

                if (valueTsObj == null) {
                    log.debug("Enum entry has no valueTs, value for C++ is used!");
                    valueTs = valueCpp;
                } else {
                    valueTs = (String) valueTsObj;
                }

                comment = (String) entryObj.get("c");
                noTs = entryObj.containsKey("no_ts");
            }
        };

        String name;
        String type;
        String comment;
        List<EnumEntry> entries = new ArrayList<>();

        private static final Set<String> allowedTypes = Set.of(
            "char", "uint8_t", "uint16_t", "uint32_t",
            "int8_t", "int16_t", "int32_t"
        );

        EnumDefinition(JSONObject value) throws RuntimeException {
            name = (String) value.get("name");
            type = (String) value.get("type");
            comment = (String) value.get("comment");

            if (name == null || name.isEmpty()) {
                log.error("Enum name is empty!");
            }

            if (type == null || type.isEmpty()) {
                log.error("Enum " + name + " has no type!");
            } else if (!allowedTypes.contains(type)) {
                throw new RuntimeException("Enum type '" + type + "' is not allowed. Allowed types are: " + allowedTypes);
            }

            JSONArray entriesRaw = (JSONArray) value.get("entries");

            if (entriesRaw == null || entriesRaw.isEmpty()) {
                throw new RuntimeException("Enum " + name + " has no entries!");
            }

            for (Object obj : entriesRaw) {
                try {
                    entries.add(new EnumEntry((JSONObject)obj));
                } catch (Exception e) {
                    throw new RuntimeException("Error parsing enum (" + name + ") entry", e);
                }
            }
        }
    }

    public static class ParsedStrings {
        List<String> cppEnums = new ArrayList<>();
        List<String> definesTxt = new ArrayList<>();
        List<String> definesIni = new ArrayList<>();
        List<String> replacementsCfg = new ArrayList<>();
    };

    public static ParsedStrings parseDefinition(String definitionPath) throws Exception {
        ParsedStrings parsedStrings = new ParsedStrings();

        JSONObject root = getRootObjFromJsonFile(definitionPath);

        for (Object kObj : root.keySet()) {
            String key = String.valueOf(kObj);
            JSONObject value = (JSONObject) root.get(kObj);

            if (!key.equals("enum")) {
                log.error("Unexpected key '" + key + "' in definition file on top level!");
                System.exit(1);
            }

            EnumDefinition definition = new EnumDefinition(value);

            parsedStrings.cppEnums.add(getCppEnums(definition));
            parsedStrings.definesTxt.add(getDefinesTxt(definition));
            parsedStrings.definesIni.add(getDefinesIni(definition));
            parsedStrings.replacementsCfg.add(getReplacementsCfg(definition));
        }

        return parsedStrings;
    }

    private static String getCppEnums(EnumDefinition enumDef) {
        String enumName = enumDef.name;
        String enumType = enumDef.type;
        List<EnumDefinition.EnumEntry> entries = enumDef.entries;

        StringBuilder cpp = new StringBuilder();

        // Enum declaration
        if (enumDef.comment != null && !enumDef.comment.isEmpty() ) {
            cpp.append("/* ");
            cpp.append(enumDef.comment.replaceAll("\n", "\n * "));
            cpp.append(" */\n");
        }
        cpp.append("enum ").append(enumName).append(": ").append(enumType).append(" {\n");
        for (EnumDefinition.EnumEntry entry : entries) {
            if (entry.comment != null && !entry.comment.isEmpty() ) {
                cpp.append("    /* ");
                cpp.append(entry.comment.replaceAll("\n", "\n     * "));
                cpp.append(" */\n");
            }
            cpp.append("    ").append(entry.valueCpp).append(" = ").append(entry.index).append(",\n");
        }
        cpp.append("    ").append(enumName).append("_Unsupported_Value\n");
        cpp.append("};\n\n");

        // enum2String
        cpp.append("template<> constexpr char const* Enum2String<").append(enumName).append(">(").append(enumName).append(" const e) {\n");
        cpp.append("    switch(e) {\n");
        for (EnumDefinition.EnumEntry entry : entries) {
            cpp.append("        case ").append(enumName).append("::").append(entry.valueCpp)
                .append(": return \"").append(entry.valueCpp).append("\";\n");
        }
        cpp.append("        default: return \"").append(enumName).append("_Unsupported_Value\";\n");
        cpp.append("    }\n");
        cpp.append("};\n\n");

        // string2Enum
        cpp.append("template<> constexpr ").append(enumName).append(" String2Enum<").append(enumName).append(">(char const* const s){\n");
        for (EnumDefinition.EnumEntry entry : entries) {
            int length = entry.valueCpp.length();
            cpp.append("    if (strncmp(Enum2String<").append(enumName)
                .append(">(").append(enumName).append("::").append(entry.valueCpp)
                .append("), s, ").append(length).append("+1) == 0) return ")
                .append(enumName).append("::").append(entry.valueCpp).append(";\n");
        }
        cpp.append("    return ").append(enumName).append("::").append(enumName).append("_Unsupported_Value;\n");
        cpp.append("};\n\n");

        return cpp.toString();
    }

    private static String getDefinesTxt(EnumDefinition enumDef) {
        String enumValueLengthStr = "";
        int enumValueLength = 0;

        if (Objects.equals(enumDef.type, "uint8_t") || Objects.equals(enumDef.type, "uint16_t") || Objects.equals(enumDef.type, "uint32_t")) {
            enumValueLengthStr += "U";
        } else {
            enumValueLengthStr += "S";
        }

        if (Objects.equals(enumDef.type, "uint8_t") || Objects.equals(enumDef.type, "int8_t") || Objects.equals(enumDef.type, "char")) {
            enumValueLengthStr += "08";
            enumValueLength = 1;
        }

        if (Objects.equals(enumDef.type, "uint16_t") || Objects.equals(enumDef.type, "int16_t")) {
            enumValueLengthStr += "16";
            enumValueLength = 2;
        }

        if (Objects.equals(enumDef.type, "uint32_t") || Objects.equals(enumDef.type, "int32_t")) {
            enumValueLengthStr += "32";
            enumValueLength = 4;
        }

        StringBuilder txt = new StringBuilder();

        //txt.append("#define ");
        //txt.append(enumDef.name).append("_enum ");
        //for (EnumDefinition.EnumEntry entry : enumDef.entries) {
        //    if (entry.noTs) {
        //        continue;
        //    }
        //    txt.append("\"").append(entry.valueCpp).append("\",");
        //}
        //txt.deleteCharAt(txt.length() - 1);
        //txt.append("\n");

        int totalElements = enumDef.entries.size();
        int bitsOutOf32ForMaxIndex = 32 - Integer.numberOfLeadingZeros(totalElements);

        txt.append("custom ").append(enumDef.name).append(" ").append(enumValueLength).append(" bits, ").append(enumValueLengthStr).append(", @OFFSET@, [0:").append(bitsOutOf32ForMaxIndex - 1).append("], $").append(enumDef.name).append("_list\n\n");

        return txt.toString();
    }

    private static String getReplacementsCfg(EnumDefinition enumDef) {
        StringBuilder cfg = new StringBuilder();

        for (EnumDefinition.EnumEntry entry : enumDef.entries) {
            if (entry.noTs) {
                continue;
            }

            cfg.append("#define ").append(enumDef.name).append("_").append(entry.valueCpp).append(" ").append(entry.index).append("\n");
        }

        return cfg.toString();
    }

    private static String getDefinesIni(EnumDefinition enumDef) {
        StringBuilder ini = new StringBuilder();

        ini.append("#define ");
        ini.append(enumDef.name).append("_list=");

        for (EnumDefinition.EnumEntry entry : enumDef.entries) {
            if (entry.noTs) {
                continue;
            }

            ini.append("\"").append(entry.valueTs).append("\",");
        }

        ini.deleteCharAt(ini.length() - 1);

        ini.append("\n");

        return ini.toString();
    }

    private static void writeParsedStringsIntoFiles(ParsedStrings parsedStrings, String outputPathIni, String outputPathCpp, String outputPathCfg, String outputPathTxt) throws IOException {
        try (PrintWriter out = new PrintWriter(Files.newBufferedWriter(Paths.get(outputPathCpp)))) {
            out.println("// Generated by rusefi.GenerateEnum");
            out.println("#pragma once");
            out.println("#include <cstdint>");
            out.println("namespace rusefi::generated::enums\n{");
            out.println("template<typename T>\n" +
                "constexpr char const* Enum2String(T const e);\n" +
                "\n" +
                "template<typename T>\n" +
                "constexpr T String2Enum(char const* const s);\n");
            for (String e : parsedStrings.cppEnums) {
                out.println(e);
            }
            out.println("}");
        }

        try (PrintWriter out = new PrintWriter(Files.newBufferedWriter(Paths.get(outputPathTxt)))) {
            out.println("// Generated by rusefi.GenerateEnum");
            for (String e : parsedStrings.definesTxt) {
                out.println(e);
            }
        }

        try (PrintWriter out = new PrintWriter(Files.newBufferedWriter(Paths.get(outputPathIni)))) {
            out.println("// Generated by rusefi.GenerateEnum");
            for (String e : parsedStrings.definesIni) {
                out.println(e);
            }
        }

        try (PrintWriter out = new PrintWriter(Files.newBufferedWriter(Paths.get(outputPathCfg)))) {
            out.println("// Generated by rusefi.GenerateEnum");
            for (String e : parsedStrings.replacementsCfg) {
                out.println(e);
            }
        }
    }

    public static void main(String[] args) {
        try {
            String definitionPath = System.getProperty("definition");
            String outputPathCpp = System.getProperty("outputPathCpp");
            String outputPathIni = System.getProperty("outputPathIni");
            String outputPathTxt = System.getProperty("outputPathTxt");
            String outputPathCfg = System.getProperty("outputPathCfg");

            log.info("\nDefinition file: " + definitionPath + "\noutputPathCpp: " + outputPathCpp + "\noutputPathIni: " + outputPathIni + "\noutputPathTxt: " + outputPathTxt + "\noutputPathCfg: " + outputPathCfg + "\n");

            if (definitionPath == null || outputPathCpp == null || outputPathIni == null || outputPathTxt == null || outputPathCfg == null) {
                log.error("Usage: -Ddefinition=... -DoutputPathCpp=... -DoutputPathIni=... -DoutputPathTxt=... -DoutputPathCfg=...");
                System.exit(1);
            }

            if (!Files.exists(Paths.get(definitionPath))) {
                log.error("Definition file does not exist: " + definitionPath);
                System.exit(1);
            }

            ParsedStrings parsedStrings = parseDefinition(definitionPath);

            writeParsedStringsIntoFiles(parsedStrings, outputPathIni, outputPathCpp, outputPathCfg, outputPathTxt);

            log.info("Generated " + parsedStrings.cppEnums.size() + " enums into:\n" + outputPathCpp + "\n" + outputPathTxt + "\n" + outputPathIni + "\n" + outputPathCfg);
        } catch (Exception e) {
            log.error("Error processing enum", e);
            System.exit(1);
        }

        System.exit(0);
    }
}
