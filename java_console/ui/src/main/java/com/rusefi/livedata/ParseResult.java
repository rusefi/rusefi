package com.rusefi.livedata;

import org.antlr.v4.runtime.Token;

import java.util.Collections;
import java.util.List;

public class ParseResult {
    static ParseResult VOID = new ParseResult(Collections.emptyList());

    private List<Token> configTokens;

    public ParseResult(List<Token> configTokens) {
        this.configTokens = configTokens;
    }

    public List<Token> getConfigTokens() {
        return configTokens;
    }
}
