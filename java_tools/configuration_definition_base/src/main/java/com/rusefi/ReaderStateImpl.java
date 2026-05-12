package com.rusefi;

import com.devexperts.logging.Logging;
import com.opensr5.ini.RawIniFile;
import com.rusefi.config.FieldType;
import com.rusefi.core.Pair;
import com.rusefi.enum_reader.Value;
import com.rusefi.ini.reader.EnumIniReaderHelper;
import com.rusefi.output.*;
import com.rusefi.parse.TokenUtil;
import com.rusefi.parse.TypesHelper;
import com.rusefi.tools.tune.FileLinesHelper;
import com.rusefi.util.LazyFile;
import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.util.*;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.ConfigFieldImpl.BOOLEAN_T;
import static com.rusefi.VariableRegistry.unquote;

/**
 * We keep state here as we read configuration definition
 * <p>
 * Andrey Belomutskiy, (c) 2013-2020
 * 12/19/18
 */
public class ReaderStateImpl implements ReaderState {
    private static final Logging log = getLogging(ReaderStateImpl.class);

    public static final String BIT = "bit";
    public static final String CUSTOM = "custom";
    private static final String END_STRUCT = "end_struct";
    private static final String STRUCT_NO_PREFIX = "struct_no_prefix ";
    private static final String STRUCT = "struct ";
    public static final String SPLIT_LINES = "split_lines";
    public static final String INCLUDE_FILE = "include_file";
    // used to update other files
    private final List<String> inputFiles = new ArrayList<>();
    private final Stack<ConfigStructureImpl> stack = new Stack<>();
    private final Map<String, Integer> tsCustomSize = new HashMap<>();
    private final Map<String, String> tsCustomLine = new HashMap<>();
    private final Map<String, ConfigStructureImpl> structures = new HashMap<>();
    private final ReaderProvider readerProvider;
    private final LazyFile.LazyFileFactory fileFactory;
    private String headerMessage;
    // well, technically those should be a builder for state, not this state class itself
    private String tsFileOutputName = null;
    private String definitionInputFile = null;
    String destCDefinesFileName = null;
    private boolean withC_Defines = true;
    private final List<String> prependFiles = new ArrayList<>();
    private final List<ConfigurationConsumer> destinations = new ArrayList<>();

    private final EnumsReader enumsReader = new EnumsReader();
    private final VariableRegistry variableRegistry = new VariableRegistry();
    private final Map<String, EnumGenerator.Parser.EnumDefinition> enumDefinitionMap = new HashMap<>();
    private int defaultBitNameCounter;

    public ReaderStateImpl() {
        this(ReaderProvider.REAL, LazyFile.REAL);
    }

    public ReaderStateImpl(ReaderProvider readerProvider, LazyFile.LazyFileFactory fileFactory) {
        this.readerProvider = readerProvider;
        this.fileFactory = fileFactory;
    }

    @Override
    public int getDefaultBitNameCounter() {
        return defaultBitNameCounter;
    }

    @Override
    public void intDefaultBitNameCounter() {
        defaultBitNameCounter++;
    }

    public Map<String, EnumGenerator.Parser.EnumDefinition> getEnumDefinitionMap() {
        return enumDefinitionMap;
    }

    @Override
    public void setWithC_Defines(boolean withC_Defines) {
        this.withC_Defines = withC_Defines;
    }

    @Override
    public EnumsReader getEnumsReader() {
        return enumsReader;
    }

    public List<String> getInputFiles() {
        return inputFiles;
    }

    private static void handleBitLine(ReaderStateImpl state, String line) {
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

        ConfigFieldImpl bitField = new ConfigFieldImpl(state, bitNameParts[0], comment, null, BOOLEAN_T, new int[0], null, false, false, trueName, falseName);
        if (state.isStackEmpty())
            throw new IllegalStateException("Parent structure expected");
        ConfigStructureImpl structure = state.stack.peek();
        structure.addBitField(bitField);
    }


    @Override
    public void doJob() throws IOException {

        for (String prependFile : prependFiles)
            variableRegistry.readPrependValues(prependFile, false);

        /*
         * this is the most important invocation - here we read the primary input file and generated code into all
         * the destinations/writers
         */
        log.info("Reading definition from " + Objects.requireNonNull(definitionInputFile));
        String fileNameWithRoot = RootHolder.ROOT + definitionInputFile;
        try (BufferedReader definitionReader = new BufferedReader(readerProvider.read(fileNameWithRoot))) {
            readBufferedReader(definitionReader, destinations);
        } catch (Throwable e) {
            throw new IllegalStateException("While processing " + fileNameWithRoot, e);
        }

        if (destCDefinesFileName != null) {
            CHeaderConsumer.writeDefinesToFile(getVariableRegistry(), ConfigDefinitionRootOutputFolder.getValue() + destCDefinesFileName, definitionInputFile);
        }
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

    public void handleCustomLine(String customLineWithPrefix) {
        String withoutPrefix = customLineWithPrefix.substring(CUSTOM.length() + 1).trim();
        Pair<String, String> nameAndRest = TokenUtil.grabFirstTokenAndTheRest(withoutPrefix);
        String name = nameAndRest.first;

        String autoEnumOptions = variableRegistry.getEnumOptionsForTunerStudio(enumsReader, name);
        if (autoEnumOptions != null) {
            variableRegistry.register(name + VariableRegistry.AUTO_ENUM_SUFFIX, autoEnumOptions);
        }

        String line = nameAndRest.second;
        Pair<String, String> sizeAndRest = TokenUtil.grabFirstTokenAndTheRest(line);
        String customSize = sizeAndRest.first;

        String tunerStudioLine = sizeAndRest.second;
        tunerStudioLine = variableRegistry.applyVariables(tunerStudioLine);
        int size = parseSize(customSize, line);
        tsCustomSize.put(name, size);

        RawIniFile.Line rawLine = new RawIniFile.Line(tunerStudioLine);
        if (rawLine.getTokens()[0].equals("bits")) {
            String tsTypeString = rawLine.getTokens()[1];
            FieldType typeInTsString = FieldType.parseTs(tsTypeString);
            if (size != typeInTsString.getStorageSize())
                throw new SizeMismatchException("Size mismatch " + customSize + " vs " + tsTypeString + " in " + customLineWithPrefix);
            EnumIniReaderHelper.ParseBitRange bitRange = new EnumIniReaderHelper.ParseBitRange().invoke(rawLine.getTokens()[3]);
            int totalCount = 1 << (bitRange.getBitSize0() + 1);
            List<String> enums = Arrays.asList(rawLine.getTokens()).subList(4, rawLine.getTokens().length);
            boolean isKeyValueSyntax = EnumIniReaderHelper.isKeyValueSyntax(EnumIniReaderHelper.getEnumValuesSection(tunerStudioLine));
            int enumCount = isKeyValueSyntax ? enums.size() / 2 : enums.size();
            if (enumCount > totalCount)
                throw new IllegalStateException(name + ": Too many options in " + tunerStudioLine + " capacity=" + totalCount + "/size=" + enums.size());
            boolean looksLikeListVariableSyntax = enumCount == 1;
            if (!isKeyValueSyntax && !looksLikeListVariableSyntax) {
                StringBuilder sb = new StringBuilder(tunerStudioLine);
                for (int i = enumCount; i < totalCount; i++) {
                    sb.append(", ").append(InvalidConstant.QUOTED_INVALID);
                }
                tunerStudioLine = sb.toString();
            }
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
        if (isStackEmpty())
            throw new IllegalStateException("Unexpected end_struct");
        ConfigStructureImpl structure = stack.pop();
        if (log.debugEnabled())
            log.debug("Ending structure " + structure.getName());
        structure.addAlignmentFill(this, 4);
        // Validate only on top-level structs
        if (isStackEmpty()) {
            structure.validateNoForwardReferences();
        }

        ConfigStructureImpl existing = structures.put(structure.getName(), structure);
        if (existing != null)
            throw new IllegalStateException("Same struct again: " + structure.getName());

        for (ConfigurationConsumer consumer : consumers)
            consumer.handleEndStruct(this, structure);
    }

    public void readBufferedReader(String inputString, ConfigurationConsumer... consumers) {
        try {
            readBufferedReader(new BufferedReader(new StringReader(inputString)), Arrays.asList(consumers));
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    public void readBufferedReader(BufferedReader definitionReader, List<ConfigurationConsumer> consumers) throws IOException {
        for (ConfigurationConsumer consumer : consumers)
            consumer.startFile();

        List<String> lines = new ArrayList<>();
        String lineReaded;
        while ((lineReaded = definitionReader.readLine()) != null) {
            lineReaded = ToolUtil.trimLine(lineReaded);
            if (lineReaded.startsWith(INCLUDE_FILE)) {
                String fileName = lineReaded.substring(INCLUDE_FILE.length()).trim();
                log.info("Including " + fileName);
                lines.addAll(FileLinesHelper.readAllLinesWithRoot(fileName));
            } else if (lineReaded.startsWith(SPLIT_LINES)) {
                String template = lineReaded.substring(SPLIT_LINES.length());
                String lineExpanded = variableRegistry.applyVariables(template);
                String[] sublines = lineExpanded.split("\\r?\\n");
                lines.addAll(Arrays.asList(sublines));
            } else {
                lines.add(lineReaded);
	        }
        }

        int lineIndex = 0;
        for (final String line : lines) {
            lineIndex++;
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

            } else if (VariableRegistry.looksLikeDefineLine(line)) {
                /**
                 * for example
                 * #define CLT_CURVE_SIZE 16
                 */
                variableRegistry.processLine(line);
            } else {
                if (isStackEmpty())
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
        ConfigStructure structure = peek();
        structure.addBitPadding(this);
    }

    public void ensureEmptyAfterProcessing() {
        if (!isStackEmpty())
            throw new IllegalStateException("Unclosed structure: " + peek().getName());
    }

    private static void handleStartStructure(ReaderStateImpl state, String line, boolean withPrefix) {
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
        ConfigStructure parent = state.isStackEmpty() ? null : state.peek();
        ConfigStructureImpl structure = new ConfigStructureImpl(name, comment, withPrefix, parent);
        state.stack.push(structure);
        if (log.debugEnabled())
            log.debug("Starting structure " + structure.getName());
    }

    private static void processField(ReaderStateImpl state, String line) {
        ConfigFieldImpl cf;
        try {
            cf = ConfigFieldImpl.parse(state, line);
        } catch (Throwable e) {
            throw new ParsingException("While parsing " + line, e);
        }

        if (cf == null) {
            if (ConfigFieldImpl.isPreprocessorDirective(line)) {
                cf = new ConfigFieldImpl(state, "", line, null,
                        ConfigFieldImpl.DIRECTIVE_T, new int[0], null, false, false,
                        null, null);
            } else {
                throw new IllegalStateException("Cannot parse line [" + line + "]");
            }
        }

        if (state.isStackEmpty())
            throw new IllegalStateException(cf.getName() + ": Not enclosed in a struct");
        ConfigStructureImpl structure = state.stack.peek();

        Integer getPrimitiveSize = TypesHelper.getPrimitiveSize(cf.getTypeName());
        Integer customTypeSize = state.tsCustomSize.get(cf.getTypeName());
        if (getPrimitiveSize != null && getPrimitiveSize > 1) {
            if (log.debugEnabled())
                log.debug("Need to align before " + cf.getName());
            structure.addAlignmentFill(state, getPrimitiveSize);
        } else if (state.structures.containsKey(cf.getTypeName())) {
            // we are here for struct members
            structure.addAlignmentFill(state, 4);
        } else if (customTypeSize != null) {
            structure.addAlignmentFill(state, customTypeSize % 8);
        }

        if (cf.isIterate()) {
            structure.addC(cf);
            for (int i = 1; i <= cf.getArraySizes()[0]; i++) {
                String commentWithIndex = getCommentWithIndex(cf, i);
                ConfigFieldImpl element = new ConfigFieldImpl(state, cf.getName() + i, commentWithIndex, null,
                        cf.getTypeName(), new int[0], cf.getTsInfo(), false, cf.isHasAutoscale(), null, null);
                element.setFromIterate(cf.getName(), i);
                structure.addTs(element);
            }
        } else if (cf.isDirective()) {
            structure.addTs(cf);
        } else {
            structure.addBoth(cf);
        }
    }

    @NotNull
    private static String getCommentWithIndex(ConfigFieldImpl cf, int i) {
        String unquoted = unquote(cf.getCommentOrName());
        String string = unquoted + " " + i;
        return VariableRegistry.quote(string);
    }

    @Override
    public String getHeader() {
        if (headerMessage == null)
            throw new NullPointerException("No header message yet");
        return headerMessage;
    }

    @Override
    public void setDefinitionInputFile(String definitionInputFile) {
        this.definitionInputFile = definitionInputFile;
        headerMessage = ToolUtil.getGeneratedAutomaticallyTag() + definitionInputFile;
        inputFiles.add(definitionInputFile);
    }

    @Override
    public void addCHeaderDestination(String cHeaderFileName) {
        destinations.add(new CHeaderConsumer(this, ConfigDefinitionRootOutputFolder.getValue() + cHeaderFileName, withC_Defines, fileFactory));
    }

    @Override
    public void addPrepend(String fileName) {
        if (fileName == null || fileName.isEmpty()) {
            // see LiveDataProcessor use-case with dynamic prepend usage
            return;
        }
        variableRegistry.readPrependValues(fileName, false);
        inputFiles.add(fileName);
    }

    @Override
    public void addSoftPrepend(String fileName){
        if (fileName == null || fileName.isEmpty()) {
            return;
        }
        File file = new File(IoUtil3.prependIfNotAbsolute(RootHolder.ROOT, fileName));
        if (!file.exists()){
            return;
        }
        variableRegistry.readPrependValues(fileName, false);
        inputFiles.add(fileName);
    }

    @Override
    public void addPostponedPrependNotInput(String fileName) {
        prependFiles.add(fileName);
    }

    @Override
    public void addDestination(ConfigurationConsumer... consumers) {
        destinations.addAll(Arrays.asList(consumers));
    }

    public void addInputFile(String fileName) {
        inputFiles.add(fileName);
    }

    @Override
    public VariableRegistry getVariableRegistry() {
        return variableRegistry;
    }

    @Override
    public Map<String, Integer> getTsCustomSize() {
        return tsCustomSize;
    }

    @Override
    public Map<String, ? extends ConfigStructure> getStructures() {
        return structures;
    }

    @Override
    public Map<String, String> getTsCustomLine() {
        return tsCustomLine;
    }

    @Override
    public void setHeaderMessage(String headerMessage) {
        this.headerMessage = headerMessage;
    }

    @Override
    public String getTsFileOutputName() {
        return tsFileOutputName;
    }

    @Override
    public void setTsFileOutputName(String tsFileOutputName) {
        this.tsFileOutputName = tsFileOutputName;
    }

    @Override
    public boolean isStackEmpty() {
        return stack.isEmpty();
    }

    @Override
    public ConfigStructure peek() {
        return stack.peek();
    }
}
