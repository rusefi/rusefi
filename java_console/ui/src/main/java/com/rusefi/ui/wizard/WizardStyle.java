package com.rusefi.ui.wizard;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Path2D;

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

    /** Shapes keep confirmation and cancellation distinguishable without relying on colour. */
    enum ActionIcon implements Icon {
        CONFIRM, CANCEL;

        @Override
        public void paintIcon(Component component, Graphics graphics, int x, int y) {
            Graphics2D g = (Graphics2D) graphics.create();
            try {
                g.translate(x, y);
                g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g.setColor(component.getForeground());
                g.setStroke(new BasicStroke(2, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
                if (this == CONFIRM) {
                    Path2D check = new Path2D.Float();
                    check.moveTo(3, 8);
                    check.lineTo(6, 11);
                    check.lineTo(13, 4);
                    g.draw(check);
                } else {
                    g.drawLine(4, 4, 12, 12);
                    g.drawLine(12, 4, 4, 12);
                }
            } finally {
                g.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return 16;
        }

        @Override
        public int getIconHeight() {
            return 16;
        }
    }

    private WizardStyle() {
    }
}
