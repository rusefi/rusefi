package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.Locale;
import java.util.Optional;

public enum MapSamplingValuesConverter implements TableValuesConverter {
    INSTANCE;
    private static final String MAP_SAMPLING_CURVE_FIELD_NAME = "map_samplingAngle";
    private static final Double MAP_SAMPLING_CORRECTION = 45.0;

    @Override
    public Optional<String[][]> convertTableValues(final String[][] prevValues, final TuneMigrationContext context) {
        final String previousSignature = context.getPrevIniFile().getSignature();
        final String updatedSignature = context.getUpdatedIniFile().getSignature();

        final Constant prevFieldValue = context.getPrevTune().getConstantsAsMap().get(MAP_SAMPLING_CURVE_FIELD_NAME);
        final Constant updatedFieldValue = context.getUpdatedTune().getConstantsAsMap().get(MAP_SAMPLING_CURVE_FIELD_NAME);

        final Optional<IniField> prevField = context.getPrevIniFile().findIniField(MAP_SAMPLING_CURVE_FIELD_NAME);
        final Optional<IniField> updatedField = context.getUpdatedIniFile().findIniField(MAP_SAMPLING_CURVE_FIELD_NAME);

        boolean migrationValid = previousSignature.contains("lts-25jersey") && !updatedSignature.contains("lts-25jersey");
        if (migrationValid && prevField.isPresent() && updatedField.isPresent()) {

            if (prevFieldValue != null && updatedFieldValue != null) {
                final int prevValuesRowCount = prevValues.length;
                final int prevValuesColumnCount = prevValues[0].length;
                final String[][] convertedValues = new String[prevValuesRowCount][prevValuesColumnCount];
                for (int i = 0; i < prevValuesRowCount; i++) {
                    for (int j = 0; j < prevValuesColumnCount; j++) {
                        final String prevValue = prevValues[i][j];
                        final double prevElement = Double.parseDouble(prevValue) - MAP_SAMPLING_CORRECTION;
                        final Optional<String> convertedValue = Optional.of( String.format(Locale.US, "%." + updatedFieldValue.getDigits() + "f", prevElement));
                        convertedValues[i][j] = convertedValue.get();
                    }
                }
                return Optional.of(convertedValues);
            }
        } else if (!migrationValid && prevField.isPresent() && updatedField.isPresent()) {
            return Optional.of(prevValues);
        }


        return Optional.empty();
    }
}
