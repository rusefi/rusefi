package com.rusefi.output;

import com.rusefi.ConfigField;

import java.util.List;

class PerFieldWithStructuresIterator extends FieldIterator {
    private final String prefix;
    private final Strategy strategy;
    StringBuilder sb = new StringBuilder();

    public PerFieldWithStructuresIterator(List<ConfigField> fields, String prefix, Strategy strategy) {
        super(fields);
        this.prefix = prefix;
        this.strategy = strategy;
    }

    @Override
    public void end() {
        ConfigStructure cs = cf.getState().structures.get(cf.getType());
        String content;
        if (cs != null) {
            String extraPrefix = cs.withPrefix ? cf.getName() + "_" : "";
            PerFieldWithStructuresIterator fieldIterator = new PerFieldWithStructuresIterator(cs.tsFields, extraPrefix, strategy);
            fieldIterator.loop();
            content = fieldIterator.sb.toString();
        } else {
            content = strategy.process(cf, prefix);
        }
        sb.append(content);
        super.end();
    }

    interface Strategy {
        String process(ConfigField configField, String prefix);
    }
}
