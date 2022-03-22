package com.rusefi.livedata;

import com.rusefi.livedata.generated.CPP14Parser;
import org.antlr.v4.runtime.Token;

import java.util.Collections;
import java.util.List;

public class ParseResult {
    static ParseResult VOID = new ParseResult(Collections.emptyList(), Collections.emptyList(), Collections.emptyList());

    private final List<Token> configTokens;
    private final List<String> brokenConditions;
    private final List<CPP14Parser.UnqualifiedIdContext> functions;

    public ParseResult(List<Token> configTokens, List<String> brokenConditions, List<CPP14Parser.UnqualifiedIdContext> functions) {
        this.configTokens = configTokens;
        this.brokenConditions = brokenConditions;
        this.functions = functions;
    }

    public List<Token> getConfigTokens() {
        return configTokens;
    }

    public List<String> geBrokenConditions() {
        return brokenConditions;
    }

    public List<CPP14Parser.UnqualifiedIdContext> getFunctions() {
        return functions;
    }
}
