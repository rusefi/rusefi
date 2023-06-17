package com.rusefi.newparse;

import com.rusefi.EnumsReader;
import com.rusefi.VariableRegistry;
import com.rusefi.enum_reader.Value;
import com.rusefi.newparse.parsing.Definition;

import java.util.HashMap;
import java.util.Map;

public class DefinitionsStateImpl implements DefinitionsState {
    private final Map<String, Definition> definitions = new HashMap<>();

    private final Definition.OverwritePolicy definitionOverwritePolicy = Definition.OverwritePolicy.NotAllowed;

    public DefinitionsStateImpl(EnumsReader enumsReader) {

        for (Map.Entry<String, EnumsReader.EnumState> enumType : enumsReader.getEnums().entrySet()) {
            String name = enumType.getKey();

            for (Value enumValue : enumType.getValue().values()) {
                try {
                    int value = enumValue.getIntValue();

                    this.handleIntDefinition(name + "_" + enumValue.getName(), value);
                } catch (Exception exc) {
                    // ignore parse failures
                }
            }
        }
    }

    private void handleIntDefinition(String name, int value) {
        addDefinition(name, value);

        // Also add ints as 16b hex
        addDefinition(name + "_16_hex", String.format("\\x%02x\\x%02x", (value >> 8) & 0xFF, value & 0xFF));
    }

    /**
     * we are in a lengthy period of transition between two implementations
     */
    @Override
    public void addDefinition(VariableRegistry variableRegistry, String name, String value, Definition.OverwritePolicy overwritePolicy) {
        // old implementation
        variableRegistry.register(name, value);
        // new implementation
        addDefinition(name, value, overwritePolicy);
    }

    public void addDefinition(String name, Object value, Definition.OverwritePolicy overwritePolicy) {
        Definition existingDefinition = definitions.getOrDefault(name, null);

        if (existingDefinition != null) {
            switch (existingDefinition.overwritePolicy) {
                case NotAllowed:
                    throw new IllegalStateException("Tried to add definition for " + name + ", but one already existed.");
                case IgnoreNew:
                    // ignore the new definition, do nothing
                    return;
            }
        }

        definitions.put(name, new Definition(value, overwritePolicy));
    }

    private void addDefinition(String name, Object value) {
        addDefinition(name, value, definitionOverwritePolicy);
    }
}
