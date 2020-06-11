package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;
import com.rusefi.config.Field;
import com.rusefi.config.FieldType;

import java.util.LinkedList;

public class ArrayIniField extends IniField {
    private final FieldType type;
    private final int cols;
    private final int rows;

    public ArrayIniField(String name, int offset, FieldType type, int cols, int rows) {
        super(name, offset);
        this.type = type;
        this.cols = cols;
        this.rows = rows;
    }

    public FieldType getType() {
        return type;
    }

    public int getCols() {
        return cols;
    }

    public int getRows() {
        return rows;
    }

    @Override
    public String getValue(ConfigurationImage image) {
        StringBuilder sb = new StringBuilder();
        for (int rowIndex = 0; rowIndex < rows; rowIndex++) {
            sb.append("\n\t");
            for (int colIndex = 0; colIndex < cols; colIndex++) {
                Field f = new Field("", getOffset() + rowIndex * getType().getStorageSize(), getType());
                sb.append(' ');
                sb.append(f.getAnyValue(image));
            }
        }
        sb.append("\n");
        return sb.toString();
    }

    public static IniField parse(LinkedList<String> list) {
        String name = list.get(0);
        FieldType type = FieldType.parseTs(list.get(2));
        int offset = Integer.parseInt(list.get(3));
        String size = list.get(4);
        size = size.replaceAll("[\\]\\[x]", " ").trim();
        String dimentions[] = size.split(" ");
        int cols;
        int rows;
        if (dimentions.length == 1) {
            cols = 1;
            rows = Integer.parseInt(dimentions[0]);
        } else if (dimentions.length == 2) {
            cols = Integer.parseInt(dimentions[0]);
            rows = Integer.parseInt(dimentions[1]);
        } else {
            throw new IllegalStateException("Unexpected " + size);
        }

        return new ArrayIniField(name, offset, type, cols, rows);
    }
}
