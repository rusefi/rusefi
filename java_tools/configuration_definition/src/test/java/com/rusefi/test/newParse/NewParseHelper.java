package com.rusefi.test.newParse;

import com.rusefi.RusefiParseErrorStrategy;
import com.rusefi.newparse.ParseState;

import java.io.IOException;

public class NewParseHelper {
    public static ParseState parse(String input) throws IOException {
        ParseState parseState = new ParseState();
        RusefiParseErrorStrategy.parseDefinitionString(parseState.getListener(), input);
        return parseState;
    }
}
