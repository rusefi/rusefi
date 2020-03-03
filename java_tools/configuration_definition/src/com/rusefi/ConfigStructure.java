package com.rusefi;

import java.util.ArrayList;
import java.util.List;

import static com.rusefi.ConfigField.BOOLEAN_T;

/**
 * Mutable representation of a list of related {@link ConfigField}
 * <p>
 * (c) Andrey Belomutskiy
 * 1/15/15
 */
public class ConfigStructure {

    public final String name;
    public final String comment;
    public final boolean withPrefix;
    private final boolean withConstructor;
    /**
     * We have two different collections because if 'array iterate' feature which is handled differently
     * in C and TS
     */
    public final List<ConfigField> cFields = new ArrayList<>();
    public final List<ConfigField> tsFields = new ArrayList<>();

    public int totalSize;

    public final BitState readingBitState = new BitState();

    public ConfigStructure(String name, String comment, boolean withPrefix, boolean withConstructor) {
        this.name = name;
        this.comment = comment;
        this.withPrefix = withPrefix;
        this.withConstructor = withConstructor;
    }

    void addBitField(ConfigField bitField) {
        addBoth(bitField);
        this.readingBitState.incrementBitIndex(bitField);
    }

    public boolean isWithConstructor() {
        return withConstructor;
    }

    public String getName() {
        return name;
    }

    public void addAlignmentFill(ReaderState state) {
        BitState bitState = new BitState();
        /**
         * we make alignment decision based on C fields since we expect iteration and non-iteration fields
         * to match in size
         */
        totalSize = 0;
        for (int i = 0; i < cFields.size(); i++) {
            ConfigField cf = cFields.get(i);
            ConfigField next = i == cFields.size() - 1 ? ConfigField.VOID : cFields.get(i + 1);
            bitState.incrementBitIndex(cf, next);
            totalSize += cf.getSize(next);
        }

        int fillSize = totalSize % 4 == 0 ? 0 : 4 - (totalSize % 4);

        if (fillSize != 0) {
            ConfigField fill = new ConfigField(state, "alignmentFill_at_" + totalSize, "need 4 byte alignment",
                    "" + fillSize,
                    TypesHelper.UINT8_T, fillSize, null, false, false, null, -1, null, null);
            addBoth(fill);
        }
        totalSize += fillSize;
    }

    public void addBoth(ConfigField cf) {
        cFields.add(cf);
        tsFields.add(cf);
    }

    public void addC(ConfigField cf) {
        cFields.add(cf);
    }

    public void addTs(ConfigField cf) {
        tsFields.add(cf);
    }

    public void addBitPadding(ReaderState readerState) {
        if (readingBitState.get() == 0)
            return;
        int sizeAtStartOfPadding = cFields.size();
        while (readingBitState.get() < 32) {
            ConfigField bitField = new ConfigField(readerState, "unusedBit_" + sizeAtStartOfPadding + "_" + readingBitState.get(), "", null, BOOLEAN_T, 0, null, false, false, null, -1, null, null);
            addBitField(bitField);
        }
        readingBitState.reset();
    }
}
