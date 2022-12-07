package com.rusefi.newparse;

import com.rusefi.VariableRegistry;
import com.rusefi.newparse.parsing.Definition;

public interface DefinitionsState {
    void addDefinition(VariableRegistry variableRegistry, String name, String value, Definition.OverwritePolicy overwritePolicy);

    void setDefinitionPolicy(Definition.OverwritePolicy policy);
}
