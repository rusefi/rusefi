package com.rusefi;

import java.io.IOException;

public class TriggerMetaGenerator {
    public static void main(String[] a) throws IOException {
        ReaderStateImpl reader = new ReaderStateImpl();
        reader.setDefinitionInputFile("integration/rusefi_config_trigger.txt");
        reader.addCHeaderDestination("../unit_tests/TriggerMeta.h");
        reader.addJavaDestination("../java_tools/trigger-image/src/main/java/com/rusefi/config/generated/TriggerMeta.java");

        reader.doJob();
    }
}
