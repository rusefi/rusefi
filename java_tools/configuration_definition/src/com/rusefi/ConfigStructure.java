package com.rusefi;

import java.io.IOException;
import java.io.Writer;
import java.util.ArrayList;
import java.util.List;

/**
 * (c) Andrey Belomutskiy
 * 1/15/15
 */
public class ConfigStructure {
    public static final String UINT8_T = "uint8_t";
    public static final String UINT_16_T = "uint16_t";
    public static final String INT_16_T = "int16_t";

    private final String name;
    private final String comment;
    public final boolean withPrefix;
    /**
     * We have two different collections because if 'array iterate' feature which is handled differently
     * in C and TS
     */
    private final List<ConfigField> cFields = new ArrayList<>();
    private final List<ConfigField> tsFields = new ArrayList<>();
    private int currentOffset;
    public int totalSize;
    BitState bitState = new BitState();

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
            ConfigField fill = new ConfigField(state, "alignmentFill", "need 4 byte alignment", false,
                    "" + fillSize,
                    UINT8_T, fillSize, null, false);
            addBoth(fill);
        }
        totalSize += fillSize;
    }

    /**
     * This method writes a C header version of a data structure
     */
    public static void headerWrite(ConfigStructure configStructure, Writer cHeader) throws IOException {
        if (configStructure.comment != null) {
            cHeader.write("/**" + ConfigDefinition.EOL + ConfigDefinition.packComment(configStructure.comment, "")  + ConfigDefinition.EOL + "*/" + ConfigDefinition.EOL);
        }

        cHeader.write("// start of " + configStructure.name + ConfigDefinition.EOL);
        cHeader.write("typedef struct {" + ConfigDefinition.EOL);

        configStructure.bitState.reset();
        for (int i = 0; i < configStructure.cFields.size(); i++) {
            ConfigField cf = configStructure.cFields.get(i);
            cHeader.write(cf.getHeaderText(configStructure.currentOffset, configStructure.bitState.get()));
            ConfigField next = i == configStructure.cFields.size() - 1 ? ConfigField.VOID : configStructure.cFields.get(i + 1);

            configStructure.bitState.incrementBitIndex(cf, next);
            configStructure.currentOffset += cf.getSize(next);
        }

        cHeader.write("\t/** total size " + configStructure.currentOffset + "*/" + ConfigDefinition.EOL);
        cHeader.write("} " + configStructure.name + ";" + ConfigDefinition.EOL + ConfigDefinition.EOL);
    }

    public int writeTunerStudio(String prefix, Writer tsHeader, int tsPosition) throws IOException {
        FieldIterator fieldIterator = new FieldIterator();
        for (int i = 0; i < tsFields.size(); i++) {
            ConfigField next = i == tsFields.size() - 1 ? ConfigField.VOID : tsFields.get(i + 1);
            ConfigField cf = tsFields.get(i);
            tsPosition = cf.writeTunerStudio(prefix, tsHeader, tsPosition, next, fieldIterator.bitState.get());

            fieldIterator.bitState.incrementBitIndex(cf, next);
        }
        return tsPosition;
    }

    public int writeJavaFields(ReaderState state, String prefix, Writer javaFieldsWriter, int tsPosition) throws IOException {
        FieldIterator fieldIterator = new FieldIterator();
        for (int i = 0; i < tsFields.size(); i++) {
            ConfigField next = i == tsFields.size() - 1 ? ConfigField.VOID : tsFields.get(i + 1);
            ConfigField cf = tsFields.get(i);
            tsPosition = cf.writeJavaFields(prefix, javaFieldsWriter, tsPosition, next, fieldIterator.bitState.get());

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
