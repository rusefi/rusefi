package com.rusefi;

import java.util.Stack;

/**
 * (c) Andrey Belomutskiy
 * 12/19/18
 */
class ReaderState {
    Stack<ConfigStructure> stack = new Stack<>();

    public void ensureEmptyAfterProcessing() {
        if (!this.stack.isEmpty())
            throw new IllegalStateException("Unclosed structure: " + this.stack.peek().getName());
    }
}
