package com.rusefi;

import com.rusefi.enum_reader.Value;
import com.rusefi.generated.RusefiConfigGrammarLexer;
import com.rusefi.generated.RusefiConfigGrammarParser;
import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.parsing.Definition;
import com.rusefi.output.*;
import com.rusefi.util.CachingStrategy;
import com.rusefi.util.IoUtils;
import com.rusefi.util.LazyFile;
import com.rusefi.util.SystemOut;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeListener;
import org.antlr.v4.runtime.tree.ParseTreeWalker;
import org.yaml.snakeyaml.Yaml;

import java.io.*;
import java.nio.file.Files;
import java.util.*;
import java.util.zip.CRC32;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 1/12/15
 *
 * @see ConfigurationConsumer
 */
@SuppressWarnings("StringConcatenationInsideStringBufferAppend")
public class ConfigDefinition {
    private static final String SIGNATURE_HASH = "SIGNATURE_HASH";
    public static String MESSAGE;

    private static final String ROM_RAIDER_XML_TEMPLATE = "rusefi_template.xml";
    public static final String KEY_DEFINITION = "-definition";
    private static final String KEY_ROMRAIDER_INPUT = "-romraider";
    public static final String KEY_TS_DESTINATION = "-ts_destination";
    private static final String KEY_C_DESTINATION = "-c_destination";
    private static final String KEY_C_FSIO_CONSTANTS = "-c_fsio_constants";
    private static final String KEY_C_FSIO_GETTERS = "-c_fsio_getters";
    private static final String KEY_C_FSIO_NAMES = "-c_fsio_names";
    private static final String KEY_C_FSIO_STRING = "-c_fsio_strings";
    private static final String KEY_C_DEFINES = "-c_defines";
    /**
     * @see CHeaderConsumer#withC_Defines
     */
    private static final String KEY_WITH_C_DEFINES = "-with_c_defines";
    private static final String KEY_JAVA_DESTINATION = "-java_destination";
    private static final String KEY_ROMRAIDER_DESTINATION = "-romraider_destination";
    private static final String KEY_FIRING = "-firing_order";
    public static final String KEY_PREPEND = "-prepend";
    public static final String KEY_SIGNATURE = "-signature";
    public static final String KEY_SIGNATURE_DESTINATION = "-signature_destination";
    public static final String KEY_CACHE = "-cache";
    public static final String KEY_CACHE_ZIP_FILE = "-cache_zip_file";
    private static final String KEY_ZERO_INIT = "-initialize_to_zero";
    private static final String KEY_BOARD_NAME = "-board";
    /**
     * This flag controls if we assign default zero value (useful while generating structures used for class inheritance)
     * versus not assigning default zero value like we need for non-class headers
     * This could be related to configuration header use-case versus "live data" (not very alive idea) use-case
     */
    public static boolean needZeroInit = true;
    public static String definitionInputFile = null;

    public static void main(String[] args) {
        try {
            doJob(args);
        } catch (Throwable e) {
            SystemOut.println(e);
            e.printStackTrace();
            SystemOut.close();
            System.exit(-1);
        }
        SystemOut.close();
    }

    private static void doJob(String[] args) throws IOException {
        if (args.length < 2) {
            SystemOut.println("Please specify\r\n"
                    + KEY_DEFINITION + " x\r\n"
                    + KEY_TS_DESTINATION + " x\r\n"
                    + KEY_C_DESTINATION + " x\r\n"
                    + KEY_JAVA_DESTINATION + " x\r\n"
            );
            return;
        }

        SystemOut.println(ConfigDefinition.class + " Invoked with " + Arrays.toString(args));

        String tsPath = null;
        String destCHeaderFileName = null;
        String destCDefinesFileName = null;
        String destCFsioConstantsFileName = null;
        String destCFsioGettersFileName = null;
        String namesCFileName = null;
        String stringsCFileName = null;
        String javaDestinationFileName = null;
        String romRaiderDestination = null;
        // we postpone reading so that in case of cache hit we do less work
        List<String> prependFiles = new ArrayList<>();
        String romRaiderInputFile = null;
        String firingEnumFileName = null;
        String cachePath = null;
        String cacheZipFile = null;
        String signatureDestination = null;
        String signaturePrependFile = null;
        List<String> enumInputFiles = new ArrayList<>();
        CHeaderConsumer.withC_Defines = true;
        File[] yamlFiles = null;

        // used to update other files
        List<String> inputFiles = new ArrayList<>();
        // disable the lazy checks because we use timestamps to detect changes
        LazyFile.setLazyFileEnabled(false);

        ReaderState state = new ReaderState();

        for (int i = 0; i < args.length - 1; i += 2) {
            String key = args[i];
            switch (key) {
                case "-tool":
                    ToolUtil.TOOL = args[i + 1];
                    break;
                case KEY_DEFINITION:
                    definitionInputFile = args[i + 1];
                    inputFiles.add(definitionInputFile);
                    break;
                case KEY_TS_DESTINATION:
                    tsPath = args[i + 1];
                    break;
                case KEY_C_DESTINATION:
                    destCHeaderFileName = args[i + 1];
                    break;
                case KEY_C_FSIO_GETTERS:
                    destCFsioGettersFileName = args[i + 1];
                    break;
                case KEY_C_FSIO_STRING:
                    stringsCFileName = args[i + 1];
                    break;
                case KEY_C_FSIO_NAMES:
                    namesCFileName = args[i + 1];
                    break;
                case KEY_C_FSIO_CONSTANTS:
                    destCFsioConstantsFileName = args[i + 1];
                    break;
                case KEY_ZERO_INIT:
                    needZeroInit = Boolean.parseBoolean(args[i + 1]);
                    break;
                case KEY_WITH_C_DEFINES:
                    CHeaderConsumer.withC_Defines = Boolean.parseBoolean(args[i + 1]);
                    break;
                case KEY_C_DEFINES:
                    destCDefinesFileName = args[i + 1];
                    break;
                case KEY_JAVA_DESTINATION:
                    javaDestinationFileName = args[i + 1];
                    break;
                case KEY_FIRING:
                    firingEnumFileName = args[i + 1];
                    inputFiles.add(firingEnumFileName);
                    break;
                case KEY_ROMRAIDER_DESTINATION:
                    romRaiderDestination = args[i + 1];
                    break;
                case KEY_PREPEND:
                    prependFiles.add(args[i + 1]);
                    inputFiles.add(args[i + 1]);
                    break;
                case KEY_SIGNATURE:
                    signaturePrependFile = args[i + 1];
                    prependFiles.add(args[i + 1]);
                    // don't add this file to the 'inputFiles'
                    break;
                case KEY_SIGNATURE_DESTINATION:
                    signatureDestination = args[i + 1];
                    break;
                case EnumToString.KEY_ENUM_INPUT_FILE:
                    enumInputFiles.add(args[i + 1]);
                    break;
                case KEY_CACHE:
                    cachePath = args[i + 1];
                    break;
                case KEY_CACHE_ZIP_FILE:
                    cacheZipFile = args[i + 1];
                    break;
                case "-ts_output_name":
                    TSProjectConsumer.TS_FILE_OUTPUT_NAME = args[i + 1];
                    break;
                case KEY_ROMRAIDER_INPUT:
                    String inputFilePath = args[i + 1];
                    romRaiderInputFile = inputFilePath + File.separator + ROM_RAIDER_XML_TEMPLATE;
                    inputFiles.add(romRaiderInputFile);
                    break;
                case KEY_BOARD_NAME:
                    String boardName = args[i + 1];
                    String dirPath = "./config/boards/" + boardName + "/connectors";
                    File dirName = new File(dirPath);
                    FilenameFilter filter = (f, name) -> name.endsWith(".yaml");
                    yamlFiles = dirName.listFiles(filter);
                    if (yamlFiles != null) {
                        for (File yamlFile : yamlFiles) {
                            inputFiles.add("config/boards/" + boardName + "/connectors/" + yamlFile.getName());
                        }
                    }
                    break;
            }
        }

        List<String> inputAllFiles = new ArrayList<>(inputFiles);
        if (tsPath != null) {
            // used to update .ini files
            inputAllFiles.add(TSProjectConsumer.getTsFileInputName(tsPath));
        }

        boolean needToUpdateTsFiles = isNeedToUpdateTsFiles(tsPath, cachePath, cacheZipFile, inputAllFiles);

        boolean needToUpdateOtherFiles = CachingStrategy.checkIfOutputFilesAreOutdated(inputFiles, cachePath, cacheZipFile);
        if (!needToUpdateTsFiles && !needToUpdateOtherFiles) {
            SystemOut.println("All output files are up-to-date, nothing to do here!");
            return;
        }

        if (!enumInputFiles.isEmpty()) {
            for (String ef : enumInputFiles) {
                state.read(new FileReader(ef));
            }

            SystemOut.println(state.enumsReader.getEnums() + " total enumsReader");
        }

        long crc32 = signatureHash(state, tsPath, inputAllFiles);

        handleFiringOrder(firingEnumFileName, state.variableRegistry);

        MESSAGE = ToolUtil.getGeneratedAutomaticallyTag() + definitionInputFile + " " + new Date();

        SystemOut.println("Reading definition from " + definitionInputFile);

        for (String prependFile : prependFiles)
            state.variableRegistry.readPrependValues(prependFile);

        if (yamlFiles != null) {
            processYamls(state.variableRegistry, yamlFiles, state);
        }

        // Parse the input files
        {
            ParseState parseState = new ParseState(state.enumsReader);

            // First process yaml files
            //processYamls(listener, yamlFiles);

            // Process firing order enum
            handleFiringOrder(firingEnumFileName, parseState);

            // Load prepend files
            {
                // Ignore duplicates of definitions made during prepend phase
                parseState.setDefinitionPolicy(Definition.OverwritePolicy.IgnoreNew);

                //for (String prependFile : prependFiles) {
                // TODO: fix signature define file parsing
                //parseFile(listener, prependFile);
                //}
            }

            // Now load the main config file
            {
                // don't allow duplicates in the main file
                parseState.setDefinitionPolicy(Definition.OverwritePolicy.NotAllowed);
                parseFile(parseState.getListener(), definitionInputFile);
            }

            // Write C structs
            // CStructWriter cStructs = new CStructWriter();
            // cStructs.writeCStructs(listener, destCHeaderFileName + ".test");

            // Write tunerstudio layout
            // TsWriter writer = new TsWriter();
            // writer.writeTunerstudio(listener, "TODO", tsPath + "/test.ini");

        }

        BufferedReader definitionReader = new BufferedReader(new InputStreamReader(new FileInputStream(definitionInputFile), IoUtils.CHARSET.name()));

        List<ConfigurationConsumer> destinations = new ArrayList<>();
        if (tsPath != null && needToUpdateTsFiles) {
            CharArrayWriter tsWriter = new CharArrayWriter();
            destinations.add(new TSProjectConsumer(tsWriter, tsPath, state));

            VariableRegistry tmpRegistry = new VariableRegistry();
            // store the CRC32 as a built-in variable
            tmpRegistry.register(SIGNATURE_HASH, "" + crc32);
            tmpRegistry.readPrependValues(signaturePrependFile);
            destinations.add(new SignatureConsumer(signatureDestination, tmpRegistry));
        }
        if (needToUpdateOtherFiles) {
            if (destCHeaderFileName != null) {
                destinations.add(new CHeaderConsumer(state.variableRegistry, destCHeaderFileName));
            }
            if (javaDestinationFileName != null) {
                destinations.add(new FileJavaFieldsConsumer(state, javaDestinationFileName));
            }

            if (destCFsioConstantsFileName != null || destCFsioGettersFileName != null) {
                destinations.add(new FileFsioSettingsConsumer(state,
                        destCFsioConstantsFileName,
                        destCFsioGettersFileName,
                        namesCFileName,
                        stringsCFileName));
            }
        }

        if (destinations.isEmpty())
            throw new IllegalArgumentException("No destinations specified");
        /*
         * this is the most important invocation - here we read the primary input file and generated code into all
         * the destinations/writers
         */
        state.readBufferedReader(definitionReader, destinations);


        if (destCDefinesFileName != null && needToUpdateOtherFiles)
            writeDefinesToFile(state.variableRegistry, destCDefinesFileName);

        if (romRaiderDestination != null && romRaiderInputFile != null && needToUpdateOtherFiles) {
            processTextTemplate(state, romRaiderInputFile, romRaiderDestination);
        }

        CachingStrategy.saveCachedInputFiles(inputAllFiles, cachePath, cacheZipFile);
    }

    private static void handleFiringOrder(String firingEnumFileName, VariableRegistry variableRegistry) throws IOException {
        if (firingEnumFileName != null) {
            SystemOut.println("Reading firing from " + firingEnumFileName);
            variableRegistry.register("FIRINGORDER", FiringOrderTSLogic.invoke(firingEnumFileName));
        }
    }

    private static void handleFiringOrder(String firingEnumFileName, ParseState parser) throws IOException {
        if (firingEnumFileName != null) {
            SystemOut.println("Reading firing from " + firingEnumFileName);
            parser.addDefinition("FIRINGORDER", FiringOrderTSLogic.invoke(firingEnumFileName), Definition.OverwritePolicy.NotAllowed);
        }
    }

    private static long signatureHash(ReaderState state, String tsPath, List<String> inputAllFiles) throws IOException {
        // get CRC32 of given input files
        long crc32 = 0;
        for (String iFile : inputAllFiles) {
            long c = getCrc32(iFile) & 0xffffffffL;
            SystemOut.println("CRC32 from " + iFile + " = " + c);
            crc32 ^= c;
        }
        SystemOut.println("CRC32 from all input files = " + crc32);
        // store the CRC32 as a built-in variable
        if (tsPath != null) // nasty trick - do not insert signature into live data files
            state.variableRegistry.register(SIGNATURE_HASH, "" + crc32);
        return crc32;
    }

    private static boolean isNeedToUpdateTsFiles(String tsPath, String cachePath, String cacheZipFile, List<String> inputAllFiles) {
        boolean needToUpdateTsFiles = false;
        if (tsPath != null) {
            SystemOut.println("Check the input/output TS files:");
            needToUpdateTsFiles = CachingStrategy.checkIfOutputFilesAreOutdated(inputAllFiles, cachePath, cacheZipFile);
        }
        return needToUpdateTsFiles;
    }

    public static void processYamls(VariableRegistry registry, File[] yamlFiles, ReaderState state) throws IOException {
        ArrayList<Map<String, Object>> listPins = new ArrayList<>();
        for (File yamlFile : yamlFiles) {
            processYamlFile(yamlFile, listPins);
        }
        registerPins(listPins, registry, state);
    }

    @SuppressWarnings("unchecked")
    private static void processYamlFile(File yamlFile,
                                        ArrayList<Map<String, Object>> listPins) throws IOException {
        Yaml yaml = new Yaml();
        Map<String, Object> yamlData = yaml.load(new FileReader(yamlFile));
        List<Map<String, Object>> data = (List<Map<String, Object>>) yamlData.get("pins");
        if (data == null) {
            SystemOut.println("Null yaml for " + yamlFile);
            return;
        }
        SystemOut.println(data);
        Objects.requireNonNull(data, "data");
        for (Map<String, Object> pin : data) {
            ArrayList<Map<String, Object>> thisPinList = new ArrayList<>();
            Object pinId = pin.get("id");
            Object pinClass = pin.get("class");
            Object pinName = pin.get("ts_name");
            if (pinId == null || pinClass == null || pinName == null) {
                continue;
            }
            if (pinId instanceof ArrayList) {
                ArrayList<String> pinIds = (ArrayList<String>) pinId;
                if (!(pinClass instanceof ArrayList))
                    throw new IllegalStateException("Expected multiple classes for " + pinIds);
                for (int i = 0; i < pinIds.size(); i++) {
                    String id = pinIds.get(i);
                    addPinToList(listPins, thisPinList, id, pinName, ((ArrayList<String>) pinClass).get(i));
                }
            } else if (pinId instanceof String) {
                if (((String) pinId).length() == 0) {
                    throw new IllegalStateException("Unexpected empty ID field");
                }
                addPinToList(listPins, thisPinList, pinId, pinName, pinClass);
            } else {
                throw new IllegalStateException("Unexpected type of ID field: " + pinId.getClass().getSimpleName());
            }
            listPins.addAll(thisPinList);
        }
    }

    private static void addPinToList(ArrayList<Map<String, Object>> listPins, ArrayList<Map<String, Object>> thisPinList, Object id, Object pinName, Object pinClass) {
/*
 This doesn't work as expected because it's possible that a board has multiple connector pins connected to the same MCU pin.
 https://github.com/rusefi/rusefi/issues/2897
 https://github.com/rusefi/rusefi/issues/2925
        for (int i = 0; i < listPins.size(); i++) {
            if (id.equals(listPins.get(i).get("id"))) {
                throw new IllegalStateException("ID used multiple times: " + id);
            }
        }
*/
        Map<String, Object> thisPin = new HashMap<>();
        thisPin.put("id", id);
        thisPin.put("ts_name", pinName);
        thisPin.put("class", pinClass);
        thisPinList.add(thisPin);
    }

    private static void registerPins(ArrayList<Map<String, Object>> listPins, VariableRegistry registry, ReaderState state) {
        if (listPins == null || listPins.isEmpty()) {
            return;
        }
        Map<String, ArrayList<String>> names = new HashMap<>();
        names.put("outputs", new ArrayList<>());
        names.put("analog_inputs", new ArrayList<>());
        names.put("event_inputs", new ArrayList<>());
        names.put("switch_inputs", new ArrayList<>());
        for (int i = 0; i < listPins.size(); i++) {
            String id = (String) listPins.get(i).get("id");
            String className = (String) listPins.get(i).get("class");
            ArrayList<String> classList = names.get(className);
            if (classList == null) {
                throw new IllegalStateException("Class not found:  " + className);
            }
            PinType listPinType = PinType.find((String) listPins.get(i).get("class"));
            String pinType = listPinType.getPinType();
            EnumsReader.EnumState enumList = state.enumsReader.getEnums().get(pinType);
            for (Map.Entry<String, Value> kv : enumList.entrySet()) {
                if (kv.getKey().equals(id)) {
                    int index = kv.getValue().getIntValue();
                    classList.ensureCapacity(index + 1);
                    for (int ii = classList.size(); ii <= index; ii++) {
                        classList.add(null);
                    }
                    classList.set(index, (String) listPins.get(i).get("ts_name"));
                    break;
                }
            }
        }
        for (Map.Entry<String, ArrayList<String>> kv : names.entrySet()) {
            PinType namePinType = PinType.find(kv.getKey());
            String outputEnumName = namePinType.getOutputEnumName();
            String pinType = namePinType.getPinType();
            String nothingName = namePinType.getNothingName();
            EnumsReader.EnumState enumList = state.enumsReader.getEnums().get(pinType);
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < kv.getValue().size(); i++) {
                if (sb.length() > 0)
                    sb.append(",");
                String key = "";
                for (Map.Entry<String, Value> entry : enumList.entrySet()) {
                    if (entry.getValue().getIntValue() == i) {
                        key = entry.getKey();
                        break;
                    }
                }
                if (key.equals(nothingName)) {
                    sb.append("\"NONE\"");
                } else if (kv.getValue().get(i) == null) {
                    sb.append("\"INVALID\"");
                } else {
                    sb.append("\"" + kv.getValue().get(i) + "\"");
                }
            }
            if (sb.length() > 0) {
                registry.register(outputEnumName, sb.toString());
            }
        }
    }

    private static void processTextTemplate(ReaderState state, String inputFileName, String outputFileName) throws IOException {
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

    public static String getComment(String comment, int currentOffset, String units) {
        String start = "\t/**";
        String packedComment = packComment(comment, "\t");
        String unitsComment = units.isEmpty() ? "" : "\t" + units + ToolUtil.EOL;
        return start + ToolUtil.EOL +
                packedComment +
                unitsComment +
                "\t * offset " + currentOffset + ToolUtil.EOL + "\t */" + ToolUtil.EOL;
    }

    public static String packComment(String comment, String linePrefix) {
        if (comment == null)
            return "";
        if (comment.trim().isEmpty())
            return "";
        String result = "";
        for (String line : comment.split("\\\\n")) {
            result += linePrefix + " * " + line + ToolUtil.EOL;
        }
        return result;
    }

    public static int getSize(VariableRegistry variableRegistry, String s) {
        if (variableRegistry.intValues.containsKey(s)) {
            return variableRegistry.intValues.get(s);
        }
        return Integer.parseInt(s);
    }

    private static long getCrc32(String fileName) throws IOException {
        File file = new File(fileName);
        byte[] fileContent = Files.readAllBytes(file.toPath());
        for (int i = 0; i < fileContent.length; i++) {
            byte aByte = fileContent[i];
            if (aByte == '\r')
                throw new IllegalStateException("CR \\r 0x0D byte not allowed in cacheable content " + fileName + " at index=" + i);
        }
        CRC32 c = new CRC32();
        c.update(fileContent, 0, fileContent.length);
        return c.getValue();
    }

    public static void writeDefinesToFile(VariableRegistry variableRegistry, String fileName) throws IOException {

        SystemOut.println("Writing to " + fileName);
        LazyFile cHeader = new LazyFile(fileName);

        cHeader.write("//\n// " + ToolUtil.getGeneratedAutomaticallyTag() + definitionInputFile + "\n//\n\n");
        cHeader.write(variableRegistry.getDefinesSection());
        cHeader.close();
    }

    public static class RusefiParseErrorStrategy extends DefaultErrorStrategy {
        private boolean hadError = false;

        public boolean hadError() {
            return this.hadError;
        }

        @Override
        public void recover(Parser recognizer, RecognitionException e) {
            this.hadError = true;

            super.recover(recognizer, e);
        }

        @Override
        public Token recoverInline(Parser recognizer) throws RecognitionException {
            this.hadError = true;

            return super.recoverInline(recognizer);
        }
    }

    private static void parseFile(ParseTreeListener listener, String filePath) throws IOException {
        SystemOut.println("Parsing file (Antlr) " + filePath);

        CharStream in = new ANTLRInputStream(new FileInputStream(filePath));

        long start = System.nanoTime();

        RusefiConfigGrammarParser parser = new RusefiConfigGrammarParser(new CommonTokenStream(new RusefiConfigGrammarLexer(in)));

        RusefiParseErrorStrategy errorStrategy = new RusefiParseErrorStrategy();
        parser.setErrorHandler(errorStrategy);

        ParseTree tree = parser.content();
        new ParseTreeWalker().walk(listener, tree);
        double durationMs = (System.nanoTime() - start) / 1e6;

        if (errorStrategy.hadError()) {
            throw new RuntimeException("Parse failed, see error output above!");
        }

        SystemOut.println("Successfully parsed " + filePath + " in " + durationMs + "ms");
    }
}
