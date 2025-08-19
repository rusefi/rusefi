package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.newparse.DefinitionsState;
import com.rusefi.newparse.parsing.Definition;

import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

public class ExtraUtil {
    private final static Logging log = Logging.getLogging(ExtraUtil.class);
    static void handleFiringOrder(String firingEnumFileName, VariableRegistry variableRegistry, DefinitionsState parseState) throws IOException {
        if (firingEnumFileName != null) {
            log.info("Reading firing from " + firingEnumFileName);
            String result = FiringOrderTSLogic.invoke(firingEnumFileName, variableRegistry);
            parseState.addDefinition(variableRegistry, "FIRINGORDER", result, Definition.OverwritePolicy.NotAllowed);
        }
    }

    static void handleEnumCfgDefitions(String enumCfgDefinitionFileName, VariableRegistry variableRegistry) {
        if (enumCfgDefinitionFileName != null) {
            log.info("Reading enum definitions for TS config from " + enumCfgDefinitionFileName);

            if (!Files.isRegularFile(Paths.get(enumCfgDefinitionFileName))) {
                log.error("File " + enumCfgDefinitionFileName + " does not exist!");
                System.exit(1);
            }

            variableRegistry.readPrependValues(enumCfgDefinitionFileName, true);
        }
    }

    static void handleEnumIniReplacements(String enumIniReplacementsFileName, VariableRegistry variableRegistry) throws IOException {
        if (enumIniReplacementsFileName != null) {
            log.info("Reading enum definitions for TS ini replacements from " + enumIniReplacementsFileName);

            if (!Files.isRegularFile(Paths.get(enumIniReplacementsFileName))) {
                log.error("File " + enumIniReplacementsFileName + " does not exist!");
                System.exit(1);
            }

            String generatedEnumsContent = Files.readString(Paths.get(enumIniReplacementsFileName));

            variableRegistry.put("GENERATED_ENUMS_TS_LISTS", generatedEnumsContent);
        }
    }

    static void handleEnumTxtDefinitions(String extraEnumsFileNameTxt, ReaderStateImpl state) throws IOException {
        if (extraEnumsFileNameTxt != null) {
            log.info("Reading enum definitions for rusefi_config.txt config from " + extraEnumsFileNameTxt);

            if (!Files.isRegularFile(Paths.get(extraEnumsFileNameTxt))) {
                log.error("File " + extraEnumsFileNameTxt + " does not exist!");
                System.exit(1);
            }

            List<String> generatedEnumsContent = Files.readAllLines(Paths.get(extraEnumsFileNameTxt));

            for (String line : generatedEnumsContent) {
                if (ToolUtil.isEmptyDefinitionLine(line)) {
                    continue;
                }

                if (!ToolUtil.startsWithToken(line, ReaderStateImpl.CUSTOM)) {
                    continue;
                }

                state.handleCustomLine(line);
            }
        }
    }
}
