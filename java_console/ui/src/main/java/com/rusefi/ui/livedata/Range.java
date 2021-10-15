package com.rusefi.ui.livedata;

import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.Token;

public class Range {
    private final int start;
    private final int stop;

    public Range(int start, int stop) {
        this.start = start;
        this.stop = stop;
    }

    public Range(Token start, Token stop) {
        this(start.getStartIndex(), stop.getStopIndex() + 1);
    }


    public Range(ParserRuleContext context) {
        this(context.start, context.stop);
    }

    public int getStart() {
        return start;
    }

    public int getStop() {
        return stop;
    }

    public int getLength() {
        return stop - start;
    }
}
