package com.rusefi;

import com.rusefi.VariableRegistry;
import org.yaml.snakeyaml.Yaml;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class YamlToEnum {
    private final String yamlKey;
    private final String enumName;
    private final String firstEntry;
    private final String lastEntry;

    public YamlToEnum(String yamlKey, String enumName) {
        this(yamlKey, enumName, null, null);
    }

    public YamlToEnum(String yamlKey, String enumName, String firstEntry,  String lastEntry) {
        this.yamlKey = yamlKey;
        this.enumName = enumName;
        this.firstEntry = firstEntry;
        this.lastEntry = lastEntry;
    }

    public static void main(String[] args) throws IOException {
        if (args.length < 3) {
            System.err.println("Usage: YamlToEnum -yamlKey <yamlKey> -enumName <enumName> -input <yamlFile> -prepend <name> -append <name> -output <outputFile>");
            System.exit(1);
        }

				String yamlKey = null;
        String enumName = null;
        String yamlFile = null;
				String firstEntry = null;
				String lastEntry = null;
        String outputFile = null;
				List<String> yamlFiles = new ArrayList<>();

        for (int i = 0; i < args.length - 1; i += 2) {
            String key = args[i];
            String val = args[i + 1];
            switch (key) {
                case "-yamlKey":
										yamlKey = val;
										break;
                case "-enumName":
										enumName = val;
										break;
                case "-input":
										yamlFiles.add(val);
										break;
                case "-output":
										outputFile = val;
										break;
                case "-prepend":
										firstEntry = val;
										break;
                case "-append":
										lastEntry = val;
										break;
						}
				}

        YamlToEnum converter = new YamlToEnum(yamlKey, enumName, firstEntry, lastEntry);

        String result = converter.convert(yamlFiles);

        if (outputFile != null) {
            Files.write(Paths.get(outputFile), result.getBytes());
        } else {
            System.out.println(result);
        }
    }

    public String convert(List<String> files) throws IOException {
        List<String> enumValues = new ArrayList<>();
        if (firstEntry != null) {
            enumValues.add(firstEntry);
        }
        for (String file : files) {
            enumValues.addAll(loadEnumValues(file));
        }
        if (lastEntry != null) {
            enumValues.add(lastEntry);
        }
        return generateCppEnum(enumValues);
    }

    public List<String> loadEnumValues(String yamlFilePath) throws IOException {
        List<String> result = new ArrayList<>();

        try (BufferedReader br = new BufferedReader(new FileReader(yamlFilePath))) {
            Yaml yaml = new Yaml();
            Object obj = yaml.load(br);

            if (!(obj instanceof List)) {
                throw new IllegalStateException("Expected a list in YAML file");
            }

            @SuppressWarnings("unchecked")
            List<Map<String, Object>> items = (List<Map<String, Object>>) obj;

            for (Map<String, Object> item : items) {
                Object keyValue = item.get(yamlKey);
                if (keyValue != null) {
                    result.add(keyValue.toString());
                }
            }
        }

        return result;
    }

    private String generateCppEnum(List<String> values) {
        StringBuilder sb = new StringBuilder();
				sb.append("#pragma once\n");
        sb.append("typedef enum {\n");

        for (int i = 0; i < values.size(); i++) {
            String value = values.get(i);
            sb.append("    ").append(value).append(",\n");
        }

        sb.append("} ").append(enumName).append(";\n");
        return sb.toString();
    }
}
