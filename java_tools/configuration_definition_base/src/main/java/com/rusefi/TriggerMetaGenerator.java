package com.rusefi;

import com.rusefi.output.FileJavaVariableRegistryConsumer;
import com.rusefi.util.LazyFile;

import java.io.IOException;

public class TriggerMetaGenerator {
    public static void doJob(String definitionInputFileName, String headerFileName, String javaDestinationFolder, String javaDestinationFileName) throws IOException {
        System.out.println("Running with definitionInputFileName=[" + definitionInputFileName + "]");
        System.out.println("Running with headerFileName=[" + headerFileName + "]");
        System.out.println("Running with javaDestinationFileName=[" + javaDestinationFileName + "]");
        ReaderStateImpl reader = new ReaderStateImpl();
        reader.setDefinitionInputFile(definitionInputFileName);
        reader.addCHeaderDestination(headerFileName);
        reader.addJavaDestination(javaDestinationFolder + javaDestinationFileName);
        reader.addDestination(new FileJavaVariableRegistryConsumer(reader, javaDestinationFolder, LazyFile.REAL, "TriggerVariableRegistryValues"));
        reader.doJob();
    }
}
