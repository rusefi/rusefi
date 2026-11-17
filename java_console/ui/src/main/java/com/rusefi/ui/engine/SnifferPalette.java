package com.rusefi.ui.engine;

import javax.swing.*;
import java.awt.*;

import static com.rusefi.config.generated.Integration.*;

/**
 * Colours of the Engine Sniffer plot, light or dark independently of the console theme, like the
 * Log Viewer and Knock Analyzer plots. Background and row dividers reuse Console.LogChart.*; the
 * channel and line colours come from Console.EngineSniffer.light.* / .dark.* [tag:console_theme].
 * The constants below are the same values, used when no theme is installed; SnifferPaletteTest keeps
 * both in sync. Channel colours follow the Pinout tab's pin type colours (injector blue, ignition pink).
 */
final class SnifferPalette {
    /** Channel families, picked from the sniffer channel name. */
    enum Role {
        TRIGGER, TACH, TRAILING_COIL, COIL, INJECTOR, MAP, OTHER;

        String key() {
            switch (this) {
                case TRAILING_COIL:
                    return "trailingCoil";
                default:
                    return name().toLowerCase();
            }
        }

        static Role of(String channel) {
            if (channel.startsWith("tach") || channel.startsWith("dizzy"))
                return TACH;
            if (channel.startsWith("t"))
                return TRIGGER;
            if (channel.startsWith("r"))
                return TRAILING_COIL;
            if (channel.startsWith(PROTOCOL_COIL_SHORT_PREFIX))
                return COIL;
            if (channel.startsWith(PROTOCOL_INJ_SHORT_PREFIX) || channel.startsWith(PROTOCOL_INJ_STAGE2_SHORT_PREFIX))
                return INJECTOR;
            if (channel.startsWith("map"))
                return MAP;
            return OTHER;
        }
    }

    static final SnifferPalette LIGHT = new SnifferPalette(false, 0x33,
        new Color(0xffffff), new Color(0xe3e7ed), new Color(0x1f2937), new Color(0x6b7485), new Color(0xb91c1c),
        new Color(0x16a34a), new Color(0xc3cad4),
        new Color(0x7c3aed), new Color(0xd97706), new Color(0xea580c), new Color(0xdb2777),
        new Color(0x2563eb), new Color(0x0d9488), new Color(0x5b6b80));
    static final SnifferPalette DARK = new SnifferPalette(true, 0x4d,
        new Color(0x151b26), new Color(0x374352), new Color(0xe6edf5), new Color(0xaab4c3), new Color(0xf6839d),
        new Color(0x77e2ab), new Color(0x4b5768),
        new Color(0xb09bfb), new Color(0xffbf47), new Color(0xff8f58), new Color(0xf0abfc),
        new Color(0x5bcdfa), new Color(0x76deda), new Color(0xaab4c3));

    final boolean dark;
    /** Opacity of a pulse's fill over the background; its outline is the opaque channel colour. */
    final int fillAlpha;
    final Color background;
    final Color divider;
    final Color text;
    final Color mutedText;
    final Color durationText;
    final Color engineCycle;
    final Color timeScale;
    /** Outline colour per {@link Role}, in declaration order */
    private final Color[] channels;

    private SnifferPalette(boolean dark, int fillAlpha, Color background, Color divider, Color text,
                           Color mutedText, Color durationText, Color engineCycle, Color timeScale,
                           Color... channels) {
        this.dark = dark;
        this.fillAlpha = fillAlpha;
        this.background = background;
        this.divider = divider;
        this.text = text;
        this.mutedText = mutedText;
        this.durationText = durationText;
        this.engineCycle = engineCycle;
        this.timeScale = timeScale;
        this.channels = channels;
    }

    Color outline(Role role) {
        return channels[role.ordinal()];
    }

    Color fill(Role role) {
        Color c = outline(role);
        return new Color(c.getRed(), c.getGreen(), c.getBlue(), fillAlpha);
    }

    static SnifferPalette of(boolean dark) {
        SnifferPalette fallback = dark ? DARK : LIGHT;
        String plot = "Console.LogChart." + (dark ? "dark" : "light") + ".";
        String own = "Console.EngineSniffer." + (dark ? "dark" : "light") + ".";
        Color[] channels = new Color[Role.values().length];
        for (Role role : Role.values()) {
            channels[role.ordinal()] = themed(own + role.key(), fallback.outline(role));
        }
        return new SnifferPalette(dark, fallback.fillAlpha,
            themed(plot + "background", fallback.background),
            themed(plot + "grid", fallback.divider),
            themed(own + "text", fallback.text),
            themed(own + "mutedText", fallback.mutedText),
            themed(own + "durationText", fallback.durationText),
            themed(own + "engineCycle", fallback.engineCycle),
            themed(own + "timeScale", fallback.timeScale),
            channels);
    }

    private static Color themed(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }
}
