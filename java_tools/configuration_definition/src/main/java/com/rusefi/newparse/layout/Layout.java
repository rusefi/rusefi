package com.rusefi.newparse.layout;

import com.rusefi.newparse.outputs.ILayoutVisitor;

import java.io.PrintStream;

public abstract class Layout {
    public int offset = -1;
    public int offsetWithinStruct = -1;

    public abstract int getSize();
    public int getAlignment() {
        // Default to size
        return this.getSize();
    }

    public void setOffset(int offset) {
        this.offset = offset;
    }

    public void setOffsetWithinStruct(int offset) {
        offsetWithinStruct = offset;
    }

    @Override
    public String toString() {
        return "offset = " + offset + " size = " + this.getSize();
    }

    public void writeCOffsetHeader(PrintStream ps, String comment, String units) {
        if (comment != null) {
            comment = comment.replaceAll("[+]", "");
            comment = comment.replaceAll("\\n", "\n\t// ");
            if (comment.length() == 0) {
                comment = null;
            }
        }

        if (comment != null) {
            comment = comment.replaceAll("\\\\n", "\n\t// ");

            ps.println("\t// " + comment);
        }

        if (units != null && units.length() > 2) {
            ps.println("\t// " + units.replace("\"", ""));
        }

        ps.println("\t// offset " + this.offsetWithinStruct);
    }

    public void writeCOffsetCheck(PrintStream ps, String parentTypeName) { }

    protected void doVisit(ILayoutVisitor v, PrintStream ps, StructNamePrefixer pfx, int offsetAdd, int[] arrayDims)
    {
        throw new IllegalStateException("This type is missing its visitor");
    }

    public void visit(ILayoutVisitor v, PrintStream ps, StructNamePrefixer prefixer, int offsetAdd, int[] arrayDims) {
        doVisit(v, ps, prefixer, offsetAdd, arrayDims);
    }
}
