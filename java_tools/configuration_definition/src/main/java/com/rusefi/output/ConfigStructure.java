package com.rusefi.output;

import com.rusefi.BitState;
import com.rusefi.ConfigField;
import com.rusefi.ReaderState;
import com.rusefi.TypesHelper;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import static com.rusefi.ConfigField.BOOLEAN_T;

/**
 * Mutable representation of a list of related {@link ConfigField}
 * <p>
 * Andrey Belomutskiy, (c) 2013-2020
 * 1/15/15
 */
public class ConfigStructure {
    public static final String ALIGNMENT_FILL_AT = "alignmentFill_at_";
    public static final String UNUSED_ANYTHING_PREFIX = "unused";
    public static final String UNUSED_BIT_PREFIX = "unusedBit_";

    public final String name;
    public final String comment;
    public final boolean withPrefix;
    /**
     * We have two different collections because if 'array iterate' feature which is handled differently
     * in C and TS
     */
    public final List<ConfigField> cFields = new ArrayList<>();
    public final List<ConfigField> tsFields = new ArrayList<>();

    private int totalSize;

    public final BitState readingBitState = new BitState();

    private ConfigField cPrevField = ConfigField.VOID;
    private final Set<String> names = new HashSet<>();

    public ConfigStructure(String name, String comment, boolean withPrefix) {
        this.name = name;
        this.comment = comment;
        this.withPrefix = withPrefix;
    }

    public void addBitField(ConfigField bitField) {
        addBoth(bitField);
        this.readingBitState.incrementBitIndex(bitField);
    }

    public String getName() {
        return name;
    }

    public void addAlignmentFill(ReaderState state, int alignment) {
        if (alignment == 0)
            return;
        /**
         * we make alignment decision based on C fields since we expect iteration and non-iteration fields
         * to match in size
         */
        FieldIteratorWithOffset iterator = new FieldIteratorWithOffset(cFields) {
            @Override
            public void end() {
                super.end();
                currentOffset += cf.getSize(next);
            }
        };
        iterator.loop();

        totalSize = iterator.currentOffset;
        int fillSize = totalSize % alignment == 0 ? 0 : alignment - (totalSize % alignment);
        if (fillSize > 3)
            throw new IllegalStateException("Fill size does not look right: " + fillSize);

        if (fillSize != 0) {
            int[] fillSizeArray;
            if (fillSize != 1) {
                fillSizeArray = new int[1];
                fillSizeArray[0] = fillSize;
            } else {
                fillSizeArray = new int[0];
            }
            ConfigField fill = new ConfigField(state, ALIGNMENT_FILL_AT + totalSize, "need 4 byte alignment",
                    "" + fillSize,
                    TypesHelper.UINT8_T, fillSizeArray, "\"units\", 1, 0, -20, 100, 0", false, false, false, null, null);
            addBoth(fill);
        }
        totalSize += fillSize;
    }

    public void addBoth(ConfigField cf) {
        addC(cf);
        tsFields.add(cf);
    }

    public void addC(ConfigField cf) {
        // skip duplicate names - that's the weird use-case of conditional project definition like lambdaTable
        if (cf.getName().equals(cPrevField.getName()))
            return;

        boolean isNew = names.add(cf.getName());
        if (!isNew)
            throw new IllegalStateException(cf.getName() + " name already used");

        cFields.add(cf);

        cPrevField = cf;
    }

    public void addTs(ConfigField cf) {
        tsFields.add(cf);
    }

    public void addBitPadding(ReaderState readerState) {
        if (readingBitState.get() == 0)
            return;
        int sizeAtStartOfPadding = cFields.size();
        while (readingBitState.get() < 32) {
            ConfigField bitField = new ConfigField(readerState, UNUSED_BIT_PREFIX + sizeAtStartOfPadding + "_" + readingBitState.get(), "", null, BOOLEAN_T, new int[0], null, false, false, false, null, null);
            addBitField(bitField);
        }
        readingBitState.reset();
    }

    public int getTotalSize() {
        return totalSize;
    }
}
