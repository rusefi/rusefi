package com.rusefi.output;

import com.devexperts.logging.Logging;
import com.rusefi.*;
import com.rusefi.util.LazyFile;
import com.rusefi.util.LazyFileImpl;

import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.ToolUtil.EOL;

import org.jetbrains.annotations.NotNull;

/**
 * Configuration consumer which writes C header file
 */
public class CHeaderConsumer extends BaseCHeaderConsumer {
    private static final Logging log = getLogging(CHeaderConsumer.class);
    @NotNull
    private final ReaderState state;
    /**
     * looks like sometimes we want to not include "define XXX value" into generated C headers
     * TODO: document the use-case better
     */
    private final boolean withC_Defines;
    private final LazyFile cHeader;
    private final VariableRegistry variableRegistry;

    public CHeaderConsumer(ReaderState state, String destCHeader, boolean withC_Defines, LazyFile.LazyFileFactory fileFactory) {
        this.variableRegistry = state.getVariableRegistry();
        this.state = state;
        this.withC_Defines = withC_Defines;
        log.info("Writing C header to " + destCHeader);
        cHeader = fileFactory.create(destCHeader);
    }

    public static void writeDefinesToFile(VariableRegistry variableRegistry, String fileName, String headerComment) throws IOException {

        log.info("Writing to " + fileName);
        LazyFile cHeader = new LazyFileImpl(fileName, ConfigDefinitionRootOutputFolder.getValue() + "generated.patch");

        cHeader.write("//\n// " + CHeaderConsumer.class.getSimpleName() + " " + ToolUtil.getGeneratedAutomaticallyTag() + headerComment + "\n//\n\n");
        cHeader.write(variableRegistry.getDefinesSection());
        cHeader.close();
    }

    @Override
    public void startFile() {
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
        cHeader.write(getContent());
        cHeader.write("// end" + EOL);
        cHeader.write("// this section " + state.getHeader() + EOL);
        cHeader.close();
    }
}
