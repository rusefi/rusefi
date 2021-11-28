package com.rusefi.newparse.layout;

import com.rusefi.newparse.outputs.TsMetadata;
import com.rusefi.newparse.parsing.BitGroup;
import com.rusefi.newparse.parsing.EnumField;
import com.rusefi.newparse.parsing.Type;
import com.rusefi.newparse.parsing.UnusedField;

import java.io.PrintStream;
import java.util.List;
import java.util.stream.Collectors;

public class BitGroupLayout extends Layout {
    private class BitLayout {
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

    private final List<BitLayout> bits;

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
    protected void writeTunerstudioLayout(PrintStream ps, TsMetadata meta, StructNamePrefixer prefixer, int offsetAdd) {
        int actualOffset = this.offset + offsetAdd;

        for (int i = 0; i < bits.size(); i++) {
            BitLayout bit = bits.get(i);

            String name = prefixer.get(bit.name);

            ps.print(prefixer.get(bit.name));
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
    public void writeCLayout(PrintStream ps) {
        // always emit all 32 bits
        for (int i = 0; i < 32; i++) {
            ps.print("\t/**\n\t");

            if (i < bits.size()) {
                BitLayout bit = this.bits.get(i);

                if (bit.comment != null) {
                    ps.println(" * " + bit.comment.replaceAll("[+]", "").replaceAll(";", "").replace("\\n", "\n\t * "));
                    ps.print('\t');
                }

                ps.println("offset " + this.offsetWithinStruct + " bit " + i + " */");
                ps.println("\tbool " + bit.name + " : 1 {};");
            } else {
                // Force pad out all bit groups to a full 32b/4B
                ps.println("offset " + this.offsetWithinStruct + " bit " + i + " */");
                ps.println("\tbool unusedBit_" + this.offsetWithinStruct + "_" + i + " : 1 {};");
            }
        }
    }
}
