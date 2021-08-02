package com.rusefi.ldmp;

import com.rusefi.util.SystemOut;
import org.yaml.snakeyaml.Yaml;

import java.io.FileReader;
import java.io.IOException;
import java.util.List;
import java.util.Map;

public class UsagesReader {
    public static void main(String[] args) throws IOException {
        if (args.length != 1) {
            System.err.println("One parameter expected: name of live data yaml input file");
            System.exit(-1);
        }
        String yamlFileName = args[0];
        Yaml yaml = new Yaml();
        Map<String, Object> data = yaml.load(new FileReader(yamlFileName));
        SystemOut.println(data);

        String destination = (String) data.get("Destination");

        List<String> usages = (List) data.get("Usages");
        SystemOut.println("Usages: " + usages);
        for (String fileName : usages) {
            LiveDocsMetaParser.handleMetaData(destination, fileName);
        }
    }
}
