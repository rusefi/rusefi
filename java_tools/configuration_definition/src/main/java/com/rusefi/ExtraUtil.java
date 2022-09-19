package com.rusefi;

import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.parsing.Definition;
import com.rusefi.util.LazyFile;
import com.rusefi.util.SystemOut;

import java.io.IOException;

public class ExtraUtil {
    static void handleFiringOrder(String firingEnumFileName, VariableRegistry variableRegistry, ParseState parseState) throws IOException {
        if (firingEnumFileName != null) {
            SystemOut.println("Reading firing from " + firingEnumFileName);
            String result = FiringOrderTSLogic.invoke(firingEnumFileName);
            variableRegistry.register("FIRINGORDER", result);
            parseState.addDefinition("FIRINGORDER", result, Definition.OverwritePolicy.NotAllowed);
        }
    }

    public static void writeDefinesToFile(VariableRegistry variableRegistry, String fileName, String headerComment) throws IOException {

        SystemOut.println("Writing to " + fileName);
        LazyFile cHeader = new LazyFile(fileName);

        cHeader.write("//\n// " + ToolUtil.getGeneratedAutomaticallyTag() + headerComment + "\n//\n\n");
        cHeader.write(variableRegistry.getDefinesSection());
        cHeader.close();
    }
}
