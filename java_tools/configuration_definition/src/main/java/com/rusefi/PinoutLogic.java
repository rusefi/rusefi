package com.rusefi;

import com.rusefi.enum_reader.Value;
import com.rusefi.util.SystemOut;
import org.yaml.snakeyaml.Yaml;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

public class PinoutLogic {
    public static void processYamls(VariableRegistry registry, File[] yamlFiles, ReaderState state) throws IOException {
        ArrayList<Map<String, Object>> listPins = new ArrayList<>();
        for (File yamlFile : yamlFiles) {
            processYamlFile(yamlFile, listPins);
        }
        registerPins(listPins, registry, state);
    }

    private static void registerPins(ArrayList<Map<String, Object>> listPins, VariableRegistry registry, ReaderState state) {
        if (listPins == null || listPins.isEmpty()) {
            return;
        }
        Map<String, ArrayList<String>> names = new HashMap<>();
        names.put("outputs", new ArrayList<>());
        names.put("analog_inputs", new ArrayList<>());
        names.put("event_inputs", new ArrayList<>());
        names.put("switch_inputs", new ArrayList<>());
        for (Map<String, Object> listPin : listPins) {
            String id = (String) listPin.get("id");
            String className = (String) listPin.get("class");
            ArrayList<String> classList = names.get(className);
            if (classList == null) {
                throw new IllegalStateException("Class not found:  " + className);
            }
            PinType listPinType = PinType.find((String) listPin.get("class"));
            String pinType = listPinType.getPinType();
            EnumsReader.EnumState enumList = state.enumsReader.getEnums().get(pinType);
            for (Map.Entry<String, Value> kv : enumList.entrySet()) {
                if (kv.getKey().equals(id)) {
                    int index = kv.getValue().getIntValue();
                    classList.ensureCapacity(index + 1);
                    for (int ii = classList.size(); ii <= index; ii++) {
                        classList.add(null);
                    }
                    classList.set(index, (String) listPin.get("ts_name"));
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
                                        ArrayList<Map<String, Object>> listPins) throws IOException {
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
            ArrayList<Map<String, Object>> thisPinList = new ArrayList<>();
            Object pinId = pin.get("id");
            Object pinClass = pin.get("class");
            Object pinName = pin.get("ts_name");
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
                if (((String) pinId).length() == 0) {
                    throw new IllegalStateException("Unexpected empty ID field");
                }
                addPinToList(listPins, thisPinList, pinId, pinName, pinClass);
            } else {
                throw new IllegalStateException("Unexpected type of ID field: " + pinId.getClass().getSimpleName());
            }
            listPins.addAll(thisPinList);
        }
    }

    private static void addPinToList(ArrayList<Map<String, Object>> listPins, ArrayList<Map<String, Object>> thisPinList, Object id, Object pinName, Object pinClass) {
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
        Map<String, Object> thisPin = new HashMap<>();
        thisPin.put("id", id);
        thisPin.put("ts_name", pinName);
        thisPin.put("class", pinClass);
        thisPinList.add(thisPin);
    }
}
