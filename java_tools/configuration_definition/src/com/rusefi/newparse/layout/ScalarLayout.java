package com.rusefi.newparse.layout;

import com.rusefi.newparse.parsing.FieldOptions;
import com.rusefi.newparse.parsing.ScalarField;
import com.rusefi.newparse.parsing.Type;

import java.io.PrintStream;

public class ScalarLayout extends Layout {
    private String name;
    private Type type;
    private FieldOptions options;

    public ScalarLayout(ScalarField field) {
        this.name = field.name;
        this.options = field.options;
        this.type = field.type;
    }

    @Override
    public int getSize() {
        return this.type.size;
    }

    @Override
    public String toString() {
        return "Scalar " + type.cType + " " + super.toString();
    }

    private void printBeforeArrayLength(PrintStream ps, StructNamePrefixer prefixer) {
        ps.print(prefixer.get(this.name));
        ps.print(" = scalar, ");
        ps.print(this.type.tsType);
        ps.print(", ");
        ps.print(this.offset);
        ps.print(", ");
    }

    private void printAfterArrayLength(PrintStream ps) {
        options.printTsFormat(ps);

        ps.println();
    }

    @Override
    public void writeTunerstudioLayout(PrintStream ps, StructNamePrefixer prefixer, int arrayLength) {
        printBeforeArrayLength(ps, prefixer);

        ps.print("[");
        ps.print(arrayLength);
        ps.print("], ");

        printAfterArrayLength(ps);
    }

    @Override
    public void writeTunerstudioLayout(PrintStream ps, StructNamePrefixer prefixer) {
        printBeforeArrayLength(ps, prefixer);
        printAfterArrayLength(ps);
    }

    @Override
    public void writeCLayout(PrintStream ps) {
        this.writeCOffsetHeader(ps, this.options.comment, this.options.units);
        ps.println("\t" + this.type.cType.replaceAll("^int32_t$", "int") + " " + this.name + ";");
    }

    @Override
    public void writeCLayout(PrintStream ps, int arrayLength) {
        this.writeCOffsetHeader(ps, this.options.comment, this.options.units);
        ps.println("\t" + this.type.cType.replaceAll("^int32_t$", "int") + " " + this.name + "[" + arrayLength + "];");
    }
}
