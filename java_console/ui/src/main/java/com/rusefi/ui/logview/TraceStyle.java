package com.rusefi.ui.logview;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.Stroke;
import java.awt.geom.Line2D;
import java.util.Locale;
import java.util.Objects;

/**
 * How timeline traces are drawn: a width in pixels (the chart's default or the signal's own) and a solid, dashed
 * or dotted line whose dash, gap or dot spacing follow the width until the user sets them.
 */
final class TraceStyle {
    static final double MIN_WIDTH = 0.5;
    static final double MAX_WIDTH = 8;
    static final double WIDTH_STEP = 0.5;
    static final int MIN_LENGTH = 1;
    static final int MAX_LENGTH = 60;

    private TraceStyle() {
    }

    enum Line {
        SOLID("Solid"),
        DASHED("Dashed"),
        DOTTED("Dotted");

        final String label;

        Line(String label) {
            this.label = label;
        }
    }

    /**
     * One signal's line: its width (NaN: the chart's default), style, and for a dashed line the dash and gap, for a
     * dotted one the space between dots (in {@link #gap}); NaN lengths follow the width. Immutable.
     */
    static final class Spec {
        static final Spec DEFAULT = new Spec(Double.NaN, Line.SOLID, Double.NaN, Double.NaN);

        final double width;
        final Line line;
        final double dash;
        final double gap;

        Spec(double width, Line line, double dash, double gap) {
            this.width = Double.isNaN(width) ? Double.NaN : clampWidth(width);
            this.line = line;
            this.dash = Double.isNaN(dash) ? Double.NaN : clampLength(dash);
            this.gap = Double.isNaN(gap) ? Double.NaN : clampLength(gap);
        }

        Spec withWidth(double width) {
            return new Spec(width, line, dash, gap);
        }

        Spec withLine(Line line) {
            return new Spec(width, line, dash, gap);
        }

        Spec withDash(double dash) {
            return new Spec(width, line, dash, gap);
        }

        Spec withGap(double gap) {
            return new Spec(width, line, dash, gap);
        }

        /** The width in pixels, {@code defaultWidth} when the signal has none of its own. */
        float width(float defaultWidth) {
            return (float) (Double.isNaN(width) ? defaultWidth : width);
        }

        /** The dash length drawn at {@code width}. */
        float dash(float width) {
            return (float) (Double.isNaN(dash) ? autoDash(width) : dash);
        }

        /** The gap (dashed) or the space between dots (dotted) drawn at {@code width}. */
        float gap(float width) {
            return (float) (Double.isNaN(gap) ? autoGap(line, width) : gap);
        }

        Stroke stroke(float width) {
            return TraceStyle.stroke(line, width, dash(width), gap(width));
        }

        boolean isDefault() {
            return equals(DEFAULT);
        }

        /** "DASHED;3;9;auto": line, width, dash, gap; "auto" for NaN. */
        String encode() {
            return line.name() + ';' + number(width) + ';' + number(dash) + ';' + number(gap);
        }

        /** The spec {@link #encode()} wrote, or null for anything else. */
        static Spec decode(String text) {
            if (text == null) {
                return null;
            }
            String[] parts = text.split(";", -1);
            if (parts.length != 4) {
                return null;
            }
            try {
                return new Spec(parse(parts[1]), Line.valueOf(parts[0]), parse(parts[2]), parse(parts[3]));
            } catch (IllegalArgumentException e) {
                return null;
            }
        }

        private static String number(double value) {
            return Double.isNaN(value) ? "auto" : String.format(Locale.ROOT, "%s", value);
        }

        private static double parse(String text) {
            if ("auto".equals(text)) {
                return Double.NaN;
            }
            double value = Double.parseDouble(text);
            if (!Double.isFinite(value)) {
                throw new IllegalArgumentException(text);
            }
            return value;
        }

        @Override
        public boolean equals(Object o) {
            if (!(o instanceof Spec)) {
                return false;
            }
            Spec other = (Spec) o;
            return Double.compare(width, other.width) == 0 && line == other.line
                    && Double.compare(dash, other.dash) == 0 && Double.compare(gap, other.gap) == 0;
        }

        @Override
        public int hashCode() {
            return Objects.hash(width, line, dash, gap);
        }

        @Override
        public String toString() {
            return encode();
        }
    }

    static double clampWidth(double width) {
        return Math.max(MIN_WIDTH, Math.min(MAX_WIDTH, width));
    }

    static double clampLength(double length) {
        return Math.max(MIN_LENGTH, Math.min(MAX_LENGTH, length));
    }

    /** The dash length until the user sets one: grows with the width, so a thick dashed line keeps its look. */
    static float autoDash(float width) {
        return Math.round(3 * width + 3);
    }

    /** The gap (dashed) or space between dots (dotted) until the user sets one. */
    static float autoGap(Line line, float width) {
        return Math.round(line == Line.DOTTED ? width + 2 : 2 * width + 2);
    }

    /**
     * The stroke of a trace {@code width} pixels wide. A dash is {@code dash} long with {@code gap} between dashes;
     * a dot is as wide as the line, with {@code gap} between dots. A solid line keeps the plain stroke.
     */
    static Stroke stroke(Line line, float width, float dash, float gap) {
        switch (line) {
            case DASHED:
                return new BasicStroke(width, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND, 10,
                        new float[]{dash, gap}, 0);
            case DOTTED:
                // round caps grow a near-zero dash into a dot as wide as the line, so dots are gap + width apart
                return new BasicStroke(width, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND, 10,
                        new float[]{0.01f, gap + width}, 0);
            default:
                return new BasicStroke(width);
        }
    }

    /**
     * The legend mark: a {@code width} x {@code height} bar for a solid line, two dashes for a dashed one, three
     * dots for a dotted one, so a line can be matched to its signal by style as well as colour.
     */
    static void paintMark(Graphics2D g, Line line, int x, int y, int width, int height) {
        switch (line) {
            case DASHED: {
                int gap = 3;
                int dash = (width - gap) / 2;
                g.fillRoundRect(x, y, dash, height, 3, 3);
                g.fillRoundRect(x + width - dash, y, dash, height, 3, 3);
                break;
            }
            case DOTTED: {
                // small enough that three fit along the mark with gaps between them
                int dot = Math.min(height, Math.max(2, (width - 4) / 3));
                int dotY = y + (height - dot) / 2;
                for (int i = 0; i < 3; i++) {
                    int dotX = x + Math.round(i * (width - dot) / 2f);
                    g.fillOval(dotX, dotY, dot, dot);
                }
                break;
            }
            default:
                g.fillRoundRect(x, y, width, height, 4, 4);
        }
    }

    /** A sample of a line drawn with its real stroke, e.g. under the line options. */
    static final class Sample extends javax.swing.JComponent {
        /** The signal's colour, set before the first paint; nothing is drawn without it. */
        private Color color;
        private Stroke stroke = new BasicStroke(1);

        void set(Color color, Stroke stroke) {
            this.color = color;
            this.stroke = stroke;
            repaint();
        }

        @Override
        public java.awt.Dimension getPreferredSize() {
            return new java.awt.Dimension(200, 18);
        }

        @Override
        protected void paintComponent(Graphics g) {
            if (color == null) {
                return;
            }
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setColor(color);
                g2.setStroke(stroke);
                float middle = getHeight() / 2f;
                g2.draw(new Line2D.Float(4, middle, getWidth() - 4, middle));
            } finally {
                g2.dispose();
            }
        }
    }
}
