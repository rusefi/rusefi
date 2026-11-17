package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

/**
 * Fonts for console widgets. Java's logical "Monospaced" font maps to Courier New on Windows,
 * which looks dated next to FlatLaf; prefer a modern installed monospace family instead.
 */
public final class ConsoleFonts {
    private static final String[] MONO_FAMILIES = {
        "JetBrains Mono", "Cascadia Mono", "Cascadia Code", "Consolas", "SF Mono", "Menlo",
        "Ubuntu Mono", "DejaVu Sans Mono", "Liberation Mono"
    };
    private static String monoFamily;

    private ConsoleFonts() {
    }

    public static Font mono(int style, int size) {
        return new Font(monoFamily(), style, size);
    }

    /** Size of the theme's default label font. */
    public static int baseSize() {
        Font f = UIManager.getFont("Label.font");
        return f != null ? f.getSize() : 13;
    }

    private static synchronized String monoFamily() {
        if (monoFamily == null) {
            monoFamily = Font.MONOSPACED;
            try {
                Set<String> installed = new HashSet<>(Arrays.asList(
                    GraphicsEnvironment.getLocalGraphicsEnvironment().getAvailableFontFamilyNames()));
                for (String family : MONO_FAMILIES) {
                    if (installed.contains(family)) {
                        monoFamily = family;
                        break;
                    }
                }
            } catch (Throwable ignored) {
                // headless or no font list: keep the logical monospaced font
            }
        }
        return monoFamily;
    }
}
