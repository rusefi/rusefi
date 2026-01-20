package com.rusefi.ini.reader;

import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.config.FieldType;
import org.jetbrains.annotations.NotNull;

import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.regex.Pattern;

public class EnumIniReaderHelper {
    private static final String STARTS_WITH_NUMBERS_OPTIONAL_SPACES_AND_EQUALS = "^\\d+\\s*=.*";
    private static final Pattern IS_KEY_VALUE_SYNTAX = Pattern.compile(STARTS_WITH_NUMBERS_OPTIONAL_SPACES_AND_EQUALS);

    private EnumIniReaderHelper() {
    }

    public static EnumIniField parse(LinkedList<String> list, RawIniFile.Line line, Map<String, List<String>> defines) {
        String name = list.get(0);
        FieldType type = FieldType.parseTs(list.get(2));
        int offset = Integer.parseInt(list.get(3));

        String bitRange = list.get(4);
        ParseBitRange parseBitRange = new ParseBitRange().invoke(bitRange);
        int bitPosition = parseBitRange.getBitPosition();
        int bitSize0 = parseBitRange.getBitSize0();

        EnumIniField.EnumKeyValueMap enums = EnumIniField.EnumKeyValueMap.valueOf(line.getRawText(), defines);
        return new EnumIniField(name, offset, type, enums, bitPosition, bitSize0);
    }

    public static boolean isKeyValueSyntax(String rawText) {
        String interestingPart = getEnumValuesSection(rawText);
        return IS_KEY_VALUE_SYNTAX.matcher(interestingPart).matches();
    }

    @NotNull
    public static String getEnumValuesSection(String rawText) {
        int interestingIndex = ordinalIndexOf(rawText, ",", 4);
        // yes that could have been done with a regex as well
        return rawText.substring(interestingIndex + /*skipping comma*/1).trim();
    }

    private static int ordinalIndexOf(String str, String substr, int n) {
        int pos = str.indexOf(substr);
        while (--n > 0 && pos != -1)
            pos = str.indexOf(substr, pos + 1);
        return pos;
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
