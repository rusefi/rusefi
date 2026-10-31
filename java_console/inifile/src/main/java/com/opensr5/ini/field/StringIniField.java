package com.opensr5.ini.field;

import java.util.LinkedList;

public class StringIniField extends IniField {
    private final int size;

    public StringIniField(String name, int offset, int size) {
        super(name, offset);
        this.size = size;
    }

    @Override
    public <T> T accept(IniFieldVisitor<T> visitor) {
        return visitor.visit(this);
    }

    @Override
    public int getSize() {
        return size;
    }

    @Override
    public String toString() {
        return "StringIniField{" +
            "offset=" + getOffset() +
            ", size=" + size +
            '}';
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
