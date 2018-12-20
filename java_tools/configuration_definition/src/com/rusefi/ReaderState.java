package com.rusefi;

import java.util.HashMap;
import java.util.Map;
import java.util.Stack;

/**
 * We keep state here as we read configuration definition
 *
 * (c) Andrey Belomutskiy
 * 12/19/18
 */
public class ReaderState {
    Stack<ConfigStructure> stack = new Stack<>();
    public Map<String, Integer> tsCustomSize = new HashMap<>();
    public Map<String, String> tsCustomLine = new HashMap<>();
    public Map<String, ConfigStructure> structures = new HashMap<>();

    public void ensureEmptyAfterProcessing() {
        if (!this.stack.isEmpty())
            throw new IllegalStateException("Unclosed structure: " + this.stack.peek().getName());
    }
}
