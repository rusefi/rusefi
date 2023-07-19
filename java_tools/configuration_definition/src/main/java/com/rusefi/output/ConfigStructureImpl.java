package com.rusefi.output;

import com.rusefi.*;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import static com.rusefi.ConfigFieldImpl.BOOLEAN_T;

/**
 * Mutable representation of a list of related {@link ConfigFieldImpl}
 * <p>
 * Andrey Belomutskiy, (c) 2013-2020
 * 1/15/15
 */
public class ConfigStructureImpl implements ConfigStructure {
    public static final String ALIGNMENT_FILL_AT = "alignmentFill_at_";
    public static final String UNUSED_BIT_PREFIX = "unusedBit_";

    private final String name;
    private final String comment;
    private final boolean withPrefix;
    private final List<ConfigField> cFields = new ArrayList<>();
    private final List<ConfigField> tsFields = new ArrayList<>();

    private int totalSize;

    private final BitState readingBitState = new BitState();

    private ConfigField cPrevField = ConfigFieldImpl.VOID;
    private final Set<String> names = new HashSet<>();

    public ConfigStructureImpl(String name, String comment, boolean withPrefix) {
        this.name = name;
        this.comment = comment;
        this.withPrefix = withPrefix;
    }

    public void addBitField(ConfigFieldImpl bitField) {
        addBoth(bitField);
        this.readingBitState.incrementBitIndex(bitField);
    }

    @Override
    public String getName() {
        return name;
    }

    public void addAlignmentFill(ReaderStateImpl state, int alignment) {
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
            ConfigFieldImpl fill = new ConfigFieldImpl(state, ALIGNMENT_FILL_AT + totalSize, "",
                    "" + fillSize,
                    TypesHelper.UINT8_T, fillSizeArray, "\"\", 1, 0, -20, 100, 0", false, false, false, null, null);
            addBoth(fill);
        }
        totalSize += fillSize;
    }

    public void addBoth(ConfigFieldImpl cf) {
        addC(cf);
        tsFields.add(cf);
    }

    public void addC(ConfigFieldImpl cf) {
        // skip duplicate names - that's the weird use-case of conditional project definition like lambdaTable
        if (cf.getName().equals(cPrevField.getName()))
            return;

        boolean isNew = names.add(cf.getName());
        if (!isNew)
            throw new IllegalStateException(cf.getName() + " name already used");

        cFields.add(cf);

        cPrevField = cf;
    }

    public void addTs(ConfigFieldImpl cf) {
        tsFields.add(cf);
    }

    public void addBitPadding(ReaderStateImpl readerState) {
        if (readingBitState.get() == 0)
            return;
        int sizeAtStartOfPadding = cFields.size();
        while (readingBitState.get() < 32) {
            ConfigFieldImpl bitField = new ConfigFieldImpl(readerState, UNUSED_BIT_PREFIX + sizeAtStartOfPadding + "_" + readingBitState.get(), "", null, BOOLEAN_T, new int[0], null, false, false, false, null, null);
            addBitField(bitField);
        }
        readingBitState.reset();
    }

    @Override
    public int getTotalSize() {
        return totalSize;
    }

    @Override
    public List<ConfigField> getTsFields() {
        return tsFields;
    }

    /**
     * We have two different collections because if 'array iterate' feature which is handled differently
     * in C and TS
     */
    @Override
    public List<ConfigField> getcFields() {
        return cFields;
    }

    @Override
    public boolean isWithPrefix() {
        return withPrefix;
    }

    @Override
    public String getComment() {
        return comment;
    }
}
