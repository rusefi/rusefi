package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.enum_reader.Value;
import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.parsing.Definition;
import com.rusefi.util.SystemOut;
import org.jetbrains.annotations.NotNull;
import org.yaml.snakeyaml.Yaml;

import java.io.*;
import java.util.*;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.VariableRegistry.FULL_JAVA_ENUM;
import static com.rusefi.VariableRegistry.ENUM_SUFFIX;
import static com.rusefi.output.JavaSensorsConsumer.quote;

public class PinoutLogic {
    private static final Logging log = getLogging(PinoutLogic.class);

    public static final String CONFIG_BOARDS = "config/boards/";
    private static final String CONNECTORS = "/connectors";
    private static final String NONE = "NONE";
    private static final String QUOTED_NONE = quote(NONE);
    public static final String QUOTED_INVALID = quote(VariableRegistry.INVALID);

    private final File[] boardYamlFiles;
    private final String boardName;
    private final ArrayList<PinState> globalList = new ArrayList<>();
    private final Map</*id*/String, /*tsName*/String> tsNameById = new TreeMap<>();
    private final StringBuilder header = new StringBuilder("//DO NOT EDIT MANUALLY, let automation work hard.\n\n");

    public PinoutLogic(String boardName, File[] boardYamlFiles) {
        log.info(boardName + ": " + Arrays.toString(boardYamlFiles));
        this.boardName = boardName;
        this.boardYamlFiles = boardYamlFiles;
    }

    private static Map.Entry<String, Value> find(EnumsReader.EnumState enumList, String id) {
        for (Map.Entry<String, Value> kv : enumList.entrySet()) {
            if (kv.getKey().equals(id)) {
                return kv;
            }
        }
        return null;
    }

    private static void registerPins(String boardName, ArrayList<PinState> listPins, VariableRegistry registry, ReaderState state, ParseState parseState) {
        if (listPins == null || listPins.isEmpty()) {
            return;
        }
        Map<String, ArrayList<String>> names = new HashMap<>();
        names.put("outputs", new ArrayList<>());
        names.put("analog_inputs", new ArrayList<>());
        names.put("event_inputs", new ArrayList<>());
        names.put("switch_inputs", new ArrayList<>());
        for (PinState listPin : listPins) {
            String id = listPin.getId();
            String className = listPin.getPinClass();
            ArrayList<String> classList = names.get(className);
            if (classList == null) {
                throw new IllegalStateException("Class not found:  " + className);
            }
            PinType listPinType = PinType.find(className);
            String pinType = listPinType.getPinType();
            EnumsReader.EnumState enumList = state.enumsReader.getEnums().get(pinType);
            Objects.requireNonNull(enumList, "Enum for " + pinType);
            Map.Entry<String, Value> kv = find(enumList, id);
            if (kv == null) {
                throw new IllegalStateException(boardName + ": Not found " + id + " in " + className);
            }

                    int index = kv.getValue().getIntValue();
                    classList.ensureCapacity(index + 1);
                    for (int ii = classList.size(); ii <= index; ii++) {
                        classList.add(null);
                    }
                    classList.set(index, listPin.getPinTsName());
        }
        for (Map.Entry<String, ArrayList<String>> kv : names.entrySet()) {
            PinType namePinType = PinType.find(kv.getKey());
            String outputEnumName = namePinType.getOutputEnumName();
            String pinType = namePinType.getPinType();
            String nothingName = namePinType.getNothingName();
            EnumsReader.EnumState enumList = state.enumsReader.getEnums().get(pinType);
            EnumPair pair = enumToOptionsList(nothingName, enumList, kv.getValue());
            if (pair.getSimpleForm().length() > 0) {
                // we seem to be here if specific pin category like switch_inputs has no pins
                registry.register(outputEnumName + ENUM_SUFFIX, pair.getShorterForm());
                parseState.addDefinition(outputEnumName + ENUM_SUFFIX, pair.getShorterForm(), Definition.OverwritePolicy.IgnoreNew);
            }
            registry.register(outputEnumName + FULL_JAVA_ENUM, pair.getSimpleForm());
            parseState.addDefinition(outputEnumName + FULL_JAVA_ENUM, pair.getSimpleForm(), Definition.OverwritePolicy.IgnoreNew);
        }
    }

    @NotNull
    public static EnumPair enumToOptionsList(String nothingName, EnumsReader.EnumState enumList, ArrayList<String> values) {
        StringBuilder simpleForm = new StringBuilder();

        Map<Integer, String> pinMap = new HashMap<>();

        for (int i = 0; i < values.size(); i++) {
            appendCommaIfNeeded(simpleForm);
            String key = findKey(enumList, i);

            String value = values.get(i);
            if (i == 0) {
                pinMap.put(i, NONE);
            } else if (value != null) {
                pinMap.put(i, value);
            }
            if (key.equals(nothingName)) {
                simpleForm.append(QUOTED_NONE);
            } else if (value == null) {
                simpleForm.append(QUOTED_INVALID);
            } else {
                String quotedValue = quote(value);
                simpleForm.append(quotedValue);
            }
        }
        String keyValueForm = VariableRegistry.getHumanSortedTsKeyValueString(pinMap);
        return new EnumPair(keyValueForm, simpleForm.toString());

        //        String shorterForm = smartForm.length() < simpleForm.length() ? smartForm.toString() : simpleForm.toString();
//
//        return new EnumPair(shorterForm, simpleForm.toString());
    }

    private static void appendCommaIfNeeded(StringBuilder sb) {
        if (sb.length() > 0)
            sb.append(",");
    }

    private static String findKey(EnumsReader.EnumState enumList, int i) {
        String key = "";
        for (Map.Entry<String, Value> entry : enumList.entrySet()) {
            if (entry.getValue().getIntValue() == i) {
                key = entry.getKey();
                break;
            }
        }
        return key;
    }

    @SuppressWarnings("unchecked")
    private void readMetaInfo(File yamlFile) throws IOException {
        Yaml yaml = new Yaml();
        Map<String, Object> yamlData = yaml.load(new FileReader(yamlFile));
        if (yamlData == null) {
            SystemOut.println("Null yaml for " + yamlFile);
            return;
        }
        List<Map<String, Object>> data = (List<Map<String, Object>>) yamlData.get("pins");
        if (data == null) {
            SystemOut.println("Null yaml for " + yamlFile);
            return;
        }
        log.info("Got from " + yamlFile + ": " +  data);
        Objects.requireNonNull(data, "data");
        for (Map<String, Object> pin : data) {
            Object pinId = pin.get("id");
            Object pinClass = pin.get("class");
            Object pinName = pin.get("pin");
            String pinTsName = (String) pin.get("ts_name");
            if (pinId == null || pinClass == null || pinTsName == null) {
                continue;
            }
            if (pinId instanceof ArrayList) {
                ArrayList<String> pinIds = (ArrayList<String>) pinId;
                if (!(pinClass instanceof ArrayList))
                    throw new IllegalStateException("Expected multiple classes for " + pinIds);
                ArrayList<String> pinClassArray = (ArrayList<String>) pinClass;
                if (pinIds.size() != pinClassArray.size())
                    throw new IllegalStateException(pinName + ": id array length should match class array length: " + pinId + " vs " + pinClassArray);
                for (int i = 0; i < pinIds.size(); i++) {
                    String id = pinIds.get(i);
                    addPinToList(id, pinTsName, pinClassArray.get(i));
                }
            } else if (pinId instanceof String) {
                String pinIdString = (String) pinId;
                if (pinIdString.length() == 0) {
                    throw new IllegalStateException("Unexpected empty ID field");
                }
                addPinToList(pinIdString, pinTsName, (String) pinClass);
            } else {
                throw new IllegalStateException("Unexpected type of ID field: " + pinId.getClass().getSimpleName());
            }
        }
    }

    private void addPinToList(String id, String pinTsName, String pinClass) {
        String existingTsName = tsNameById.get(id);
        if (existingTsName != null && !existingTsName.equals(pinTsName))
            throw new IllegalStateException("ID used multiple times with different ts_name: " + id);
        tsNameById.put(id, pinTsName);
        PinState thisPin = new PinState(id, pinTsName, pinClass);
        globalList.add(thisPin);
    }
/*
    public static void main(String[] args) throws IOException {
        String boardName = "hellen-gm-e67";
        PinoutLogic logic = create(boardName,"../../firmware/config/boards/hellen/");
        logic.readFiles();
        log.info(logic.toString());

        registerPins(boardName, logic.globalList, new VariableRegistry(), new ReaderState());
    }
*/

    public static PinoutLogic create(String boardName, String rootFolder) {
        String dirPath = rootFolder + boardName + PinoutLogic.CONNECTORS;
        File dirName = new File(dirPath);
        FilenameFilter filter = (f, name) -> name.endsWith(".yaml");
        File[] boardYamlFiles = dirName.listFiles(filter);
        if (boardYamlFiles == null) {
            log.info("No yaml files in " + dirPath);
            return null;
        }
        Arrays.sort(boardYamlFiles);
        return new PinoutLogic(boardName, boardYamlFiles);
    }

    public void registerBoardSpecificPinNames(VariableRegistry registry, ReaderState state, ParseState parseState) throws IOException {
        readFiles();
        registerPins(boardName, globalList, registry, state, parseState);

        try (FileWriter getTsNameByIdFile = new FileWriter(PinoutLogic.CONFIG_BOARDS + boardName + PinoutLogic.CONNECTORS + File.separator + "generated_ts_name_by_pin.cpp")) {
            getTsNameByIdFile.append(header);

            getTsNameByIdFile.append("#include \"pch.h\"\n\n");
            getTsNameByIdFile.append("// see comments at declaration in pin_repository.h\n");
            getTsNameByIdFile.append("const char * getBoardSpecificPinName(brain_pin_e brainPin) {\n");
            getTsNameByIdFile.append("\tswitch(brainPin) {\n");

            for (Map.Entry</*id*/String, /*tsName*/String> e : tsNameById.entrySet()) {
                if (e.getKey().contains("ADC")) // we only support GPIO pins at the moment no support for ADC
                    continue;
                getTsNameByIdFile.append("\t\tcase Gpio::" + e.getKey() + ": return " + quote(e.getValue()) + ";\n");
            }

            getTsNameByIdFile.append("\t\tdefault: return nullptr;\n");
            getTsNameByIdFile.append("\t}\n");

            getTsNameByIdFile.append("\treturn nullptr;\n}\n");
        }
    }

    private void readFiles() throws IOException {
        for (File yamlFile : boardYamlFiles) {
            header.append("// auto-generated by PinoutLogic.java based on " + yamlFile + "\n");
            readMetaInfo(yamlFile);
        }
        log.info("Got from " + boardYamlFiles.length + " file(s): " + this);
    }

    public List<String> getInputFiles() {
        List<String> result = new ArrayList<>();
        for (File yamlFile : boardYamlFiles) {
            result.add(PinoutLogic.CONFIG_BOARDS + boardName + PinoutLogic.CONNECTORS +
                    File.separator + yamlFile.getName());
        }
        return result;
    }

    private static class PinState {
        /**
         * ID is not unique
         */
        private final String id;
        private final String pinTsName;
        private final String pinClass;

        public PinState(String id, String pinName, String pinClass) {
            this.id = id;
            this.pinTsName = pinName;
            this.pinClass = pinClass;
        }

        public String getId() {
            return id;
        }

        public String getPinTsName() {
            return pinTsName;
        }

        public String getPinClass() {
            return pinClass;
        }

        @Override
        public String toString() {
            return "PinState{" +
                    "id='" + id + '\'' +
                    ", pinTsName='" + pinTsName + '\'' +
                    ", pinClass='" + pinClass + '\'' +
                    '}';
        }
    }

    @Override
    public String toString() {
        return "PinoutLogic{" +
                "boardName='" + boardName + '\'' +
                ", globalList=" + globalList +
                ", tsNameById=" + tsNameById +
                '}';
    }
}
