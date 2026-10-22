package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;

import java.awt.Color;
import java.util.ArrayList;
import java.util.List;

/**
 * Colours the user chose for signals, kept by signal label in the console config so a signal has the same colour
 * in every log. A choice is a palette swatch, kept as its index so it follows the Light / Dark plot, or a custom
 * colour, which stays the same on both. Signals without a choice get an automatic colour ({@link TraceColors}).
 */
final class SignalColors {
    private static final String SWATCH = "swatch:";

    private final Node node;
    private final List<Runnable> listeners = new ArrayList<>();

    /** Choices that are forgotten with the viewer, e.g. in the sandbox. */
    SignalColors() {
        this(new Node());
    }

    SignalColors(Node node) {
        this.node = node;
    }

    /** Called after every change, e.g. to repaint and save the config. */
    void addListener(Runnable listener) {
        listeners.add(listener);
    }

    private void changed() {
        for (Runnable listener : listeners) {
            listener.run();
        }
    }

    /** The chosen colour of a signal on the given plot palette, or null for automatic. */
    Color resolve(String label, PlotPalette palette) {
        int swatch = swatch(label);
        return swatch >= 0 ? palette.trace(swatch) : custom(label);
    }

    /** The chosen palette swatch, or -1. */
    int swatch(String label) {
        String value = node.getProperty(label, null);
        if (value == null || !value.startsWith(SWATCH)) {
            return -1;
        }
        try {
            int swatch = Integer.parseInt(value.substring(SWATCH.length()));
            return swatch >= 0 && swatch < PlotPalette.SWATCH_COUNT ? swatch : -1;
        } catch (NumberFormatException corrupt) {
            return -1;
        }
    }

    /** The chosen custom colour, or null. */
    Color custom(String label) {
        String value = node.getProperty(label, null);
        if (value == null || !value.startsWith("#")) {
            return null;
        }
        try {
            return new Color(Integer.parseInt(value.substring(1), 16));
        } catch (NumberFormatException corrupt) {
            return null;
        }
    }

    void setSwatch(String label, int swatch) {
        node.setProperty(label, SWATCH + swatch);
        changed();
    }

    void setCustom(String label, Color color) {
        node.setProperty(label, String.format("#%06x", color.getRGB() & 0xffffff));
        changed();
    }

    /** Back to the automatic colour. */
    void setAutomatic(String label) {
        node.getConfig().remove(label);
        changed();
    }
}
