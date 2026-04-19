package com.rusefi;

import com.rusefi.output.FileJavaVariableRegistryConsumer;
import com.rusefi.util.LazyFile;

import java.io.IOException;

public class TriggerMetaGenerator {
    public static void doJob(String definitionInputFileName, String headerFileName, String javaDestinationFolder) throws IOException {
        System.out.println("Running with definitionInputFileName=[" + definitionInputFileName + "]");
        System.out.println("Running with headerFileName=[" + headerFileName + "]");
        ReaderStateImpl reader = new ReaderStateImpl();
        reader.setDefinitionInputFile(definitionInputFileName);
        reader.addCHeaderDestination(headerFileName);
        reader.addDestination(new FileJavaVariableRegistryConsumer(reader, javaDestinationFolder, LazyFile.REAL, "TriggerVariableRegistryValues"));
        reader.doJob();
    }
}
