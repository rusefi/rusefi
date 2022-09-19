package com.rusefi.output;

import com.rusefi.ReaderState;

import java.io.*;

/**
 * TODO: We have to move either forward or backwards with newparse #4441
 * [OutputChannels]
 */
public class OutputsSectionConsumer implements ConfigurationConsumer {
    private final String tsOutputsSectionFileName;
    private final TsOutput tsOutput;
    private int sensorTsPosition;

    public OutputsSectionConsumer(String tsOutputsSectionFileName) {
        this.tsOutputsSectionFileName = tsOutputsSectionFileName;
        tsOutput = new TsOutput(false, false);
    }

    public String getContent() {
        return tsOutput.getContent();
    }

    @Override
    public void handleEndStruct(ReaderState readerState, ConfigStructure structure) throws IOException {
        System.out.println("handleEndStruct");

        sensorTsPosition = tsOutput.run(readerState, structure, sensorTsPosition);

        if (readerState.stack.isEmpty()) {
            if (tsOutputsSectionFileName != null) {
                FileWriter fos = new FileWriter(tsOutputsSectionFileName);
                fos.write(tsOutput.getContent());
                fos.close();
            }
        }
    }
}
