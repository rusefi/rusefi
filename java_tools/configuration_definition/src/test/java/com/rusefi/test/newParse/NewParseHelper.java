package com.rusefi.test.newParse;

import com.rusefi.RusefiParseErrorStrategy;
import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.outputs.CStructWriter;
import com.rusefi.newparse.outputs.TsWriter;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.nio.charset.StandardCharsets;

public class NewParseHelper {
    public static ParseState parse(String input) throws IOException {
        ParseState parseState = new ParseState();
        RusefiParseErrorStrategy.parseDefinitionString(parseState.getListener(), input);
        return parseState;
    }

    public static String parseToTs(String input) throws IOException {
        ParseState state = parse(input);

        TsWriter writer = new TsWriter();

        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        final String utf8 = StandardCharsets.UTF_8.name();

        PrintStream ps = new PrintStream(baos, true, utf8);

        writer.writeLayoutAndComments(state, ps);

        return baos.toString(utf8);
    }

    public static String parseToC(String input) throws IOException {
        ParseState state = parse(input);

        CStructWriter writer = new CStructWriter();

        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        final String utf8 = StandardCharsets.UTF_8.name();

        PrintStream ps = new PrintStream(baos, true, utf8);

        writer.writeCStructs(state, ps);

        return baos.toString(utf8);
    }
}
