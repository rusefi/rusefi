package com.rusefi;

import com.rusefi.enum_reader.Value;
import com.rusefi.util.LazyFile;
import com.rusefi.util.SystemOut;

import java.io.*;
import java.util.*;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * <p/>
 * 10/6/14
 */
@SuppressWarnings("StringConcatenationInsideStringBufferAppend")
public class EnumToString {
    private final static StringBuilder cppFileContent = new StringBuilder();
    private final static StringBuilder includesSection = new StringBuilder();

    private final static StringBuilder bothFilesHeader = new StringBuilder("// by enum2string.jar tool " +
            "on " + new Date() + "\n" +
            "// see also gen_config_and_enums.bat\n" +
            "\n" +
            "\n" +
            "\n");

    private final static StringBuilder headerFileContent = new StringBuilder();

    private final static String KEY_INPUT_PATH = "-enumInputPath";
    public final static String KEY_ENUM_INPUT_FILE = "-enumInputFile";
    private final static String KEY_OUTPUT = "-outputPath";
    private final static String KEY_OUTPUT_FILE = "-generatedFile";
    private static String fileSuffix = "enums";

    public static void main(String[] args) throws IOException {
        if (args.length < 4) {
            SystemOut.println("Please specify at least\n\n" +
                            KEY_ENUM_INPUT_FILE + "XXX\n" +
                            KEY_OUTPUT + "XXX\n"
            );
            return;
        }

        String inputPath = ".";
        String outputPath = null;
        EnumsReader enumsReader = new EnumsReader();
        for (int i = 0; i < args.length - 1; i += 2) {
            String key = args[i];
            if (key.equals(KEY_INPUT_PATH)) {
                inputPath = Objects.requireNonNull(args[i + 1], KEY_INPUT_PATH);
            } else if (key.equals(KEY_ENUM_INPUT_FILE)) {
                String headerInputFile = args[i + 1];
                consumeFile(enumsReader, inputPath, headerInputFile);
            } else if (key.equals(KEY_OUTPUT_FILE)) {
                fileSuffix = args[i + 1];
            } else if (key.equals(KEY_OUTPUT)) {
                outputPath = args[i + 1];
            }
        }

        headerFileContent.append("#pragma once\n");

        outputData(enumsReader);

        cppFileContent.insert(0, bothFilesHeader.toString());

        cppFileContent.insert(0, includesSection);
        headerFileContent.insert(0, includesSection);

        SystemOut.println("includesSection:\n" + includesSection + "end of includesSection\n");

        cppFileContent.insert(0, "#include \"global.h\"\n");
        headerFileContent.insert(0, bothFilesHeader.toString());

        new File(outputPath).mkdirs();
        writeCppAndHeaderFiles(outputPath + File.separator + "auto_generated_" +
                fileSuffix);
        SystemOut.close();
    }

    private static void writeCppAndHeaderFiles(String outFileName) throws IOException {
        LazyFile bw = new LazyFile(outFileName + ".cpp");
        bw.write(cppFileContent.toString());
        bw.close();

        bw = new LazyFile(outFileName + ".h");
        bw.write(headerFileContent.toString());
        bw.close();
    }

    private static void consumeFile(EnumsReader enumsReader, String inputPath, String headerInputFileName) throws IOException {
        Objects.requireNonNull(inputPath, "inputPath");
        File f = new File(inputPath + File.separator + headerInputFileName);
        SystemOut.println("Reading enums from " + headerInputFileName);
        String simpleFileName = f.getName();

        bothFilesHeader.insert(0, "// " + LazyFile.LAZY_FILE_TAG + " from " + simpleFileName + " ");

        includesSection.append("#include \"" + simpleFileName + "\"\n");
        enumsReader.process(new FileReader(f));
    }

    public static void outputData(EnumsReader enumsReader) {
        SystemOut.println("Preparing output for " + enumsReader.getEnums().size() + " enums\n");

        for (Map.Entry<String, Map<String, Value>> e : enumsReader.getEnums().entrySet()) {
            String enumName = e.getKey();
            cppFileContent.append(makeCode(enumName, e.getValue().values()));
            headerFileContent.append(getMethodSignature(enumName) + ";\n");
        }
        SystemOut.println("EnumToString: " + headerFileContent.length() + " bytes of content\n");
    }

    public static void clear() {
        cppFileContent.setLength(0);
    }

    private static String makeCode(String enumName, Collection<Value> values) {
        StringBuilder sb = new StringBuilder();
        sb.append(getMethodSignature(enumName) + "{\n");

        sb.append("switch(value) {\n");

        for (Value e : values) {
            sb.append("case " + e.getName() + ":\n");
            sb.append("  return \"" + e.getName() + "\";\n");
        }

        sb.append("  }\n");
        sb.append(" return NULL;\n");
        sb.append("}\n");

        return sb.toString();
    }

    private static String getMethodSignature(String enumName) {
        return "const char *get" + capitalize(enumName) + "(" + enumName + " value)";
    }

    private static String capitalize(String enumName) {
        return Character.toUpperCase(enumName.charAt(0)) + enumName.substring(1);
    }

    public static String getCppFileContent() {
        return cppFileContent.toString();
    }
}
