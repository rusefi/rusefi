package com.rusefi;

import java.io.BufferedWriter;
import java.io.IOException;

import static com.rusefi.ConfigDefinition.EOL;

/**
 * Configuration consumer which writes C header file
 */
public class CHeaderConsumer implements ConfigurationConsumer {
    private final BufferedWriter cHeader;

    public CHeaderConsumer(BufferedWriter cHeader) {
        this.cHeader = cHeader;
    }

    @Override
    public void startFile() throws IOException {
        cHeader.write("// this section " + ConfigDefinition.MESSAGE + EOL);
        cHeader.write("// begin" + EOL);
        cHeader.write("#ifndef ENGINE_CONFIGURATION_GENERATED_H_" + EOL);
        cHeader.write("#define ENGINE_CONFIGURATION_GENERATED_H_" + EOL);
        cHeader.write("#include \"rusefi_types.h\"" + EOL);
    }

    @Override
    public void endFile() throws IOException {
        cHeader.write("#endif" + EOL);
        cHeader.write("// end" + EOL);
        cHeader.write("// this section " + ConfigDefinition.MESSAGE + EOL);
    }

    @Override
    public void handleEndStruct(ConfigStructure structure) throws IOException {
        ConfigStructure.headerWrite(structure, cHeader);
    }

    @Override
    public void onTotalEnd() {

    }
}
