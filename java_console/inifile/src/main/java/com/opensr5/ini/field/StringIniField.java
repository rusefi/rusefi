package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;
import com.rusefi.tune.xml.Constant;
import org.jetbrains.annotations.NotNull;

import java.util.LinkedList;

public class StringIniField extends IniField {
    private final int size;

    public StringIniField(String name, int offset, int size) {
        super(name, offset);
        this.size = size;
    }

    @Override
    public int getSize() {
        return size;
    }

    @Override
    public String getValue(ConfigurationImage image) {
        String value = new String(image.getContent(), getOffset(), size);
        value = value.trim();
        value = trimAtZeroSymbol(value);
        return value;
    }

    @Override
    public String toString() {
        return "StringIniField{" +
            "offset=" + getOffset() +
            ", size=" + size +
            '}';
    }

    @NotNull
    private static String trimAtZeroSymbol(String value) {
        for (int i = 0; i < value.length(); i++) {
            // C/C++ zero string is terminated but java XML looks for all 'size' of symbols, let's convert
            if (value.charAt(i) == 0) {
                value = value.substring(0, i);
                break;
            }
        }
        return value;
    }

    @Override
    public void setValue(ConfigurationImage image, Constant constant) {
        String value = constant.getValue();
        for (int i = 0; i < value.length(); i++)
            image.getContent()[getOffset() + i] = (byte) value.charAt(i);
    }

    public static IniField parse(LinkedList<String> list) {
        String name = list.get(0);
        int offset = Integer.parseInt(list.get(3));
        if (!list.get(2).equalsIgnoreCase("ASCII"))
            throw new IllegalStateException("Do not understand " + name + " at " + offset);
        int size = Integer.parseInt(list.get(4));
        return new StringIniField(name, offset, size);
    }
}
