package com.rusefi;

import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.parsing.Definition;
import com.rusefi.util.LazyFile;
import com.rusefi.util.SystemOut;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Date;

public class ExtraUtil {
    static void handleFiringOrder(String firingEnumFileName, VariableRegistry variableRegistry, ParseState parseState) throws IOException {
        if (firingEnumFileName != null) {
            SystemOut.println("Reading firing from " + firingEnumFileName);
            String result = FiringOrderTSLogic.invoke(firingEnumFileName);
            variableRegistry.register("FIRINGORDER", result);
            parseState.addDefinition("FIRINGORDER", result, Definition.OverwritePolicy.NotAllowed);
        }
    }

    static void processTextTemplate(ReaderState state, String inputFileName, String outputFileName) throws IOException {
        SystemOut.println("Reading from " + inputFileName);
        SystemOut.println("Writing to " + outputFileName);

        state.variableRegistry.put("generator_message", ToolUtil.getGeneratedAutomaticallyTag() + new Date());

        File inputFile = new File(inputFileName);

        BufferedReader fr = new BufferedReader(new FileReader(inputFile));
        LazyFile fw = new LazyFile(outputFileName);

        String line;
        while ((line = fr.readLine()) != null) {
            line = state.variableRegistry.applyVariables(line);
            fw.write(line + ToolUtil.EOL);
        }
        fw.close();
    }

    public static void writeDefinesToFile(VariableRegistry variableRegistry, String fileName, String headerComment) throws IOException {

        SystemOut.println("Writing to " + fileName);
        LazyFile cHeader = new LazyFile(fileName);

        cHeader.write("//\n// " + ToolUtil.getGeneratedAutomaticallyTag() + headerComment + "\n//\n\n");
        cHeader.write(variableRegistry.getDefinesSection());
        cHeader.close();
    }
}
