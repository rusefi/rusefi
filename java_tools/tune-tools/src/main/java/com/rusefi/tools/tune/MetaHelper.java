package com.rusefi.tools.tune;

import com.devexperts.logging.Logging;
import com.rusefi.*;
import com.rusefi.output.ConfigStructure;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import static com.devexperts.logging.Logging.getLogging;

/**
 * utility methods to get relevant ConfigField from current rusefi_config.txt
 */
public class MetaHelper {
    private static final Logging log = getLogging(MetaHelper.class);
    public static final String PERSISTENT_CONFIG_S = "persistent_config_s";
    public static final String ENGINE_CONFIGURATION_S = "engine_configuration_s";

    @NotNull
    static ReaderStateImpl getReaderState(String boardPath) throws IOException {
        List<String> options = FileLinesHelper.readAllLinesWithRoot("/../" + ConfigDefinition.CONFIG_PATH);
        List<String> boardOptions = FileLinesHelper.readAllLines2(boardPath + "board_config.txt");

        options.add(ConfigDefinition.KEY_PREPEND);
        options.add("integration/rusefi_config_shared.txt");
        options.add(ConfigDefinition.KEY_PREPEND);
        options.add("integration/fields_api.txt");

        // add board prepend
        options.add(ConfigDefinition.KEY_PREPEND);
        options.add(boardPath + "prepend.txt");

        // add board config
        // TODO: this is not correctly registered, see hack below on "state.getVariableRegistry().register"
        options.add(ConfigDefinition.READFILE_OPTION);
        options.add(BoardConfigStrategy.BOARD_CONFIG_FROM_FILE);
        options.add(boardPath + "board_config.txt");

        // add default (empty) board config
        options.add(ConfigDefinition.READFILE_OPTION);
        options.add("BOARD_ENGINE_CONFIGURATION_FROM_FILE");
        options.add("tunerstudio/empty_board_options.ini");

        String[] totalArgs = options.toArray(new String[0]);

        ReaderStateImpl state = new ReaderStateImpl();
        if (TuneContext.boardPrepend != null)
            state.getVariableRegistry().readPrependValues(TuneContext.boardPrepend, false);

        state.getVariableRegistry().register(BoardConfigStrategy.BOARD_CONFIG_FROM_FILE, String.join("\n", boardOptions));

        ConfigDefinition.doJob(totalArgs, state);
        return state;
    }

    // nasty: context is a return parameter
    @Nullable
    static ConfigField lookForFieldWithinSpecificStruct(ReaderState state, String name, StringBuffer context, String parentStructName) {
        ConfigStructure s = state.getStructures().get(parentStructName);
//                log.info("We have a custom value " + name);
        ConfigField cf = s.getTsFieldByName(name);
        if (cf != null) {
            return cf;
        }
        int fromIndex = 0;
        while (true) {
            fromIndex = name.indexOf('_', fromIndex);
            if (fromIndex == -1) {
                // no struct names
                return null;
            }
            String parentName = name.substring(0, fromIndex);
            cf = s.getTsFieldByName(parentName);
            fromIndex++; // skip underscore
            if (cf == null)
                continue;
            String type = cf.getTypeName();
            s = state.getStructures().get(type);

            if (s != null) {
                String substring = name.substring(fromIndex);
                ConfigField tsFieldByName = s.getTsFieldByName(substring);
                if (tsFieldByName == null) {
                    log.info("Not located " + substring + " in " + s);
                } else {
                    context.append(cf.getOriginalArrayName()).append(".");
                    log.info(name + ": Located " + tsFieldByName + " in " + s);
                }
                return tsFieldByName;
            }
        }
    }

    //
    static ConfigField findField(ReaderState state, String name, StringBuffer context) {
        ConfigField field = lookForFieldWithinSpecificStruct(state, name, context, ENGINE_CONFIGURATION_S);
        if (field != null)
            return field;
        // TuneCanTool has a smarter version of similar parent lookup logic?
        return lookForFieldWithinSpecificStruct(state, name, context, PERSISTENT_CONFIG_S);
    }
}
