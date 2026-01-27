package com.opensr5;

import com.opensr5.ini.field.*;

public class ConfigurationImageGetterSetter {
    /**
     * @see com.rusefi.config.Field#getValue
     */
    public static String getStringValue(IniField iniField, ConfigurationImage image) {
        return iniField.accept(new IniFieldVisitor<String>() {
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

    public static void setValue2(IniField iniField, ConfigurationImage image, final String name, final String value) {
        iniField.accept(new IniFieldVisitor<Void>() {
            @Override
            public Void visit(ScalarIniField field) {
                java.util.Objects.requireNonNull(image, "image for setter");
                com.rusefi.config.Field f = new com.rusefi.config.Field(field.getName(), field.getOffset(), field.getType());
                java.nio.ByteBuffer wrapped = image.getByteBuffer(field.getOffset(), field.getType().getStorageSize());
                ConfigurationImage.setScalarValue(wrapped, field.getType(), value, f.getBitOffset(), field.getMultiplier(), field.getSerializationOffset());
                return null;
            }

            @Override
            public Void visit(EnumIniField field) {
                String v = value;
                int ordinal = field.getEnums().indexOf(v);
                if (ordinal == -1)
                    throw new IllegalArgumentException(name + ": Enum name not found " + v);
                image.setBitValue(field, ordinal);
                return null;
            }

            @Override
            public Void visit(ArrayIniField field) {
                java.util.Objects.requireNonNull(image, "image array setter");
                final String[][] values = field.getValues(value);
                for (int rowIndex = 0; rowIndex < values.length; rowIndex++) {
                    final String[] row = values[rowIndex];
                    for (int colIndex = 0; colIndex < row.length; colIndex++) {
                        java.nio.ByteBuffer wrapped = image.getByteBuffer(field.getOffset(rowIndex, colIndex), field.getType().getStorageSize());
                        ConfigurationImage.setScalarValue(
                            wrapped,
                            field.getType(),
                            values[rowIndex][colIndex],
                            com.rusefi.config.Field.NO_BIT_OFFSET,
                            field.getMultiplier(),
                            0
                        );
                    }
                }
                return null;
            }

            @Override
            public Void visit(StringIniField field) {
                for (int i = 0; i < value.length(); i++)
                    image.getContent()[field.getOffset() + i] = (byte) value.charAt(i);
                return null;
            }
        });
    }
}
