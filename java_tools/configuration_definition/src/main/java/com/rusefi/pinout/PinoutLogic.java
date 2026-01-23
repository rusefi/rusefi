package com.rusefi.pinout;

import com.devexperts.logging.Logging;
import com.rusefi.*;
import com.rusefi.enum_reader.Value;
import com.rusefi.newparse.DefinitionsState;
import com.rusefi.newparse.parsing.Definition;
import org.jetbrains.annotations.NotNull;
import org.yaml.snakeyaml.Yaml;

import java.io.*;
import java.util.*;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.VariableRegistry.ARRAY_FORMAT_ENUM;
import static com.rusefi.VariableRegistry.KEY_VALUE_FORMAT_ENUM;
import static com.rusefi.output.JavaSensorsConsumer.quote;

public class PinoutLogic {
    private static final Logging log = getLogging(PinoutLogic.class);

    private static final String NONE = "NONE";
    private static final String QUOTED_NONE = quote(NONE);
    private static final String ID = "id";
    private static final String META = "meta";
    private static final String CLASS = "class";
    private static final String PIN = "pin";
    private static final String TS_NAME = "ts_name";
    private static final String FUNCTION = "function";
    private static final String TYPE = "type";

    private final ArrayList<PinState> globalList = new ArrayList<>();
    private final Map</*id*/String, /*tsName*/String> tsNameById = new TreeMap<>();
    private final Map</*id*/String, /*tsName*/String> tsNameByMeta = new TreeMap<>();
    private final StringBuilder header = new StringBuilder("//DO NOT EDIT MANUALLY, let automation work hard.\n\n");
    private final StringBuffer pinNames = new StringBuffer();
    private final BoardInputs boardInputs;
    private final List<String> lowSideOutputs = new ArrayList<>();
    private final List<String> highSideOutputs = new ArrayList<>();

    public PinoutLogic(BoardInputs boardInputs) {
        log.info("init " + boardInputs.getBoardYamlKeys().size() + " file(s)");
        this.boardInputs = boardInputs;
    }

    private static Map.Entry<String, Value> find(EnumsReader.EnumState enumList, String id) {
        for (Map.Entry<String, Value> kv : enumList.entrySet()) {
            if (kv.getKey().equals(id)) {
                return kv;
            }
        }
        return null;
    }

    private static void registerPins(String boardName, ArrayList<PinState> listPins, VariableRegistry registry, DefinitionsState parseState, EnumsReader enumsReader) {
        if (listPins == null || listPins.isEmpty()) {
            return;
        }
        Map<String, ArrayList<String>> names = new HashMap<>();
        for (PinType pinType : PinType.values())
            names.put(pinType.name().toLowerCase(), new ArrayList<>());
        for (PinState listPin : listPins) {
            String id = listPin.getId();
            String className = listPin.getPinClass();
            ArrayList<String> classList = names.get(className);
            if (classList == null) {
                throw new IllegalStateException(boardName + ": Class not found:  " + className + " for " + id);
            }

            PinType pinType = PinType.find(className);
            addToPinType(boardName, enumsReader, listPin, pinType.getPinType(), id, className, classList);
            if (pinType == PinType.EVENT_INPUTS)
                addToPinType(boardName, enumsReader, listPin, PinType.SWITCH_INPUTS.getPinType(), id, className, classList);
        }
        for (Map.Entry<String, ArrayList<String>> kv : names.entrySet()) {
            PinType namePinType = PinType.find(kv.getKey());
            String outputEnumName = namePinType.getOutputEnumName();
            String pinType = namePinType.getPinType();
            String nothingName = namePinType.getNothingName();
            EnumsReader.EnumState enumList = enumsReader.getEnums().get(pinType);
            EnumPair pair = enumToOptionsList(nothingName, enumList, kv.getValue());
            if (!pair.isEmpty()) {
                // we seem to be here if specific pin category like switch_inputs has no pins
                parseState.addDefinition(registry, outputEnumName + KEY_VALUE_FORMAT_ENUM, pair.getKeyValueForm(), Definition.OverwritePolicy.IgnoreNew);
                parseState.addDefinition(registry, outputEnumName + ARRAY_FORMAT_ENUM, pair.getArrayForm(), Definition.OverwritePolicy.IgnoreNew);
            }
        }
    }

    private static void addToPinType(String boardName, EnumsReader enumsReader, PinState listPin, String pinType, String id, String className, ArrayList<String> classList) {
        EnumsReader.EnumState enumList = enumsReader.getEnums().get(pinType);
        Objects.requireNonNull(enumList, "Enum for " + pinType);
        Map.Entry<String, Value> kv = find(enumList, id);
        if (kv == null) {
            if (id.toLowerCase().contains("gpio"))
                throw new IllegalStateException(boardName + ": Not found looks like legacy 'Gpio::' notation " + id + " in " + className);
            throw new IllegalStateException(boardName + ": Not found " + id + " in " + className);
        }

        Value value = kv.getValue();
        addPin(listPin, value, classList);
    }

    private static void addPin(PinState listPin, Value value, ArrayList<String> classList) {
        int index = value.getIntValue();
        classList.ensureCapacity(index + 1);
        for (int ii = classList.size(); ii <= index; ii++) {
            classList.add(null);
        }
        classList.set(index, listPin.getPinTsName());
    }

    @NotNull
    public static EnumPair enumToOptionsList(String nothingName, EnumsReader.EnumState enumList, ArrayList<String> values) {
        // "value0", "value1", "value2" format
        StringBuilder arrayFormat = new StringBuilder();

        Map<Integer, String> pinMap = new HashMap<>();

        // Calculate target size by rounding up values.size() to the next power of 2
        // 17 values -> 32 (2^5), 33 values -> 64 (2^6), etc.
        int targetSize = values.size();
        if (values.size() > 1) {
            // Find the next power of 2 that can hold all values
            int bits = 32 - Integer.numberOfLeadingZeros(values.size() - 1);
            targetSize = 1 << bits;
        }

        for (int i = 0; i < targetSize; i++) {
            appendCommaIfNeeded(arrayFormat);
            String key = enumList.findByValue(i);

            String value = i < values.size() ? values.get(i) : null;
            if (i == 0) {
                pinMap.put(i, NONE);
            } else if (value != null) {
                pinMap.put(i, value);
            }
            if (key.equals(nothingName)) {
                arrayFormat.append(QUOTED_NONE);
            } else if (value == null) {
                arrayFormat.append(InvalidConstant.QUOTED_INVALID);
            } else {
                String quotedValue = quote(value);
                arrayFormat.append(quotedValue);
            }
        }
        // 2="Value2",5="value5" format
        String keyValueForm = VariableRegistry.getHumanSortedTsKeyValueString(pinMap);
        return new EnumPair(keyValueForm, arrayFormat.toString());
    }

    private static void appendCommaIfNeeded(StringBuilder sb) {
        if (sb.length() > 0)
            sb.append(",");
    }

    @SuppressWarnings("unchecked")
    private void readMetaInfo(File yamlFile, Reader reader) {
        Yaml yaml = new Yaml();
        Map<String, Object> yamlData = yaml.load(reader);
        if (yamlData == null) {
            log.info("Null yaml for " + yamlFile);
            return;
        }
        Map</*meta name*/String, /*native name*/String> metaMapping = processMetaHeader(yamlData);

        List<Map<String, Object>> data = (List<Map<String, Object>>) yamlData.get("pins");
        if (data == null) {
            log.info("Null yaml for " + yamlFile);
            return;
        }
        log.info("Got from " + yamlFile + ": " + data);
        Objects.requireNonNull(data, "data");
        for (Map<String, Object> pin : data) {
            validateKeys(pin.keySet());
            Object pinId = pin.get(ID);
            Object metaAsObject = pin.get(META);
            if (metaAsObject != null && !(metaAsObject instanceof String))
                throw new IllegalStateException("[" + metaAsObject + "] meta could only be a string for pinId=[" + pinId + "]. For arrays use 'id', it's inconsisten.");
            String meta = (String) metaAsObject;
            if (meta != null && pinId != null) {
                throw new IllegalStateException("Please use either meta or id, not both. id=" + pinId + " not expected with meta=" + meta);
            }
            String headerValue;
            if (meta != null) {
                headerValue = meta;
                pinId = metaMapping.get(meta);
                if (pinId == null) {
                    if (metaMapping.isEmpty())
                        throw new IllegalStateException("Empty meta mapping");
                    throw new IllegalStateException("Failing to resolve [" + meta + "]");
                }
            } else {
                headerValue = (pinId instanceof String) ? (String) pinId : null;
            }
            Object pinClass = pin.get(CLASS);
            Object pinName = pin.get(PIN);
            Object pinTsName = pin.get(TS_NAME);
            Object pinFunction = pin.get(FUNCTION);
            String pinNameString = pinName == null ? null : pinName.toString();
            if (pinTsName == null && pinFunction != null)
                pinTsName = pinFunction;
            Object pinType = pin.get(TYPE);
            if (pinId == null || pinClass == null || pinTsName == null) {
                log.info("Skipping incomplete section pinId=" + pinId + "/pinClass=" + pinClass + "/pinTsName=" + pinTsName);
                continue;
            }
            if (pinName != null) {
                pinTsName = pinTsName.toString().replace("___", pinName.toString());
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
                    String originalValue = id;
                    // we are a bit inconsistent between single-function and array syntax:
                    // for array syntax we just apply mapping on the fly while for single we use 'meta' keyword instead of 'pin' keyword
                    id = applyMetaMapping(metaMapping, id);
                    addPinToList(id, originalValue, null, (String) pinTsName, pinClassArray.get(i), pinNameString, true);
                }
            } else if (pinId instanceof String) {
                String pinIdString = (String) pinId;
                if (pinIdString.isEmpty()) {
                    throw new IllegalStateException("Unexpected empty ID field");
                }
                // array type is allowed even for pins with non-array class
                String stringPinType = pinType instanceof String ? (String) pinType : null;
                if (!(pinTsName instanceof String))
                    throw new IllegalStateException("Wrong TsName: " + pinTsName + " while " + pinIdString);
                if (!(pinClass instanceof String))
                    throw new IllegalStateException("Wrong class: " + pinClass + " while " + pinIdString);
                addPinToList(pinIdString, headerValue, stringPinType, (String) pinTsName, (String) pinClass, pinNameString, false);
            } else {
                throw new IllegalStateException("Unexpected type of ID field: " + pinId.getClass().getSimpleName());
            }
        }
    }

    private static void validateKeys(Set<String> key) {
        Set<String> copy = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);
        copy.addAll(key);
        copy.removeAll(Arrays.asList(ID, CLASS, META, FUNCTION, TS_NAME, PIN, TYPE, "color"));
        if (!copy.isEmpty())
            throw new IllegalStateException("Unexpected key(s): " + copy);
    }

    private static String applyMetaMapping(Map<String, String> metaMapping, String id) {
        return metaMapping.getOrDefault(id, id);
    }

    private Map<String, String> processMetaHeader(Map<String, Object> yamlData) {
        String metaHeader = (String) yamlData.get("meta");
        if (metaHeader == null)
            return Collections.emptyMap();

        return getStringStringMap(boardInputs.getBoardMeta(metaHeader));
    }

    @NotNull
    public static Map<String, String> getStringStringMap(List<String> lines) {
        Map</*meta name*/String, /*native name*/String> map = new HashMap<>();

        for (String line : lines) {
            line = line.replace('\t', ' ');
            line = line.trim();
            if (ToolUtil.startsWithToken(line, VariableRegistry.DEFINE)) {
                line = line.substring(VariableRegistry.DEFINE.length() + 1).trim();

                int index = line.indexOf(' ');

                if (index == -1)
                    continue;
                String name = line.substring(0, index);
                String value = line.substring(index).trim();

                map.put(name, value);
            }
        }
        return map;
    }

    private void addPinToList(String id, String headerValue,
                              String pinType,
                              String pinTsName,
                              String pinClass,
                              String pinName,
                              boolean isMultiPin) {
        String existingTsName = tsNameById.get(id);
        if (existingTsName != null && !existingTsName.equals(pinTsName))
            throw new IllegalStateException("ID [" + id + "] used multiple times with different ts_name " + existingTsName + "/" + pinTsName);
        tsNameById.put(id, pinTsName);
        tsNameByMeta.put(headerValue, pinTsName);
        if (pinName != null) {
            String pinNameForDefine = pinName.replaceAll("-", "_").replaceAll("\\s", "_");
            pinNames.append("#define PIN_" + pinNameForDefine + (isMultiPin ? "_" + pinClass : "") + " " + headerValue + "\n");
        }

        if ("outputs".equalsIgnoreCase(pinClass)) {
            if ("ls".equalsIgnoreCase(pinType) || "inj".equalsIgnoreCase(pinType)) {
                lowSideOutputs.add(headerValue);
            } else {
                highSideOutputs.add(headerValue);
            }
        }
        PinState thisPin = new PinState(id, pinTsName, pinClass, pinName);
        globalList.add(thisPin);
    }

    public static PinoutLogic create(String boardName) {
        return new PinoutLogic(new FileSystemBoardInputsImpl(boardName));
    }

    public void registerBoardSpecificPinNames(VariableRegistry registry, DefinitionsState parseState, EnumsReader enumsReader) throws IOException {
        if (boardInputs.getBoardYamlKeys().isEmpty()) {
            // we have a board without yaml so no reason to generate board-specific .cpp file
            log.info("Not generating .cpp since no yaml files");
            return;
        }
        readFiles();
        registerPins(boardInputs.getName(), globalList, registry, parseState, enumsReader);

        try (Writer getTsNameByIdFile = boardInputs.getBoardNamesWriter()) {
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

        try (Writer outputs = boardInputs.getBoardPinNamesWriter()) {
            outputs.append(header);
            outputs.write("#pragma once\n\n");
            outputs.write(pinNames.toString());
        }

        StringBuilder pinNamesForHwQC = new StringBuilder();

        try (Writer outputs = boardInputs.getOutputsWriter()) {
            outputs.append(header);
            outputs.write("#pragma once\n\n");

            outputs.write("Gpio GENERATED_OUTPUTS[] = {\n");

            for (String output : lowSideOutputs) {
                String tsName = tsNameByMeta.get(output);
                outputs.write("\tGpio::" + output + ", // " + tsName + "\n");
                pinNamesForHwQC.append("// " + quote(tsName) + ",\n");
            }
            for (String output : highSideOutputs) {
                String tsName = tsNameByMeta.get(output);
                outputs.write("\tGpio::" + output + ", // " + tsName + "\n");
                pinNamesForHwQC.append("// " + quote(tsName) + ",\n");
            }

            outputs.write(pinNamesForHwQC.toString());
            outputs.write("}\n");

        }

    }

    private void readFiles() throws IOException {
        for (File yamlFile : boardInputs.getBoardYamlKeys()) {
			// workaround to use the same slashes on windows
			String filename = yamlFile.toString().replace('\\', '/');
            header.append("// auto-generated by PinoutLogic.java based on " + filename + "\n");
            readMetaInfo(yamlFile, boardInputs.getReader(yamlFile));
            log.info("Got so far: " + this);
        }
        log.info("Got from " + boardInputs.getBoardYamlKeys().size() + " file(s): " + this);
    }

    /**
     * @return list of affected files so that we can generate total unique ID
     */
    public List<String> getInputFiles() {
        return boardInputs.getInputFiles();
    }

    private static class PinState {
        /**
         * ID is not unique
         */
        private final String id;
        private final String pinTsName;
        private final String pinClass;
        private final String name;

        public PinState(String id, String pinName, String pinClass, String name) {
            this.id = id;
            this.pinTsName = pinName;
            this.pinClass = pinClass;
            this.name = name;
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
                   ", name='" + name + '\'' +
                    ", pinTsName='" + pinTsName + '\'' +
                    ", pinClass='" + pinClass + '\'' +
                    '}';
        }
    }

    @Override
    public String toString() {
        return "PinoutLogic{" +
                "boardName='" + boardInputs.getName() + '\'' +
                ", globalList=" + globalList +
                ", tsNameById=" + tsNameById +
                '}';
    }
}
