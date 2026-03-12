package com.rusefi.ui.util;

import java.awt.Color;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/**
 * Pin marker color utilities for the Pinout tab.
 * Type colors mirror the rusEFI web CSS rules.
 * Pigtail colors are resolved from the wire color strings in connectors.zip yamls.
 */
public final class PinColors {

    private PinColors() {}

    public static final Color FALLBACK_NORMAL = new Color(0, 120, 255, 200);
    public static final Color COLOR_HIGHLIGHT  = new Color(255, 80, 0, 230);

    /** A keyword→color mapping entry for a pin type. Used for both lookup and legend rendering. */
    public static final class TypeEntry {
        public final String keyword;
        public final Color color;
        TypeEntry(String keyword, Color color) { this.keyword = keyword; this.color = color; }
    }

    /**
     * Ordered list of type keyword→color mappings. Order is significant: longer/more-specific
     * keywords (pgnd, sgnd) appear before their shorter prefixes (gnd) to avoid false matches.
     * {@link #typeToColor} iterates over it.
     */
    public static final List<TypeEntry> TYPE_ENTRIES = Collections.unmodifiableList(Arrays.asList(
            new TypeEntry("pgnd", new Color(0xFF, 0x7F, 0x50)), // coral
            new TypeEntry("sgnd", new Color(0x80, 0x80, 0x00)), // olive
            new TypeEntry("ign",  new Color(0xFF, 0x00, 0xFF)), // #f0f
            new TypeEntry("inj",  new Color(0x80, 0x00, 0x00)), // maroon
            new TypeEntry("ls",   new Color(0x90, 0xEE, 0x90)), // #90ee90
            new TypeEntry("mr",   new Color(0xB2, 0x22, 0x22)), // #b22222
            new TypeEntry("usb",  new Color(0x20, 0xB2, 0xAA)), // #20b2aa
            new TypeEntry("vr",   new Color(0xA0, 0x52, 0x2D))  // sienna
    ));

    /**
     * Maps pin type string to marker color. Uses substring matching in TYPE_ENTRIES order.
     * Returns null for unrecognised types.
     */
    public static Color typeToColor(String type) {
        if (type == null || type.isEmpty()) return null;
        String t = type.toLowerCase();
        for (TypeEntry e : TYPE_ENTRIES) {
            if (t.contains(e.keyword)) return e.color;
        }
        return null;
    }

    /**
     * Returns 1 or 2 colors for a pigtail wire color string.
     * Strings like "white/red" produce two colors (left half, right half for split-circle rendering).
     * Returns null for unknown or empty strings.
     */
    public static Color[] pigtailToColors(String wireColor) {
        if (wireColor == null || wireColor.isEmpty()) return null;
        String[] tokens = wireColor.split("[/,]");
        Color c1 = pigtailTokenToColor(tokens[0].trim());
        if (tokens.length >= 2) {
            Color c2 = pigtailTokenToColor(tokens[1].trim());
            if (c1 != null && c2 != null) return new Color[]{c1, c2};
        }
        return c1 != null ? new Color[]{c1} : null;
    }

    /** Maps a single pigtail wire color token (no slashes) to a Color. Returns null if unrecognised. */
    public static Color pigtailTokenToColor(String token) {
        if (token == null) return null;
        switch (token.toLowerCase().trim()) {
            case "black":  return new Color(30, 30, 30);
            case "red":    return new Color(200, 0, 0);
            case "white":  return new Color(230, 230, 230);
            case "yellow": return new Color(220, 200, 0);
            case "green":  return new Color(0, 150, 0);
            case "blue":   return new Color(0, 60, 200);
            case "orange": return new Color(255, 140, 0);
            case "purple":
            case "violet": return new Color(128, 0, 128);
            case "brown":  return new Color(139, 69, 19);
            case "grey":
            case "gray":   return new Color(120, 120, 120);
            case "pink":   return new Color(255, 160, 160);
            default:       return null;
        }
    }

    /** Returns true if the color is perceived as light (needs dark text for contrast). */
    public static boolean isLight(Color c) {
        double lum = 0.299 * c.getRed() + 0.587 * c.getGreen() + 0.114 * c.getBlue();
        return lum > 160;
    }
}
