package com.rusefi;

import java.util.HashMap;
import java.util.Map;
import java.util.Stack;

/**
 * (c) Andrey Belomutskiy
 * 12/19/18
 */
public class ReaderState {
    Stack<ConfigStructure> stack = new Stack<>();
    public Map<String, Integer> tsCustomSize = new HashMap<>();
    public Map<String, String> tsCustomLine = new HashMap<>();

    public void ensureEmptyAfterProcessing() {
        if (!this.stack.isEmpty())
            throw new IllegalStateException("Unclosed structure: " + this.stack.peek().getName());
    }
}
