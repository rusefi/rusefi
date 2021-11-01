package com.rusefi.newparse.layout;

import com.rusefi.ConfigDefinition;
import com.rusefi.newparse.outputs.TsMetadata;
import com.rusefi.newparse.parsing.FieldOptions;
import com.rusefi.newparse.parsing.ScalarField;
import com.rusefi.newparse.parsing.Type;

import java.io.PrintStream;

public class ScalarLayout extends Layout {
    private String name;
    private Type type;
    private FieldOptions options;
    private boolean autoscale;

    public ScalarLayout(ScalarField field) {
        this.name = field.name;
        this.options = field.options;
        this.type = field.type;
        this.autoscale = field.autoscale;
    }

    @Override
    public int getSize() {
        return this.type.size;
    }

    @Override
    public String toString() {
        return "Scalar " + type.cType + " " + super.toString();
    }

    private void printBeforeArrayLength(PrintStream ps, TsMetadata meta, StructNamePrefixer prefixer, String fieldType, int offsetAdd) {
        String name = prefixer.get(this.name);
        ps.print(name);
        ps.print(" = " + fieldType + ", ");
        ps.print(this.type.tsType);
        ps.print(", ");
        ps.print(this.offset + offsetAdd);
        ps.print(", ");

        meta.addComment(name, this.options.comment);
    }

    private void printAfterArrayLength(PrintStream ps) {
        options.printTsFormat(ps);

        ps.println();
    }

    @Override
    protected void writeTunerstudioLayout(PrintStream ps, TsMetadata meta, StructNamePrefixer prefixer, int offsetAdd, int[] arrayLength) {
        if (arrayLength[0] == 0) {
            // Skip zero length arrays, they may be used for dynamic padding but TS doesn't like them
            return;
        } else if (arrayLength[0] == 1) {
            // For 1-length arrays, emit as a plain scalar instead
            writeTunerstudioLayout(ps, meta, prefixer, offsetAdd);
            return;
        }

        printBeforeArrayLength(ps, meta, prefixer, "array", offsetAdd);

        ps.print("[");
        ps.print(arrayLength[0]);

        for (int i = 1; i < arrayLength.length; i++) {
            if (arrayLength[i] == 1) {
                continue;
            }

            ps.print('x');
            ps.print(arrayLength[i]);
        }

        ps.print("], ");

        printAfterArrayLength(ps);
    }

    @Override
    protected void writeTunerstudioLayout(PrintStream ps, TsMetadata meta, StructNamePrefixer prefixer, int offsetAdd) {
        printBeforeArrayLength(ps, meta, prefixer, "scalar", offsetAdd);
        printAfterArrayLength(ps);
    }

    @Override
    public void writeCLayout(PrintStream ps) {
        this.writeCOffsetHeader(ps, this.options.comment, this.options.units);

        String cTypeName = this.type.cType.replaceAll("^int32_t$", "int");

        if (this.autoscale) {
            cTypeName = "scaled_channel<" + cTypeName + ", " + Math.round(1 / this.options.scale) + ">";
        }

        ps.print("\t" + cTypeName + " " + this.name);

        if (ConfigDefinition.needZeroInit) {
            ps.print(" = (" + this.type.cType.replaceAll("^int32_t$", "int") + ")0");
        }

        ps.println(";");
    }

    @Override
    public void writeCLayout(PrintStream ps, int[] arrayLength) {
        this.writeCOffsetHeader(ps, this.options.comment, this.options.units);

        StringBuilder al = new StringBuilder();

        al.append(arrayLength[0]);

        for (int i = 1; i < arrayLength.length; i++) {
            al.append("][");
            al.append(arrayLength[i]);
        }

        String cTypeName = this.type.cType.replaceAll("^int32_t$", "int");

        if (this.autoscale) {
            cTypeName = "scaled_channel<" + cTypeName + ", " + Math.round(1 / this.options.scale) + ">";
        }

        ps.println("\t" + cTypeName + " " + this.name + "[" + al + "];");
    }
}
