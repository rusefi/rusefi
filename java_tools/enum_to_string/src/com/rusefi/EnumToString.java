package com.rusefi;

import com.rusefi.enum_reader.Value;

import java.io.*;
import java.util.*;

/**
 * (c) Andrey Belomutskiy
 * <p/>
 * 10/6/14
 */
@SuppressWarnings("StringConcatenationInsideStringBufferAppend")
public class EnumToString {
    private final static StringBuilder cppFileContent = new StringBuilder();
    private final static StringBuilder includesSection = new StringBuilder();

    private final static StringBuilder bothFilesHeader = new StringBuilder("// by enum2string.jar tool\r\n" +
            "// on " + new Date() + "\r\n" +
            "// see also gen_config_and_enums.bat\r\n" +
            "\r\n" +
            "\r\n" +
            "\r\n");

    private final static StringBuilder headerFileContent = new StringBuilder();

    private final static String KEY_INPUT_PATH = "-inputPath";
    private final static String KEY_ENUM_INPUT_FILE = "-enumInputFile";
    private final static String KEY_OUTPUT = "-outputPath";

    public static void main(String[] args) throws IOException {
        if (args.length < 4) {
            System.out.println("Please specify at least\n\n" +
                            KEY_INPUT_PATH + "XXX\r\n" +
//                            KEY_INPUT_FILE + "XXX" +
                            KEY_OUTPUT + "XXX\r\n"
            );
            return;
        }

        String inputPath = null;
        String outputPath = null;
        for (int i = 0; i < args.length - 1; i += 2) {
            String key = args[i];
            if (key.equals(KEY_INPUT_PATH)) {
                inputPath = args[i + 1];
            } else if (key.equals(KEY_ENUM_INPUT_FILE)) {
                String inputFile = args[i + 1];
                consumeFile(inputPath, inputFile);
            } else if (key.equals(KEY_OUTPUT)) {
                outputPath = args[i + 1];
            }

        }

        headerFileContent.append("#ifndef _A_H_HEADER_\r\n");
        headerFileContent.append("#define _A_H_HEADER_\r\n");

        outputData();

        cppFileContent.insert(0, bothFilesHeader.toString());

        cppFileContent.insert(0, includesSection);
        headerFileContent.insert(0, includesSection);

        System.out.println("includesSection:\r\n" + includesSection + "end of includesSection\r\n");

        cppFileContent.insert(0, "#include \"global.h\"\r\n");
        headerFileContent.insert(0, bothFilesHeader.toString());

        headerFileContent.append("#endif /*_A_H_HEADER_ */\r\n");

        writeCppAndHeaderFiles(outputPath + File.separator + "auto_generated_enums");

    }

    private static void writeCppAndHeaderFiles(String outFileName) throws IOException {
        BufferedWriter bw = new BufferedWriter(new FileWriter(outFileName + ".cpp"));
        bw.write(cppFileContent.toString());
        bw.close();

        bw = new BufferedWriter(new FileWriter(outFileName + ".h"));
        bw.write(headerFileContent.toString());
        bw.close();
    }

    private static void consumeFile(String inputPath, String inFileName) throws IOException {
        File f = new File(inputPath + File.separator + inFileName);
        System.out.println("Reading from " + inFileName);
        String simpleFileName = f.getName();

        bothFilesHeader.insert(0, "// auto-generated from " + simpleFileName + "\r\n");

        includesSection.append("#include \"" + simpleFileName + "\"\r\n");
        EnumsReader.process(new FileReader(inFileName));
    }

    public static void outputData() {
        for (Map.Entry<String, Map<String, Value>> e : EnumsReader.enums.entrySet()) {
            String enumName = e.getKey();
            cppFileContent.append(makeCode(enumName, e.getValue().values()));
            EnumToString.headerFileContent.append(getMethodSignature(enumName) + ";\r\n");
        }
    }

    public static void clear() {
        cppFileContent.setLength(0);
    }

    private static String makeCode(String enumName, Collection<Value> values) {
        StringBuilder sb = new StringBuilder();
        sb.append(getMethodSignature(enumName) + "{\r\n");

        sb.append("switch(value) {\r\n");

        for (Value e : values) {
            sb.append("case " + e.getName() + ":\r\n");
            sb.append("  return \"" + e.getName() + "\";\r\n");
        }

        sb.append("  }\r\n");
        sb.append(" return NULL;\r\n");
        sb.append("}\r\n");

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
