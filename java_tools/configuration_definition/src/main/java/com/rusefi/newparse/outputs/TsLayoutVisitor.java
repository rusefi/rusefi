package com.rusefi.newparse.outputs;

import com.rusefi.newparse.layout.*;

import java.io.PrintStream;

public class TsLayoutVisitor extends ILayoutVisitor {
    private final TsMetadata meta;

    public TsLayoutVisitor(TsMetadata meta) {
        this.meta = meta;
    }

    @Override
    public void visit(StructLayout struct, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        if (arrayDims.length != 0) {
            throw new IllegalStateException("TS layout struct array not supported");
        }

        visit(struct, ps, prefixer, offsetAdd, struct.name);
    }

    private static void writeEnumVal(PrintStream ps, String enumVal) {
        ps.print('"');
        ps.print(enumVal);
        ps.print('"');
    }

    @Override
    public void visit(EnumLayout e, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        String name = prefixer.get(e.name);
        ps.print(name);
        ps.print(" = bits, ");
        ps.print(e.type.tsType);
        ps.print(", ");
        ps.print(e.offset + offsetAdd);
        ps.print(", ");

        ps.print("[0:");
        ps.print(e.endBit);
        ps.print("], ");

        writeEnumVal(ps, e.values[0]);

        for (int i = 1; i < e.values.length; i++) {
            ps.print(", ");
            writeEnumVal(ps, e.values[i]);
        }

        ps.println();

        meta.addComment(name, e.options.comment);
    }

    @Override
    public void visit(StringLayout str, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        String name = prefixer.get(str.name);
        ps.print(name);
        ps.print(" = string, ASCII, ");
        ps.print(str.offset + offsetAdd);
        ps.print(", ");
        ps.print(str.size);

        ps.println();

        if (!str.comment.isEmpty()) {
            meta.addComment(name, str.comment);
        }
    }

    private void printBeforeArrayLength(ScalarLayout scalar, PrintStream ps, TsMetadata meta, StructNamePrefixer prefixer, String fieldType, int offsetAdd) {
        String name = prefixer.get(scalar.name);
        ps.print(name);
        ps.print(" = " + fieldType + ", ");
        ps.print(scalar.type.tsType);
        ps.print(", ");
        ps.print(scalar.offset + offsetAdd);
        ps.print(", ");

        meta.addComment(name, scalar.options.comment);
    }

    private void printAfterArrayLength(ScalarLayout scalar, PrintStream ps) {
        scalar.options.printTsFormat(ps);

        ps.println();
    }


    @Override
    public void visit(ScalarLayout scalar, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        if (arrayDims.length == 0) {
            // plain scalar, not array
            printBeforeArrayLength(scalar, ps, meta, prefixer, "scalar", offsetAdd);
            printAfterArrayLength(scalar, ps);
        } else {
            if (arrayDims[0] == 0) {
                // Skip zero length arrays, they may be used for dynamic padding but TS doesn't like them
                return;
            } else if (arrayDims[0] == 1) {
                // For 1-length arrays, emit as a plain scalar instead
                visit(scalar, ps, prefixer, offsetAdd, new int[0]);
                return;
            }

            printBeforeArrayLength(scalar, ps, meta, prefixer, "array", offsetAdd);
            ps.print("[");
            ps.print(arrayDims[0]);

            for (int i = 1; i < arrayDims.length; i++) {
                if (arrayDims[i] == 1) {
                    continue;
                }

                ps.print('x');
                ps.print(arrayDims[i]);
            }

            ps.print("], ");

            printAfterArrayLength(scalar, ps);
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

            ps.print("], " + bit.falseValue + ", " + bit.trueValue);

            ps.println();

            meta.addComment(name, bit.comment);
        }
    }

    @Override
    public void visit(UnionLayout u, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        u.children.forEach(c -> c.visit(this, ps, prefixer, offsetAdd, new int[0]));
    }

    @Override
    public void visit(UnusedLayout u, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        ps.println("; unused " + u.size + " bytes at offset " + (u.offset + offsetAdd));
    }

    @Override
    public void visit(ArrayIterateScalarLayout arr, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        offsetAdd += arr.offset;

        for (int i = 0; i < arrayDims[0]; i++) {
            // Set element's position within the array
            arr.prototypeLayout.setOffset(offsetAdd + arr.prototypeLayout.getSize() * i);

            // Put a 1-based index on the end of the name to distinguish in TS
            prefixer.setIndex(i);
            arr.prototypeLayout.visit(this, ps, prefixer, 0, new int[0]);
            prefixer.clearIndex();
        }
    }

    @Override
    public void visit(ArrayIterateStructLayout arr, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        for (int i = 0; i < arrayDims[0]; i++) {
            // Put a 1-based index on the end of the name to distinguish in TS
            prefixer.setIndex(i);
            arr.prototypeLayout.visit(this, ps, prefixer, arr.offset + offsetAdd + arr.prototypeLayout.getSize() * i, new int[0]);
            prefixer.clearIndex();
        }
    }
}
