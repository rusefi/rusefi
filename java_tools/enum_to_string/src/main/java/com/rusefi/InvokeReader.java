package com.rusefi;

import com.rusefi.util.SystemOut;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

class InvokeReader {
    private final static String KEY_INPUT_PATH = "-enumInputPath";
    private final static String KEY_OUTPUT = "-outputPath";
    private final static String KEY_OUTPUT_FILE = "-generatedFile";
    private static final String KEY_DEFINITION = "-definition";
    public static String fileSuffix = "enums";

    private String[] args;
    private String outputPath;
    private List<String> definitionInputFiles = new ArrayList<>();
    private List<String> inputFiles = new ArrayList<>();
    private String inputPath = ".";

    public InvokeReader(String... args) {
        if (args.length < 4) {
            SystemOut.println("Please specify at least\n\n" +
                    EnumToString.KEY_ENUM_INPUT_FILE + "XXX\n" +
                    KEY_OUTPUT + "XXX\n"
            );
            System.exit(-1);
        }

        this.args = args;
    }

    public String getInputPath() {
        return inputPath;
    }

    public List<String> getDefinitionInputFiles() {
        return definitionInputFiles;
    }

    public String getOutputPath() {
        return outputPath;
    }

    public List<String> getInputFiles() {
        return inputFiles;
    }

    public InvokeReader invoke() throws IOException {
        outputPath = null;
        for (int i = 0; i < args.length - 1; i += 2) {
            String key = args[i];
            switch (key) {
                case KEY_DEFINITION:
                    definitionInputFiles.add(args[i + 1]);
                    break;
                case KEY_INPUT_PATH:
                    inputPath = Objects.requireNonNull(args[i + 1], KEY_INPUT_PATH);
                    break;
                case EnumToString.KEY_ENUM_INPUT_FILE:
                    String headerInputFile = args[i + 1];
                    inputFiles.add(headerInputFile);
                    break;
                case KEY_OUTPUT_FILE:
                    fileSuffix = args[i + 1];
                    break;
                case KEY_OUTPUT:
                    outputPath = args[i + 1];
                    break;
            }
        }
        return this;
    }
}
