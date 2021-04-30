package com.rusefi;

import com.rusefi.enum_reader.Value;
import org.yaml.snakeyaml.Yaml;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Map;
import java.util.TreeMap;

public class TriggerProcessor {

    private static final String CONTROLLERS_TRIGGER_DECODERS_TRIGGERS_META_YAML = "firmware/controllers/trigger/decoders/triggers-meta.yaml";

    public void run(EnumsReader enumsReader, VariableRegistry instance) {
        Map<String, Map<String, Value>> enums = enumsReader.getEnums();

        Map<String/*enum member*/, Value> triggers = enums.get("trigger_type_e");
        System.out.println("triggers2: " + triggers);

        System.out.println(instance.intValues);

        TreeMap<Integer, String> valueNameById = instance.resolveEnumValues(enumsReader, "trigger_type_e");

        System.out.println("resolved: " + valueNameById);
        System.exit(-1);

    }


    /**
     * this method is used for manual testing only
     */
    public static void main(String[] args) throws FileNotFoundException {
        readTriggers("../../" + CONTROLLERS_TRIGGER_DECODERS_TRIGGERS_META_YAML);
    }

    private static void readTriggers(String triggerMetaYamlFile) throws FileNotFoundException {
        Yaml yaml = new Yaml();
        yaml.load(new FileReader(triggerMetaYamlFile));
    }
}
