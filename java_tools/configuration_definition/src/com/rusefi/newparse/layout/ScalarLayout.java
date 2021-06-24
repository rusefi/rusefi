package com.rusefi.newparse.layout;

import com.rusefi.ConfigDefinition;
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

    private void printBeforeArrayLength(PrintStream ps, StructNamePrefixer prefixer, String fieldType, int offsetAdd) {
        ps.print(prefixer.get(this.name));
        ps.print(" = " + fieldType + ", ");
        ps.print(this.type.tsType);
        ps.print(", ");
        ps.print(this.offset + offsetAdd);
        ps.print(", ");
    }

    private void printAfterArrayLength(PrintStream ps) {
        options.printTsFormat(ps);

        ps.println();
    }

    @Override
    protected void writeTunerstudioLayout(PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int arrayLength) {
        if (arrayLength == 0) {
            // Skip zero length arrays, they may be used for dynamic padding but TS doesn't like them
            return;
        } else if (arrayLength == 1) {
            // For 1-length arrays, emit as a plain scalar instead
            writeTunerstudioLayout(ps, prefixer, offsetAdd);
            return;
        }

        printBeforeArrayLength(ps, prefixer, "array", offsetAdd);

        ps.print("[");
        ps.print(arrayLength);
        ps.print("], ");

        printAfterArrayLength(ps);
    }

    @Override
    protected void writeTunerstudioLayout(PrintStream ps, StructNamePrefixer prefixer, int offsetAdd) {
        printBeforeArrayLength(ps, prefixer, "scalar", offsetAdd);
        printAfterArrayLength(ps);
    }

    @Override
    public void writeCLayout(PrintStream ps) {
        this.writeCOffsetHeader(ps, this.options.comment, this.options.units);
        ps.print("\t" + this.type.cType.replaceAll("^int32_t$", "int") + " " + this.name);

        if (ConfigDefinition.needZeroInit) {
            ps.print(" = (" + this.type.cType.replaceAll("^int32_t$", "int") + ")0");
        }

        ps.println(";");
    }

    @Override
    public void writeCLayout(PrintStream ps, int arrayLength) {
        this.writeCOffsetHeader(ps, this.options.comment, this.options.units);
        ps.println("\t" + this.type.cType.replaceAll("^int32_t$", "int") + " " + this.name + "[" + arrayLength + "];");
    }
}
