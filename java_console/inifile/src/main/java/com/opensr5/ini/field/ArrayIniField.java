package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;
import com.rusefi.config.Field;
import com.rusefi.config.FieldType;
import com.rusefi.tune.xml.Constant;
import org.jetbrains.annotations.Nullable;

import java.nio.ByteBuffer;
import java.util.LinkedList;
import java.util.Objects;

public class ArrayIniField extends IniField {
    private final String unit;
    private final FieldType type;
    private final int cols;
    private final int rows;
    private final double multiplier;
    @Nullable
    private final String min;
    @Nullable
    private final String max;
    private final String digits;

    public ArrayIniField(
        final String name,
        final int offset,
        final FieldType type,
        final int cols,
        final int rows,
        final String unit,
        final double multiplier,
        final String min,
        final String max,
        String digits
    ) {
        super(name, offset);
        this.unit = unit;
        this.type = type;
        this.cols = cols;
        this.rows = rows;
        this.multiplier = multiplier;
        this.min = min;
        this.max = max;
        this.digits = digits;
    }

    @Override
    public String getUnits() {
        return unit;
    }

    public FieldType getType() {
        return type;
    }

    /**
     * todo: spell out if 'cols' is first or second dimension of C arrays
     * @return 10 in case of TS definition [10x6]
     * @return 1 for curves
     */
    public int getCols() {
        return cols;
    }

    public int getRows() {
        return rows;
    }

    public double getMultiplier() {
        return multiplier;
    }

    public String getMin() {
        return min;
    }

    public String getMax() {
        return max;
    }

    @Override
    public String getDigits() {
        return digits;
    }

    @Override
    public int getSize() {
        return type.getStorageSize() * cols * rows;
    }

    public String formatValue(final String[][] values) {
        StringBuilder sb = new StringBuilder();
        for (final String[] row : values) {
            sb.append("\n        ");
            for (final String element : row) {
                sb.append(' ');
                sb.append(element);
            }
        }
        sb.append("\n");
        return sb.toString();
    }

    @Override
    public String getValue(final ConfigurationImage image) {
        final String[][] values = new String[rows][cols];
        for (int rowIndex = 0; rowIndex < rows; rowIndex++) {
            for (int colIndex = 0; colIndex < cols; colIndex++) {
                final Field f = new Field(getName() + "_" + colIndex, getOffset(rowIndex, colIndex), getType());
                values[rowIndex][colIndex] = f.getAnyValue(image, multiplier);
            }
        }
        return formatValue(values);
    }

    private int getOffset(int rowIndex, int colIndex) {
        return getOffset() + (rowIndex * cols + colIndex) * getType().getStorageSize();
    }

    public String[][] getValues(final String value) {
        final String[] values = value.trim().split("\\s+");
        if (values.length != rows * cols) {
            throw new IllegalStateException(getName() + ": " + values.length + " values while expecting " + getRows() + " by " + getCols() + " total " + rows * cols);
        } else {
            final String[][] result = new String[rows][cols];
            for (int i = 0; i < values.length; i++) {
                final int rowIndex = i / cols;
                final int colIndex = i % cols;
                result[rowIndex][colIndex] = values[i];
            }
            return result;
        }
    }

    @Override
    public void setValue(ConfigurationImage image, Constant constant) {
        Objects.requireNonNull(image, "image array setter");
        final String[][] values = getValues(constant.getValue());
        for (int rowIndex = 0; rowIndex < values.length; rowIndex++) {
            final String[] row = values[rowIndex];
            for (int colIndex = 0; colIndex < row.length; colIndex++) {
                ByteBuffer wrapped = image.getByteBuffer(getOffset(rowIndex, colIndex), type.getStorageSize());
                ConfigurationImage.setScalarValue(
                    wrapped,
                    type,
                    values[rowIndex][colIndex],
                    Field.NO_BIT_OFFSET,
                    multiplier,
                    0
                );
            }
        }
    }

    @Override
    public String toString() {
        return "ArrayIniField{" +
            "super='" + super.toString() + '\'' +
            ", unit='" + unit + '\'' +
            ", type=" + type +
            ", cols=" + cols +
            ", rows=" + rows +
            ", multiplier=" + multiplier +
            ", min='" + min + '\'' +
            ", max='" + max + '\'' +
            ", digits='" + digits + '\'' +
            '}';
    }

    public static IniField parse(LinkedList<String> list) {
        String name = list.get(0);
        FieldType type = FieldType.parseTs(list.get(2));
        int offset = Integer.parseInt(list.get(3));
        String size = list.get(4);
        String unit = list.size() > 5 ? list.get(5) : "error";
        final String min = list.size() > 8 ? list.get(8) : null;
        final String max = list.size() > 9 ? list.get(9) : null;
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

        return new ArrayIniField(name, offset, type, cols, rows, unit, multiplier, min, max, digits);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof ArrayIniField)) return false;
        if (!super.equals(o)) return false;
        ArrayIniField that = (ArrayIniField) o;
        return getCols() == that.getCols()
            && getRows() == that.getRows()
            && Double.compare(getMultiplier(), that.getMultiplier()) == 0
            && Objects.equals(unit, that.unit)
            && getType() == that.getType()
            && Objects.equals(getMin(), that.getMin())
            && Objects.equals(getMax(), that.getMax())
            && Objects.equals(getDigits(), that.getDigits());
    }

    @Override
    public int hashCode() {
        return Objects.hash(
            super.hashCode(),
            unit,
            getType(),
            getCols(),
            getRows(),
            getMultiplier(),
            getMin(),
            getMax(),
            getDigits()
        );
    }
}
