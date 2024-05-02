package com.rusefi.newparse.layout;

import com.rusefi.newparse.outputs.ILayoutVisitor;
import com.rusefi.newparse.parsing.BitGroup;

import java.io.PrintStream;
import java.util.List;
import java.util.stream.Collectors;

public class BitGroupLayout extends Layout {
    public static class BitLayout {
        public final String name;
        public final String comment;
        public final String trueValue;
        public final String falseValue;

        public BitLayout(String name, String comment, String trueValue, String falseValue) {
            this.name = name;
            this.comment = comment;
            this.trueValue = trueValue;
            this.falseValue = falseValue;
        }
    }

    public final List<BitLayout> bits;

    public BitGroupLayout(BitGroup bitGroup) {
        int size = bitGroup.bitFields.size();
        if (size > 32) {
            throw new RuntimeException("tried to create bit group starting with " + bitGroup.bitFields.get(0).name + " but it contained " + size + " which is more than the maximum of 32.");
        }

        this.bits = bitGroup.bitFields.stream().map(bf -> new BitLayout(bf.name, bf.comment, bf.trueValue, bf.falseValue)).collect(Collectors.toList());
    }

    @Override
    public int getSize() {
        return 4;
    }

    @Override
    public String toString() {
        return "Bit group " + super.toString();
    }

    @Override
    protected void doVisit(ILayoutVisitor v, PrintStream ps, StructNamePrefixer pfx, int offsetAdd, int[] arrayDims) {
        v.visit(this, ps, pfx, offsetAdd, arrayDims);
    }
}
