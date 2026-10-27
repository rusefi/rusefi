package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Path2D;
import java.awt.geom.RoundRectangle2D;

/**
 * Small line icons for console commands, e.g. in the Messages tab's quick commands. Drawn on a
 * transparent background so they sit cleanly on any row and hover colour [tag:console_theme].
 */
public enum CommandIcon implements Icon {
    /** Status/report commands such as sensorinfo. */
    INFO(new Color(0x1D4ED8)) {
        @Override
        void paint(Graphics2D g) {
            g.draw(new Ellipse2D.Double(1.5, 1.5, 13, 13));
            g.fill(new Ellipse2D.Double(7.1, 4.1, 1.8, 1.8));
            g.draw(new Line(8, 7.5, 8, 11.5));
        }
    },
    HELP(new Color(0x1D4ED8)) {
        @Override
        void paint(Graphics2D g) {
            g.draw(new Ellipse2D.Double(1.5, 1.5, 13, 13));
            Path2D q = new Path2D.Double();
            q.moveTo(6, 6.3);
            q.curveTo(6, 3.8, 10.2, 3.8, 10.2, 6.3);
            q.curveTo(10.2, 7.9, 8, 7.9, 8, 9.6);
            g.draw(q);
            g.fill(new Ellipse2D.Double(7.1, 11, 1.8, 1.8));
        }
    },
    STOP(new Color(0xDC2626)) {
        @Override
        void paint(Graphics2D g) {
            g.fill(new RoundRectangle2D.Double(2.5, 2.5, 11, 11, 3, 3));
        }
    },
    /** Trigger wheel signal: a square pulse train. */
    TRIGGER(new Color(0x586273)) {
        @Override
        void paint(Graphics2D g) {
            Path2D wave = new Path2D.Double();
            wave.moveTo(1, 11.5);
            wave.lineTo(3.5, 11.5);
            wave.lineTo(3.5, 4.5);
            wave.lineTo(7, 4.5);
            wave.lineTo(7, 11.5);
            wave.lineTo(9.5, 11.5);
            wave.lineTo(9.5, 4.5);
            wave.lineTo(13, 4.5);
            wave.lineTo(13, 11.5);
            wave.lineTo(15, 11.5);
            g.draw(wave);
        }
    },
    SD_CARD(new Color(0x586273)) {
        @Override
        void paint(Graphics2D g) {
            Path2D card = new Path2D.Double();
            card.moveTo(3.5, 2);
            card.lineTo(10, 2);
            card.lineTo(12.5, 4.5);
            card.lineTo(12.5, 14);
            card.lineTo(3.5, 14);
            card.closePath();
            g.draw(card);
            g.draw(new Line(6, 4.5, 6, 6.5));
            g.draw(new Line(8, 4.5, 8, 6.5));
            g.draw(new Line(10, 5.5, 10, 6.5));
        }
    },
    /** Write the configuration to flash: an arrow into a tray. */
    SAVE(new Color(0x586273)) {
        @Override
        void paint(Graphics2D g) {
            g.draw(new Line(8, 2, 8, 10));
            Path2D head = new Path2D.Double();
            head.moveTo(4.8, 7);
            head.lineTo(8, 10.2);
            head.lineTo(11.2, 7);
            g.draw(head);
            Path2D tray = new Path2D.Double();
            tray.moveTo(2, 10.5);
            tray.lineTo(2, 14);
            tray.lineTo(14, 14);
            tray.lineTo(14, 10.5);
            g.draw(tray);
        }
    },
    /** enable injection, enable ignition, ... */
    ENABLE(new Color(0x16A34A)) {
        @Override
        void paint(Graphics2D g) {
            g.draw(new Ellipse2D.Double(1.5, 1.5, 13, 13));
            Path2D check = new Path2D.Double();
            check.moveTo(5, 8.2);
            check.lineTo(7.2, 10.4);
            check.lineTo(11.2, 5.8);
            g.draw(check);
        }
    },
    /** disable injection, disable ignition, ... */
    DISABLE(new Color(0xDC2626)) {
        @Override
        void paint(Graphics2D g) {
            g.draw(new Ellipse2D.Double(1.5, 1.5, 13, 13));
            g.draw(new Line(3.9, 12.1, 12.1, 3.9));
        }
    },
    /** sparkbench: a lightning bolt. */
    SPARK(new Color(0xD97706)) {
        @Override
        void paint(Graphics2D g) {
            Path2D bolt = new Path2D.Double();
            bolt.moveTo(9.5, 1.5);
            bolt.lineTo(3.5, 9);
            bolt.lineTo(8, 9);
            bolt.lineTo(6.5, 14.5);
            bolt.lineTo(12.5, 7);
            bolt.lineTo(8, 7);
            bolt.closePath();
            g.draw(bolt);
        }
    },
    /** fuelbench: a fuel drop. */
    FUEL(new Color(0x0891B2)) {
        @Override
        void paint(Graphics2D g) {
            Path2D drop = new Path2D.Double();
            drop.moveTo(8, 1.8);
            drop.curveTo(8, 1.8, 3.2, 7.6, 3.2, 10);
            drop.curveTo(3.2, 12.7, 5.3, 14.5, 8, 14.5);
            drop.curveTo(10.7, 14.5, 12.8, 12.7, 12.8, 10);
            drop.curveTo(12.8, 7.6, 8, 1.8, 8, 1.8);
            drop.closePath();
            g.draw(drop);
        }
    },
    /** Idle control: a dial with its needle. */
    IDLE(new Color(0x586273)) {
        @Override
        void paint(Graphics2D g) {
            g.draw(new java.awt.geom.Arc2D.Double(1.5, 3, 13, 13, -20, 220, java.awt.geom.Arc2D.OPEN));
            g.draw(new Line(8, 9.5, 11, 6));
            g.fill(new Ellipse2D.Double(6.8, 8.3, 2.4, 2.4));
        }
    },
    /** luareset: a circular arrow. */
    RESET(new Color(0x586273)) {
        @Override
        void paint(Graphics2D g) {
            g.draw(new java.awt.geom.Arc2D.Double(2.5, 2.5, 11, 11, 60, 280, java.awt.geom.Arc2D.OPEN));
            Path2D head = new Path2D.Double();
            head.moveTo(10.8, 1.6);
            head.lineTo(11.2, 4.1);
            head.lineTo(8.7, 4.6);
            g.draw(head);
        }
    },
    /** showconfig: lines of a listing. */
    LIST(new Color(0x586273)) {
        @Override
        void paint(Graphics2D g) {
            for (double y = 3.5; y < 14; y += 3) {
                g.fill(new Ellipse2D.Double(1.6, y - 0.9, 1.8, 1.8));
                g.draw(new Line(5.5, y, 14, y));
            }
        }
    },
    /** Start something, e.g. a capture: a play triangle. */
    PLAY(new Color(0x1D4ED8)) {
        @Override
        void paint(Graphics2D g) {
            Path2D play = new Path2D.Double();
            play.moveTo(4.5, 2.5);
            play.lineTo(13, 8);
            play.lineTo(4.5, 13.5);
            play.closePath();
            g.fill(play);
        }
    },
    /** Connect a device: a plug. */
    PLUG(new Color(0x586273)) {
        @Override
        void paint(Graphics2D g) {
            g.draw(new Line(6, 1.5, 6, 4.5));
            g.draw(new Line(10, 1.5, 10, 4.5));
            Path2D body = new Path2D.Double();
            body.moveTo(3.5, 4.5);
            body.lineTo(12.5, 4.5);
            body.lineTo(12.5, 8);
            body.curveTo(12.5, 10.5, 10.5, 12, 8, 12);
            body.curveTo(5.5, 12, 3.5, 10.5, 3.5, 8);
            body.closePath();
            g.draw(body);
            g.draw(new Line(8, 12, 8, 14.5));
        }
    },
    /**
     * Firmware update: a solid green up arrow, the flat form of the console's earlier 48 px upload arrow
     * (upload48.png), so the update action keeps its familiar mark.
     */
    UPLOAD(new Color(0x16A34A)) {
        @Override
        void paint(Graphics2D g) {
            Path2D arrow = new Path2D.Double();
            arrow.moveTo(8, 1.5);
            arrow.lineTo(14.5, 8);
            arrow.lineTo(10.75, 8);
            arrow.lineTo(10.75, 14.5);
            arrow.lineTo(5.25, 14.5);
            arrow.lineTo(5.25, 8);
            arrow.lineTo(1.5, 8);
            arrow.closePath();
            g.fill(arrow);
            // the round-joined outline softens the corners
            g.draw(arrow);
        }
    },
    /** Any other command: a console prompt. */
    COMMAND(new Color(0x586273)) {
        @Override
        void paint(Graphics2D g) {
            Path2D prompt = new Path2D.Double();
            prompt.moveTo(2.5, 4);
            prompt.lineTo(6.5, 8);
            prompt.lineTo(2.5, 12);
            g.draw(prompt);
            g.draw(new Line(8.5, 12.5, 13.5, 12.5));
        }
    };

    static final int SIZE = 16;
    private final Color color;

    CommandIcon(Color color) {
        this.color = color;
    }

    abstract void paint(Graphics2D g);

    @Override
    public void paintIcon(Component c, Graphics g, int x, int y) {
        Graphics2D g2 = (Graphics2D) g.create();
        try {
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setRenderingHint(RenderingHints.KEY_STROKE_CONTROL, RenderingHints.VALUE_STROKE_PURE);
            g2.translate(x, y);
            g2.setColor(c != null && !c.isEnabled() ? UIManager.getColor("Label.disabledForeground") : color);
            g2.setStroke(new BasicStroke(1.5f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
            paint(g2);
        } finally {
            g2.dispose();
        }
    }

    @Override
    public int getIconWidth() {
        return SIZE;
    }

    @Override
    public int getIconHeight() {
        return SIZE;
    }

    private static final class Line extends java.awt.geom.Line2D.Double {
        Line(double x1, double y1, double x2, double y2) {
            super(x1, y1, x2, y2);
        }
    }
}
