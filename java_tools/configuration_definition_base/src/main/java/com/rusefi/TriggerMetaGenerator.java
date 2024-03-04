package com.rusefi;

import java.io.IOException;

public class TriggerMetaGenerator {
    public static void doJob(String definitionInputFileName, String headerFileName, String javaDestinationFileName) throws IOException {
        System.out.println("Running with definitionInputFileName=[" + definitionInputFileName + "]");
        System.out.println("Running with headerFileName=[" + headerFileName + "]");
        System.out.println("Running with javaDestinationFileName=[" + javaDestinationFileName + "]");
        ReaderStateImpl reader = new ReaderStateImpl();
        reader.setDefinitionInputFile(definitionInputFileName);
        reader.addCHeaderDestination(headerFileName);
        reader.addJavaDestination(javaDestinationFileName);
        reader.doJob();
    }
}
