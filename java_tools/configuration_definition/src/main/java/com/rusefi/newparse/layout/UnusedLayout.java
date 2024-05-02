package com.rusefi.newparse.layout;

import com.rusefi.newparse.outputs.ILayoutVisitor;
import com.rusefi.newparse.parsing.UnusedField;

import java.io.PrintStream;

public class UnusedLayout extends Layout {
    public final int size;

    public UnusedLayout(int size) {
        this.size = size;
    }

    public UnusedLayout(UnusedField field) {
        this.size = field.size;
    }

    @Override
    public int getSize() {
        return this.size;
    }

    @Override
    public int getAlignment() {
        return 1;
    }

    @Override
    public String toString() {
        return "Unused " + super.toString();
    }

    @Override
    protected void doVisit(ILayoutVisitor v, PrintStream ps, StructNamePrefixer pfx, int offsetAdd, int[] arrayDims) {
        v.visit(this, ps, pfx, offsetAdd, arrayDims);
    }
}
