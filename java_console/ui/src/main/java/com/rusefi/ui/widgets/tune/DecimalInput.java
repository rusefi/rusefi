package com.rusefi.ui.widgets.tune;

/**
 * Numbers the user types into tables, curves, the delta field and the set-value dialog. Both "." and
 * "," are decimal separators, so "2,5" is 2.5: users with regional settings type a comma, and the
 * numpad decimal key gives one. There are no thousands separators.
 */
final class DecimalInput {
    private DecimalInput() {
    }

    /** The typed number; throws {@link NumberFormatException} for anything else, as Double.parseDouble. */
    static double parse(String text) {
        return Double.parseDouble(text.trim().replace(',', '.'));
    }

    static boolean isSeparator(char character) {
        return character == '.' || character == ',';
    }
}
