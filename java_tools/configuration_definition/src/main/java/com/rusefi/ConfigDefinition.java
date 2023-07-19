package com.rusefi;

// import com.rusefi.newparse.outputs.CStructWriter;
import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.parsing.Definition;
import com.rusefi.output.*;
import com.rusefi.pinout.PinoutLogic;
import com.rusefi.trigger.TriggerWheelTSLogic;
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

    private static final String KEY_DEFINITION = "-definition";
    private static final String KEY_TS_DESTINATION = "-ts_destination";
    private static final String KEY_C_DESTINATION = "-c_destination";
    private static final String KEY_C_DEFINES = "-c_defines";
    public static final String KEY_WITH_C_DEFINES = "-with_c_defines";
    private static final String KEY_JAVA_DESTINATION = "-java_destination";
    private static final String KEY_FIRING = "-firing_order";
    public static final String KEY_PREPEND = "-prepend";
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
            doJob(args, new ReaderStateImpl());
        } catch (Throwable e) {
            SystemOut.println(e);
            e.printStackTrace();
            SystemOut.close();
            System.exit(-1);
        }
        SystemOut.close();
    }

    public static void doJob(String[] args, ReaderStateImpl state) throws IOException {
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
        String cHeaderDestination = null;
        // we postpone reading so that in case of cache hit we do less work
        String firingEnumFileName = null;
        String triggersInputFolder = null;
        String signatureDestination = null;
        String signaturePrependFile = null;
        List<String> enumInputFiles = new ArrayList<>();
        PinoutLogic pinoutLogic = null;

        for (int i = 0; i < args.length - 1; i += 2) {
            String key = args[i];
            switch (key) {
                case KEY_DEFINITION:
                    // lame: order of command line arguments is important, these arguments should be AFTER '-tool' argument
                    state.setDefinitionInputFile(args[i + 1]);
                    break;
                case KEY_TS_DESTINATION:
                    tsInputFileFolder = args[i + 1];
                    break;
                case KEY_C_DESTINATION:
                    cHeaderDestination = args[i + 1];
                    state.addCHeaderDestination(args[i + 1]);
                    break;
                case KEY_ZERO_INIT:
                    needZeroInit = Boolean.parseBoolean(args[i + 1]);
                    break;
                case KEY_WITH_C_DEFINES:
                    state.setWithC_Defines(Boolean.parseBoolean(args[i + 1]));
                    break;
                case KEY_C_DEFINES:
                    destCDefinesFileName = args[i + 1];
                    break;
                case KEY_JAVA_DESTINATION:
                    state.addJavaDestination(args[i + 1]);
                    break;
                case "-field_lookup_file": {
                    String cppFile = args[i + 1];
                    String mdFile = args[i + 2];
                    i++;
                    state.addDestination(new GetConfigValueConsumer(cppFile, mdFile));
                }
                    break;
                case "-readfile":
                    String keyName = args[i + 1];
                    // yes, we take three parameters here thus pre-increment!
                    String fileName = args[++i + 1];
                    try {
                        state.getVariableRegistry().register(keyName, IoUtil2.readFile(fileName));
                    } catch (RuntimeException e) {
                        throw new IllegalStateException("While processing " + fileName, e);
                    }
                    state.addInputFile(fileName);
                case KEY_FIRING:
                    firingEnumFileName = args[i + 1];
                    state.addInputFile(firingEnumFileName);
                    break;
                case "-triggerInputFolder":
                    triggersInputFolder = args[i + 1];
                    break;
                case KEY_PREPEND:
                    state.addPrepend(args[i + 1].trim());
                    break;
                case KEY_SIGNATURE:
                    signaturePrependFile = args[i + 1];
                    state.getPrependFiles().add(args[i + 1]);
                    // don't add this file to the 'inputFiles'
                    break;
                case KEY_SIGNATURE_DESTINATION:
                    signatureDestination = args[i + 1];
                    break;
                case EnumToString.KEY_ENUM_INPUT_FILE:
                    enumInputFiles.add(args[i + 1]);
                    break;
                case "-ts_output_name":
                    state.setTsFileOutputName(args[i + 1]);
                    break;
                case KEY_BOARD_NAME:
                    String boardName = args[i + 1];
                    pinoutLogic = PinoutLogic.create(boardName);
                    for (String inputFile : pinoutLogic.getInputFiles())
                        state.addInputFile(inputFile);
                    break;
            }
        }

        if (tsInputFileFolder != null) {
            // used to update .ini files
            state.addInputFile(TSProjectConsumer.getTsFileInputName(tsInputFileFolder));
        }

        if (!enumInputFiles.isEmpty()) {
            for (String ef : enumInputFiles) {
                state.read(new FileReader(ef));
            }

            SystemOut.println(state.getEnumsReader().getEnums().size() + " total enumsReader");
        }

        ParseState parseState = new ParseState(state.getEnumsReader());
        // Add the variable for the config signature
        FirmwareVersion uniqueId = new FirmwareVersion(IoUtil2.getCrc32(state.getInputFiles()));
        SignatureConsumer.storeUniqueBuildId(state, parseState, tsInputFileFolder, uniqueId);

        new TriggerWheelTSLogic().execute(triggersInputFolder, state.getVariableRegistry());

        if (pinoutLogic != null) {
            pinoutLogic.registerBoardSpecificPinNames(state.getVariableRegistry(), parseState, state.getEnumsReader());
        }

        // Parse the input files
        {
            // Load prepend files
            {
                // Ignore duplicates of definitions made during prepend phase
                parseState.setDefinitionPolicy(Definition.OverwritePolicy.IgnoreNew);

                for (String prependFile : state.getPrependFiles()) {
                    RusefiParseErrorStrategy.parseDefinitionFile(parseState.getListener(), prependFile);
                }
            }

            // Now load the main config file
            {
                // don't allow duplicates in the main file
                parseState.setDefinitionPolicy(Definition.OverwritePolicy.NotAllowed);
                RusefiParseErrorStrategy.parseDefinitionFile(parseState.getListener(), state.getDefinitionInputFile());
            }

            // Write C structs
            // CStructWriter cStructs = new CStructWriter();
            // cStructs.writeCStructs(parseState, cHeaderDestination + ".test");

            // Write tunerstudio layout
            // TsWriter writer = new TsWriter();
            // writer.writeTunerstudio(parseState, tsInputFileFolder + "/rusefi.input", tsInputFileFolder + "/" + state.getTsFileOutputName() + ".test");
        }

        if (tsInputFileFolder != null) {
            state.addDestination(new TSProjectConsumer(tsInputFileFolder, state));

            VariableRegistry tmpRegistry = new VariableRegistry();
            // store the CRC32 as a built-in variable
            tmpRegistry.register(SIGNATURE_HASH, uniqueId.encode());
            tmpRegistry.readPrependValues(signaturePrependFile);
            state.addDestination(new SignatureConsumer(signatureDestination, tmpRegistry));
        }

        if (state.isDestinationsEmpty())
            throw new IllegalArgumentException("No destinations specified");

        state.doJob();

        if (destCDefinesFileName != null) {
            ExtraUtil.writeDefinesToFile(state.getVariableRegistry(), destCDefinesFileName);
        }
    }
}
