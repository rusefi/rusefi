package com.rusefi.ui.livedata;

import org.antlr.v4.runtime.ParserRuleContext;

public class Range {
    private final int start;
    private final int stop;

    public Range(int start, int stop) {
        this.start = start;
        this.stop = stop;
    }

    public Range(ParserRuleContext context) {
        this(context.start.getStartIndex(), context.stop.getStopIndex() + 1);
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
