package com.rusefi.ui.widgets.tune;

/**
 * [tag:unburned_marks] What the reference tune of the marks is called. Connected, it is the tune last
 * burned to (or read from) the ECU. Offline it is the tune loaded from a file: saving to a file does not
 * replace it, because reconnecting uses it to find the edited settings ({@code OfflineEditMigration}),
 * so the marks then show changes since loading, which are not burned to any ECU yet.
 */
public enum UnburnedWording {
    BURNED("Burned", "Not burned to the ECU yet", "Set back to the burned value"),
    LOADED("Loaded", "Changed since the tune was loaded, not burned to an ECU yet", "Set back to the loaded value");

    /** Prefix of the reference value: "Burned: 440". */
    final String reference;
    /** Last line of a cell tooltip. */
    final String cellNote;
    final String revertTooltip;

    UnburnedWording(String reference, String cellNote, String revertTooltip) {
        this.reference = reference;
        this.cellNote = cellNote;
        this.revertTooltip = revertTooltip;
    }

    public static UnburnedWording of(boolean offline) {
        return offline ? LOADED : BURNED;
    }
}
