package com.rusefi.output;

import com.rusefi.*;
import com.rusefi.util.LazyFile;
import com.rusefi.util.SystemOut;

import java.io.IOException;

import static com.rusefi.ToolUtil.EOL;

import org.jetbrains.annotations.NotNull;

/**
 * Configuration consumer which writes C header file
 */
public class CHeaderConsumer extends BaseCHeaderConsumer {
    @NotNull
    private final ReaderState state;
    /**
     * looks like sometimes we want to not include "define XXX value" into generated C headers
     * TODO: document the use-case better
     */
    private final boolean withC_Defines;
    private final LazyFile cHeader;
    private final VariableRegistry variableRegistry;

    public CHeaderConsumer(ReaderState state, String destCHeader, boolean withC_Defines) {
        this.variableRegistry = state.variableRegistry;
        this.state = state;
        this.withC_Defines = withC_Defines;
        SystemOut.println("Writing C header to " + destCHeader);
        cHeader = new LazyFile(destCHeader);
        cHeader.write("// this section " + state.getHeader() + EOL);
        cHeader.write("// by " + getClass() + EOL);
        cHeader.write("// begin" + EOL);
        cHeader.write("#pragma once" + EOL);
        cHeader.write("#include \"rusefi_types.h\"" + EOL);
    }

    @Override
    public void endFile() throws IOException {
        if (withC_Defines)
            cHeader.write(variableRegistry.getDefinesSection());
        cHeader.write(getContent().toString());
        cHeader.write("// end" + EOL);
        cHeader.write("// this section " + state.getHeader() + EOL);
        cHeader.close();
    }
}
