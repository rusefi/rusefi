package com.rusefi;

import com.rusefi.generated.RusefiConfigGrammarLexer;
import com.rusefi.generated.RusefiConfigGrammarParser;
import com.rusefi.util.SystemOut;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeListener;
import org.antlr.v4.runtime.tree.ParseTreeWalker;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class RusefiParseErrorStrategy extends DefaultErrorStrategy {
    private boolean hadError = false;

    public static void parseDefinitionFile(ParseTreeListener listener, String filePath) throws IOException {
        SystemOut.println("Parsing file (Antlr) " + filePath);

        CharStream in = new ANTLRInputStream(Files.newInputStream(Paths.get(filePath)));

        long start = System.nanoTime();
        parse(listener, in);
        double durationMs = (System.nanoTime() - start) / 1e6;

        SystemOut.println("Successfully parsed (Antlr) " + filePath + " in " + durationMs + "ms");
    }

    public static void parseDefinitionString(ParseTreeListener listener, String content) {
        SystemOut.println("Parsing string (Antlr)");

        CharStream in = new ANTLRInputStream(content);

        long start = System.nanoTime();
        parse(listener, in);
        double durationMs = (System.nanoTime() - start) / 1e6;

        SystemOut.println("Successfully parsed (Antlr) in " + durationMs + "ms");
    }

    private static void parse(ParseTreeListener listener, CharStream in) {
        RusefiConfigGrammarParser parser = new RusefiConfigGrammarParser(new CommonTokenStream(new RusefiConfigGrammarLexer(in)));

        RusefiParseErrorStrategy errorStrategy = new RusefiParseErrorStrategy();
        parser.setErrorHandler(errorStrategy);

        ParseTree tree = parser.content();
        new ParseTreeWalker().walk(listener, tree);

        if (errorStrategy.hadError()) {
            throw new RuntimeException("Parse failed, see error output above!");
        }
    }

    public boolean hadError() {
        return this.hadError;
    }

    @Override
    public void recover(Parser recognizer, RecognitionException e) {
        this.hadError = true;

        super.recover(recognizer, e);
    }

    @Override
    public Token recoverInline(Parser recognizer) throws RecognitionException {
        this.hadError = true;

        return super.recoverInline(recognizer);
    }
}
