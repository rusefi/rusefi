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
    private final StringBuilder cppFileContent = new StringBuilder();
    private final StringBuilder includesSection = new StringBuilder();

    private final static StringBuilder bothFilesHeader = new StringBuilder("// by enum2string.jar tool " +
            "on " + new Date() + "\n" +
            "// see also gen_config_and_enums.bat\n" +
            "\n" +
            "\n" +
            "\n");

    private final static StringBuilder headerFileContent = new StringBuilder();

    public final static String KEY_ENUM_INPUT_FILE = "-enumInputFile";

    public static void main(String[] args) throws IOException {
        InvokeReader invokeReader = new InvokeReader(args).invoke();
        String outputPath = invokeReader.getOutputPath();

        EnumsReader enumsReader = new EnumsReader();
        EnumToString state = new EnumToString();

        for (String inputFile : invokeReader.getInputFiles()) {
            state.consumeFile(enumsReader, invokeReader.getInputPath(), inputFile);
        }

        headerFileContent.append("#pragma once\n");

        state.outputData(enumsReader);

        state.cppFileContent.insert(0, bothFilesHeader.toString());

        state.cppFileContent.insert(0, state.includesSection);
        headerFileContent.insert(0, state.includesSection);

        SystemOut.println("includesSection:\n" + state.includesSection + "end of includesSection\n");

        state.cppFileContent.insert(0, "#include \"global.h\"\n");
        headerFileContent.insert(0, bothFilesHeader.toString());

        new File(outputPath).mkdirs();
        state.writeCppAndHeaderFiles(outputPath + File.separator + "auto_generated_" +
                InvokeReader.fileSuffix);
        SystemOut.close();
    }

    private void writeCppAndHeaderFiles(String outFileName) throws IOException {
        LazyFile bw = new LazyFile(outFileName + ".cpp");
        bw.write(cppFileContent.toString());
        bw.close();

        bw = new LazyFile(outFileName + ".h");
        bw.write(headerFileContent.toString());
        bw.close();
    }

    public void consumeFile(EnumsReader enumsReader, String inputPath, String headerInputFileName) throws IOException {
        Objects.requireNonNull(inputPath, "inputPath");
        File f = new File(inputPath + File.separator + headerInputFileName);
        SystemOut.println("Reading enums from " + headerInputFileName);

        bothFilesHeader.insert(0, "// " + LazyFile.LAZY_FILE_TAG + " from " + f.getName() + " ");

        includesSection.append("#include \"" + f.getName() + "\"\n");
        enumsReader.read(new FileReader(f));
    }

    public EnumToString outputData(EnumsReader enumsReader) {
        SystemOut.println("Preparing output for " + enumsReader.getEnums().size() + " enums\n");

        for (Map.Entry<String, EnumsReader.EnumState> e : enumsReader.getEnums().entrySet()) {
            String enumName = e.getKey();
            EnumsReader.EnumState enumState = e.getValue();
            cppFileContent.append(makeCode(enumName, enumState));
            headerFileContent.append(getMethodSignature(enumName) + ";\n");
        }
        SystemOut.println("EnumToString: " + headerFileContent.length() + " bytes of content\n");
        return this;
    }

    private static String makeCode(String enumName, EnumsReader.EnumState enumState) {
        StringBuilder sb = new StringBuilder();
        Collection<Value> values = enumState.values.values();
        sb.append(getMethodSignature(enumName) + "{\n");

        sb.append("switch(value) {\n");

        for (Value e : values) {
            sb.append("case ");
            if (enumState.isEnumClass) {
                sb.append(enumState.enumName).append("::");
            }
            sb.append(e.getName() + ":\n");
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

    public String getCppFileContent() {
        return cppFileContent.toString();
    }

}
