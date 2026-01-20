package com.opensr5.ini.field;

import com.opensr5.ConfigurationImage;
import com.rusefi.tune.xml.Constant;

import java.util.Objects;

public abstract class IniField {
    private final String name;
    // offset within binary page
    private final int offset;

    public IniField(String name, int offset) {
        this.name = name;
        this.offset = offset;
    }

    public static double parseDouble(String s) {
        // todo: real implementation
        s = s.replaceAll("\\{", "").replaceAll("\\}", "");
        int dividerIndex = s.indexOf('/');
        if (dividerIndex != -1) {
            return Double.parseDouble(s.substring(0, dividerIndex)) / Double.parseDouble(s.substring(dividerIndex + 1));
        } else {
            return Double.parseDouble(s);
        }
    }

    public String getName() {
        return name;
    }

    public String getUnits() {
        return null;
    }

    public String getDigits() {
        return null;
    }

    public int getOffset() {
        return offset;
    }

    public abstract int getSize();

    public abstract <T> T accept(IniFieldVisitor<T> visitor);

    /**
     * @see com.rusefi.config.Field#getValue
     */
    public String getValue(ConfigurationImage image) {
        return accept(new IniFieldVisitor<String>() {
            @Override
            public String visit(ScalarIniField field) {
                com.rusefi.config.Field f = new com.rusefi.config.Field(field.getName(), field.getOffset(), field.getType());
                try {
                    Double value = f.getValue(image, field.getMultiplier()) + field.getSerializationOffset();
                    return com.rusefi.config.StringFormatter.niceToString(value, com.rusefi.config.StringFormatter.FIELD_PRECISION);
                } catch (Throwable e) {
                    throw new IllegalStateException("While getting " + field.getName(), e);
                }
            }

            @Override
            public String visit(EnumIniField field) {
                int ordinal = image.getByteBuffer(field).getInt();
                ordinal = ConfigurationImage.getBitRange(ordinal, field.getBitPosition(), field.getBitSize0() + 1);

                if (ordinal >= field.getEnums().size())
                    throw new OrdinalOutOfRangeException("Ordinal out of range " + ordinal + " in " + field.getName() + " while " + field.getEnums().size() + " " + field.getType());
                return "\"" + field.getEnums().get(ordinal) + "\"";
            }

            @Override
            public String visit(ArrayIniField field) {
                final String[][] values = new String[field.getRows()][field.getCols()];
                for (int rowIndex = 0; rowIndex < field.getRows(); rowIndex++) {
                    for (int colIndex = 0; colIndex < field.getCols(); colIndex++) {
                        final com.rusefi.config.Field f = new com.rusefi.config.Field(field.getName() + "_" + colIndex, field.getOffset(rowIndex, colIndex), field.getType());
                        values[rowIndex][colIndex] = f.getAnyValue(image, field.getMultiplier());
                    }
                }
                return field.formatValue(values);
            }

            @Override
            public String visit(StringIniField field) {
                String value = new String(image.getContent(), field.getOffset(), field.getSize());
                value = value.trim();
                value = trimAtZeroSymbol(value);
                return value;
            }

            @org.jetbrains.annotations.NotNull
            private String trimAtZeroSymbol(String value) {
                for (int i = 0; i < value.length(); i++) {
                    // C/C++ zero string is terminated but java XML looks for all 'size' of symbols, let's convert
                    if (value.charAt(i) == 0) {
                        value = value.substring(0, i);
                        break;
                    }
                }
                return value;
            }
        });
    }

    public abstract void setValue(ConfigurationImage image, Constant constant);

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        IniField iniField = (IniField) o;
        return getOffset() == iniField.getOffset() && Objects.equals(getName(), iniField.getName());
    }

    @Override
    public int hashCode() {
        return Objects.hash(getName(), getOffset());
    }

    @Override
    public String toString() {
        return "IniField{" +
            "name='" + name + '\'' +
            ", offset=" + offset +
            '}';
    }
}
