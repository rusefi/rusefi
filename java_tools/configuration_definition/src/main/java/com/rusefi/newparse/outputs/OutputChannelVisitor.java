package com.rusefi.newparse.outputs;

import com.rusefi.newparse.layout.*;
import com.rusefi.newparse.parsing.FieldOptions;

import java.io.PrintStream;

public class OutputChannelVisitor extends ILayoutVisitor {
    @Override
    public void visit(StructLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        if (arrayDims.length == 0) {
            visit(struct, ps, prefixer, offsetAdd, struct.name);
        } else if (arrayDims.length == 1) {
            int elementOffset = offsetAdd + struct.offset;
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
        ps.print(prefixer.get(e.name));
        ps.print(" = scalar, ");
        ps.print(e.type.tsType);
        ps.print(", ");
        ps.print(e.offset + offsetAdd);
        ps.println(", \"\", 1, 0");
    }

    @Override
    public void visit(StringLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        throw new IllegalStateException("String layout not supported for output channels");
    }

    private void visit(ScalarLayout scalar, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int idx) {
        String nameWithoutSpace = prefixer.get(idx > 0 ? (scalar.name + idx) : scalar.name);
        String nameWithSpace = prefixer.get(idx > 0 ? (scalar.name + " " + idx) : scalar.name);

        ps.print(nameWithoutSpace);
        //ps.print(" = " + fieldType + ", ");
        ps.print(" = scalar, ");
        ps.print(scalar.type.tsType);
        ps.print(", ");
        ps.print(scalar.offset + offsetAdd);
        ps.print(", ");
        ps.print(scalar.options.units);
        ps.print(", ");
        ps.print(FieldOptions.tryRound(scalar.options.scale));
        ps.print(", ");
        ps.print(FieldOptions.tryRound(scalar.options.offset));
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

            String name = prefixer.get(bit.name);
            ps.print(name);
            ps.print(" = bits, U32, ");
            ps.print(actualOffset);
            ps.print(", [");
            ps.print(i + ":" + i);

            ps.print("]");

            ps.println();
        }
    }

    @Override
    public void visit(UnionLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        throw new IllegalStateException("Output channels don't support unions");
    }

    @Override
    public void visit(UnusedLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        // Do nothing
    }
}
