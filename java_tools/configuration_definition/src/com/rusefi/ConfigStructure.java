package com.rusefi;

import com.rusefi.output.TSProjectConsumer;

import java.io.IOException;
import java.io.Writer;
import java.util.ArrayList;
import java.util.List;

/**
 * Mutable representation of a list of related {@link ConfigField}
 *
 * (c) Andrey Belomutskiy
 * 1/15/15
 */
public class ConfigStructure {

    public final String name;
    public final String comment;
    public final boolean withPrefix;
    /**
     * We have two different collections because if 'array iterate' feature which is handled differently
     * in C and TS
     */
    public final List<ConfigField> cFields = new ArrayList<>();
    public final List<ConfigField> tsFields = new ArrayList<>();

    public int currentOffset;
    public int totalSize;
    public BitState bitState = new BitState();

    public ConfigStructure(String name, String comment, boolean withPrefix) {
        this.name = name;
        this.comment = comment;
        this.withPrefix = withPrefix;
    }

    public String getName() {
        return name;
    }

    public void addAlignmentFill(ReaderState state) {
        bitState.reset();
        /**
         * we make alignment decision based on C fields since we expect interation and non-iteration fields
         * to match in size
         */
        for (int i = 0; i < cFields.size(); i++) {
            ConfigField cf = cFields.get(i);
            ConfigField next = i == cFields.size() - 1 ? ConfigField.VOID : cFields.get(i + 1);
            bitState.incrementBitIndex(cf, next);
            totalSize += cf.getSize(next);
        }

        int fillSize = totalSize % 4 == 0 ? 0 : 4 - (totalSize % 4);

        if (fillSize != 0) {
            ConfigField fill = new ConfigField(state, "alignmentFill", "need 4 byte alignment",
                    "" + fillSize,
                    TypesHelper.UINT8_T, fillSize, null, false);
            addBoth(fill);
        }
        totalSize += fillSize;
    }

    public int writeTunerStudio(String prefix, Writer tsHeader, int tsPosition) throws IOException {
        FieldIterator fieldIterator = new FieldIterator();
        for (int i = 0; i < tsFields.size(); i++) {
            ConfigField next = i == tsFields.size() - 1 ? ConfigField.VOID : tsFields.get(i + 1);
            ConfigField cf = tsFields.get(i);
            tsPosition = TSProjectConsumer.writeTunerStudio(cf, prefix, tsHeader, tsPosition, next, fieldIterator.bitState.get());

            fieldIterator.bitState.incrementBitIndex(cf, next);
        }
        return tsPosition;
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
}
