package com.rusefi.newparse.outputs;

import com.rusefi.newparse.layout.*;

import java.io.PrintStream;

public class DatalogVisitor extends ILayoutVisitor {
    private static void writeDatalogName(PrintStream ps, String name, String comment) {
        String text = (comment == null || comment.isEmpty()) ? name : comment;

        // Delete anything after a newline
        text = text.split("\\\\n")[0];

        ps.print(text);
    }

    @Override
    public void visit(StructLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        if (arrayDims.length == 0) {
            visit(struct, ps, prefixer, offsetAdd, struct.name);
        } else if (arrayDims.length == 1) {
            int elementOffset = offsetAdd;

            for (int i = 0; i < arrayDims[0]; i++) {
                visit(struct, ps, prefixer, elementOffset, struct.name + (i + 1));
                elementOffset += struct.size;
            }
        } else {
            throw new IllegalStateException("Output channels don't support multi dimension arrays");
        }
    }

    @Override
    public void visit(EnumLayout e, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        // Output an enum as a scalar, since there's no TS support for enum output channels
        // Write the datalog entry as an integer, since there's no support for enums.
        ps.print("entry = ");
        ps.print(e.name);
        ps.print(", \"");
        writeDatalogName(ps, e.name, e.options.comment);
        ps.print("\", int,    \"%d\"");
        ps.println();
    }

    private void visit(ScalarLayout scalar, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int idx) {
        if (scalar.name.startsWith("unused")) {
            return;
        }

        String nameWithoutSpace = prefixer.get(idx > 0 ? (scalar.name + idx) : scalar.name);
        String nameWithSpace = prefixer.get(idx > 0 ? (scalar.name + " " + idx) : scalar.name);

        ps.print("entry = ");
        ps.print(nameWithoutSpace);
        ps.print(", \"");

        String commentWithIndex = (idx <= 0 || scalar.options.comment.isEmpty()) ? scalar.options.comment : scalar.options.comment + " " + idx;

        writeDatalogName(ps, nameWithSpace, commentWithIndex);
        ps.print("\", ");

        if (scalar.type.tsType.equals("F32") || scalar.options.scale != 1) {
            ps.print("float,  \"%.3f\"");
        } else {
            ps.print("int,    \"%d\"");
        }

        ps.println();
    }

    @Override
    public void visit(ScalarLayout scalar, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        if (arrayDims.length == 0) {
            visit(scalar, ps, prefixer, offsetAdd, -1);
        } else if (arrayDims.length == 1) {
            int elementOffset = offsetAdd;

            for (int i = 0; i < arrayDims[0]; i++) {
                visit(scalar, ps, prefixer, elementOffset, i + 1);
                elementOffset += scalar.type.size;
            }
        } else {
            throw new IllegalStateException("Output channels don't support multi dimension arrays");
        }
    }

    @Override
    public void visit(BitGroupLayout b, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        int actualOffset = b.offset + offsetAdd;

        for (int i = 0; i < b.bits.size(); i++) {
            BitGroupLayout.BitLayout bit = b.bits.get(i);

            if (bit.name.startsWith("unused")) {
                continue;
            }

            String name = prefixer.get(bit.name);

            ps.print("entry = ");
            ps.print(name);
            ps.print(", \"");
            writeDatalogName(ps, name, bit.comment);
            ps.print("\", int,    \"%d\"");

            ps.println();
        }
    }

    @Override
    public void visit(UnionLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {

    }

    @Override
    public void visit(UnusedLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        // Do nothing
    }
}
