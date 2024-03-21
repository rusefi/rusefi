package com.rusefi;

import com.rusefi.newparse.DefinitionsState;
import com.rusefi.output.*;
import com.rusefi.pinout.PinoutLogic;
import com.rusefi.trigger.TriggerWheelTSLogic;
import com.rusefi.util.LazyFile;
import com.rusefi.util.SystemOut;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
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
    private static final String KEY_TS_TEMPLATE = "-ts_template";
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
    public static final String CONFIG_PATH = "java_tools/configuration_definition/src/main/resources/config_definition.options";

    public static void main(String[] args) {
        try {
            List<String> options = Files.readAllLines(Paths.get("../" + CONFIG_PATH));
            options.addAll(Arrays.asList(args));
            String[] totalArgs = options.toArray(new String[0]);
            if (totalArgs.length < 2) {
                SystemOut.println("Please specify\r\n"
                        + KEY_DEFINITION + " x\r\n"
                        + KEY_TS_TEMPLATE + " x\r\n"
                        + KEY_C_DESTINATION + " x\r\n"
                        + KEY_JAVA_DESTINATION + " x\r\n"
                );
                return;
            }
            doJob(totalArgs, new ReaderStateImpl());
        } catch (Throwable e) {
            SystemOut.println(e);
            e.printStackTrace();
            System.exit(-1);
        } finally {
            SystemOut.close();
        }
    }

    public static void doJob(String[] args, ReaderStateImpl state) throws IOException {
        SystemOut.println(ConfigDefinition.class + " Invoked with " + Arrays.toString(args));

        String tsTemplateFile = null;

        DefinitionsState parseState = state.getEnumsReader().parseState;
        String signatureDestination = null;
        String signaturePrependFile = null;
        List<String> enumInputFiles = new ArrayList<>();
        PinoutLogic pinoutLogic = null;

        for (int i = 0; i < args.length - 1; i += 2) {
            String key = args[i];
            switch (key) {
                case "-tool":
                    // lame: order of command line arguments is important
                    ToolUtil.TOOL = args[i + 1];
                    break;
                case KEY_DEFINITION:
                    // lame: order of command line arguments is important, these arguments should be AFTER '-tool' argument
                    state.setDefinitionInputFile(args[i + 1]);
                    break;
                case KEY_TS_TEMPLATE:
                    tsTemplateFile = args[i + 1];
                    break;
                case KEY_C_DESTINATION:
                    state.addCHeaderDestination(args[i + 1]);
                    break;
                case KEY_ZERO_INIT:
                    BaseCHeaderConsumer.needZeroInit = Boolean.parseBoolean(args[i + 1]);
                    break;
                case KEY_WITH_C_DEFINES:
                    state.setWithC_Defines(Boolean.parseBoolean(args[i + 1]));
                    break;
                case KEY_C_DEFINES:
                    state.destCDefinesFileName = args[i + 1];
                    break;
                case KEY_JAVA_DESTINATION:
                    state.addJavaDestination(args[i + 1]);
                    break;
                case "-field_lookup_file": {
                    String cppFile = args[i + 1];
                    String mdFile = args[i + 2];
                    i++;
                    state.addDestination(new GetConfigValueConsumer(cppFile, mdFile, LazyFile.REAL));
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
                    break;
                case KEY_FIRING: {
                    String firingEnumFileName = args[i + 1];
                    ExtraUtil.handleFiringOrder(firingEnumFileName, state.getVariableRegistry(), parseState);
                    state.addInputFile(firingEnumFileName);
                    }
                    break;
                case "-triggerInputFolder": {
                    String triggersInputFolder = args[i + 1];
                    new TriggerWheelTSLogic().execute(triggersInputFolder, state.getVariableRegistry());
                }
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
                case EnumToString.KEY_ENUM_INPUT_FILE: {
                    String enumInputFile = args[i + 1];
                    enumInputFiles.add(enumInputFile);
                    File file = new File(RootHolder.ROOT + enumInputFile);
                    try (Reader r = new FileReader(file)) {
                        state.read(r);
                    } catch (Throwable e) {
                        throw new IllegalStateException("Reading " + file.getAbsolutePath(), e);
                    }
                }
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

        if (tsTemplateFile != null) {
            // used to update .ini files
            state.addInputFile(tsTemplateFile);
        }

        SystemOut.println(state.getEnumsReader().getEnums().size() + " total enumsReader");

        // Add the variable for the config signature
        FirmwareVersion uniqueId = new FirmwareVersion(IoUtil2.getCrc32(state.getInputFiles()));
        SignatureConsumer.storeUniqueBuildId(state, parseState, tsTemplateFile, uniqueId);


        if (pinoutLogic != null) {
            pinoutLogic.registerBoardSpecificPinNames(state.getVariableRegistry(), parseState, state.getEnumsReader());
        }

        if (tsTemplateFile != null) {
            state.addDestination(new TSProjectConsumer(tsTemplateFile, state));

            VariableRegistry tmpRegistry = new VariableRegistry();
            // store the CRC32 as a built-in variable
            tmpRegistry.register(SIGNATURE_HASH, uniqueId.encode());
            tmpRegistry.readPrependValues(signaturePrependFile);
            state.addDestination(new SignatureConsumer(signatureDestination, tmpRegistry));
        }

        state.doJob();
    }
}
