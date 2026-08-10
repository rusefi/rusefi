package com.rusefi.pinout;

import java.awt.Color;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;

/**
 * Wire color strings from connector yamls ("red", "orange/brown") to paintable colors.
 */
public final class WireColors {
    private WireColors() {
    }

    public static final Color UNKNOWN = Color.MAGENTA;

    private static final Map<String, Color> BY_NAME = new HashMap<>();

    static {
        BY_NAME.put("red", new Color(220, 30, 30));
        BY_NAME.put("black", Color.BLACK);
        BY_NAME.put("white", Color.WHITE);
        BY_NAME.put("grey", Color.GRAY);
        BY_NAME.put("gray", Color.GRAY);
        BY_NAME.put("green", new Color(20, 150, 40));
        BY_NAME.put("blue", new Color(30, 70, 220));
        BY_NAME.put("yellow", new Color(240, 210, 0));
        BY_NAME.put("orange", new Color(255, 140, 0));
        BY_NAME.put("brown", new Color(130, 75, 30));
        BY_NAME.put("purple", new Color(130, 30, 180));
        BY_NAME.put("pink", Color.PINK);
    }

    /**
     * @return one entry per stripe, e.g. "orange/brown" gives two colors
     */
    public static Color[] resolve(String wireColor) {
        if (wireColor == null || wireColor.trim().isEmpty()) {
            return new Color[]{UNKNOWN};
        }
        String[] parts = wireColor.toLowerCase(Locale.ROOT).split("[/,]");
        Color[] result = new Color[parts.length];
        for (int i = 0; i < parts.length; i++) {
            Color color = BY_NAME.get(parts[i].trim());
            result[i] = color == null ? UNKNOWN : color;
        }
        return result;
    }

    public static boolean isLight(Color color) {
        return (0.299 * color.getRed() + 0.587 * color.getGreen() + 0.114 * color.getBlue()) > 150;
    }
}
