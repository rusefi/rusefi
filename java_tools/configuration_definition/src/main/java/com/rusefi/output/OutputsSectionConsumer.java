package com.rusefi.output;

import com.rusefi.ReaderState;

import java.io.*;

public class OutputsSectionConsumer implements ConfigurationConsumer {
    private final String tsOutputsSectionFileName;
    private final TsOutput tsOutput;
    private final ReaderState state;
    private final CharArrayWriter tsWriter = new CharArrayWriter();

    public OutputsSectionConsumer(String tsOutputsSectionFileName, ReaderState state) {
        this.tsOutputsSectionFileName = tsOutputsSectionFileName;
        tsOutput = new TsOutput(state, false);
        this.state = state;
    }

    @Override
    public void startFile() throws IOException {
    }

    public CharArrayWriter getTsWriter() {
        return tsWriter;
    }

    @Override
    public void endFile() throws IOException {
    }

    @Override
    public void handleEndStruct(ConfigStructure structure) throws IOException {
        System.out.println("handleEndStruct");

        if (state.stack.isEmpty()) {
            tsOutput.writeTunerStudio(structure, "", tsWriter, 0);

            if (tsOutputsSectionFileName != null) {
                FileWriter fos = new FileWriter(tsOutputsSectionFileName);
                fos.write(tsWriter.toCharArray());
                fos.close();
            }
        }
    }
}
