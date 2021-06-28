package com.rusefi.newparse.layout;

import com.rusefi.newparse.outputs.TsMetadata;
import com.rusefi.newparse.parsing.EnumField;
import com.rusefi.newparse.parsing.Type;
import com.rusefi.newparse.parsing.UnusedField;

import java.io.PrintStream;
import java.util.Random;

public class UnusedLayout extends Layout {
    private final int size;

    public UnusedLayout(int size) {
        this.size = size;
    }

    public UnusedLayout(UnusedField field) {
        this.size = field.size;
    }

    @Override
    public int getSize() {
        return this.size;
    }

    @Override
    public int getAlignment() {
        return 1;
    }

    @Override
    public String toString() {
        return "Unused " + super.toString();
    }

    @Override
    protected void writeTunerstudioLayout(PrintStream ps, TsMetadata meta, StructNamePrefixer prefixer, int offsetAdd) {
        ps.println("; unused " + this.size + " bytes at offset " + (this.offset + offsetAdd));
    }

    @Override
    public void writeCLayout(PrintStream ps) {
        this.writeCOffsetHeader(ps, null, null);
        ps.println("\tchar unused" + this.offsetWithinStruct + "[" + this.size + "];");
    }
}
