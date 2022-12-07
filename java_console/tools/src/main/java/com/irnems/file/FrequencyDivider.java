package com.irnems.file;

import com.rusefi.core.EngineState;
import com.rusefi.file.FileUtils;

import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Simple command line utility
 *
 *
 * Date: 3/8/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class FrequencyDivider {
    public static void main(String[] args) throws IOException {
        String filename = args.length > 1 ? args[0] : "a.csv";
        System.out.println("Reading from " + filename);

        final int divider = 10;

        String output = divider + "_" + filename;
        System.out.println("Writing to " + output);
        final FileOutputStream fos = new FileOutputStream(output);


        EngineState.EngineStateListener listener = new EngineState.EngineStateListenerImpl() {
            int lineCounter = 0;

            @Override
            public void beforeLine(String fullLine) {
                if (lineCounter < 5 || lineCounter % divider == 0) {
                    String w = fullLine + "\r\n";
                    try {
                        fos.write(w.getBytes());
                    } catch (IOException e) {
                        throw new IllegalStateException(e);
                    }
                }
                lineCounter++;
            }
        };
        FileUtils.readFile(filename, listener);
        fos.close();
    }
}
