package com.rusefi.newparse.parsing;

import java.util.ArrayList;
import java.util.List;

public class BitGroup implements Field {
    public final List<BitField> bitFields = new ArrayList<>();

    public void addBitField(BitField b) {
        bitFields.add(b);
    }

    @Override
    public String toString() {
        return "BitGroup: " + bitFields.size() + " bits";
    }
}
