package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;
import com.rusefi.config.FieldType;
import com.rusefi.tune.xml.Constant;

import javax.management.ObjectName;
import java.util.LinkedList;
import java.util.List;

public class EnumIniField extends IniField {
    private final FieldType type;
    private final List<String> enums;
    private final int bitPosition;
    private final int bitSize;

    public EnumIniField(String name, int offset, FieldType type, List<String> enums, int bitPosition, int bitSize) {
        super(name, offset);
        this.type = type;
        this.enums = enums;
        this.bitPosition = bitPosition;
        this.bitSize = bitSize;
    }

    public int getBitPosition() {
        return bitPosition;
    }

    public int getBitSize() {
        return bitSize;
    }

    public List<String> getEnums() {
        return enums;
    }

    public FieldType getType() {
        return type;
    }

    @Override
    public String getValue(ConfigurationImage image) {
        int ordinal = image.getByteBuffer(getOffset(), type.getStorageSize()).get();
        ordinal = getBitRange(ordinal, bitPosition, bitSize);

        if (ordinal >= enums.size())
            throw new IllegalStateException(ordinal + " in " + getName());
        return enums.get(ordinal);
    }

    private static boolean isQuoted(String q) {
        final int len = q.length();
        return (len >= 2 && q.charAt(0) == '"' && q.charAt(len - 1) == '"');
    }

    @Override
    public void setValue(ConfigurationImage image, Constant constant) {
        String v = constant.getValue();
        int ordinal = enums.indexOf(isQuoted(v) ? ObjectName.unquote(v) : v);
        if (ordinal == -1)
            throw new IllegalArgumentException("Not found " + v);
    }

    public static boolean getBit(int ordinal, int bitPosition) {
        return getBitRange(ordinal, bitPosition, 0) == 1;
    }

    public static int getBitRange(int ordinal, int bitPosition, int bitSize) {
        ordinal = ordinal >> bitPosition;
        ordinal = ordinal & ((1 << (bitSize + 1)) - 1);
        return ordinal;
    }

    public static EnumIniField parse(LinkedList<String> list) {
        String name = list.get(0);
        FieldType type = FieldType.parseTs(list.get(2));
        int offset = Integer.parseInt(list.get(3));

        String bitRange = list.get(4);
        bitRange = bitRange.replaceAll("[\\]\\[:]", " ").trim();
        String bitPositions[] = bitRange.split(" ");
        if (bitPositions.length != 2)
            throw new IllegalStateException("Bit position " + bitRange);
        int bitPosition = Integer.parseInt(bitPositions[0]);
        int bitSize = Integer.parseInt(bitPositions[1]) - bitPosition;

        List<String> enums = list.subList(5, list.size());

        return new EnumIniField(name, offset, type, enums, bitPosition, bitSize);
    }
}
