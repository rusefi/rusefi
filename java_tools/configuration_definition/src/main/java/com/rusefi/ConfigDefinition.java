package com.rusefi;

import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.parsing.Definition;
import com.rusefi.output.*;
import com.rusefi.trigger.TriggerWheelTSLogic;
import com.rusefi.util.IoUtils;
import com.rusefi.util.SystemOut;

import java.io.*;
import java.util.*;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 1/12/15
 *
 * @see ConfigurationConsumer
 */
public class ConfigDefinition {
    public static final String SIGNATURE_HASH = "SIGNATURE_HASH";

    private static final String ROM_RAIDER_XML_TEMPLATE = "rusefi_template.xml";
    public static final String KEY_DEFINITION = "-definition";
    private static final String KEY_ROMRAIDER_INPUT = "-romraider";
    private static final String KEY_TS_DESTINATION = "-ts_destination";
    public static final String KEY_C_DESTINATION = "-c_destination";
    private static final String KEY_C_DEFINES = "-c_defines";
    private static final String KEY_WITH_C_DEFINES = "-with_c_defines";
    public static final String KEY_JAVA_DESTINATION = "-java_destination";
    private static final String KEY_ROMRAIDER_DESTINATION = "-romraider_destination";
    private static final String KEY_FIRING = "-firing_order";
    private static final String KEY_PREPEND = "-prepend";
    private static final String KEY_SIGNATURE = "-signature";
    private static final String KEY_SIGNATURE_DESTINATION = "-signature_destination";
    private static final String KEY_ZERO_INIT = "-initialize_to_zero";
    private static final String KEY_BOARD_NAME = "-board";
    /**
     * This flag controls if we assign default zero value (useful while generating structures used for class inheritance)
     * versus not assigning default zero value like we need for non-class headers
     * This could be related to configuration header use-case versus "live data" (not very alive idea) use-case
     */
    public static boolean needZeroInit = true;

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

        String tsInputFileFolder = null;
        String destCDefinesFileName = null;
        String romRaiderDestination = null;
        // we postpone reading so that in case of cache hit we do less work
        List<String> prependFiles = new ArrayList<>();
        String romRaiderInputFile = null;
        String firingEnumFileName = null;
        String triggersFolder = null;
        String signatureDestination = null;
        String signaturePrependFile = null;
        List<String> enumInputFiles = new ArrayList<>();
        boolean withC_Defines = true;
        PinoutLogic pinoutLogic = null;
        String tsOutputsDestination = null;
        String definitionInputFile = null;

        // used to update other files
        List<String> inputFiles = new ArrayList<>();

        List<ConfigurationConsumer> destinations = new ArrayList<>();
        ReaderState state = new ReaderState();

        for (int i = 0; i < args.length - 1; i += 2) {
            String key = args[i];
            switch (key) {
                case "-tool":
                    ToolUtil.TOOL = args[i + 1];
                    break;
                case KEY_DEFINITION:
                    definitionInputFile = args[i + 1];
                    state.headerMessage = ToolUtil.getGeneratedAutomaticallyTag() + definitionInputFile + " " + new Date();
                    inputFiles.add(definitionInputFile);
                    break;
                case KEY_TS_DESTINATION:
                    tsInputFileFolder = args[i + 1];
                    break;
                case "-ts_outputs_section":
                    tsOutputsDestination = args[i + 1];
                    break;
                case KEY_C_DESTINATION:
                    destinations.add(new CHeaderConsumer(state, args[i + 1], withC_Defines));
                    break;
                case KEY_ZERO_INIT:
                    needZeroInit = Boolean.parseBoolean(args[i + 1]);
                    break;
                case KEY_WITH_C_DEFINES:
                    withC_Defines = Boolean.parseBoolean(args[i + 1]);
                    break;
                case KEY_C_DEFINES:
                    destCDefinesFileName = args[i + 1];
                    break;
                case KEY_JAVA_DESTINATION:
                    destinations.add(new FileJavaFieldsConsumer(state, args[i + 1]));
                    break;
                case "-field_lookup_file":
                    destinations.add(new GetConfigValueConsumer(args[i + 1]));
                    break;
                case "-output_lookup_file":
                    destinations.add(new GetOutputValueConsumer(args[i + 1]));
                    break;
                case "-readfile":
                    String keyName = args[i + 1];
                    // yes, we take three parameters here thus pre-increment!
                    String fileName = args[++i + 1];
                    state.variableRegistry.register(keyName, IoUtil2.readFile(fileName));
                    inputFiles.add(fileName);
                case KEY_FIRING:
                    firingEnumFileName = args[i + 1];
                    inputFiles.add(firingEnumFileName);
                    break;
                case "-triggerFolder":
                    triggersFolder = args[i + 1];
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
                    pinoutLogic = PinoutLogic.create(boardName);
                    if (pinoutLogic != null)
                        inputFiles.addAll(pinoutLogic.getInputFiles());
                    break;
            }
        }

        if (tsInputFileFolder != null) {
            // used to update .ini files
            inputFiles.add(TSProjectConsumer.getTsFileInputName(tsInputFileFolder));
        }

        if (!enumInputFiles.isEmpty()) {
            for (String ef : enumInputFiles) {
                state.read(new FileReader(ef));
            }

            SystemOut.println(state.enumsReader.getEnums().size() + " total enumsReader");
        }

        ParseState parseState = new ParseState(state.enumsReader);
        // Add the variable for the config signature
        long crc32 = IoUtil2.signatureHash(state, parseState, tsInputFileFolder, inputFiles);

        ExtraUtil.handleFiringOrder(firingEnumFileName, state.variableRegistry, parseState);

        new TriggerWheelTSLogic().execute(triggersFolder, state.variableRegistry);


        for (String prependFile : prependFiles)
            state.variableRegistry.readPrependValues(prependFile);

        if (pinoutLogic != null) {
            pinoutLogic.processYamls(state.variableRegistry, state);
        }

        // Parse the input files
        {

            // Load prepend files
            {
                // Ignore duplicates of definitions made during prepend phase
                parseState.setDefinitionPolicy(Definition.OverwritePolicy.IgnoreNew);

                for (String prependFile : prependFiles) {
                    RusefiParseErrorStrategy.parseDefinitionFile(parseState.getListener(), prependFile);
                }
            }

            // Now load the main config file
            {
                // don't allow duplicates in the main file
                parseState.setDefinitionPolicy(Definition.OverwritePolicy.NotAllowed);
                RusefiParseErrorStrategy.parseDefinitionFile(parseState.getListener(), definitionInputFile);
            }

            // Write C structs
            // CStructWriter cStructs = new CStructWriter();
            // cStructs.writeCStructs(parseState, destCHeaderFileName + ".test");

            // Write tunerstudio layout
            // TsWriter writer = new TsWriter();
            // writer.writeTunerstudio(parseState, tsPath + "/rusefi.input", tsPath + "/" + TSProjectConsumer.TS_FILE_OUTPUT_NAME);
        }

        if (tsOutputsDestination != null) {
            destinations.add(new OutputsSectionConsumer(tsOutputsDestination + File.separator + "generated/output_channels.ini", state));
            destinations.add(new DataLogConsumer(tsOutputsDestination + File.separator + "generated/data_logs.ini"));
            destinations.add(new GaugeConsumer(tsOutputsDestination + File.separator + "generated/gauges.ini", state));
        }
        if (tsInputFileFolder != null) {
            CharArrayWriter tsWriter = new CharArrayWriter();
            destinations.add(new TSProjectConsumer(tsWriter, tsInputFileFolder, state));

            VariableRegistry tmpRegistry = new VariableRegistry();
            // store the CRC32 as a built-in variable
            tmpRegistry.register(SIGNATURE_HASH, "" + crc32);
            tmpRegistry.readPrependValues(signaturePrependFile);
            destinations.add(new SignatureConsumer(signatureDestination, tmpRegistry));
        }

        if (destinations.isEmpty())
            throw new IllegalArgumentException("No destinations specified");
        /*
         * this is the most important invocation - here we read the primary input file and generated code into all
         * the destinations/writers
         */
        SystemOut.println("Reading definition from " + definitionInputFile);
        BufferedReader definitionReader = new BufferedReader(new InputStreamReader(new FileInputStream(definitionInputFile), IoUtils.CHARSET.name()));
        state.readBufferedReader(definitionReader, destinations);

        if (destCDefinesFileName != null) {
            ExtraUtil.writeDefinesToFile(state.variableRegistry, destCDefinesFileName, definitionInputFile);
        }

        if (romRaiderDestination != null && romRaiderInputFile != null) {
            ExtraUtil.processTextTemplate(state, romRaiderInputFile, romRaiderDestination);
        }
    }
}
