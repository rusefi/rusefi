package com.rusefi.ui.basic;

import org.jetbrains.annotations.NotNull;

public class PanamaArgumentValidator {
    public static @NotNull String secureInstanceName(String originalString) {
        return originalString.replaceAll("[^0-9a-zA-Z \t]+", "");
    }
}
