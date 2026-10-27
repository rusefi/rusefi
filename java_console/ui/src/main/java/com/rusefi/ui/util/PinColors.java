package com.rusefi.ui.util;

import javax.swing.UIManager;
import java.awt.Color;
import java.util.Locale;
import java.util.regex.Pattern;

/**
 * Pin marker colours for the Pinout tab. Every pin {@code type} the boards' connector YAMLs use maps to a
 * {@link PinType}, coloured by family so related pins read together: power reds, grounds greys, analog
 * inputs teals, speed and switch inputs purples, outputs, communication. A pin without a type takes one from
 * its function or class where that is clear ({@link #resolve}), marked as inferred. Colours come from the
 * Console.Pinout.type.* theme keys [tag:console_theme], with the light theme's values as fallbacks.
 * Pigtail colours are resolved from the wire colour strings in the connector YAMLs.
 */
public final class PinColors {

    private PinColors() {}

    /** What a pin is for, as the Pinout tab colours and names it. */
    public enum PinType {
        POWER("12V supply", "power", "#dc2626", "12v", "vign", "mr"),
        SUPPLY_5V("5V supply", "supply5v", "#ea580c", "5v"),
        GROUND("Power ground", "ground", "#334155", "gnd", "pgnd"),
        SENSOR_GROUND("Sensor ground", "sensorGround", "#94a3b8", "agnd", "sgnd", "sqnd"),
        ANALOG("Analog input", "analog", "#0d9488", "av", "tps"),
        TEMPERATURE("Temperature input", "temperature", "#0891b2", "at"),
        WIDEBAND("Wideband O2", "wideband", "#4f46e5", "wbo"),
        HALL("Hall / digital input", "hall", "#7c3aed", "hall"),
        VR("VR input", "vr", "#a21caf", "vr"),
        SWITCH("Switch input", "switchInput", "#c4b5fd", "din"),
        INJECTOR("Injector", "injector", "#2563eb", "inj"),
        IGNITION("Ignition coil", "ignition", "#db2777", "ign"),
        LOW_SIDE("Low-side output", "lowSide", "#16a34a", "ls", "gp_low", "mil"),
        HIGH_SIDE("High-side output", "highSide", "#a16207", "hs", "gp_high", "gp_pp"),
        HBRIDGE("H-bridge / ETB", "hbridge", "#92400e", "etb"),
        CAN("CAN bus", "can", "#0284c7", "can"),
        USB("USB", "usb", "#0369a1", "usb"),
        /** The board file gives no type and none can be inferred. */
        UNTYPED("Untyped", "untyped", "#e5e7eb");

        public final String label;
        private final String themeKey;
        private final Color fallback;
        private final String[] keywords;

        PinType(String label, String themeKey, String fallback, String... keywords) {
            this.label = label;
            this.themeKey = "Console.Pinout.type." + themeKey;
            this.fallback = Color.decode(fallback);
            this.keywords = keywords;
        }

        public Color color() {
            Color themed = UIManager.getColor(themeKey);
            return themed != null ? themed : fallback;
        }

        /** Marker text colour: dark on the light fills, white otherwise. */
        public Color textColor() {
            return isLight(color()) ? new Color(0x1f2937) : Color.WHITE;
        }

        /** The type a YAML {@code type} names, or null for an unknown or empty one. */
        public static PinType of(String yamlType) {
            String token = yamlType == null ? "" : yamlType.toLowerCase(Locale.ROOT)
                    // a list such as "[gp_low, gp_high]" counts as its first entry
                    .replaceAll("[\\[\\]]", "").split(",")[0].trim();
            if (token.isEmpty()) {
                return null;
            }
            for (PinType type : values()) {
                for (String keyword : type.keywords) {
                    if (keyword.equals(token)) {
                        return type;
                    }
                }
            }
            return null;
        }
    }

    /** A pin's type and whether it came from the YAML or was inferred. */
    public static final class Resolved {
        public final PinType type;
        /** "function" or "class" when inferred from those, else null. */
        public final String inferredFrom;

        Resolved(PinType type, String inferredFrom) {
            this.type = type;
            this.inferredFrom = inferredFrom;
        }

        public boolean isInferred() {
            return inferredFrom != null;
        }
    }

    /**
     * Function words that say what a pin is, checked in order; the first match wins. Grounds come first
     * ("Power Ground / VR Shield" is a ground), then signals; power words only after the pin's class
     * ({@link #POWER_RULES}), since "Battery Voltage Analog Input" is an analog input.
     */
    private static final Object[][] FUNCTION_RULES = {
            {"\\bsensor (ground|gnd)\\b|\\bsignal (ground|gnd)\\b|\\bagnd\\b", PinType.SENSOR_GROUND},
            {"\\bground\\b|\\bgnd\\b", PinType.GROUND},
            {"\\bvr\\s?\\d*[+-]?(\\s|$|/)|\\bvariable reluctance\\b", PinType.VR},
            {"\\bhall\\b|\\bcam\\b|\\bcrank\\b|\\bvss\\b", PinType.HALL},
            {"\\bwideband\\b|\\bwbo\\b|\\blsu\\b", PinType.WIDEBAND},
            {"\\binjector\\b|\\binj\\b", PinType.INJECTOR},
            {"\\bignition (coil|output)\\b|\\bcoil\\b", PinType.IGNITION},
            {"\\bh-?bridge\\b|\\betb\\b|\\bdc[12][+-]", PinType.HBRIDGE},
            {"\\bhigh[- ]side\\b", PinType.HIGH_SIDE},
            {"\\blow[- ]side\\b", PinType.LOW_SIDE},
            {"\\bcan ?(h|l|bus|high|low)\\b", PinType.CAN},
            {"\\busb\\b", PinType.USB},
            {"\\b(clt|iat|temperature|temp)\\b", PinType.TEMPERATURE},
    };

    /** Power words, checked after the class. */
    private static final Object[][] POWER_RULES = {
            {"\\+?5 ?v\\b.*\\b(power|supply|feed|out|reference)\\b|\\bsensor supply\\b", PinType.SUPPLY_5V},
            {"\\+?12 ?v\\b.*\\b(power|supply|feed|relay)\\b|\\bmain relay\\b|\\bbattery\\b", PinType.POWER},
    };

    /** Classes, by their YAML names, that say what a pin is when its function does not. */
    private static final Object[][] CLASS_RULES = {
            {"switch_inputs", PinType.SWITCH},
            {"event_inputs", PinType.HALL},
            {"analog_inputs", PinType.ANALOG},
    };

    private static final Pattern[] FUNCTION_PATTERNS = compile(FUNCTION_RULES);
    private static final Pattern[] POWER_PATTERNS = compile(POWER_RULES);

    private static Pattern[] compile(Object[][] rules) {
        Pattern[] patterns = new Pattern[rules.length];
        for (int i = 0; i < rules.length; i++) {
            patterns[i] = Pattern.compile((String) rules[i][0], Pattern.CASE_INSENSITIVE);
        }
        return patterns;
    }

    /**
     * The type of a pin: its YAML {@code type} when that is known, else one inferred from the words of its
     * function, its class (a pin with several classes takes the first rule that matches: a pin that is both a
     * switch and an event input is shown as a switch input) or power words in its function, in that order
     * (see {@link #FUNCTION_RULES}), else {@link PinType#UNTYPED}.
     *
     * @param classes the YAML {@code class}, comma separated when a list
     */
    public static Resolved resolve(String yamlType, String function, String classes) {
        PinType declared = PinType.of(yamlType);
        if (declared != null) {
            return new Resolved(declared, null);
        }
        String text = function == null ? "" : function;
        for (int i = 0; i < FUNCTION_PATTERNS.length; i++) {
            if (FUNCTION_PATTERNS[i].matcher(text).find()) {
                return new Resolved((PinType) FUNCTION_RULES[i][1], "function");
            }
        }
        String cls = classes == null ? "" : classes.toLowerCase(Locale.ROOT);
        for (Object[] rule : CLASS_RULES) {
            if (cls.contains((String) rule[0])) {
                return new Resolved((PinType) rule[1], "class");
            }
        }
        for (int i = 0; i < POWER_PATTERNS.length; i++) {
            if (POWER_PATTERNS[i].matcher(text).find()) {
                return new Resolved((PinType) POWER_RULES[i][1], "function");
            }
        }
        return new Resolved(PinType.UNTYPED, null);
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
            case "black":  return new Color(0x111827);
            case "red":    return new Color(0xdc2626);
            case "white":  return new Color(0xffffff);
            case "yellow": return new Color(0xfacc15);
            case "green":  return new Color(0x16a34a);
            case "blue":   return new Color(0x2563eb);
            case "orange": return new Color(0xf97316);
            case "purple":
            case "violet": return new Color(0x7e22ce);
            case "brown":  return new Color(0x92400e);
            case "grey":
            case "gray":   return new Color(0x9ca3af);
            case "pink":   return new Color(0xf9a8d4);
            default:       return null;
        }
    }

    /** Returns true if the color is perceived as light (needs dark text for contrast). */
    public static boolean isLight(Color c) {
        double lum = 0.299 * c.getRed() + 0.587 * c.getGreen() + 0.114 * c.getBlue();
        return lum > 160;
    }
}
