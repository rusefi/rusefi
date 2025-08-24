package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.enum_reader.Value;
import com.rusefi.newparse.DefinitionsState;
import com.rusefi.newparse.parsing.Definition;

import com.rusefi.EnumGenerator;
import org.jetbrains.annotations.NotNull;

import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

public class ExtraUtil {
    private final static Logging log = Logging.getLogging(ExtraUtil.class);
    static void handleFiringOrder(String firingEnumFileName, VariableRegistry variableRegistry, DefinitionsState parseState) throws IOException {
        if (firingEnumFileName != null) {
            log.info("Reading firing from " + firingEnumFileName);
            String result = FiringOrderTSLogic.invoke(firingEnumFileName, variableRegistry);
            parseState.addDefinition(variableRegistry, "FIRINGORDER", result, Definition.OverwritePolicy.NotAllowed);
        }
    }

    // This function initiates parsing enums config json file into:
    // 1. Definition of "_list" which contains text for TS
    // 2. Definition of custom type i.e. type + offset + text link
    // 3. Replacements for enum values in format (enum_name + "_" + enum_entry_name) by index/number
    static void handleEnumsDefinitions(String enumsDefinitionsFilePath, ReaderStateImpl state) {
        if (enumsDefinitionsFilePath == null || enumsDefinitionsFilePath.isEmpty()) {
            throw new IllegalStateException("Enum definitions file name is not specified");
        }

        log.info("Reading enum definitions from '" + enumsDefinitionsFilePath + "'");

        EnumGenerator.Parser parser = getParser(enumsDefinitionsFilePath);

        log.info("Parsed " + parser.getEnumDefinitions().size() + " enums from '" + enumsDefinitionsFilePath + "'");

        VariableRegistry variableRegistry = state.getVariableRegistry();

        parser.getDefinitions(EnumGenerator.Parser.Definitions.TsTextRepresentations).forEach(tunerStudioEnumTextRepresentation -> {
            try {
                String[] splitResult = tunerStudioEnumTextRepresentation.split(":");
                String key = splitResult[0];
                String value = splitResult[1];
                variableRegistry.put(key, value);
            } catch (Exception e) {
                throw new IllegalStateException("Unable to add TS enum text defines: '" + tunerStudioEnumTextRepresentation + "' to variables registry", e);
            }
        });

        parser.getDefinitions(EnumGenerator.Parser.Definitions.TsEnumValueToIndexMap).forEach(enumValueToIndexDefinitionStr -> {
            try {
                variableRegistry.processDefine(enumValueToIndexDefinitionStr);
            } catch (Exception e) {
                throw new IllegalStateException("Unable to add TS enum value to index map defines: '" + enumValueToIndexDefinitionStr + "' to variables registry", e);
            }
        });

        parser.getDefinitions(EnumGenerator.Parser.Definitions.CustomTypes).forEach(customType -> {
            try {
                state.handleCustomLine(ReaderStateImpl.CUSTOM + " " + customType);
            } catch (Exception e) {
                throw new IllegalStateException("Unable to parse custom type definition: '" + customType + "'", e);
            }
        });

        // The next whole block is questionable as it is only used to pass tests which are unable to reprocess generated ini
        // And try to look for enums in sourceCodeEnums (originally coming from cpp code parsing)
        Map<String, EnumsReader.EnumState> stateEnums = state.getEnumsReader().getEnums();

        parser.getEnumDefinitions().forEach((enumName, enumDefinition) -> {
            Map<String, Value> currentValues = new HashMap<>();
            enumDefinition.entries.forEach(enumEntry -> {
                Value val = new Value(enumEntry.valueCpp, Long.toString(enumEntry.index));
                currentValues.put(enumEntry.valueCpp, val);
            });

            EnumsReader.EnumState st = new EnumsReader.EnumState(currentValues, enumName, true);

            stateEnums.put(enumName, st);
        });

        log.info("Finished processing enum definitions from '" + enumsDefinitionsFilePath + "'");
    }

    private static EnumGenerator.@NotNull Parser getParser(String enumsDefinitionsFilePath) {
        FileReader reader;

        try {
            reader = new FileReader(enumsDefinitionsFilePath);
        } catch (IOException e) {
            throw new IllegalStateException("Unable to access enum definitions file using path: '" + enumsDefinitionsFilePath + "'", e);
        }

        EnumGenerator.Parser parser;

        try {
            parser = new EnumGenerator.Parser(reader);
        } catch (Exception e) {
            throw new IllegalStateException("Unable to parse enum definitions file using path: '" + enumsDefinitionsFilePath + "'", e);
        }

        return parser;
    }
}
