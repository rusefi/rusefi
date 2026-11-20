package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.Field;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.geom.Path2D;

/**
 * [tag:unburned_marks] Marks table and curve cells whose value differs from the last burned tune
 * (#10671): a warning-coloured triangle in the cell's top-left corner, edged so it stays visible on
 * every gradient colour and on the orange selection, and a tooltip with the burned value. The
 * triangle is a border wrapped around the renderer's own, so the cell keeps its gradient and text.
 */
final class UnburnedCellMark {
    /** Size of the coloured triangle; the edge adds {@link #EDGE} around it. */
    static final int SIZE = 12;
    private static final int EDGE = 2;
    /**
     * Below this contrast with the cell the warning colour is replaced by the cell's text colour. The dark
     * theme's amber is nearly the orange of selected cells and of high gradient values.
     */
    static final double MIN_CONTRAST = 2.5;

    private UnburnedCellMark() {
    }

    /**
     * Wraps or unwraps the corner mark on a (reused) renderer component.
     *
     * @param burnedText the burned value for the tooltip, or null to clear the mark
     */
    static void apply(Component renderer, @Nullable String nowText, @Nullable String burnedText) {
        apply(renderer, nowText, burnedText, UnburnedWording.BURNED);
    }

    static void apply(Component renderer, @Nullable String nowText, @Nullable String burnedText,
                      UnburnedWording wording) {
        if (!(renderer instanceof JComponent)) {
            return;
        }
        JComponent component = (JComponent) renderer;
        Border border = component.getBorder();
        if (border instanceof Corner) {
            border = ((Corner) border).inner;
        }
        boolean marked = burnedText != null;
        component.setBorder(marked ? new Corner(border) : border);
        component.setToolTipText(marked ? tooltip(nowText, burnedText, wording) : null);
    }

    static boolean isMarked(Component renderer) {
        return renderer instanceof JComponent && ((JComponent) renderer).getBorder() instanceof Corner;
    }

    static String tooltip(@Nullable String nowText, String burnedText, UnburnedWording wording) {
        String now = nowText == null ? "" : "Now: " + escape(nowText) + "<br>";
        return "<html>" + now + wording.reference + ": " + escape(burnedText)
            + "<br><i>" + wording.cellNote + "</i></html>";
    }

    /** One stored cell of {@code field} as a number in the burned tune. */
    static double burnedValue(ArrayIniField field, int row, int col, ConfigurationImage burned) {
        return new Field(field.getName(), field.getOffset(row, col), field.getType())
            .getValue(burned, field.getMultiplier());
    }

    /** The label beside a table or curve: "3 cells not burned", hidden when nothing is changed. */
    static JLabel createCountLabel() {
        JLabel label = new JLabel();
        label.setName("unburnedCellCount");
        label.setForeground(UnburnedRowMark.foreground());
        label.setVisible(false);
        return label;
    }

    static void updateCountLabel(JLabel label, int changedCells) {
        label.setText(countText(changedCells));
        label.setVisible(changedCells > 0);
    }

    static String countText(int changedCells) {
        if (changedCells <= 0) {
            return "";
        }
        return changedCells + (changedCells == 1 ? " cell not burned" : " cells not burned");
    }

    /**
     * The triangle's colour on a cell: the warning colour, or the cell's black or white text colour when
     * the warning colour would not stand out from the cell (selected orange, orange or dark gradient).
     */
    static Color markColor(Color cellBackground, Color warning) {
        if (cellBackground == null || contrast(warning, cellBackground) >= MIN_CONTRAST) {
            return warning;
        }
        return CalibrationColorPalette.foreground(cellBackground);
    }

    /** WCAG contrast ratio of two colours, 1 to 21. */
    static double contrast(Color a, Color b) {
        double la = luminance(a);
        double lb = luminance(b);
        return (Math.max(la, lb) + 0.05) / (Math.min(la, lb) + 0.05);
    }

    private static double luminance(Color color) {
        return 0.2126 * channel(color.getRed()) + 0.7152 * channel(color.getGreen()) + 0.0722 * channel(color.getBlue());
    }

    private static double channel(int value) {
        double c = value / 255.0;
        return c <= 0.03928 ? c / 12.92 : Math.pow((c + 0.055) / 1.055, 2.4);
    }

    private static String escape(String text) {
        return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;");
    }

    /** The triangle, drawn over the cell after its own border. */
    static final class Corner implements Border {
        final Border inner;

        Corner(Border inner) {
            this.inner = inner;
        }

        @Override
        public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
            if (inner != null) {
                inner.paintBorder(c, g, x, y, width, height);
            }
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setColor(edge(c));
                g2.fill(triangle(x, y, SIZE + EDGE));
                g2.setColor(markColor(c.getBackground(), UnburnedRowMark.bar()));
                g2.fill(triangle(x, y, SIZE));
            } finally {
                g2.dispose();
            }
        }

        /** The table's background, so the edge separates the mark from any cell colour. */
        private static Color edge(Component c) {
            Container parent = c.getParent();
            while (parent != null && !(parent instanceof JTable) && !(parent instanceof JComponent && parent.isOpaque())) {
                parent = parent.getParent();
            }
            Color background = parent != null ? parent.getBackground() : null;
            return background != null ? background : UIManager.getColor("Table.background");
        }

        private static Shape triangle(int x, int y, int size) {
            Path2D.Double path = new Path2D.Double();
            path.moveTo(x, y);
            path.lineTo(x + size, y);
            path.lineTo(x, y + size);
            path.closePath();
            return path;
        }

        @Override
        public Insets getBorderInsets(Component c) {
            return inner != null ? inner.getBorderInsets(c) : new Insets(0, 0, 0, 0);
        }

        @Override
        public boolean isBorderOpaque() {
            return inner != null && inner.isBorderOpaque();
        }
    }
}
