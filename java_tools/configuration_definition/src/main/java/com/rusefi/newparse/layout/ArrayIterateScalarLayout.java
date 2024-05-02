package com.rusefi.newparse.layout;

import com.rusefi.newparse.outputs.ILayoutVisitor;
import com.rusefi.newparse.parsing.*;

import java.io.PrintStream;

public class ArrayIterateScalarLayout extends ArrayLayout {
    public ArrayIterateScalarLayout(PrototypeField prototype, int[] length) {
        super(prototype, length);
    }

    @Override
    protected void doVisit(ILayoutVisitor v, PrintStream ps, StructNamePrefixer pfx, int offsetAdd, int[] arrayDims) {
        if (arrayDims.length != 0) {
            throw new IllegalStateException("ArrayIterateScalarLayout got called with array dims?");
        }

        v.visit(this, ps, pfx, offsetAdd, this.length);
    }
}
