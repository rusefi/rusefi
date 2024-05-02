package com.rusefi.newparse.outputs;

import com.rusefi.newparse.layout.*;

import java.io.PrintStream;

public abstract class ILayoutVisitor {
    protected void visit(StructLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, String name) {
        if (!struct.noPrefix) {
            prefixer.push(name);
        }

        struct.children.forEach(c -> c.visit(this, ps, prefixer, offsetAdd, new int[0]));

        if (!struct.noPrefix) {
            prefixer.pop();
        }
    }

    public void visit(StructLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        throw new IllegalStateException("StructLayout not supported");
    }

    public void visit(EnumLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        throw new IllegalStateException("EnumLayout not supported");
    }

    public void visit(StringLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        throw new IllegalStateException("StringLayout not supported");
    }

    public void visit(ScalarLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        throw new IllegalStateException("ScalarLayout not supported");
    }

    public void visit(BitGroupLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        throw new IllegalStateException("BitGroupLayout not supported");
    }

    public void visit(UnionLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        throw new IllegalStateException("UnionLayout not supported");
    }

    public void visit(UnusedLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        // throw new IllegalStateException("UnusedLayout not supported");
        // I guess we allow unusedlayout, it is unused, after all
    }

    public void visit(ArrayLayout arr, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        arr.prototypeLayout.visit(this, ps, prefixer, offsetAdd, arr.length);
    }

    public void visit(ArrayIterateStructLayout arr, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        // Default to use normal array behavior, a visitor can override if desired
        visit((ArrayLayout) arr, ps, prefixer, offsetAdd, arrayDims);
    }

    public void visit(ArrayIterateScalarLayout arr, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        // Default to use normal array behavior, a visitor can override if desired
        visit((ArrayLayout) arr, ps, prefixer, offsetAdd, arrayDims);
    }
}
