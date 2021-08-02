package com.rusefi;

import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.output.ConfigurationConsumer;
import com.rusefi.util.SystemOut;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.*;

import static com.rusefi.ConfigField.BOOLEAN_T;

/**
 * We keep state here as we read configuration definition
 * <p>
 * Andrey Belomutskiy, (c) 2013-2020
 * 12/19/18
 */
public class ReaderState {
    public static final String BIT = "bit";
    public static final String DEFINE = "#define";
    private static final String CUSTOM = "custom";
    private static final String END_STRUCT = "end_struct";
    private static final String STRUCT_NO_PREFIX = "struct_no_prefix ";
    private static final String STRUCT = "struct ";
    private static final String DEFINE_CONSTRUCTOR = "define_constructor";
    public static final char MULT_TOKEN = '*';
    public Stack<ConfigStructure> stack = new Stack<>();
    public Map<String, Integer> tsCustomSize = new HashMap<>();
    public Map<String, String> tsCustomLine = new HashMap<>();
    public Map<String, ConfigStructure> structures = new HashMap<>();

    public EnumsReader enumsReader = new EnumsReader();


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
        String[] bitNameParts = bitName.split(",");

        String trueName = bitNameParts.length > 1 ? bitNameParts[1].replaceAll("\"", "") : null;
        String falseName = bitNameParts.length > 2 ? bitNameParts[2].replaceAll("\"", "") : null;

        ConfigField bitField = new ConfigField(state, bitNameParts[0], comment, null, BOOLEAN_T, 0, null, false, false, null, -1, trueName, falseName);
        if (state.stack.isEmpty())
            throw new IllegalStateException("Parent structure expected");
        ConfigStructure structure = state.stack.peek();
        structure.addBitField(bitField);
    }

    static boolean isEmptyDefinitionLine(String line) {
        /**
         * historically somehow '!' was the start of comment line
         * '//' is the later added alternative.
         */
        return line.length() == 0 || line.startsWith("!") || line.startsWith("//");
    }

    private static void handleCustomLine(ReaderState state, String line) {
        line = line.substring(CUSTOM.length() + 1).trim();
        int index = line.indexOf(' ');
        String name = line.substring(0, index);

        String autoEnumOptions = VariableRegistry.INSTANCE.getEnumOptionsForTunerStudio(state.enumsReader, name);
        if (autoEnumOptions != null) {
            VariableRegistry.INSTANCE.register(name + "_auto_enum", autoEnumOptions);
        }
        
        line = line.substring(index).trim();
        index = line.indexOf(' ');
        String customSize = line.substring(0, index);

        String tunerStudioLine = line.substring(index).trim();
        tunerStudioLine = VariableRegistry.INSTANCE.applyVariables(tunerStudioLine);
        int size = parseSize(customSize, line);
        state.tsCustomSize.put(name, size);

        RawIniFile.Line rawLine = new RawIniFile.Line(tunerStudioLine);
        if (rawLine.getTokens()[0].equals("bits")) {
            EnumIniField.ParseBitRange bitRange = new EnumIniField.ParseBitRange().invoke(rawLine.getTokens()[3]);
            int totalCount = 1 << (bitRange.getBitSize0() + 1);
            List<String> enums = Arrays.asList(rawLine.getTokens()).subList(4, rawLine.getTokens().length);
            if (enums.size() > totalCount)
                throw new IllegalStateException(name + ": Too many options in " + tunerStudioLine + " capacity=" + totalCount + "/size=" + enums.size());
/*
    this does not work right now since smt32 and kinetis enum sizes could be different but same .txt file
    todo: identify relevant bitsizes and use variables for bitsizes?
            if (enums.size() <= totalCount / 2)
                throw new IllegalStateException("Too many bits allocated for " + enums + " capacity=" + totalCount + "/size=" + enums.size());
*/
            for (int i = enums.size(); i < totalCount; i++)
                tunerStudioLine += ", \"INVALID\"";
        }

        state.tsCustomLine.put(name, tunerStudioLine);
    }

    public static int parseSize(String customSize, String line) {
        customSize = VariableRegistry.INSTANCE.applyVariables(customSize);
        customSize = customSize.replaceAll("x", "*");
        line = VariableRegistry.INSTANCE.applyVariables(line);

        int multPosition = customSize.indexOf(MULT_TOKEN);
        if (multPosition != -1) {
            String firstPart = customSize.substring(0, multPosition).trim();
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

    private void handleEndStruct(List<ConfigurationConsumer> consumers) throws IOException {
        if (stack.isEmpty())
            throw new IllegalStateException("Unexpected end_struct");
        ConfigStructure structure = stack.pop();
        SystemOut.println("Ending structure " + structure.getName());
        structure.addAlignmentFill(this);

        structures.put(structure.getName(), structure);

        for (ConfigurationConsumer consumer : consumers)
            consumer.handleEndStruct(structure);
    }

    public void readBufferedReader(BufferedReader definitionReader, List<ConfigurationConsumer> consumers) throws IOException {
        for (ConfigurationConsumer consumer : consumers)
            consumer.startFile();

        int lineIndex = 0;
        String line;
        while ((line = definitionReader.readLine()) != null) {
            lineIndex++;
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
                addBitPadding();
                handleEndStruct(consumers);
            } else if (line.startsWith(BIT)) {
                handleBitLine(this, line);

            } else if (ConfigDefinition.startsWithToken(line, CUSTOM)) {
                handleCustomLine(this, line);

            } else if (ConfigDefinition.startsWithToken(line, DEFINE)) {
                /**
                 * for example
                 * #define CLT_CURVE_SIZE 16
                 */
                ConfigDefinition.processDefine(VariableRegistry.INSTANCE, line.substring(DEFINE.length()).trim());
            } else {
                if (stack.isEmpty())
                    throw new IllegalStateException("Expected to be within structure at line " + lineIndex + ": " + line);
                addBitPadding();
                processField(this, line);
            }
        }
        for (ConfigurationConsumer consumer : consumers)
            consumer.endFile();
        ensureEmptyAfterProcessing();
    }

    private void addBitPadding() {
        ConfigStructure structure = stack.peek();
        structure.addBitPadding(this);
    }

    public void ensureEmptyAfterProcessing() {
        if (!stack.isEmpty())
            throw new IllegalStateException("Unclosed structure: " + stack.peek().getName());
    }

    private static void handleStartStructure(ReaderState state, String line, boolean withPrefix) {
        boolean withConstructor;
        if (line.toLowerCase().startsWith(DEFINE_CONSTRUCTOR)) {
            withConstructor = true;
            line = line.substring(DEFINE_CONSTRUCTOR.length()).trim();
        } else {
            withConstructor = false;
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
        ConfigStructure structure = new ConfigStructure(name, comment, withPrefix, withConstructor);
        state.stack.push(structure);
        SystemOut.println("Starting structure " + structure.getName());
    }

    private static void processField(ReaderState state, String line) {

        ConfigField cf = ConfigField.parse(state, line);

        if (cf == null) {
            if (ConfigField.isPreprocessorDirective(state, line)) {
                cf = new ConfigField(state, "", line, null,
                        ConfigField.DIRECTIVE_T, 0, null, false, false, null, 0, null, null);
            } else {
                throw new IllegalStateException("Cannot parse line [" + line + "]");
            }
        }

        if (state.stack.isEmpty())
            throw new IllegalStateException(cf.getName() + ": Not enclosed in a struct");
        ConfigStructure structure = state.stack.peek();

        Integer getPrimitiveSize = TypesHelper.getPrimitiveSize(cf.getType());
        if (getPrimitiveSize != null && getPrimitiveSize % 4 == 0) {
            SystemOut.println("Need to align before " + cf.getName());
            structure.addAlignmentFill(state);
        } else {
            // adding a structure instance - had to be aligned
            // todo?           structure.addAlignmentFill(state);
        }

        if (cf.isIterate()) {
            structure.addC(cf);
            for (int i = 1; i <= cf.getArraySize(); i++) {
                ConfigField element = new ConfigField(state, cf.getName() + i, cf.getComment(), null,
                        cf.getType(), 1, cf.getTsInfo(), false, false, cf.getName(), i, null, null);
                structure.addTs(element);
            }
        } else if (cf.isDirective()) {
            structure.addTs(cf);
        } else {
            structure.addBoth(cf);
        }
    }

}
