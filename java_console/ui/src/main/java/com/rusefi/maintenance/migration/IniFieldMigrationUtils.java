package com.rusefi.maintenance.migration;

import com.rusefi.CompatibilitySet;
import com.rusefi.config.FieldType;
import org.jetbrains.annotations.Nullable;

import java.util.Arrays;
import java.util.List;
import java.util.Objects;
import java.util.Set;

import static com.rusefi.config.FieldType.*;

public class IniFieldMigrationUtils {
    public static boolean checkIfTypeCanBeMigrated(final FieldType prevType, final FieldType newType) {
        if (Objects.equals(prevType, newType)) {
            return true;
        } else {
            switch (prevType) {
                case INT8: {
                    switch (newType) {
                        case INT16:
                        case INT:
                        case FLOAT: {
                            return true;
                        }
                    }
                }
                case INT16: {
                    switch (newType) {
                        case INT:
                        case FLOAT: {
                            return true;
                        }
                    }
                }
                case UINT8: {
                    switch (newType) {
                        case UINT16:
                        case INT16:
                        case INT:
                        case FLOAT: {
                            return true;
                        }
                    }
                }
            }
            return false;
        }
    }

    //ATTENTION! please add only lower-cased value in the list below:
    private final static List<Set<String>> UNITS_SYNONYMS = Arrays.asList(
        CompatibilitySet.of("%", "percent"),
        CompatibilitySet.of("deg c", "c"),
        CompatibilitySet.of("voltage", "volts")
    );

    public static boolean checkIfUnitsCanBeMigrated(@Nullable final String prevUnits, @Nullable final String newUnits) {
        if ((prevUnits == null) || prevUnits.isEmpty()) {
            return true;
        } else if (newUnits != null) {
            final String lcPrevUnits = prevUnits.toLowerCase();
            final String lcNewUnits = prevUnits.toLowerCase();
            if (lcPrevUnits.equals(lcNewUnits)) {
                return true;
            } else {
                for (final Set<String> synonyms: UNITS_SYNONYMS) {
                    if (synonyms.contains(lcPrevUnits) && synonyms.contains(lcNewUnits)) {
                        return true;
                    }
                };
            }
        }
        return false;
    }

    public static boolean checkIfDigitsCanBeMigrated(final String prevDigits, final String newDigits) {
        if (Objects.equals(prevDigits, newDigits)) {
            return true;
        } else {
            final int prevDecimalCount = Integer.parseInt(prevDigits);
            final int newDecimalCount = Integer.parseInt(newDigits);
            return prevDecimalCount <= newDecimalCount;
        }
    }
}
