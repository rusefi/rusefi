package com.rusefi.output;

import com.rusefi.ReaderState;
import com.rusefi.ReaderStateImpl;
import com.rusefi.VariableRegistry;

import java.io.IOException;
import java.util.TreeSet;

public class PlainTsProjectConsumer implements ConfigurationConsumer {

    private final TSProjectConsumer.TSProjectConsumerState consumerState;
    private final ReaderStateImpl readerState;
    private final int pageIndex;

    public PlainTsProjectConsumer(ReaderStateImpl readerState) {
        this(readerState, new TreeSet<>(String.CASE_INSENSITIVE_ORDER));
    }

    public PlainTsProjectConsumer(ReaderStateImpl readerState, TreeSet<String> usedNames) {
        this(readerState, usedNames, 1);
    }

    public PlainTsProjectConsumer(ReaderStateImpl readerState, TreeSet<String> usedNames, int pageIndex) {
        this.readerState = readerState;
        this.pageIndex = pageIndex;
        // [tag:wue_analyzer] Keep alias offsets tied to the page being generated.
        consumerState = new TSProjectConsumer.TSProjectConsumerState(readerState, new TsOutput(true, usedNames, pageIndex));
    }

    /** Publish a secondary page's template variables to the main configuration registry. */
    public void registerPage(VariableRegistry registry) {
        registry.put("PAGE_CONTENT_" + pageIndex, getContent());
        registry.register("PAGE_SIZE_" + pageIndex, Integer.toString(getTotalSize()));
        VariableRegistry pageRegistry = readerState.getVariableRegistry();
        for (String key : pageRegistry.getKeys()) {
            if (key.startsWith(TsOutput.pageOffsetPrefix(pageIndex))) {
                registry.put(key, pageRegistry.get(key));
            }
        }
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        consumerState.handleEndStruct(readerState, structure);
    }

    public String getContent() {
        return consumerState.tsOutput.getContent();
    }

    public int getTotalSize() {
        return consumerState.getTotalSize();
    }
}
