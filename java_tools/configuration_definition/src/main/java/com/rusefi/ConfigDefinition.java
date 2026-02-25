package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.newparse.DefinitionsState;
import com.rusefi.output.*;
import com.rusefi.pinout.PinoutLogic;
import com.rusefi.trigger.TriggerWheelTSLogic;
import com.rusefi.util.LazyFile;

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
    private final static Logging log = Logging.getLogging(ConfigDefinition.class);
    public static final String SIGNATURE_HASH = "SIGNATURE_HASH";

    private static final String KEY_DEFINITION = "-definition";
    private static final String KEY_TS_TEMPLATE = "-ts_destination";
    private static final String KEY_C_DESTINATION = "-c_destination";
    private static final String KEY_C_DEFINES = "-c_defines";
    public static final String KEY_WITH_C_DEFINES = "-with_c_defines";
    private static final String KEY_JAVA_DESTINATION = "-java_destination";
    private static final String KEY_FIRING = "-firing_order";
    public static final String KEY_PREPEND = "-prepend";
    public static final String KEY_SOFT_PREPEND = "-soft_prepend";
    private static final String KEY_SIGNATURE = "-signature";
    private static final String KEY_SIGNATURE_DESTINATION = "-signature_destination";
    private static final String KEY_ZERO_INIT = "-initialize_to_zero";
    private static final String KEY_BOARD_NAME = "-board";
    public static final String CONFIG_PATH = "java_tools/configuration_definition/src/main/resources/config_definition.options";
    public static final String READFILE_OPTION = "-readfile";
    public static final String KEY_ENUMS_CONFIG_PATH = "-enumsConfig";

    public static void main(String[] args) {
        try {
            List<String> options = Files.readAllLines(Paths.get("../" + CONFIG_PATH));
            options.addAll(Arrays.asList(args));
            String[] totalArgs = options.toArray(new String[0]);
            if (totalArgs.length < 2) {
                log.error("Please specify\r\n"
                    + KEY_DEFINITION + " x\n"
                    + KEY_TS_TEMPLATE + " x\n"
                    + KEY_C_DESTINATION + " x\n"
                    + KEY_JAVA_DESTINATION + " x\n"
                );
                return;
            }
            ReaderStateImpl state = new ReaderStateImpl();
            doJob(totalArgs, state);
            int frenchBooleanNameLimit = Integer.parseInt(state.getVariableRegistry().get("TRUE_FALSE_COUNT_LIMIT"));
            if (state.getDefaultBitNameCounter() > frenchBooleanNameLimit) {
                throw new IllegalStateException("We are trying to reduce inhumane true/false bitNames: " + state.getDefaultBitNameCounter());
            }
        } catch (Throwable e) {
            log.error("unexpected", e);
            e.printStackTrace();
            System.exit(-1);
        }
    }

    public static void doJob(String[] args, ReaderStateImpl state) throws IOException {
        log.info(ConfigDefinition.class + " Invoked with " + Arrays.toString(args));

        String tsInputFileFolder = null;
        List<String> softPrePrependsFileNames = new ArrayList<>();

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
                    tsInputFileFolder = args[i + 1];
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
                    String folderName = args[i + 1];
                    state.addDestination(new FileJavaVariableRegistryConsumer(state, folderName, LazyFile.REAL, "VariableRegistryValues"));
                    break;
                case "-field_lookup_file": {
                    String cppFile = args[i + 1];
                    String mdFile = args[i + 2];
                    i++;
                    state.addDestination(new GetConfigValueConsumer(cppFile, mdFile, LazyFile.REAL));
                }
                break;
                case READFILE_OPTION:
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
                case KEY_SOFT_PREPEND: {
                    String softPrependFileName = args[i + 1].trim();
                    softPrePrependsFileNames.add(softPrependFileName);
                    state.addSoftPrepend(softPrependFileName);
                }
                    break;
                case KEY_SIGNATURE:
                    signaturePrependFile = args[i + 1];
                    state.addPostponedPrependNotInput(signaturePrependFile);
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
                case KEY_ENUMS_CONFIG_PATH:
                    String enumsDefinitionsFilePath = args[i + 1];
                    String enumsDefinitionsFilePathFixed = IoUtil3.prependIfNotAbsolute(RootHolder.ROOT, enumsDefinitionsFilePath);
                    ExtraUtil.handleEnumsDefinitions(enumsDefinitionsFilePathFixed, state);
                    break;
            }
        }

        FieldsApiGenerator.run();
        handlePage(state, 1, softPrePrependsFileNames);
        handlePage(state, 2, softPrePrependsFileNames);
        handlePage(state, 3, softPrePrependsFileNames);

        if (tsInputFileFolder != null) {
            // used to update .ini files
            state.addInputFile(TSProjectConsumer.getTsFileInputName(tsInputFileFolder));
        }

        log.info(state.getEnumsReader().getEnums().size() + " total enumsReader");

        // Add the variable for the config signature
        FirmwareVersion uniqueId = new FirmwareVersion(IoUtil2.getCrc32(state.getInputFiles()));
        SignatureConsumer.storeUniqueBuildId(state, parseState, tsInputFileFolder, uniqueId);


        if (pinoutLogic != null) {
            pinoutLogic.registerBoardSpecificPinNames(state.getVariableRegistry(), parseState, state.getEnumsReader());
        }

        if (tsInputFileFolder != null) {
            state.addDestination(new TSProjectConsumer(tsInputFileFolder, state));

            VariableRegistry tmpRegistry = new VariableRegistry();
            // store the CRC32 as a built-in variable
            tmpRegistry.register(SIGNATURE_HASH, uniqueId.encode());
            tmpRegistry.readPrependValues(signaturePrependFile, false);
            state.addDestination(new SignatureConsumer(signatureDestination, tmpRegistry));
        }

        state.doJob();
    }

    private static void handlePage(ReaderStateImpl parentState, int pageIndex, List<String> softPrepends) throws IOException {
        PlainConfigHandler page = new PlainConfigHandler("integration/config_page_" + pageIndex + ".txt", pageIndex, softPrepends);
        page.doJob();
        // PAGE_CONTENT_1 is handled here!
        parentState.getVariableRegistry().put("PAGE_CONTENT_" + pageIndex, page.tsProjectConsumer.getContent());
        parentState.getVariableRegistry().register("PAGE_SIZE_" + pageIndex, Integer.toString(page.tsProjectConsumer.getTotalSize()));
    }
}
