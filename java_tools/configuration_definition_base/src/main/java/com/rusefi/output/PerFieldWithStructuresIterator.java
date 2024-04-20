package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;

import java.util.List;

/**
 * @see FieldsStrategy
 * @see FieldIterator is there a duplication?
 */
class PerFieldWithStructuresIterator extends FieldIterator {
    private final ReaderState state;
    private final String variableNamePrefix;
    private final Strategy strategy;
    private final String prefixSeparator;
    private final StringBuilder sb = new StringBuilder();

    public PerFieldWithStructuresIterator(ReaderState state, List<ConfigField> fields, String variableNamePrefix, Strategy strategy, String prefixSeparator) {
        super(fields);
        this.state = state;
        this.variableNamePrefix = variableNamePrefix;
        this.strategy = strategy;
        this.prefixSeparator = prefixSeparator;
    }

    public PerFieldWithStructuresIterator(ReaderState state, List<ConfigField> fields, String variableNamePrefix, Strategy strategy) {
        this(state, fields, variableNamePrefix, strategy, "_");
    }

    @Override
    public void end() {
        ConfigStructure cs = cf.getState().getStructures().get(cf.getTypeName());
        String content;
        if (cs != null) {
            if (strategy.skip(cf)) {
                // do not support this case yet
                content = "";
            } else {
                // java side of things does not care for 'cs.withPrefix'
                String extraPrefix = variableNamePrefix + strategy.getArrayElementName(cf) + prefixSeparator;
                PerFieldWithStructuresIterator fieldIterator = new PerFieldWithStructuresIterator(state, cs.getTsFields(), extraPrefix, strategy, prefixSeparator);
                fieldIterator.loop();
                content = fieldIterator.sb.toString();
            }
        } else {
            content = strategy.process(state, cf, variableNamePrefix);
        }
        sb.append(content);
        super.end();
    }

    public String getContent() {
        return sb.toString();
    }

    interface Strategy {
        String process(ReaderState state, ConfigField configField, String prefix);

        default String getArrayElementName(ConfigField cf) {
            return cf.getName();
        }

        default boolean skip(ConfigField cf) {
            return cf.isFromIterate();
        }
    }
}
