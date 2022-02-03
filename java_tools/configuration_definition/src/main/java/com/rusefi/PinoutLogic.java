package com.rusefi;

import com.rusefi.enum_reader.Value;
import com.rusefi.util.SystemOut;
import org.yaml.snakeyaml.Yaml;

import java.io.File;
import java.io.FileReader;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.*;

public class PinoutLogic {
    private static final String CONFIG_BOARDS = "config/boards/";
    private static final String CONNECTORS = "/connectors";

    private final File[] boardYamlFiles;
    private final String boardName;

    public PinoutLogic(String boardName, File[] boardYamlFiles) {
        this.boardName = boardName;
        this.boardYamlFiles = boardYamlFiles;
    }

    private static void registerPins(ArrayList<PinState> listPins, VariableRegistry registry, ReaderState state) {
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
            for (Map.Entry<String, Value> kv : enumList.entrySet()) {
                if (kv.getKey().equals(id)) {
                    int index = kv.getValue().getIntValue();
                    classList.ensureCapacity(index + 1);
                    for (int ii = classList.size(); ii <= index; ii++) {
                        classList.add(null);
                    }
                    classList.set(index, listPin.getPinName());
                    break;
                }
            }
        }
        for (Map.Entry<String, ArrayList<String>> kv : names.entrySet()) {
            PinType namePinType = PinType.find(kv.getKey());
            String outputEnumName = namePinType.getOutputEnumName();
            String pinType = namePinType.getPinType();
            String nothingName = namePinType.getNothingName();
            EnumsReader.EnumState enumList = state.enumsReader.getEnums().get(pinType);
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < kv.getValue().size(); i++) {
                if (sb.length() > 0)
                    sb.append(",");
                String key = "";
                for (Map.Entry<String, Value> entry : enumList.entrySet()) {
                    if (entry.getValue().getIntValue() == i) {
                        key = entry.getKey();
                        break;
                    }
                }
                if (key.equals(nothingName)) {
                    sb.append("\"NONE\"");
                } else if (kv.getValue().get(i) == null) {
                    sb.append("\"INVALID\"");
                } else {
                    sb.append("\"" + kv.getValue().get(i) + "\"");
                }
            }
            if (sb.length() > 0) {
                registry.register(outputEnumName, sb.toString());
            }
        }
    }

    @SuppressWarnings("unchecked")
    private static void processYamlFile(File yamlFile,
                                        ArrayList<PinState> listPins) throws IOException {
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
        SystemOut.println(data);
        Objects.requireNonNull(data, "data");
        for (Map<String, Object> pin : data) {
            ArrayList<PinState> thisPinList = new ArrayList<>();
            Object pinId = pin.get("id");
            Object pinClass = pin.get("class");
            String pinName = (String) pin.get("ts_name");
            if (pinId == null || pinClass == null || pinName == null) {
                continue;
            }
            if (pinId instanceof ArrayList) {
                ArrayList<String> pinIds = (ArrayList<String>) pinId;
                if (!(pinClass instanceof ArrayList))
                    throw new IllegalStateException("Expected multiple classes for " + pinIds);
                for (int i = 0; i < pinIds.size(); i++) {
                    String id = pinIds.get(i);
                    addPinToList(listPins, thisPinList, id, pinName, ((ArrayList<String>) pinClass).get(i));
                }
            } else if (pinId instanceof String) {
                String pinIdString = (String) pinId;
                if (pinIdString.length() == 0) {
                    throw new IllegalStateException("Unexpected empty ID field");
                }
                addPinToList(listPins, thisPinList, pinIdString, pinName, (String) pinClass);
            } else {
                throw new IllegalStateException("Unexpected type of ID field: " + pinId.getClass().getSimpleName());
            }
            listPins.addAll(thisPinList);
        }
    }

    private static void addPinToList(ArrayList<PinState> listPins, ArrayList<PinState> thisPinList, String id, String pinName, String pinClass) {
/*
 This doesn't work as expected because it's possible that a board has multiple connector pins connected to the same MCU pin.
 https://github.com/rusefi/rusefi/issues/2897
 https://github.com/rusefi/rusefi/issues/2925
        for (int i = 0; i < listPins.size(); i++) {
            if (id.equals(listPins.get(i).get("id"))) {
                throw new IllegalStateException("ID used multiple times: " + id);
            }
        }
*/
        PinState thisPin = new PinState(id, pinName, pinClass);
        thisPinList.add(thisPin);
    }

    public static PinoutLogic create(String boardName) {
        String dirPath = PinoutLogic.CONFIG_BOARDS + boardName + PinoutLogic.CONNECTORS;
        File dirName = new File(dirPath);
        FilenameFilter filter = (f, name) -> name.endsWith(".yaml");
        File[] boardYamlFiles = dirName.listFiles(filter);
        if (boardYamlFiles == null)
            return null;
        return new PinoutLogic(boardName, boardYamlFiles);
    }

    public void processYamls(VariableRegistry registry, ReaderState state) throws IOException {
        ArrayList<PinState> listPins = new ArrayList<>();
        for (File yamlFile : boardYamlFiles) {
            processYamlFile(yamlFile, listPins);
        }
        registerPins(listPins, registry, state);
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
        private final String id;
        private final String pinName;
        private final String pinClass;

        public PinState(String id, String pinName, String pinClass) {
            this.id = id;
            this.pinName = pinName;
            this.pinClass = pinClass;
        }

        public String getId() {
            return id;
        }

        public String getPinName() {
            return pinName;
        }

        public String getPinClass() {
            return pinClass;
        }
    }
}
