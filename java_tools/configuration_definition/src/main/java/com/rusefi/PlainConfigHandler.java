package com.rusefi;

import com.rusefi.output.PlainTsProjectConsumer;

import java.io.IOException;
import java.util.List;
import java.util.TreeSet;

/**
 * used to prepare secondary pages in TS .ini file
 */
class PlainConfigHandler {
    ReaderStateImpl readerState = new ReaderStateImpl();
    PlainTsProjectConsumer tsProjectConsumer;

    public PlainConfigHandler(String definitionInputFile, int pageIndex, List<String> softPrepends) {
        this(definitionInputFile, pageIndex, softPrepends, new TreeSet<>(String.CASE_INSENSITIVE_ORDER));
    }

    public PlainConfigHandler(String definitionInputFile, int pageIndex, List<String> softPrepends, TreeSet<String> usedNames) {
        tsProjectConsumer = new PlainTsProjectConsumer(readerState, usedNames);
        readerState.setDefinitionInputFile(definitionInputFile);
        for (String soft : softPrepends)
            readerState.addSoftPrepend(soft);
        readerState.addPrepend("integration/rusefi_config_shared.txt");
        readerState.addPrepend("integration/ts_protocol.txt");

        readerState.addDestination(tsProjectConsumer);
        readerState.setWithC_Defines(false);
        // Path is relative to META_OUTPUT_ROOT_FOLDER (prepended in ReaderStateImpl.addCHeaderDestination)
        // so it redirects correctly when building custom-fw repos that keep generated files outside the rusefi submodule.
        readerState.addCHeaderDestination(RootHolder.ROOT + "controllers/generated/page_" + pageIndex + "_generated.h");
    }

    public void doJob() throws IOException {
        readerState.doJob();
    }
}
