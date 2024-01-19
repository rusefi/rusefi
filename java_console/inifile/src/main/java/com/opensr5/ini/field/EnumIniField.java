package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;
import com.rusefi.config.FieldType;
import org.jetbrains.annotations.NotNull;

import javax.management.ObjectName;
import java.nio.ByteBuffer;
import java.util.LinkedList;
import java.util.List;

public class EnumIniField extends IniField {
    private final FieldType type;
    private final List<String> enums;
    private final int bitPosition;
    // weird format where 'one bit' width means 0 and "two bits" means "1"
    private final int bitSize0;

    public EnumIniField(String name, int offset, FieldType type, List<String> enums, int bitPosition, int bitSize0) {
        super(name, offset);
        this.type = type;
        this.enums = enums;
        this.bitPosition = bitPosition;
        this.bitSize0 = bitSize0;
    }

    @Override
    public int getSize() {
        return type.getStorageSize();
    }

    public int getBitPosition() {
        return bitPosition;
    }

    public int getBitSize0() {
        return bitSize0;
    }

    public List<String> getEnums() {
        return enums;
    }

    public FieldType getType() {
        return type;
    }

    @Override
    public String getValue(ConfigurationImage image) {
        int ordinal = getByteBuffer(image).getInt();
        ordinal = getBitRange(ordinal, bitPosition, bitSize0 + 1);

        if (ordinal >= enums.size())
            throw new IllegalStateException(ordinal + " in " + getName());
        return "\"" + enums.get(ordinal) + "\"";
    }

    @NotNull
    private ByteBuffer getByteBuffer(ConfigurationImage image) {
        return image.getByteBuffer(getOffset(), 4);
    }

    public static boolean isQuoted(String q) {
        final int len = q.length();
        return (len >= 2 && q.charAt(0) == '"' && q.charAt(len - 1) == '"');
    }

    @Override
    public String toString() {
        return "EnumIniField{" +
                "name=" + getName() +
                ", offset=" + getOffset() +
                ", type=" + type +
                ", enums=" + enums +
                ", bitPosition=" + bitPosition +
                ", bitSize=" + bitSize0 +
                '}';
    }

    public static int setBitRange(int value, int ordinal, int bitPosition, int bitSize) {
        if (ordinal >= (1 << bitSize))
            throw new IllegalArgumentException("Ordinal overflow " + ordinal + " does not fit in " + bitSize + " bit(s)");
        int num = ((1 << bitSize) - 1) << bitPosition;
        int clearBitRange = value & ~num;
        return (clearBitRange + (ordinal << bitPosition));
    }

    public static boolean getBit(int ordinal, int bitPosition) {
        return getBitRange(ordinal, bitPosition, 1) == 1;
    }

    public static int getBitRange(int ordinal, int bitPosition, int bitSize) {
        ordinal = ordinal >> bitPosition;
        ordinal = ordinal & ((1 << (bitSize)) - 1);
        return ordinal;
    }

    public static EnumIniField parse(LinkedList<String> list) {
        String name = list.get(0);
        FieldType type = FieldType.parseTs(list.get(2));
        int offset = Integer.parseInt(list.get(3));

        String bitRange = list.get(4);
        ParseBitRange parseBitRange = new ParseBitRange().invoke(bitRange);
        int bitPosition = parseBitRange.getBitPosition();
        int bitSize0 = parseBitRange.getBitSize0();

        List<String> enums = list.subList(5, list.size());

        return new EnumIniField(name, offset, type, enums, bitPosition, bitSize0);
    }

    public static class ParseBitRange {
        private int bitPosition;
        private int bitSize0;

        public int getBitPosition() {
            return bitPosition;
        }

        public int getBitSize0() {
            return bitSize0;
        }

        public ParseBitRange invoke(String bitRange) {
            bitRange = bitRange.replaceAll("[\\]\\[:]", " ").trim();
            String bitPositions[] = bitRange.split(" ");
            if (bitPositions.length != 2)
                throw new IllegalStateException("Bit position " + bitRange);
            bitPosition = Integer.parseInt(bitPositions[0]);
            bitSize0 = Integer.parseInt(bitPositions[1]) - bitPosition;
            return this;
        }
    }
}
