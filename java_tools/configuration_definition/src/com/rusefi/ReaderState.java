package com.rusefi;

import com.rusefi.output.ConfigurationConsumer;
import com.rusefi.util.SystemOut;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Stack;

import static com.rusefi.ConfigField.BOOLEAN_T;

/**
 * We keep state here as we read configuration definition
 *
 * (c) Andrey Belomutskiy
 * 12/19/18
 */
public class ReaderState {
    private static final String BIT = "bit";
    protected static final String DEFINE = "#define";
    private static final String CUSTOM = "custom";
    private static final String END_STRUCT = "end_struct";
    private static final String STRUCT_NO_PREFIX = "struct_no_prefix ";
    private static final String STRUCT = "struct ";
    private static final String DEFINE_CONTRUCTOR = "define_contructor";
    public Stack<ConfigStructure> stack = new Stack<>();
    public Map<String, Integer> tsCustomSize = new HashMap<>();
    public Map<String, String> tsCustomLine = new HashMap<>();
    public Map<String, ConfigStructure> structures = new HashMap<>();

    private static void handleBitLine(ReaderState state, String line) {
        line = line.substring(BIT.length() + 1).trim();

        String bitName;
        String comment;
        if (!line.contains(";")) {
            bitName = line;
            comment = "";
        } else {
            int index = line.indexOf(";");
            bitName = line.substring(0, index);
            comment = line.substring(index + 1);
        }

        ConfigField bitField = new ConfigField(state, bitName, comment, null, BOOLEAN_T, 0, null, false);
        state.stack.peek().addBoth(bitField);
    }

    static boolean isEmptyDefinitionLine(String line) {
        return line.length() == 0 || line.startsWith("!");
    }

    private static void handleCustomLine(ReaderState state, String line) {
        line = line.substring(CUSTOM.length() + 1).trim();
        int index = line.indexOf(' ');
        String name = line.substring(0, index);
        line = line.substring(index).trim();
        index = line.indexOf(' ');
        String customSize = line.substring(0, index);

        String tunerStudioLine = line.substring(index).trim();
        tunerStudioLine = VariableRegistry.INSTANCE.applyVariables(tunerStudioLine);
        int size = parseSize(customSize, line);
        state.tsCustomSize.put(name, size);
        state.tsCustomLine.put(name, tunerStudioLine);
    }

    public static int parseSize(String customSize, String line) {
        customSize = VariableRegistry.INSTANCE.applyVariables(customSize);
        customSize = customSize.replaceAll("x", "*");
        line = VariableRegistry.INSTANCE.applyVariables(line);

        int multPosition = customSize.indexOf('*');
        if (multPosition != -1) {
            String firstPart = customSize.substring(0, multPosition);
            int first;
            try {
                first = Integer.parseInt(firstPart);
            } catch (NumberFormatException e) {
                throw new IllegalStateException("Size in " + line);
            }
            return first * parseSize(customSize.substring(multPosition + 1), line);
        }

        try {
            return Integer.parseInt(customSize);
        } catch (NumberFormatException e) {
            throw new IllegalStateException("Size in " + line);
        }
    }

    void readBufferedReader(BufferedReader definitionReader, List<ConfigurationConsumer> consumers) throws IOException {
        for (ConfigurationConsumer consumer : consumers)
            consumer.startFile();

        String line;
        while ((line = definitionReader.readLine()) != null) {
            line = ConfigDefinition.trimLine(line);
            /**
             * we should ignore empty lines and comments
             */
            if (isEmptyDefinitionLine(line))
                continue;

            if (line.startsWith(STRUCT)) {
                handleStartStructure(this, line.substring(STRUCT.length()), true);
            } else if (line.startsWith(STRUCT_NO_PREFIX)) {
                handleStartStructure(this, line.substring(STRUCT_NO_PREFIX.length()), false);
            } else if (line.startsWith(END_STRUCT)) {
                handleEndStruct(this, consumers);
            } else if (line.startsWith(BIT)) {
                handleBitLine(this, line);

            } else if (ConfigDefinition.startsWithToken(line, CUSTOM)) {
                handleCustomLine(this, line);

            } else if (ConfigDefinition.startsWithToken(line, DEFINE)) {
                /**
                 * for example
                 * #define CLT_CURVE_SIZE 16
                 */
                ConfigDefinition.processDefine(line.substring(DEFINE.length()).trim());
            } else {
                processField(this, line);
            }
        }
        for (ConfigurationConsumer consumer : consumers)
            consumer.endFile();
    }

    public void ensureEmptyAfterProcessing() {
        if (!this.stack.isEmpty())
            throw new IllegalStateException("Unclosed structure: " + this.stack.peek().getName());
    }

    private static void handleStartStructure(ReaderState state, String line, boolean withPrefix) {
        boolean withContructor;
        if (line.toLowerCase().startsWith(DEFINE_CONTRUCTOR)) {
            withContructor = true;
            line = line.substring(DEFINE_CONTRUCTOR.length()).trim();
        } else {
            withContructor = false;
        }


        String name;
        String comment;
        if (line.contains(" ")) {
            int index = line.indexOf(' ');
            name = line.substring(0, index);
            comment = line.substring(index + 1).trim();
        } else {
            name = line;
            comment = null;
        }
        ConfigStructure structure = new ConfigStructure(name, comment, withPrefix, withContructor);
        state.stack.push(structure);
        SystemOut.println("Starting structure " + structure.getName());
    }

    private static void handleEndStruct(ReaderState state, List<ConfigurationConsumer> consumers) throws IOException {
        if (state.stack.isEmpty())
            throw new IllegalStateException("Unexpected end_struct");
        ConfigStructure structure = state.stack.pop();
        SystemOut.println("Ending structure " + structure.getName());
        structure.addAlignmentFill(state);

        state.structures.put(structure.getName(), structure);

        for (ConfigurationConsumer consumer : consumers)
            consumer.handleEndStruct(structure);
    }

    private static void processField(ReaderState state, String line) {

        ConfigField cf = ConfigField.parse(state, line);
        if (cf == null)
            throw new IllegalStateException("Cannot parse line [" + line + "]");

        if (state.stack.isEmpty())
            throw new IllegalStateException(cf.getName() + ": Not enclosed in a struct");
        ConfigStructure structure = state.stack.peek();

        if (cf.isIterate()) {
            structure.addC(cf);
            for (int i = 1; i <= cf.getArraySize(); i++) {
                ConfigField element = new ConfigField(state, cf.getName() + i, cf.getComment(), null,
                        cf.getType(), 1, cf.getTsInfo(), false);
                structure.addTs(element);
            }
        } else {
            structure.addBoth(cf);
        }
    }

}
