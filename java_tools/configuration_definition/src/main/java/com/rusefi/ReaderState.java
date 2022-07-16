package com.rusefi;

import com.devexperts.logging.Logging;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.enum_reader.Value;
import com.rusefi.output.*;
import com.rusefi.util.IoUtils;
import com.rusefi.util.SystemOut;
import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.util.*;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.ConfigField.BOOLEAN_T;
import static com.rusefi.ConfigField.unquote;
import static com.rusefi.output.JavaSensorsConsumer.quote;

/**
 * We keep state here as we read configuration definition
 * <p>
 * Andrey Belomutskiy, (c) 2013-2020
 * 12/19/18
 */
public class ReaderState {
    // used to update other files
    public List<String> inputFiles = new ArrayList<>();

    private static final Logging log = getLogging(ReaderState.class);

    public static final String BIT = "bit";
    private static final String CUSTOM = "custom";
    private static final String END_STRUCT = "end_struct";
    private static final String STRUCT_NO_PREFIX = "struct_no_prefix ";
    private static final String STRUCT = "struct ";
    public final Stack<ConfigStructure> stack = new Stack<>();
    public final Map<String, Integer> tsCustomSize = new HashMap<>();
    public final Map<String, String> tsCustomLine = new HashMap<>();
    public final Map<String, ConfigStructure> structures = new HashMap<>();
    public String headerMessage;
    // well, technically those should be a builder for state, not this state class itself
    public String tsFileOutputName = "rusefi.ini";
    String definitionInputFile = null;
    public boolean withC_Defines = true;
    List<String> prependFiles = new ArrayList<>();
    List<ConfigurationConsumer> destinations = new ArrayList<>();

    public final EnumsReader enumsReader = new EnumsReader();
    public final VariableRegistry variableRegistry = new VariableRegistry();

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

        if (log.debugEnabled())
            log.debug("Need to align before bit " + bitName);
        state.stack.peek().addAlignmentFill(state, 4);

        String trueName = bitNameParts.length > 1 ? bitNameParts[1].replaceAll("\"", "") : null;
        String falseName = bitNameParts.length > 2 ? bitNameParts[2].replaceAll("\"", "") : null;

        ConfigField bitField = new ConfigField(state, bitNameParts[0], comment, null, BOOLEAN_T, new int[0], null, false, false, false, trueName, falseName);
        if (state.stack.isEmpty())
            throw new IllegalStateException("Parent structure expected");
        ConfigStructure structure = state.stack.peek();
        structure.addBitField(bitField);
    }

    public void doJob() throws IOException {
        for (String prependFile : prependFiles)
            variableRegistry.readPrependValues(prependFile);

        /*
         * this is the most important invocation - here we read the primary input file and generated code into all
         * the destinations/writers
         */
        SystemOut.println("Reading definition from " + this.definitionInputFile);
        BufferedReader definitionReader = new BufferedReader(new InputStreamReader(new FileInputStream(this.definitionInputFile), IoUtils.CHARSET.name()));
        readBufferedReader(definitionReader, this.destinations);
    }

    public void read(Reader reader) throws IOException {
        Map<String, EnumsReader.EnumState> newEnums = EnumsReader.readStatic(reader);

        for (Map.Entry<String, EnumsReader.EnumState> enumFamily : newEnums.entrySet()) {

            for (Map.Entry<String, Value> enumValue : enumFamily.getValue().entrySet()) {

                String key = enumFamily.getKey() + "_" + enumValue.getKey();
                String value = enumValue.getValue().getValue();
                variableRegistry.register(key, value);

                try {
                    int numericValue = enumValue.getValue().getIntValue();
                    variableRegistry.registerHex(key, numericValue);
                } catch (NumberFormatException ignore) {
                    // ENUM_32_BITS would be an example of a non-numeric enum, let's just skip for now
                }
            }
        }

        enumsReader.enums.putAll(newEnums);
    }

    private void handleCustomLine(String line) {
        line = line.substring(CUSTOM.length() + 1).trim();
        int index = line.indexOf(' ');
        String name = line.substring(0, index);

        String autoEnumOptions = variableRegistry.getEnumOptionsForTunerStudio(enumsReader, name);
        if (autoEnumOptions != null) {
            variableRegistry.register(name + VariableRegistry.AUTO_ENUM_SUFFIX, autoEnumOptions);
        }

        line = line.substring(index).trim();
        index = line.indexOf(' ');
        String customSize = line.substring(0, index);

        String tunerStudioLine = line.substring(index).trim();
        tunerStudioLine = variableRegistry.applyVariables(tunerStudioLine);
        int size = parseSize(customSize, line);
        tsCustomSize.put(name, size);

        RawIniFile.Line rawLine = new RawIniFile.Line(tunerStudioLine);
        //boolean isKeyValueForm = tunerStudioLine.contains("=\"");
        if (rawLine.getTokens()[0].equals("bits")) {
            EnumIniField.ParseBitRange bitRange = new EnumIniField.ParseBitRange().invoke(rawLine.getTokens()[3]);
            int totalCount = 1 << (bitRange.getBitSize0() + 1);
            List<String> enums = Arrays.asList(rawLine.getTokens()).subList(4, rawLine.getTokens().length);
            // at the moment we read 0=NONE as two tokens, thus enums.size() is divided by two
            if (enums.size() / 2 > totalCount)
                throw new IllegalStateException(name + ": Too many options in " + tunerStudioLine + " capacity=" + totalCount + "/size=" + enums.size());
/*
    this does not work right now since smt32 and kinetis enum sizes could be different but same .txt file
    todo: identify relevant bitsizes and use variables for bitsizes?
            if (enums.size() <= totalCount / 2)
                throw new IllegalStateException("Too many bits allocated for " + enums + " capacity=" + totalCount + "/size=" + enums.size());
*/
        }

        tsCustomLine.put(name, tunerStudioLine);
    }

    public int parseSize(String customSize, String line) {
        customSize = variableRegistry.applyVariables(customSize);
        customSize = customSize.replaceAll("x", "*");
        line = variableRegistry.applyVariables(line);

        int multPosition = customSize.indexOf(VariableRegistry.MULT_TOKEN);
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
        if (log.debugEnabled())
            log.debug("Ending structure " + structure.getName());
        structure.addAlignmentFill(this, 4);

        structures.put(structure.getName(), structure);

        for (ConfigurationConsumer consumer : consumers)
            consumer.handleEndStruct(this, structure);
    }

    public void readBufferedReader(String inputString, ConfigurationConsumer... consumers) throws IOException {
        readBufferedReader(new BufferedReader(new StringReader(inputString)), Arrays.asList(consumers));
    }

    public void readBufferedReader(BufferedReader definitionReader, List<ConfigurationConsumer> consumers) throws IOException {
        for (ConfigurationConsumer consumer : consumers)
            consumer.startFile();

        int lineIndex = 0;
        String line;
        while ((line = definitionReader.readLine()) != null) {
            lineIndex++;
            line = ToolUtil.trimLine(line);
            /**
             * we should ignore empty lines and comments
             */
            if (ToolUtil.isEmptyDefinitionLine(line))
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

            } else if (ToolUtil.startsWithToken(line, CUSTOM)) {
                handleCustomLine(line);

            } else if (ToolUtil.startsWithToken(line, VariableRegistry.DEFINE)) {
                /**
                 * for example
                 * #define CLT_CURVE_SIZE 16
                 */
                variableRegistry.processDefine(line.substring(VariableRegistry.DEFINE.length()).trim());
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
        ConfigStructure structure = new ConfigStructure(name, comment, withPrefix);
        state.stack.push(structure);
        if (log.debugEnabled())
            log.debug("Starting structure " + structure.getName());
    }

    private static void processField(ReaderState state, String line) {

        ConfigField cf = ConfigField.parse(state, line);

        if (cf == null) {
            if (ConfigField.isPreprocessorDirective(line)) {
                cf = new ConfigField(state, "", line, null,
                        ConfigField.DIRECTIVE_T, new int[0], null, false, false, false,
                        null, null);
            } else {
                throw new IllegalStateException("Cannot parse line [" + line + "]");
            }
        }

        if (state.stack.isEmpty())
            throw new IllegalStateException(cf.getName() + ": Not enclosed in a struct");
        ConfigStructure structure = state.stack.peek();

        Integer getPrimitiveSize = TypesHelper.getPrimitiveSize(cf.getType());
        Integer customTypeSize = state.tsCustomSize.get(cf.getType());
        if (getPrimitiveSize != null && getPrimitiveSize > 1) {
            if (log.debugEnabled())
                log.debug("Need to align before " + cf.getName());
            structure.addAlignmentFill(state, getPrimitiveSize);
        } else if (state.structures.containsKey(cf.getType())) {
            // we are here for struct members
            structure.addAlignmentFill(state, 4);
        } else if (customTypeSize != null) {
            structure.addAlignmentFill(state, customTypeSize % 8);
        }

        if (cf.isIterate()) {
            structure.addC(cf);
            for (int i = 1; i <= cf.getArraySizes()[0]; i++) {
                String commentWithIndex = getCommentWithIndex(cf, i);
                ConfigField element = new ConfigField(state, cf.getName() + i, commentWithIndex, null,
                        cf.getType(), new int[0], cf.getTsInfo(), false, false, cf.isHasAutoscale(), null, null);
                element.isFromIterate(true);
                structure.addTs(element);
            }
        } else if (cf.isDirective()) {
            structure.addTs(cf);
        } else {
            structure.addBoth(cf);
        }
    }

    @NotNull
    private static String getCommentWithIndex(ConfigField cf, int i) {
        String unquoted = unquote(cf.getCommentOrName());
        String string = unquoted + " " + i;
        return quote(string);
    }

    public String getHeader() {
        if (headerMessage == null)
            throw new NullPointerException("No header message yet");
        return headerMessage;
    }

    public void setDefinitionInputFile(String definitionInputFile) {
        this.definitionInputFile = definitionInputFile;
        headerMessage = ToolUtil.getGeneratedAutomaticallyTag() + definitionInputFile + " " + new Date();
        inputFiles.add(definitionInputFile);
    }

    public void addCHeaderDestination(String cHeader) {
        destinations.add(new CHeaderConsumer(this, cHeader, withC_Defines));
    }

    public void addJavaDestination(String fileName) {
        destinations.add(new FileJavaFieldsConsumer(this, fileName));
    }

    public void addPrepend(String fileName) {
        if (fileName == null || fileName.isEmpty()) {
            // see UsagesReader use-case with dynamic prepend usage
            return;
        }
        prependFiles.add(fileName);
        inputFiles.add(fileName);
    }

    public void addDestination(ConfigurationConsumer... consumers) {
        destinations.addAll(Arrays.asList(consumers));
    }
}
