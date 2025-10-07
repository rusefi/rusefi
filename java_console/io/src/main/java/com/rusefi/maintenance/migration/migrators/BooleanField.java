package com.rusefi.maintenance.migration.migrators;

import java.util.Optional;
import static javax.management.ObjectName.quote;

// public only for tests!
public class BooleanField {
    private String fieldName;   // ex: launchControlEnabled
    private String trueValue;   // ex: "enabled"
    private String falseValue;  // ex: "disabled"

    public BooleanField(String fieldName, String trueValue, String falseValue) {
        this.fieldName = fieldName;
        this.falseValue = quote(falseValue);
        this.trueValue = quote(trueValue);
    }

    public String migrateField(String oldTuneFieldValue) {
        if (oldTuneFieldValue.startsWith(quote("true"))) {
            return trueValue;
        }
        return falseValue;
    }

    public Optional<Boolean> shouldMigrateField(String oldTuneFieldValue, String newTuneFalseValue) {
        boolean isLegacyValue = oldTuneFieldValue.startsWith(quote("true")) || oldTuneFieldValue.startsWith(quote("false"));
        // ie: already migrated on previous run/migration/etc
        boolean isMigratedValue = oldTuneFieldValue.startsWith(trueValue) || oldTuneFieldValue.startsWith(falseValue);

        // ie: on the case of loading old tune on newer rusefi console on older fw
        boolean isValidNewerIni = falseValue.startsWith(quote(newTuneFalseValue));

        if (!isLegacyValue && !isMigratedValue || !isValidNewerIni) {
            // invalid!, error on migration code? or on updated .ini?
            return Optional.empty();
        }
        return Optional.of(isLegacyValue);
    }

    public String getFieldName() {
        return fieldName;
    }

    public String getTrueValue() {
        return trueValue;
    }

    public String getFalseValue() {
        return falseValue;
    }

}
