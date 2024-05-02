package com.rusefi.newparse.layout;

import com.rusefi.newparse.outputs.ILayoutVisitor;
import com.rusefi.newparse.parsing.*;

import java.io.PrintStream;

public class ArrayLayout extends Layout {
    public final int[] length;

    public final Layout prototypeLayout;

    public ArrayLayout(PrototypeField prototype, int[] length) {
        this.length = length;

        if (prototype instanceof ScalarField) {
            prototypeLayout = new ScalarLayout((ScalarField)prototype);
        } else if (prototype instanceof EnumField) {
            prototypeLayout = new EnumLayout((EnumField) prototype);
        } else if (prototype instanceof StringField) {
            prototypeLayout = new StringLayout((StringField) prototype);
        } else if (prototype instanceof StructField) {
            StructField structPrototype = (StructField)prototype;
            prototypeLayout = new StructLayout(0, prototype.name, structPrototype.struct);
        } else {
            throw new RuntimeException("unexpected field type during array layout");
        }
    }

    @Override
    public int getSize() {
        int size = this.prototypeLayout.getSize();

        for (int x : this.length) {
            size *= x;
        }

        return size;
    }

    @Override
    public int getAlignment() {
        // Arrays only need to be aligned on the alignment of the element, not the whole array
        return this.prototypeLayout.getAlignment();
    }

    @Override
    public void setOffset(int offset) {
        super.setOffset(offset);
        this.prototypeLayout.setOffset(offset);
    }

    @Override
    public void setOffsetWithinStruct(int offset) {
        super.setOffsetWithinStruct(offset);
        this.prototypeLayout.setOffsetWithinStruct(offset);
    }

    @Override
    public String toString() {
        return "Array of " + this.prototypeLayout + " length " + this.length[0] + " " + super.toString();
    }

    @Override
    public void writeCOffsetCheck(PrintStream ps, String parentTypeName) {
        this.prototypeLayout.writeCOffsetCheck(ps, parentTypeName);
    }

    @Override
    protected void writeSdLogLayout(PrintStream ps, StructNamePrefixer prefixer, String sourceName) {
        this.prototypeLayout.writeSdLogLayout(ps, prefixer, sourceName, this.length);
    }

    @Override
    protected void doVisit(ILayoutVisitor v, PrintStream ps, StructNamePrefixer pfx, int offsetAdd, int[] arrayDims) {
        if (arrayDims.length != 0) {
            throw new IllegalStateException("ArrayLayout got called with array dims?");
        }

        v.visit(this, ps, pfx, offsetAdd, arrayDims);
    }
}
