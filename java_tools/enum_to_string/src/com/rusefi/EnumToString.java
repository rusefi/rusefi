package com.rusefi;

import java.io.*;
import java.util.Date;
import java.util.Map;
import java.util.Set;

/**
 * (c) Andrey Belomutskiy
 * <p/>
 * 10/6/14
 */
@SuppressWarnings("StringConcatenationInsideStringBufferAppend")
public class EnumToString {
    public final static StringBuilder cppFileContent = new StringBuilder();
    private final static StringBuilder headerFileContent = new StringBuilder();

    public static void main(String[] args) throws IOException {
        if (args.length != 2) {
            System.out.println("Please specify two parameters: path to firmware folder and path to output folder");
            return;
        }
        String inputPath = args[0];
        String outputPath = args[1];

        headerFileContent.append("#ifndef _A_H_HEADER_\r\n");
        headerFileContent.append("#define _A_H_HEADER_\r\n");

        processFile(inputPath + File.separator + "controllers/algo/rusefi_enums.h");

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

    private static void processFile(String inFileName) throws IOException {
        String header = "// auto-generated from" + inFileName + "\r\n" +
                "// by enum2string.jar tool\r\n" +
                "// on " + new Date() +"\r\n" +
                "// see also gen_config_and_enums.bat\r\n" +
                "\r\n" +
                "\r\n" +
                "\r\n";

        cppFileContent.append(header);
        EnumToString.headerFileContent.insert(0, header);


        File f = new File(inFileName);
        System.out.println("Reading from " + inFileName);
        String simpleFileName = f.getName();

        cppFileContent.append("#include \"global.h\"\r\n");
        cppFileContent.append("#include \"" + simpleFileName + "\"\r\n");
        EnumToString.headerFileContent.append("#include \"" + simpleFileName + "\"\r\n");

        process(new FileReader(inFileName));
    }

    public static void process(Reader reader) throws IOException {
        EnumsReader.process(reader);
        for (Map.Entry<String, Set<String>> e : EnumsReader.enums.entrySet()) {
            String enumName = e.getKey();
            cppFileContent.append(makeCode(enumName, e.getValue()));
            EnumToString.headerFileContent.append(getMethodSignature(enumName) + ";\r\n");
        }
    }

    public static void clear() {
        cppFileContent.setLength(0);
    }

    private static String makeCode(String enumName, Set<String> values) {
        StringBuilder sb = new StringBuilder();
        sb.append(getMethodSignature(enumName) + "{\r\n");

        sb.append("switch(value) {\r\n");

        for (String e : values) {
            sb.append("case " + e + ":\r\n");
            sb.append("  return \"" + e + "\";\r\n");
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
}
