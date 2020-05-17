package com.opensr5.ini.field;

import com.rusefi.config.FieldType;

import java.util.LinkedList;
import java.util.List;

public class BitIniField extends IniField {
    private final FieldType type;
    private final List<String> enums;

    public BitIniField(String name, int offset, FieldType type, List<String> enums) {
        super(name, offset);
        this.type = type;
        this.enums = enums;
    }

    public FieldType getType() {
        return type;
    }

    public List<String> getEnums() {
        return enums;
    }

    public static BitIniField parse(LinkedList<String> list) {
        String name = list.get(0);
        FieldType type = FieldType.parseTs(list.get(2));
        int offset = Integer.parseInt(list.get(3));

        List<String> enums = list.subList(5, list.size() - 1);

        return new BitIniField(name, offset, type, enums);
    }
}
