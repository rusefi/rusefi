package com.rusefi.ui.wizard;

import javax.swing.*;
import java.awt.*;

final class WizardStyle {
    static final int GAP = 12;
    static final int LARGE_GAP = 20;
    static final int ARC = 4;

    static Color color(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }

    static Color background() {
        return color("Panel.background", new Color(0xf5f6f8));
    }

    static Color surface() {
        return color("TextField.background", Color.WHITE);
    }

    static Color border() {
        return color("Component.borderColor", new Color(0xc8ccd2));
    }

    static Color text() {
        return color("Label.foreground", new Color(0x20242a));
    }

    static Color muted() {
        return color("Label.disabledForeground", new Color(0x707780));
    }

    static Color accent() {
        return color("Component.accentColor", new Color(0x2f6fed));
    }

    static Color success() {
        return color("Actions.Green", new Color(0x237a45));
    }

    static Color focus() {
        return color("Component.focusColor", new Color(0x4d86f7));
    }

    private WizardStyle() {
    }
}
