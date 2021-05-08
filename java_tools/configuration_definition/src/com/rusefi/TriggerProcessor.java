package com.rusefi;

import com.rusefi.enum_reader.Value;
import com.rusefi.models.trigger.WheelMetaReader;

import java.io.FileNotFoundException;
import java.util.Map;
import java.util.TreeMap;

public class TriggerProcessor {

    public void run(EnumsReader enumsReader, VariableRegistry instance) {
        Map<String, Map<String, Value>> enums = enumsReader.getEnums();

        Map<String/*enum member*/, Value> triggers = enums.get("trigger_type_e");
        System.out.println("triggers2: " + triggers);

        System.out.println(instance.intValues);

        TreeMap<Integer, String> valueNameById = instance.resolveEnumValues(enumsReader, "trigger_type_e");

        System.out.println("resolved: " + valueNameById);
    }


    /**
     * this method is used for manual testing only
     */
    public static void main(String[] args) throws FileNotFoundException {
        WheelMetaReader.readTriggers("../../" + WheelMetaReader.CONTROLLERS_TRIGGER_DECODERS_TRIGGERS_META_YAML);
    }
}
