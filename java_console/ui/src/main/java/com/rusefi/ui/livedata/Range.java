package com.rusefi.ui.livedata;

import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.Token;

public class Range {
    private final int start;
    private final int stop;

    public Range(int start, int stop) {
        if (start < 0)
            throw new IllegalArgumentException("Negative start " + start);
        if (stop < 0)
            throw new IllegalArgumentException("Negative stop " + stop);
        this.start = start;
        this.stop = stop;
        if (getLength() < 0)
            throw new IllegalArgumentException("Negative length " + start + "/" + stop);
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

    @Override
    public String toString() {
        return "Range{" +
                "start=" + start +
                ", stop=" + stop +
                '}';
    }
}
