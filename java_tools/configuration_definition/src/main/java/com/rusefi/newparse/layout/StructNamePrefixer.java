package com.rusefi.newparse.layout;

import java.util.Stack;

public class StructNamePrefixer {
    private final Stack<String> stack = new Stack<>();
    private int idx = -1;
    private final char prefixChar;

    public StructNamePrefixer(char prefixChar) {
        this.prefixChar = prefixChar;
    }

    public void pop() {
        stack.pop();
    }

    public void push(String name) {
        if (this.idx != -1) {
            // If we push with an index set, it means we're inside an array of structs.
            // In that case, we embed the index (of the struct in the array) within the array's name
            name = name + this.idx;
            this.idx = -1;
        }

        stack.push(name + this.prefixChar);
    }

    void setIndex(int idx) {
        if (idx >= 0) {
            this.idx = idx + 1;
        } else {
            throw new RuntimeException("Invalid StructNamePrefixer index: " + idx);
        }
    }

    void clearIndex() {
        this.idx = -1;
    }

    String get(String name) {
        if (name == null || name.isEmpty()) {
            return name;
        }

        // stack has no prefixes, just return the plain name (no join necessary)
        name = String.join("", stack) + name;

        // Append the array index if necessary (for iterated arrays)
        if (this.idx != -1) {
            return name + idx;
        } else {
            return name;
        }
    }
}
