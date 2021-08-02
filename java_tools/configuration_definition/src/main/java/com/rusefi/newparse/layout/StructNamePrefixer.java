package com.rusefi.newparse.layout;

import java.util.Stack;
import java.util.stream.Collectors;

public class StructNamePrefixer {
    private Stack<String> stack = new Stack<>();
    private int idx = -1;

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

        stack.push(name + "_");
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
        // stack has no prefixes, just return the plain name (no join necessary)
        name = stack.stream().collect(Collectors.joining()) + name;

        // Append the array index if necessary (for iterated arrays)
        if (this.idx != -1) {
            return name + idx;
        } else {
            return name;
        }
    }
}
