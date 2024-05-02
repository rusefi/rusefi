package com.rusefi.newparse.layout;

import com.rusefi.newparse.outputs.ILayoutVisitor;
import com.rusefi.newparse.parsing.EnumField;
import com.rusefi.newparse.parsing.FieldOptions;
import com.rusefi.newparse.parsing.Type;

import java.io.PrintStream;

public class EnumLayout extends Layout {
    public final String name;
    public final Type type;
    public final String enumType;
    public final int endBit;
    public final String[] values;
    public final FieldOptions options;

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

    @Override
    protected void doVisit(ILayoutVisitor v, PrintStream ps, StructNamePrefixer pfx, int offsetAdd, int[] arrayDims) {
        v.visit(this, ps, pfx, offsetAdd, arrayDims);
    }
}
