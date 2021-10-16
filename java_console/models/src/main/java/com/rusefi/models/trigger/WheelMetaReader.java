package com.rusefi.models.trigger;

import org.yaml.snakeyaml.Yaml;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.List;
import java.util.Map;

public class WheelMetaReader {
    public static final String CONTROLLERS_TRIGGER_DECODERS_TRIGGERS_META_YAML = "firmware/controllers/trigger/decoders/triggers-meta.yaml";

    public static List<Map<String, String>> readTriggers(String triggerMetaYamlFile) throws FileNotFoundException {
        Yaml yaml = new Yaml();
        Map<String, Object> data = yaml.load(new FileReader(triggerMetaYamlFile));
        System.out.println(data);

        List<Map<String, String>> triggers = (List) data.get("triggers");
        System.out.println(triggers);
        for (Map<String, String> trigger : triggers)
            System.out.println("Trigger " + trigger);
        return triggers;
    }
}
