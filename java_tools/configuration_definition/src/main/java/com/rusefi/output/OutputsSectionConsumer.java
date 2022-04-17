package com.rusefi.output;

import com.rusefi.ReaderState;

import java.io.*;

/**
 * [OutputChannels]
 */
public class OutputsSectionConsumer implements ConfigurationConsumer {
    private final String tsOutputsSectionFileName;
    private final TsOutput tsOutput;
    private final ReaderState state;
    private final StringBuilder tsWriter = new StringBuilder();

    public OutputsSectionConsumer(String tsOutputsSectionFileName, ReaderState state) {
        this.tsOutputsSectionFileName = tsOutputsSectionFileName;
        tsOutput = new TsOutput(state, false);
        this.state = state;
    }

    public String getContent() {
        return tsWriter.toString();
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        System.out.println("handleEndStruct");

        if (state.stack.isEmpty()) {
            tsOutput.writeTunerStudio(structure, "", tsWriter, 0);

            if (tsOutputsSectionFileName != null) {
                FileWriter fos = new FileWriter(tsOutputsSectionFileName);
                fos.write(tsWriter.toString());
                fos.close();
            }
        }
    }
}
