package com.rusefi.newparse.outputs;

import com.rusefi.newparse.layout.*;

import java.io.PrintStream;

public class SdLogVisitor extends ILayoutVisitor {
    private final String mSourceName;

    public SdLogVisitor(String sourceName) {
        mSourceName = sourceName;
    }

    @Override
    public void visit(StructLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        if (arrayDims.length == 0) {
            visit(struct, ps, prefixer, offsetAdd, struct.name);
        } else if (arrayDims.length == 1) {
            int elementOffset = offsetAdd;

            for (int i = 0; i < arrayDims[0]; i++) {
                visit(struct, ps, prefixer, elementOffset, struct.name + "[" + i + "]");
                elementOffset += struct.size;
            }
        } else {
            throw new IllegalStateException("Output channels don't support multi dimension arrays");
        }
    }

    private void visitScalar(ScalarLayout scalar, PrintStream ps, StructNamePrefixer prefixer, String arraySub, String commentSuffix) {
        final String prefixedName = prefixer.get(scalar.name);

        ps.print("\t{");
        ps.print(mSourceName);
        ps.print(prefixedName);
        ps.print(arraySub);
        ps.print(", \"");

        String comment = scalar.options.comment;

        // default to name in case of no comment
        if (comment == null || comment.isEmpty()) {
            comment = prefixedName;
        }

        ps.print(comment.split("\\n")[0]);
        ps.print(commentSuffix);
        ps.print("\", ");
        ps.print(scalar.options.units);
        ps.print(", ");
        ps.print(scalar.options.digits);
        ps.println("},");
    }

    @Override
    public void visit(ScalarLayout scalar, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        if (scalar.name.startsWith("unused")) {
            return;
        }

        if (arrayDims.length == 0) {
            visitScalar(scalar, ps, prefixer, "", "");
        } else if (arrayDims.length == 1) {
            for (int i = 0; i < arrayDims[0]; i++) {
                visitScalar(scalar, ps, prefixer, "[" + i + "]", " " + (i + 1));
            }
        } else {
            throw new IllegalStateException("SD log doesn't support multi dimension arrays");
        }
    }

    @Override
    public void visit(BitGroupLayout bitGroup, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {

    }
}
