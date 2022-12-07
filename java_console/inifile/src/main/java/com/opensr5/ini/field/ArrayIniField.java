package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;
import com.rusefi.config.Field;
import com.rusefi.config.FieldType;
import com.rusefi.tune.xml.Constant;

import java.nio.ByteBuffer;
import java.util.LinkedList;

public class ArrayIniField extends IniField {
    private final FieldType type;
    private final int cols;
    private final int rows;
    private final double multiplier;
    private final String digits;

    public ArrayIniField(String name, int offset, FieldType type, int cols, int rows, String unit, double multiplier, String digits) {
        super(name, offset);
        this.type = type;
        this.cols = cols;
        this.rows = rows;
        this.multiplier = multiplier;
        this.digits = digits;
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
    public String getDigits() {
        return digits;
    }

    @Override
    public int getSize() {
        return type.getStorageSize() * cols * rows;
    }

    @Override
    public String getValue(ConfigurationImage image) {
        StringBuilder sb = new StringBuilder();
        for (int rowIndex = 0; rowIndex < rows; rowIndex++) {
            sb.append("\n        ");
            for (int colIndex = 0; colIndex < cols; colIndex++) {
                Field f = new Field("", getOffset(rowIndex, colIndex), getType());
                sb.append(' ');
                sb.append(f.getAnyValue(image, multiplier));
            }
        }
        sb.append("\n");
        return sb.toString();
    }

    private int getOffset(int rowIndex, int colIndex) {
        return getOffset() + (rowIndex * cols + colIndex) * getType().getStorageSize();
    }

    @Override
    public void setValue(ConfigurationImage image, Constant constant) {
        String[] values = constant.getValue().trim().split("\\s+");
        if (values.length != getRows() * getCols())
            throw new IllegalStateException(values.length + " values while expecting " + getRows() + " by " + getCols() + " total " + getRows() * getCols());

        for (int rowIndex = 0; rowIndex < rows; rowIndex++) {
            for (int colIndex = 0; colIndex < cols; colIndex++) {
                String value = values[rowIndex * cols + colIndex];
                ByteBuffer wrapped = image.getByteBuffer(getOffset(rowIndex, colIndex), type.getStorageSize());
                ScalarIniField.setValue(wrapped, type, value, Field.NO_BIT_OFFSET, multiplier);
            }
        }
    }

    @Override
    public String toString() {
        return "ArrayIniField{" +
                "name=" + getName() +
                ", offset=" + getOffset() +
                ", type=" + type +
                ", cols=" + cols +
                ", rows=" + rows +
                '}';
    }

    public static IniField parse(LinkedList<String> list) {
        String name = list.get(0);
        FieldType type = FieldType.parseTs(list.get(2));
        int offset = Integer.parseInt(list.get(3));
        String size = list.get(4);
        String unit = list.size() > 5 ? list.get(5) : "error";
        String digits = list.size() > 10 ? list.get(10) : "0";
        double multiplier = IniField.parseDouble(list.size() > 6 ? list.get(6) : "1");

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

        return new ArrayIniField(name, offset, type, cols, rows, unit, multiplier, digits);
    }
}
