package com.rusefi.newparse.outputs;

import com.rusefi.ConfigDefinition;
import com.rusefi.newparse.layout.*;

import java.io.PrintStream;

public class CStructsVisitor extends ILayoutVisitor {
    public void visitRoot(StructLayout sl, PrintStream ps) {
        ps.println("struct " + sl.typeName + " {");

        sl.children.forEach(c -> c.visit(this, ps, null, 0, new int[0]));

        ps.println("};");

        // Emit an assertion for the size of the whole thing
        ps.println("static_assert(sizeof(" + sl.typeName + ") == " + sl.getSize() + ");");

        // Emit assertions to check that the offset of each child is correct according to the C++ compiler
        sl.children.forEach(c -> c.writeCOffsetCheck(ps, sl.typeName));

        ps.println();
    }

    public void visit(StructLayout sl, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        sl.writeCOffsetHeader(ps, null, null);

        if (arrayDims.length == 0) {
            ps.println("\t" + sl.typeName + " " + sl.name + ";");
        } else if (arrayDims.length == 1) {
            ps.println("\t" + sl.typeName + " " + sl.name + "[" + arrayDims[0] + "];");
        } else {
            throw new IllegalStateException("Multi dim array of structs not supported");
        }
    }

    @Override
    public void visit(ScalarLayout scalar, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        scalar.writeCOffsetHeader(ps, scalar.options.comment, scalar.options.units);

        String cTypeName = scalar.type.cType.replaceAll("^int32_t$", "int");

        if (scalar.autoscale) {
            cTypeName = "scaled_channel<" + cTypeName + ", " + scalar.makeScaleString() + ">";
        }

        ps.print("\t" + cTypeName + " " + scalar.name);

        if (arrayDims.length > 0) {
            StringBuilder al = new StringBuilder();

            al.append('[');
            al.append(arrayDims[0]);

            for (int i = 1; i < arrayDims.length; i++) {
                al.append("][");
                al.append(arrayDims[i]);
            }

            al.append(']');

            ps.print(al);
        } else {
            if (ConfigDefinition.needZeroInit) {
                ps.print(" = (" + scalar.type.cType.replaceAll("^int32_t$", "int") + ")0");
            }
        }

        ps.println(";");
    }

    @Override
    public void visit(BitGroupLayout bg, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        // always emit all 32 bits
        for (int i = 0; i < 32; i++) {
            if (i < bg.bits.size()) {
                BitGroupLayout.BitLayout bit = bg.bits.get(i);

                if (bit.comment != null) {
                    ps.println("\t// " + bit.comment.replaceAll("[+]", "").replaceAll(";", "").replace("\\n", "\n\t// "));
                }

                ps.println("\t// offset " + bg.offsetWithinStruct + " bit " + i);
                ps.println("\tbool " + bit.name + " : 1 {};");
            } else {
                // Force pad out all bit groups to a full 32b/4B
                ps.println("\t// offset " + bg.offsetWithinStruct + " bit " + i);
                ps.println("\tbool unusedBit_" + bg.offsetWithinStruct + "_" + i + " : 1 {};");
            }
        }
    }

    @Override
    public void visit(EnumLayout e, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        e.writeCOffsetHeader(ps, e.options.comment, e.options.units);

        if (arrayDims.length == 0) {
            ps.println("\t" + e.enumType + " " + e.name + ";");
        } else if (arrayDims.length == 1) {
            ps.println("\t" + e.enumType + " " + e.name + "[" + arrayDims[0] + "];");
        } else {
            throw new IllegalStateException("Multi dim array of enums not supported");
        }
    }

    @Override
    public void visit(UnusedLayout u, PrintStream ps, StructNamePrefixer prefixer, int offestAdd, int[] arrayDims) {
        u.writeCOffsetHeader(ps, null, null);
        ps.println("\tuint8_t alignmentFill_at_" + u.offsetWithinStruct + "[" + u.size + "];");
    }

    @Override
    public void visit(StringLayout str, PrintStream ps, StructNamePrefixer prefixer, int offestAdd, int[] arrayDims) {
        str.writeCOffsetHeader(ps, str.comment, null);

        if (arrayDims.length == 0) {
            ps.println("\tchar " + str.name + "[" + str.size + "];");
        } else if (arrayDims.length == 1) {
            ps.println("\tchar " + str.name + "[" + arrayDims[0] + "][" + str.size + "];");
        } else {
            throw new IllegalStateException("Multi dim array of strings not supported");
        }
    }

    @Override
    public void visit(UnionLayout u, PrintStream ps, StructNamePrefixer prefixer, int offestAdd, int[] arrayDims) {
        u.writeCOffsetHeader(ps, "union size " + u.getSize() + ", " + u.children.size() + " members", null);

        // emit an anonymous union that contains all our members
        ps.println("\tunion {");
        u.children.forEach(c -> c.visit(this, ps, prefixer, offestAdd, new int[0]));
        ps.println("\t};");
    }

    @Override
    public void visit(ArrayLayout arr, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        // Skip zero length arrays, they may be used for padding
        if (arrayDims[0] > 0) {
            super.visit(arr, ps, prefixer, offsetAdd, arrayDims);
        }
    }
}
