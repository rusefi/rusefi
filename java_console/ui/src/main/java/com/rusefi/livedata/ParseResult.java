package com.rusefi.livedata;

import org.antlr.v4.runtime.Token;

import java.util.Collections;
import java.util.List;

public class ParseResult {
    static ParseResult VOID = new ParseResult(Collections.emptyList(), Collections.emptyList());

    private final List<Token> configTokens;
    private final List<String> brokenConditions;

    public ParseResult(List<Token> configTokens, java.util.List<String> brokenConditions) {
        this.configTokens = configTokens;
        this.brokenConditions = brokenConditions;
    }

    public List<Token> getConfigTokens() {
        return configTokens;
    }

    public List<String> geBrokenConditions() {
        return brokenConditions;
    }
}
