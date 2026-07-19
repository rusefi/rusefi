package com.rusefi.tune.ve;

import java.util.Collections;
import java.util.List;

/**
 * Common interface for all VE table generator implementations.
 * Each implementation owns its own configuration/request type and profile ID.
 * The shared output is {@link Result}.
 */
public interface VeGenerator {

    /** Short versioned identifier for this generator, e.g. {@code "archetype-base-ve-v1"}. */
    String getProfileId();

    /** Human-readable name shown in the generator selection UI. */
    String getDisplayName();

    /**
     * Generate a VE surface for the provided ECU axes.
     *
     * @param rpmAxis strictly ascending, finite RPM axis values (length >= 1)
     * @param mapAxis strictly ascending, finite MAP/load axis values in kPa (length >= 1)
     * @return generated surface and any applicable warnings
     */
    Result generate(double[] rpmAxis, double[] mapAxis);

    /**
     * Shared immutable output for all VE generators.
     * {@code veTable[loadRow][rpmCol]} — same orientation as ECU storage.
     */
    final class Result {
        public final double[][] veTable;
        public final String profileId;
        public final List<String> warnings;

        public Result(double[][] veTable, String profileId, List<String> warnings) {
            this.veTable  = veTable;
            this.profileId = profileId;
            this.warnings  = Collections.unmodifiableList(warnings);
        }
    }
}
