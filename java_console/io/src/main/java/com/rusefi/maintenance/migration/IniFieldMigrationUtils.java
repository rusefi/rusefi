package com.rusefi.maintenance.migration;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.CompatibilitySet;
import com.rusefi.config.FieldType;
import com.rusefi.tune.xml.Constant;

import org.jetbrains.annotations.Nullable;

import java.util.Arrays;
import java.util.List;
import java.util.Objects;
import java.util.Set;

import static com.devexperts.logging.Logging.getLogging;

public class IniFieldMigrationUtils {
    private static final Logging log = getLogging(IniFieldMigrationUtils.class);

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

    public static boolean checkIfDigitsCanBeMigrated(final String prevDigits, final String newDigits, String name) {
        if (Objects.equals(prevDigits, newDigits)) {
            return true;
        } else {
            if (prevDigits == null) {
                log.warn(name + " no prevDigits");
                return false;
            }
            if (newDigits == null) {
                log.warn(name + " no newDigits");
                return false;
            }
            final int prevDecimalCount = Integer.parseInt(prevDigits);
            final int newDecimalCount = Integer.parseInt(newDigits);
            return prevDecimalCount <= newDecimalCount;
        }
    }

    //TODO: reuse on BattLagCorrExtensionMigrator.java
	public static Constant generateConstant(final ArrayIniField iniField, final String value) {
		return new Constant(iniField.getName(), iniField.getUnits(), value, iniField.getDigits(),
				String.valueOf(iniField.getRows()), String.valueOf(iniField.getCols()));
	}
}
