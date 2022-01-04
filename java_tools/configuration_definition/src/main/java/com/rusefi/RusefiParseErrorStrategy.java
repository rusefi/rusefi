package com.rusefi;

import com.rusefi.generated.RusefiConfigGrammarLexer;
import com.rusefi.generated.RusefiConfigGrammarParser;
import com.rusefi.util.SystemOut;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeListener;
import org.antlr.v4.runtime.tree.ParseTreeWalker;

import java.io.FileInputStream;
import java.io.IOException;

public class RusefiParseErrorStrategy extends DefaultErrorStrategy {
    private boolean hadError = false;

    static void parseDefinitionFile(ParseTreeListener listener, String filePath) throws IOException {
        SystemOut.println("Parsing file (Antlr) " + filePath);

        CharStream in = new ANTLRInputStream(new FileInputStream(filePath));

        long start = System.nanoTime();

        RusefiConfigGrammarParser parser = new RusefiConfigGrammarParser(new CommonTokenStream(new RusefiConfigGrammarLexer(in)));

        RusefiParseErrorStrategy errorStrategy = new RusefiParseErrorStrategy();
        parser.setErrorHandler(errorStrategy);

        ParseTree tree = parser.content();
        new ParseTreeWalker().walk(listener, tree);
        double durationMs = (System.nanoTime() - start) / 1e6;

        if (errorStrategy.hadError()) {
            throw new RuntimeException("Parse failed, see error output above!");
        }

        SystemOut.println("Successfully parsed " + filePath + " in " + durationMs + "ms");
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
