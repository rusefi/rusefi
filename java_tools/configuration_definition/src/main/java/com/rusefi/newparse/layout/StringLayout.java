package com.rusefi.newparse.layout;

import com.rusefi.newparse.outputs.ILayoutVisitor;
import com.rusefi.newparse.parsing.StringField;

import java.io.PrintStream;

public class StringLayout extends Layout {
    public final String name;
    public final int size;
    public final String comment;

    public StringLayout(StringField field) {
        this.name = field.name;
        this.size = field.size;
        this.comment = field.comment;
    }

    @Override
    public int getSize() {
        return this.size;
    }

    @Override
    public int getAlignment() {
        // char can be single aligned
        return 1;
    }

    @Override
    public String toString() {
        return "String " + super.toString();
    }

    @Override
    public void writeCOffsetCheck(PrintStream ps, String parentTypeName) {
        ps.print("static_assert(offsetof(");
        ps.print(parentTypeName);
        ps.print(", ");
        ps.print(this.name);
        ps.print(") == ");
        ps.print(this.offsetWithinStruct);
        ps.println(");");
    }

    @Override
    protected void doVisit(ILayoutVisitor v, PrintStream ps, StructNamePrefixer pfx, int offsetAdd, int[] arrayDims) {
        v.visit(this, ps, pfx, offsetAdd, arrayDims);
    }
}
