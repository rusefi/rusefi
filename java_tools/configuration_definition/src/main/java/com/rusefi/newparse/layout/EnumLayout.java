package com.rusefi.newparse.layout;

import com.rusefi.newparse.outputs.TsMetadata;
import com.rusefi.newparse.parsing.EnumField;
import com.rusefi.newparse.parsing.FieldOptions;
import com.rusefi.newparse.parsing.Type;

import java.io.PrintStream;

public class EnumLayout extends Layout {
    private final String name;
    private final Type type;
    private final String enumType;
    private final int endBit;
    private final String[] values;
    private final FieldOptions options;

    public EnumLayout(EnumField field) {
        this.name = field.name;
        this.type = field.type;
        this.enumType = field.enumType;
        this.endBit = field.endBit;
        this.values = field.values;
        this.options = field.options;
    }

    @Override
    public int getSize() {
        return this.type.size;
    }

    private static void writeEnumVal(PrintStream ps, String enumVal) {
        ps.print('"');
        ps.print(enumVal);
        ps.print('"');
    }

    @Override
    protected void writeTunerstudioLayout(PrintStream ps, TsMetadata meta, StructNamePrefixer prefixer, int offsetAdd) {
        String name = prefixer.get(this.name);
        ps.print(name);
        ps.print(" = bits, ");
        ps.print(this.type.tsType);
        ps.print(", ");
        ps.print(this.offset + offsetAdd);
        ps.print(", ");

        ps.print("[0:");
        ps.print(this.endBit);
        ps.print("], ");

        writeEnumVal(ps, this.values[0]);

        for (int i = 1; i < this.values.length; i++) {
            ps.print(", ");
            writeEnumVal(ps, this.values[i]);
        }

        ps.println();

        meta.addComment(name, this.options.comment);
    }

    @Override
    public void writeCLayout(PrintStream ps) {
        this.writeCOffsetHeader(ps, this.options.comment, this.options.units);
        ps.println("\t" + this.enumType + " " + this.name + ";");
    }

    @Override
    public void writeCLayout(PrintStream ps, int[] arrayLength) {
        this.writeCOffsetHeader(ps, this.options.comment, this.options.units);
        ps.println("\t" + this.enumType + " " + this.name + "[" + arrayLength[0] + "];");
    }

    @Override
    protected void writeOutputChannelLayout(PrintStream ps, StructNamePrefixer prefixer, int offsetAdd) {
        // Output an enum as a scalar, since there's no TS support for enum output channels
        ps.print(prefixer.get(name));
        ps.print(" = scalar, ");
        ps.print(this.type.tsType);
        ps.print(", ");
        ps.print(this.offset + offsetAdd);
        ps.println(", \"\", 1, 0");
    }
}
