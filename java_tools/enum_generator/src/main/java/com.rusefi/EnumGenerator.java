package com.rusefi;

import java.io.FileReader;
import java.util.List;

import com.devexperts.logging.Logging;

import java.util.*;
import java.util.HashMap;
import java.util.stream.Collectors;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import static com.devexperts.logging.Logging.getLogging;

public class EnumGenerator {
    private static final Logging log = getLogging(EnumGenerator.class);

    public static class Parser {

        public static String TS_TEXT_DEFINE_SUFFIX = "_list";

        /* Supported format:
        {
          "enum":{
            "name":"engine_load_mode_e",
            "type":"uint8_t",
            "comment":"This enum is used to select your desired Engine Load calculation algorithm",
            "entries":[
              {"i":0,"v":"LM_SPEED_DENSITY","n":"Speed Density", "c":"Speed Density algorithm - Engine Load is a function of MAP, VE and target AFR\nhttp://articles.sae.org/8539/"},
              {"i":1,"v":"LM_REAL_MAF","n":"MAF Air Charge", "c":"MAF with a known kg/hour function"},
              {"i":2,"v":"LM_ALPHA_N","n":"Alpha-N"},
              {"i":3,"v":"LM_LUA","n":"Lua"}
            ]
          }
        }
        * */

        public static class EnumDefinition {

            public static class EnumEntry {
                List<String> supportedEntries = List.of(
                    "i", // Numeric index, must be sequential
                    "v", // Enum entry short name for c++ code and value referencing in ini generation
                    "n", // Enum entry text for tuner studio
                    "c"  // Comment for entry
                );

                public long index;
                public String valueCpp;
                public String valueTs;
                public String comment;

                EnumEntry(JSONObject entryObj) {
                    if (entryObj == null) {
                        throw new RuntimeException("Enum entry is null!");
                    }

                    for (Object key : entryObj.keySet()) {
                        String keyStr = String.valueOf(key);
                        if (!supportedEntries.contains(keyStr)) {
                            throw new RuntimeException("Enum entry has unsupported key '" + keyStr + "'!");
                        }
                    }

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
                }
            }

            public String name;
            public String type;
            public String comment;
            public List<EnumEntry> entries = new ArrayList<>();

            private static final List<String> supportedEntries = List.of(
                "name",    // Name to be used in code and as the type reference in txt configs for ini generation
                "type",    // c++ type of enum class (i.e. uint8_t -> "enum class my_enum : uint8_t")
                "comment", // Comment for c++ code
                "entries"  // List of enum values
            );

            private static final Set<String> allowedTypes = Set.of(
                "char", "uint8_t", "uint16_t", "uint32_t",
                "int8_t", "int16_t", "int32_t"
            );

            EnumDefinition(JSONObject value) throws RuntimeException {
                if (value == null) {
                    throw new RuntimeException("Enum obj is null!");
                }

                for (Object key : value.keySet()) {
                    String keyStr = String.valueOf(key);
                    if (!supportedEntries.contains(keyStr)) {
                        throw new RuntimeException("Enum obj has unsupported key '" + keyStr + "'!");
                    }
                }

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

        private static JSONObject getRootObjFromJsonFile(FileReader reader) {
            try{
                JSONParser parser = new JSONParser();
                return (JSONObject) parser.parse(reader);
            } catch (Exception e){
                throw new IllegalStateException("Error parsing JSON file", e);
            }
        }

        public enum Definitions {
            TsTextRepresentations,
            TsEnumValueToIndexMap,
            CustomTypes
        }

        private static class ParsedData {
            List<String> tsTextRepresentations = new ArrayList<>();
            List<String> tsEnumValueToIndexMap = new ArrayList<>();
            List<String> customTypes = new ArrayList<>();
            List<EnumDefinition> enumDefs = new ArrayList<>();
        }

        private static final ParsedData parsedData = new ParsedData();

        public Parser(FileReader reader) {
            JSONObject rootObj = getRootObjFromJsonFile(reader);

            for (Object kObj : rootObj.keySet()) {
                String key = String.valueOf(kObj);
                JSONObject enumDefinitionObj = (JSONObject) rootObj.get(kObj);

                if (!key.equals("enum")) {
                    throw new RuntimeException("Unexpected key '" + key + "' in definition file on top level!");
                }

                EnumDefinition parsedDefinition = new EnumDefinition(enumDefinitionObj);

                log.info("Adding Enum definition: " + parsedDefinition.name + " (" + parsedDefinition.type + "): ");
                parsedData.enumDefs.add(parsedDefinition);
                generateDefinitionTsTextRepresentations(parsedDefinition);
                generateDefinitionCustomTypes(parsedDefinition);
                generateDefinitionTsEnumValueToIndexMap(parsedDefinition);
            }
        }

        public List<String> getDefinitions(Definitions definition) {
            switch (definition) {
                case TsTextRepresentations:
                    return parsedData.tsTextRepresentations;
                case TsEnumValueToIndexMap:
                    return parsedData.tsEnumValueToIndexMap;
                case CustomTypes:
                    return parsedData.customTypes;
                default:
                    throw new RuntimeException("Unknown definition: " + definition);
            }
        }

        public Map<String, EnumDefinition> getEnumDefinitions() {
            Map<String, EnumDefinition> enumDefsMap = new HashMap<>();
            for (EnumDefinition enumDef : parsedData.enumDefs) {
                enumDefsMap.put(enumDef.name, enumDef);
            }
            return enumDefsMap;
        }

        private static void generateDefinitionTsTextRepresentations(EnumDefinition parsedDefinition) {
            StringBuilder sb = new StringBuilder();

            //log.info("Generating TS enum text representations for enum " + parsedDefinition.name);

            sb.append(parsedDefinition.name).append("_list").append(":");
            for (EnumDefinition.EnumEntry entry : parsedDefinition.entries) {
                sb.append("\"").append(entry.valueTs).append("\"").append(",");
            }

            sb.setLength(sb.length() - 1);

            //log.info("Generated TS enum text representations for enum " + parsedDefinition.name + ": '" + sb + "'");

            parsedData.tsTextRepresentations.add(sb.toString());
        }

        private static void generateDefinitionTsEnumValueToIndexMap(EnumDefinition parsedDefinition) {
            //log.info("Generating TS enum value to index map for enum " + parsedDefinition.name);

            StringBuilder sb = new StringBuilder();

            for (EnumDefinition.EnumEntry entry : parsedDefinition.entries) {
                String enumValueToIndexDefine = parsedDefinition.name + "_" + entry.valueCpp + " " + entry.index;
                parsedData.tsEnumValueToIndexMap.add(enumValueToIndexDefine);
                sb.append(enumValueToIndexDefine).append("\n");
            }

            //log.info("Generated TS enum value to index map for enum " + parsedDefinition.name + ":\n" + sb);
        }

        private static void generateDefinitionCustomTypes(EnumDefinition enumDef) {
            //log.info("Generating TS enum custom type for enum for enum " + enumDef.name);

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

            StringBuilder customType = new StringBuilder();

            int totalElements = enumDef.entries.size();
            int bitsOutOf32ForMaxIndex = 32 - Integer.numberOfLeadingZeros(totalElements);

            customType.append(enumDef.name).append(" ").append(enumValueLength).append(" bits, ").append(enumValueLengthStr).append(", @OFFSET@, [0:").append(bitsOutOf32ForMaxIndex - 1).append("], ").append("@@").append(enumDef.name).append(TS_TEXT_DEFINE_SUFFIX).append("@@");

            //log.info("Generated TS enum custom type for enum " + enumDef.name + ": '" + customType + "'");

            parsedData.customTypes.add(customType.toString());
        }
    }
}
