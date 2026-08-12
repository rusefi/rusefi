package com.rusefi.core.io;

import com.rusefi.core.net.PropertiesHolder;

/**
 * A console bundle declares which ECUs it is allowed to serve via the
 * {@code board_compatibility} property in {@code shared_io.properties}:
 * <ul>
 *     <li>absent/blank - legacy behavior: only the exact bundle target (plus the {@code _QC_} hack)</li>
 *     <li>{@code *} - universal bundle, compatible with any ECU target</li>
 *     <li>comma list (e.g. {@code uaefi,uaefi_pro,super-uaefi}) - case-insensitive allowlist</li>
 * </ul>
 */
public class BoardCompatibility {
    public static final String BOARD_COMPATIBILITY_KEY = "board_compatibility";
    public static final String WILDCARD = "*";

    /**
     * @return trimmed {@code board_compatibility} value, or null when absent/blank
     */
    public static String getBoardCompatibility() {
        String value = PropertiesHolder.getProperty(BOARD_COMPATIBILITY_KEY);
        if (value == null)
            return null;
        value = value.trim();
        return value.isEmpty() ? null : value;
    }

    /**
     * Whether this bundle may talk to / flash an ECU reporting {@code ecuTarget}.
     *
     * @param bundleTarget local bundle target (from release.txt), may be null
     * @param ecuTarget    connected ECU's bundle target (from its signature)
     */
    public static boolean isEcuCompatible(String bundleTarget, String ecuTarget) {
        return isEcuCompatible(bundleTarget, ecuTarget, getBoardCompatibility());
    }

    /**
     * Pure variant taking the compatibility value explicitly (testable, no property lookup).
     */
    public static boolean isEcuCompatible(String bundleTarget, String ecuTarget, String compat) {
        if (ecuTarget == null) {
            // nothing to compare against - stay tolerant, matching prior callers
            return true;
        }
        if (bundleTarget != null && bundleTarget.equalsIgnoreCase(ecuTarget)) {
            return true;
        }
        // hack: QC firmware self-identifies as "normal" firmware [tag:QC_firmware]
        if (bundleTarget != null && bundleTarget.contains("_QC_")) {
            return true;
        }
        return matchesCompatibility(ecuTarget, compat);
    }

    /**
     * Does the {@code board_compatibility} property alone allow this ECU target?
     * {@code *} matches anything; otherwise a case-insensitive comma list.
     */
    public static boolean matchesCompatibility(String ecuTarget) {
        return matchesCompatibility(ecuTarget, getBoardCompatibility());
    }

    public static boolean matchesCompatibility(String ecuTarget, String compat) {
        if (compat == null)
            return false;
        if (WILDCARD.equals(compat))
            return true;
        for (String token : compat.split(",")) {
            if (token.trim().equalsIgnoreCase(ecuTarget))
                return true;
        }
        return false;
    }
}
