package com.rusefi;

import com.rusefi.ldmp.LiveDataProcessor;
import com.rusefi.output.FileJavaFieldsConsumer;
import com.rusefi.util.LazyFile;

import java.io.BufferedReader;
import java.io.IOException;

import static com.rusefi.ToolUtil.EOL;

public class SimpleDefinesProcessor {
    public static void main(String[] args) throws IOException {
        if (args.length != 1)
            throw new IllegalStateException("Arguments expected");

        VariableRegistry variableRegistry = new VariableRegistry();
        String definitionInputFile = args[0];

        read(variableRegistry, definitionInputFile);

        String className = "Integration";
        LazyFile javaFields = LazyFile.REAL.create(LiveDataProcessor.JAVA_DESTINATION + className + ".java");
        FileJavaFieldsConsumer.writePackageLine(javaFields);
        FileJavaFieldsConsumer.writeClassOpenLine(javaFields, className);
        javaFields.write(variableRegistry.getJavaConstants());
        javaFields.write("}" + EOL);
        javaFields.close();
    }

    private static void read(VariableRegistry variableRegistry, String definitionInputFile) throws IOException {
        String fileNameWithRoot = RootHolder.ROOT + definitionInputFile;

        try (BufferedReader definitionReader = new BufferedReader(ReaderProvider.REAL.read(fileNameWithRoot))) {
            String line;
            while ((line = definitionReader.readLine()) != null) {
                if (VariableRegistry.looksLikeDefineLine(line))
                    variableRegistry.processLine(line);
            }
        }
    }
}
