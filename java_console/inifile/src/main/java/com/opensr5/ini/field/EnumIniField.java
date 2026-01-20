package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;
import com.rusefi.ini.reader.EnumIniReaderHelper;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.config.FieldType;
import com.rusefi.tune.xml.Constant;
import org.jetbrains.annotations.NotNull;

import javax.management.ObjectName;
import java.nio.ByteBuffer;
import java.util.*;

public class EnumIniField extends IniField {
    private final FieldType type;
    private final EnumKeyValueMap enums;
    private final int bitPosition;
    // weird format where 'one bit' width means 0 and "two bits" means "1"
    private final int bitSize0;

    public EnumIniField(String name, int offset, FieldType type, EnumKeyValueMap enums, int bitPosition, int bitSize0) {
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

    public EnumKeyValueMap getEnums() {
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
            throw new OrdinalOutOfRangeException("Ordinal out of range " + ordinal + " in " + getName() + " while " + enums.size() + " " + type);
        return "\"" + enums.get(ordinal) + "\"";
    }

    @NotNull
    private ByteBuffer getByteBuffer(ConfigurationImage image) {
        Objects.requireNonNull(image, "image enum getter");
        if (image.getSize() < getOffset() + 4)
            throw new IllegalArgumentException("OutOfBounds while " + getName() + " " + getOffset());
        return image.getByteBuffer(getOffset(), 4);
    }

    public static boolean isQuoted(String q) {
        final int len = q.length();
        return (len >= 2 && q.charAt(0) == '"' && q.charAt(len - 1) == '"');
    }

    @Override
    public void setValue(ConfigurationImage image, Constant constant) {
        String v = constant.getValue();
        int ordinal = enums.indexOf(v);
        if (ordinal == -1)
            throw new IllegalArgumentException(constant.getName() + ": Enum name not found " + v);
        int value = getByteBuffer(image).getInt();
        value = setBitRange(value, ordinal, bitPosition, bitSize0 + 1);
        getByteBuffer(image).putInt(value);
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

    public static int ordinalIndexOf(String str, String substr, int n) {
        int pos = str.indexOf(substr);
        while (--n > 0 && pos != -1)
            pos = str.indexOf(substr, pos + 1);
        return pos;
    }

    public static class EnumKeyValueMap {

        private final Map<Integer, String> keyValues;

        public EnumKeyValueMap(Map<Integer, String> keyValues) {
            this.keyValues = keyValues;
        }

        public boolean isBitField() {
            return (keyValues.size() == 2)
                && (keyValues.keySet().stream().allMatch(ordinal -> (0 <= ordinal) && (ordinal <= 1)));
        }

        public static EnumKeyValueMap valueOf(String rawText, Map<String, List<String>> defines) {
            Map<Integer, String> keyValues = new TreeMap<>();

            boolean isKeyValueSyntax = EnumIniReaderHelper.isKeyValueSyntax(rawText);
            int offset = 5;
            String[] tokens = IniFileReaderUtil.splitTokens(rawText);

            if (isKeyValueSyntax) {
                for (int i = 0; i < tokens.length - offset; i += 2) {
                    keyValues.put(Integer.valueOf(tokens[i + offset]), tokens[i + offset + 1]);
                }


            } else {
                String firstValue = tokens[offset];
                String trimmed = firstValue.trim();
                if (trimmed.startsWith("$")) {
                    String key = trimmed.substring(1);
                    List<String> elements = defines.get(key);
                    Objects.requireNonNull(elements, "Elements for " + key);
                    for (int i = 0; i < elements.size(); i++) {
                        keyValues.put(i, elements.get(i));
                    }
                } else {
                    for (int i = 0; i < tokens.length - offset; i++) {
                        keyValues.put(i, tokens[i + offset]);
                    }
                }
            }


            return new EnumKeyValueMap(keyValues);
        }

        public int size() {
            return keyValues.size();
        }

        public String get(int ordinal) {
            return keyValues.get(ordinal);
        }

        public int indexOf(String value) {
            final String valueToSearch = isQuoted(value) ? ObjectName.unquote(value) : value;
            for (Map.Entry<Integer, String> e : keyValues.entrySet()) {
                if (e.getValue().equals(valueToSearch))
                    return e.getKey();
            }
            return -1;
        }
    }

}
