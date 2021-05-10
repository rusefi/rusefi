package com.rusefi.output;

import com.rusefi.*;
import com.rusefi.util.LazyFile;
import com.rusefi.util.SystemOut;

import java.io.IOException;

import static com.rusefi.ConfigDefinition.EOL;

/**
 * Configuration consumer which writes C header file
 */
public class CHeaderConsumer extends BaseCHeaderConsumer {
    /**
     * looks like sometimes we want to not include "define XXX value" into generated C headers
     * TODO: document the use-case better
     */
    public static boolean withC_Defines;
    private final LazyFile cHeader;

    public CHeaderConsumer(String destCHeader) {
        SystemOut.println("Writing C header to " + destCHeader);
        cHeader = new LazyFile(destCHeader);
        cHeader.write("// this section " + ConfigDefinition.MESSAGE + EOL);
        cHeader.write("// by " + getClass() + EOL);
        cHeader.write("// begin" + EOL);
        cHeader.write("#pragma once" + EOL);
        cHeader.write("#include \"rusefi_types.h\"" + EOL);
    }

    @Override
    public void endFile() throws IOException {
        if (withC_Defines)
            cHeader.write(VariableRegistry.INSTANCE.getDefinesSection());
        cHeader.write(getContent().toString());
        cHeader.write("// end" + EOL);
        cHeader.write("// this section " + ConfigDefinition.MESSAGE + EOL);
        cHeader.close();
    }
}
