package com.rusefi;

import com.rusefi.newparse.DefinitionsState;
import com.rusefi.newparse.parsing.Definition;
import com.rusefi.util.SystemOut;

import java.io.IOException;

public class ExtraUtil {
    static void handleFiringOrder(String firingEnumFileName, VariableRegistry variableRegistry, DefinitionsState parseState) throws IOException {
        if (firingEnumFileName != null) {
            SystemOut.println("Reading firing from " + firingEnumFileName);
            String result = FiringOrderTSLogic.invoke(firingEnumFileName);
            parseState.addDefinition(variableRegistry, "FIRINGORDER", result, Definition.OverwritePolicy.NotAllowed);
        }
    }
}
