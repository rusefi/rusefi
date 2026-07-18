package com.opensr5.ini;

/**
 * A "std_separator" entry in a TunerStudio [Menu] section: a horizontal divider between menu items.
 */
public class SeparatorMenuItem implements MenuItem {
    public static final String STD_SEPARATOR = "std_separator";

    public static final SeparatorMenuItem INSTANCE = new SeparatorMenuItem();

    private SeparatorMenuItem() {
    }

    @Override
    public String toString() {
        return "";
    }
}
