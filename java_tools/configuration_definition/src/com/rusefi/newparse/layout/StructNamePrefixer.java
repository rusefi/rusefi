package com.rusefi.newparse.layout;

import java.util.Stack;
import java.util.stream.Collectors;

public class StructNamePrefixer {
    private Stack<String> stack = new Stack<>();

    public void pop() {
        stack.pop();
    }

    public void push(String name) {
        stack.push(name + "_");
    }

    private String suffix = new String();

    public void setSuffix(String suffix) {
        this.suffix = suffix;
    }

    public void resetSuffix() {
        this.suffix = new String();
    }

    String get(String name) {
        return stack.stream().collect(Collectors.joining()) + name + suffix;
    }
}
