package com.rusefi;

import com.rusefi.output.*;
import com.rusefi.util.*;
import com.rusefi.enum_reader.Value;
import org.yaml.snakeyaml.Yaml;

import java.io.*;
import java.math.BigInteger;
import java.nio.file.*;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.*;
import java.util.zip.CRC32;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 1/12/15
 * @see ConfigurationConsumer
 */
@SuppressWarnings("StringConcatenationInsideStringBufferAppend")
public class ConfigDefinition {
    public static final String EOL = "\n";
    private static final String SIGNATURE_HASH = "SIGNATURE_HASH";
    public static String MESSAGE;

    public static String TOOL = "(unknown script)";
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

    public static String getGeneratedAutomaticallyTag() {
        return LazyFile.LAZY_FILE_TAG + "ConfigDefinition.jar based on " + TOOL + " ";
    }

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

        SystemOut.println("Invoked with " + Arrays.toString(args));

        String tsPath = null;
        String destCHeaderFileName = null;
        String destCDefinesFileName = null;
        String destCFsioConstantsFileName = null;
        String destCFsioGettersFileName = null;
        String namesCFileName = null;
        String stringsCFileName = null;
        String javaDestinationFileName = null;
        String romRaiderDestination = null;
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
                    ConfigDefinition.TOOL = args[i + 1];
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
                        for (int f = 0; f < yamlFiles.length; f++) {
                            inputFiles.add("config/boards/" + boardName + "/connectors/" + yamlFiles[f].getName());
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
                state.enumsReader.process(".", ef);
            }

            SystemOut.println(state.enumsReader.getEnums() + " total enumsReader");
        }

        long crc32 = signatureHash(tsPath, inputAllFiles);

        handleFiringOrder(firingEnumFileName);

        MESSAGE = getGeneratedAutomaticallyTag() + definitionInputFile + " " + new Date();

        SystemOut.println("Reading definition from " + definitionInputFile);

        for (String prependFile : prependFiles)
            readPrependValues(VariableRegistry.INSTANCE, prependFile);

        if (yamlFiles != null) {
           processYamls(VariableRegistry.INSTANCE, yamlFiles, state);
        }

        BufferedReader definitionReader = new BufferedReader(new InputStreamReader(new FileInputStream(definitionInputFile), IoUtils.CHARSET.name()));

        List<ConfigurationConsumer> destinations = new ArrayList<>();
        if (tsPath != null && needToUpdateTsFiles) {
            CharArrayWriter tsWriter = new CharArrayWriter();
            destinations.add(new TSProjectConsumer(tsWriter, tsPath, state));

            VariableRegistry tmpRegistry = new VariableRegistry();
            // store the CRC32 as a built-in variable
            tmpRegistry.register(SIGNATURE_HASH, "" + crc32);
            readPrependValues(tmpRegistry, signaturePrependFile);
            destinations.add(new SignatureConsumer(signatureDestination, tmpRegistry));
        }
        if (needToUpdateOtherFiles) {
            if (destCHeaderFileName != null) {
                destinations.add(new CHeaderConsumer(destCHeaderFileName));
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
            VariableRegistry.INSTANCE.writeDefinesToFile(destCDefinesFileName);

        if (romRaiderDestination != null && romRaiderInputFile != null && needToUpdateOtherFiles) {
            processTextTemplate(romRaiderInputFile, romRaiderDestination);
        }

        CachingStrategy.saveCachedInputFiles(inputAllFiles, cachePath, cacheZipFile);
    }

    private static void handleFiringOrder(String firingEnumFileName) throws IOException {
        if (firingEnumFileName != null) {
            SystemOut.println("Reading firing from " + firingEnumFileName);
            VariableRegistry.INSTANCE.register("FIRINGORDER", FiringOrderTSLogic.invoke(firingEnumFileName));
        }
    }

    private static long signatureHash(String tsPath, List<String> inputAllFiles) throws IOException {
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
            VariableRegistry.INSTANCE.register(SIGNATURE_HASH, "" + crc32);
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

    public static void readPrependValues(VariableRegistry registry, String prependFile) throws IOException {
        BufferedReader definitionReader = new BufferedReader(new FileReader(prependFile));
        String line;
        while ((line = definitionReader.readLine()) != null) {
            line = trimLine(line);
            /**
             * we should ignore empty lines and comments
             */
            if (ReaderState.isEmptyDefinitionLine(line))
                continue;
            if (startsWithToken(line, ReaderState.DEFINE)) {
                processDefine(registry, line.substring(ReaderState.DEFINE.length()).trim());
            }
        }
    }

    public static void processYamls(VariableRegistry registry, File[] yamlFiles, ReaderState state) throws IOException {
        Map<Integer, String> listOutputs = new HashMap<>();
        Map<Integer, String> listAnalogInputs = new HashMap<>();
        Map<Integer, String> listEventInputs = new HashMap<>();
        Map<Integer, String> listSwitchInputs = new HashMap<>();
        for (File yamlFile : yamlFiles) {
            processYamlFile(yamlFile, state, listOutputs, listAnalogInputs, listEventInputs, listSwitchInputs);
        }
        registerPins(listOutputs, "output_pin_e_enum", registry);
        registerPins(listAnalogInputs, "adc_channel_e_enum", registry);
        registerPins(listEventInputs, "brain_input_pin_e_enum", registry);
        registerPins(listSwitchInputs, "switch_input_pin_e_enum", registry);
    }

    @SuppressWarnings("unchecked")
    private static void processYamlFile(File yamlFile, ReaderState state,
                                        Map<Integer, String> listOutputs,
                                        Map<Integer, String> listAnalogInputs,
                                        Map<Integer, String> listEventInputs,
                                        Map<Integer, String> listSwitchInputs) throws IOException {
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
            Object idObject = pin.get("id");
            if (idObject instanceof ArrayList) {
                ArrayList IDs = (ArrayList) idObject;
                Object classes = pin.get("class");
                if (!(classes instanceof ArrayList))
                    throw new IllegalStateException("Expected multiple classes for " + IDs);
                for (int i = 0; i < IDs.size(); i++) {
                    String id = (String) IDs.get(i);
                    findMatchingEnum(id,
                            (String) pin.get("ts_name"),
                            (String) ((ArrayList) classes).get(i),
                            state, listOutputs, listAnalogInputs, listEventInputs, listSwitchInputs);
                }
            } else if (idObject instanceof String ) {
                findMatchingEnum((String) idObject, (String) pin.get("ts_name"), (String) pin.get("class"), state, listOutputs, listAnalogInputs, listEventInputs, listSwitchInputs);
            }
        }
    }

    private static void registerPins(Map<Integer, String> listPins, String outputEnumName, VariableRegistry registry) {
        if (listPins == null || listPins.isEmpty()) {
            return;
        }
        StringBuilder sb = new StringBuilder();
        int maxValue = listPins.keySet().stream().max(Integer::compare).get();
        for (int i = 0; i <= maxValue; i++) {
            if (sb.length() > 0)
                sb.append(",");

            if (listPins.get(i) == null) {
                sb.append("\"INVALID\"");
            } else {
                sb.append("\"" + listPins.get(i) + "\"");
            }
        }
        registry.register(outputEnumName, sb.toString());
    }

    private static void findMatchingEnum(String id, String ts_name, String className, ReaderState state,
                                      Map<Integer, String> listOutputs,
                                      Map<Integer, String> listAnalogInputs,
                                      Map<Integer, String> listEventInputs,
                                      Map<Integer, String> listSwitchInputs) {
        Objects.requireNonNull(id, "id");
        Objects.requireNonNull(className, "classname for " + id);

        switch (className) {
            case "outputs":
                assignPinName("brain_pin_e", ts_name, id, listOutputs, state, "GPIO_UNASSIGNED");
                break;
            case "analog_inputs":
                assignPinName("adc_channel_e", ts_name, id, listAnalogInputs, state, "EFI_ADC_NONE");
                break;
            case "event_inputs":
                assignPinName("brain_pin_e", ts_name, id, listEventInputs, state, "GPIO_UNASSIGNED");
                break;
            case "switch_inputs":
                assignPinName("brain_pin_e", ts_name, id, listSwitchInputs, state, "GPIO_UNASSIGNED");
                break;
        }
    }

    private static void assignPinName(String enumName, String ts_name, String id, Map<Integer, String> list, ReaderState state, String nothingName) {
        Map<String, Value> enumList = state.enumsReader.getEnums().get(enumName);
        for(Map.Entry<String, Value> kv : enumList.entrySet()){
            if(kv.getKey().equals(id)){
                list.put(kv.getValue().getIntValue(), ts_name);
            } else if (kv.getKey().equals(nothingName)) {
                list.put(kv.getValue().getIntValue(), "NONE");
            }
        }
    }

    private static void processTextTemplate(String inputFileName, String outputFileName) throws IOException {
        SystemOut.println("Reading from " + inputFileName);
        SystemOut.println("Writing to " + outputFileName);

        VariableRegistry.INSTANCE.put("generator_message", ConfigDefinition.getGeneratedAutomaticallyTag() + new Date());

        File inputFile = new File(inputFileName);

        BufferedReader fr = new BufferedReader(new FileReader(inputFile));
        LazyFile fw = new LazyFile(outputFileName);

        String line;
        while ((line = fr.readLine()) != null) {
            line = VariableRegistry.INSTANCE.applyVariables(line);
            fw.write(line + ConfigDefinition.EOL);
        }
        fw.close();
    }

    static String trimLine(String line) {
        line = line.trim();
        line = line.replaceAll("\\s+", " ");
        return line;
    }

    static boolean startsWithToken(String line, String token) {
        return line.startsWith(token + " ") || line.startsWith(token + "\t");
    }


    public static String getComment(String comment, int currentOffset, String units) {
        String start = "\t/**";
        String packedComment = packComment(comment, "\t");
        String unitsComment = units.isEmpty() ? "" : "\t" + units + EOL;
        return start + EOL +
                packedComment +
                unitsComment +
                "\t * offset " + currentOffset + EOL + "\t */" + EOL;
    }

    public static String packComment(String comment, String linePrefix) {
        if (comment == null)
            return "";
        if (comment.trim().isEmpty())
            return "";
        String result = "";
        for (String line : comment.split("\\\\n")) {
            result += linePrefix + " * " + line + EOL;
        }
        return result;
    }

    public static int getSize(String s) {
        if (VariableRegistry.INSTANCE.intValues.containsKey(s)) {
            return VariableRegistry.INSTANCE.intValues.get(s);
        }
        return Integer.parseInt(s);
    }

    static void processDefine(VariableRegistry registry, String line) {
        int index = line.indexOf(' ');
        String name;
        if (index == -1) {
            name = line;
            line = "";
        } else {
            name = line.substring(0, index);
            line = line.substring(index).trim();
        }
        if (VariableRegistry.isNumeric(line)) {
            Integer v = Integer.valueOf(line);
            registry.register(name, v);
        } else {
            registry.register(name, line);
        }
    }

    private static String getMd5(byte[] content) {
        try {
            // Static getInstance method is called with hashing MD5
            MessageDigest md = MessageDigest.getInstance("MD5");

            // digest() method is called to calculate message digest
            //  of an input digest() return array of byte
            byte[] messageDigest = md.digest(content);

            // Convert byte array into signum representation
            BigInteger no = new BigInteger(1, messageDigest);

            // Convert message digest into hex value
            String hashtext = no.toString(16);
            while (hashtext.length() < 32) {
                hashtext = "0" + hashtext;
            }
            return hashtext;
        } catch (NoSuchAlgorithmException e) {
            // For specifying wrong message digest algorithms
            throw new RuntimeException(e);
        }
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

}
