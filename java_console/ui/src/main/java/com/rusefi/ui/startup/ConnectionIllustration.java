package com.rusefi.ui.startup;

import javax.accessibility.AccessibleContext;
import javax.accessibility.AccessibleRole;
import javax.swing.*;
import java.awt.*;
import java.awt.geom.Line2D;
import java.awt.geom.RoundRectangle2D;

/**
 * Decorative "laptop — USB cable — ECU" drawing above the Connect controls, in the muted line
 * colour of the theme [tag:console_theme]. Drawn with Java2D so it scales and follows dark mode.
 */
public class ConnectionIllustration extends JComponent {
    private static final int W = 380;
    private static final int H = 96;

    public ConnectionIllustration() {
        Dimension size = new Dimension(W, H);
        setPreferredSize(size);
        setMinimumSize(size);
        setMaximumSize(size);
    }

    @Override
    public AccessibleContext getAccessibleContext() {
        // a plain JComponent has no accessible context of its own
        if (accessibleContext == null) {
            accessibleContext = new AccessibleJComponent() {
                @Override
                public AccessibleRole getAccessibleRole() {
                    return AccessibleRole.ICON;
                }
            };
            accessibleContext.setAccessibleDescription("A laptop connected to an ECU with a USB cable");
        }
        return accessibleContext;
    }

    @Override
    protected void paintComponent(Graphics g) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setRenderingHint(RenderingHints.KEY_STROKE_CONTROL, RenderingHints.VALUE_STROKE_PURE);
            g2.translate((getWidth() - W) / 2.0, (getHeight() - H) / 2.0);
            Color line = StartupCard.themed("Console.Illustration.line", new Color(0x7b8494));
            Color fill = StartupCard.themed("Console.Illustration.fill", new Color(0xf1f3f6));
            g2.setStroke(new BasicStroke(4f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));

            // laptop: screen and base
            RoundRectangle2D screen = new RoundRectangle2D.Float(14, 8, 104, 66, 8, 8);
            g2.setColor(fill);
            g2.fill(screen);
            g2.setColor(line);
            g2.draw(screen);
            g2.draw(new RoundRectangle2D.Float(24, 18, 84, 46, 3, 3));
            g2.fill(new RoundRectangle2D.Float(0, 78, 132, 9, 6, 6));

            // cable and USB plug
            g2.draw(new Line2D.Float(142, 48, 196, 48));
            RoundRectangle2D plug = new RoundRectangle2D.Float(196, 36, 40, 24, 6, 6);
            g2.setColor(fill);
            g2.fill(plug);
            g2.setColor(line);
            g2.draw(plug);
            g2.fill(new RoundRectangle2D.Float(236, 40, 14, 16, 3, 3));
            // USB trident
            g2.setStroke(new BasicStroke(2f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
            g2.draw(new Line2D.Float(204, 48, 228, 48));
            g2.draw(new Line2D.Float(212, 48, 218, 42));
            g2.draw(new Line2D.Float(218, 42, 224, 42));
            g2.draw(new Line2D.Float(214, 48, 220, 54));
            g2.draw(new Line2D.Float(220, 54, 224, 54));

            // ECU box with mounting tabs and a label plate
            g2.setStroke(new BasicStroke(4f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
            RoundRectangle2D ecu = new RoundRectangle2D.Float(272, 14, 96, 68, 14, 14);
            g2.setColor(fill);
            g2.fill(ecu);
            g2.setColor(line);
            g2.draw(ecu);
            g2.setStroke(new BasicStroke(3f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
            g2.draw(new RoundRectangle2D.Float(262, 30, 10, 36, 4, 4));
            g2.draw(new RoundRectangle2D.Float(368, 30, 10, 36, 4, 4));
            g2.draw(new Line2D.Float(296, 36, 344, 36));
            g2.draw(new Line2D.Float(296, 48, 344, 48));
            g2.draw(new Line2D.Float(296, 60, 344, 60));
        } finally {
            g2.dispose();
        }
    }
}
