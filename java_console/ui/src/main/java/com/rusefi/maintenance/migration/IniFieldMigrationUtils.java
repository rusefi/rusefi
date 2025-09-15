package com.rusefi.maintenance.migration;

import com.rusefi.CompatibilitySet;
import org.jetbrains.annotations.Nullable;

import java.util.Arrays;
import java.util.List;
import java.util.Set;

public class IniFieldMigrationUtils {

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
}
