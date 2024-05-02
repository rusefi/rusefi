package com.rusefi.newparse.layout;

import com.rusefi.newparse.outputs.ILayoutVisitor;
import com.rusefi.newparse.parsing.FieldOptions;
import com.rusefi.newparse.parsing.ScalarField;
import com.rusefi.newparse.parsing.Type;

import java.io.PrintStream;

public class ScalarLayout extends Layout {
    public final String name;
    public final Type type;
    public final FieldOptions options;
    public final boolean autoscale;

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

    public String makeScaleString() {
        double scale = this.options.scale;

        long mul, div;

        if (scale < 1) {
            mul = Math.round(1 / scale);
            div = 1;
        } else {
            mul = 1;
            div = Math.round(scale);
        }

        double actualScale = (double)mul / div;

        if (mul < 1 || div < 1 || (Math.abs(scale - actualScale) < 0.0001)) {
            throw new RuntimeException("assertion failure: scale string generation failure for " + this.name + " mul " + mul + " div " + div);
        }

        return mul + ", " + div;
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
    protected void writeSdLogLayout(PrintStream ps, StructNamePrefixer prefixer, String sourceName) {
        writeSdLogLayout(ps, prefixer, "", "", sourceName);
    }

    @Override
    protected void writeSdLogLayout(PrintStream ps, StructNamePrefixer prefixer, String sourceName, int[] arrayLength) {
        if (arrayLength.length != 1) {
            throw new IllegalStateException("Output channels don't support multi dimension arrays");
        }

        for (int i = 0; i < arrayLength[0]; i++) {
            writeSdLogLayout(ps, prefixer, "[" + i + "]", " " + (i + 1), sourceName);
        }
    }

    private void writeSdLogLayout(PrintStream ps, StructNamePrefixer prefixer, String arraySub, String commentSuffix, String sourceName) {
        if (this.name.startsWith("unused")) {
            return;
        }
        
        // {engine->outputChannels.mafMeasured, "MAF", "kg/h", 1},

        final String prefixedName = prefixer.get(this.name);

        ps.print("\t{");
        ps.print(sourceName);
        ps.print(prefixedName);
        ps.print(arraySub);
        ps.print(", \"");

        String comment = this.options.comment;

        // default to name in case of no comment
        if (comment == null || comment.length() == 0) {
            comment = prefixedName;
        }

        ps.print(comment);
        ps.print(commentSuffix);
        ps.print("\", ");
        ps.print(this.options.units);
        ps.print(", ");
        ps.print(this.options.digits);
        ps.println("},");
    }

    @Override
    protected void doVisit(ILayoutVisitor v, PrintStream ps, StructNamePrefixer pfx, int offsetAdd, int[] arrayDims) {
        v.visit(this, ps, pfx, offsetAdd, arrayDims);
    }
}
