package com.irnems.file;

import com.irnems.core.EngineState;

import java.io.*;
import java.util.List;

/**
 * Date: 3/8/13
 * (c) Andrey Belomutskiy
 */
public class FileUtils {
    static void readFile(String filename, EngineState.EngineStateListener listener) {
        readFile2(filename, new EngineState(listener));
    }

    public static void readFile2(String filename, EngineState engineState) {

        BufferedReader reader;
        try {
            reader = new BufferedReader(new FileReader(filename));
            String line;
            while ((line = reader.readLine()) != null) {
                String packed = EngineState.packString(line);
                engineState.append(packed + "\r\n");
            }
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    public static void saveList(String filename, List<Double> values) {
        try {
            BufferedWriter w = new BufferedWriter(new FileWriter(filename));


            for (Double v : values)
                w.write(v + "\r\n");

            w.close();
        } catch (IOException e) {
            e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
        }
    }
}
