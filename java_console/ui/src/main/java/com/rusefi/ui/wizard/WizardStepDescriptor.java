package com.rusefi.ui.wizard;

import com.rusefi.ui.UIContext;

import java.util.function.Function;
import java.util.function.Predicate;

/**
 * Metadata describing a wizard step in the catalog.
 * <p>
 * Two shapes:
 * <ul>
 *   <li>Flagged step — part of the numbered full wizard; {@code flagName} non-null, {@code factory} unused.
 *   Needs-attention is derived from the flag's state.</li>
 *   <li>Standalone step — independent auto-launch target; {@code flagName} null, {@code factory} required.
 *   {@code needsAttention} decides whether to launch (e.g. "VIN is empty").</li>
 * </ul>
 */
public class WizardStepDescriptor {
    public static final Predicate<UIContext> ALWAYS = ctx -> true;

    /** INI wizard progress flag (e.g. "wizardNumberOfCylinders"), or null for standalone steps. */
    public final String flagName;

    /** If true, this step participates in auto-launch scanning. */
    public final boolean autoLaunch;

    /** Board-level applicability filter. When false the step is treated as already satisfied. */
    public final Predicate<UIContext> applicable;

    /** Standalone-only: whether the step currently needs user attention (e.g. field empty). */
    public final Predicate<UIContext> needsAttention;

    /** Standalone-only: creates the WizardStep instance on demand. */
    public final Function<UIContext, WizardStep> factory;

    public WizardStepDescriptor(String flagName,
                                boolean autoLaunch,
                                Predicate<UIContext> applicable,
                                Predicate<UIContext> needsAttention,
                                Function<UIContext, WizardStep> factory) {
        this.flagName = flagName;
        this.autoLaunch = autoLaunch;
        this.applicable = applicable != null ? applicable : ALWAYS;
        this.needsAttention = needsAttention;
        this.factory = factory;
    }

    public boolean isFlagged() {
        return flagName != null;
    }
}
