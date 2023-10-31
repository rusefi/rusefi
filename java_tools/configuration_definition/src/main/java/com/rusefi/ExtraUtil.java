package com.rusefi;

import com.rusefi.util.LazyFile;
import com.rusefi.util.SystemOut;

import java.io.IOException;

public class ExtraUtil {
    public static void writeDefinesToFile(VariableRegistry variableRegistry, String fileName) throws IOException {

        SystemOut.println("Writing to " + fileName);
        LazyFile cHeader = new LazyFile(fileName);

        cHeader.write(variableRegistry.getDefinesSection());
        cHeader.close();
    }
}
