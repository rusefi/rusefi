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
    private final String prefix;
    private final Strategy strategy;
    private final String prefixSeparator;
    private final StringBuilder sb = new StringBuilder();

    public PerFieldWithStructuresIterator(ReaderState state, List<ConfigField> fields, String prefix, Strategy strategy, String prefixSeparator) {
        super(fields);
        this.state = state;
        this.prefix = prefix;
        this.strategy = strategy;
        this.prefixSeparator = prefixSeparator;
    }

    public PerFieldWithStructuresIterator(ReaderState state, List<ConfigField> fields, String prefix, Strategy strategy) {
        this(state, fields, prefix, strategy, "_");
    }

    @Override
    public void end() {
        ConfigStructure cs = cf.getState().getStructures().get(cf.getType());
        String content;
        if (cs != null) {
            if (cf.isFromIterate()) {
                // do not support this case yet
                content = "";
            } else {
                // java side of things does not care for 'cs.withPrefix'
                String extraPrefix = prefix + cf.getName() + prefixSeparator;
                PerFieldWithStructuresIterator fieldIterator = new PerFieldWithStructuresIterator(state, cs.getTsFields(), extraPrefix, strategy, prefixSeparator);
                fieldIterator.loop();
                content = fieldIterator.sb.toString();
            }
        } else {
            content = strategy.process(state, cf, prefix);
        }
        sb.append(content);
        super.end();
    }

    public String getContent() {
        return sb.toString();
    }

    interface Strategy {
        String process(ReaderState state, ConfigField configField, String prefix);
    }
}
