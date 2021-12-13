package com.rusefi.output;

import com.rusefi.ConfigField;
import com.rusefi.ReaderState;

import java.util.List;

class PerFieldWithStructuresIterator extends FieldIterator {
    private final ReaderState state;
    private final String prefix;
    private final Strategy strategy;
    private final String prefixSeparator;
    StringBuilder sb = new StringBuilder();

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
        ConfigStructure cs = cf.getState().structures.get(cf.getType());
        String content;
        if (cs != null) {
            String extraPrefix = cs.withPrefix ? cf.getName() + prefixSeparator : "";
            PerFieldWithStructuresIterator fieldIterator = new PerFieldWithStructuresIterator(state, cs.tsFields, extraPrefix, strategy);
            fieldIterator.loop();
            content = fieldIterator.sb.toString();
        } else {
            content = strategy.process(state, cf, prefix);
        }
        sb.append(content);
        super.end();
    }

    interface Strategy {
        String process(ReaderState state, ConfigField configField, String prefix);
    }
}
