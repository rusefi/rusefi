package com.rusefi.ui.widgets.tune;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class Surface3DView extends JPanel {
    static final double DEFAULT_YAW = Math.toRadians(45);
    static final double DEFAULT_PITCH = Math.toRadians(30);

    private Double[][] data;
    private Double[] xBins;
    private Double[] yBins;
    private double[] xPositions;
    private double[] yPositions;
    private double minValue;
    private double maxValue;
    /** Selected cells by data row and column, as in the table; null when nothing is selected. */
    private boolean[][] selection;
    private String xTitle;
    private String yTitle;
    private String zTitle;

    private double yaw = DEFAULT_YAW;
    private double pitch = DEFAULT_PITCH;
    private Point lastMousePos;

    private static final double ZOOM = 0.95;
    // Projected half extents of the [-0.5, 0.5] cube: yaw spreads x and y over sqrt(0.5),
    // then pitch mixes that with z over sqrt(0.5 + 0.25).
    private static final double MAX_HALF_WIDTH = Math.sqrt(0.5);
    private static final double MAX_HALF_HEIGHT = Math.sqrt(0.75);
    /** Axis labels sit this far outside the cube, in cube units. */
    static final double LABEL_OFFSET = 0.1;
    /** Labels are centered on their anchor; keep room for half a label beyond the anchors. */
    static final int LABEL_HALF_WIDTH = 28;
    static final int LABEL_HALF_HEIGHT = 8;
    /** The live point is white, unlike the selection-coloured selected points, so the two never look alike. */
    static final Color LIVE_POINT_COLOR = Color.WHITE; // [tag:fixed_color] ringed in black below
    private static final int LIVE_POINT_RADIUS = 5;
    /** Live operating point trail as {x index, y index, opacity}, oldest first; null for none. */
    private double[][] liveTrail;

    public Surface3DView() {
        setBackground(TuningStyle.color("Console.Tuning.surface.background", Color.WHITE));
        setPreferredSize(new Dimension(400, 400));
        MouseAdapter mouseAdapter = new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                lastMousePos = e.getPoint();
            }

            @Override
            public void mouseDragged(MouseEvent e) {
                if (lastMousePos != null) {
                    int dx = e.getX() - lastMousePos.x;
                    int dy = e.getY() - lastMousePos.y;
                    yaw += Math.toRadians(dx * 0.5);
                    pitch += Math.toRadians(dy * 0.5);
                    lastMousePos = e.getPoint();
                    repaint();
                }
            }
        };
        addMouseListener(mouseAdapter);
        addMouseMotionListener(mouseAdapter);
    }

    public void setData(Double[][] data, Double[] xBins, Double[] yBins, double minValue, double maxValue) {
        this.data = data;
        this.xBins = xBins;
        this.yBins = yBins;
        this.xPositions = axisPositions(xBins, data.length == 0 ? 0 : data[0].length);
        this.yPositions = axisPositions(yBins, data.length);
        this.minValue = minValue;
        this.maxValue = maxValue;
        repaint();
    }

    /** Names appended to each axis' last value; null or empty for none. */
    public void setAxisTitles(String xTitle, String yTitle, String zTitle) {
        this.xTitle = xTitle;
        this.yTitle = yTitle;
        this.zTitle = zTitle;
        repaint();
    }

    /** Selected cells indexed like the data passed to {@link #setData}; null for none. */
    public void setSelection(boolean[][] selection) {
        this.selection = selection;
        repaint();
    }

    /**
     * The live operating point and its recent trail (issue #10670), oldest first and ending at the
     * current point, as {x index, y index, opacity} with fractional indexes into the data passed to
     * {@link #setData}; null for none.
     */
    public void setLiveTrail(double[][] trail) {
        this.liveTrail = trail;
        repaint();
    }

    double[][] liveTrailForUnitTest() {
        return liveTrail;
    }

    boolean isSelected(int row, int column) {
        return selection != null && row < selection.length && column < selection[row].length
            && selection[row][column];
    }

    static double[] axisPositions(Double[] bins, int count) {
        double[] positions = new double[count];
        boolean valid = hasUsableAxis(bins, count);
        for (int i = 0; i < count; i++) {
            positions[i] = valid ? (bins[i] - bins[0]) / (bins[count - 1] - bins[0]) - 0.5
                : (count > 1 ? (double) i / (count - 1) - 0.5 : 0);
        }
        return positions;
    }

    static boolean hasUsableAxis(Double[] bins, int count) {
        if (count < 2 || bins == null || bins.length != count) {
            return false;
        }
        for (Double bin : bins) {
            if (bin == null || !Double.isFinite(bin)) {
                return false;
            }
        }
        double span = bins[count - 1] - bins[0];
        if (!Double.isFinite(span) || span == 0) {
            return false;
        }
        for (int i = 1; i < count; i++) {
            if ((span > 0 && bins[i] <= bins[i - 1]) || (span < 0 && bins[i] >= bins[i - 1])) {
                return false;
            }
        }
        return true;
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        if (data == null || data.length == 0 || data[0].length == 0) return;

        Graphics2D g2d = (Graphics2D) g;
        g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

        int rows = data.length;
        int cols = data[0].length;

        int width = getWidth();
        int height = getHeight();
        int centerX = width / 2;
        int centerY = height / 2;

        double scale = fitScale(width, height);

        Point3D[][] points = new Point3D[rows][cols];
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                double x = xPositions[c];
                double y = yPositions[r];
                double z = 0;
                if (maxValue > minValue) {
                    z = (data[r][c] - minValue) / (maxValue - minValue) - 0.5;
                }
                points[r][c] = project(x, y, z, yaw, pitch, scale, centerX, centerY);
            }
        }

        for (int r = 0; r < rows - 1; r++) {
            for (int c = 0; c < cols - 1; c++) {
                Point3D p1 = points[r][c];
                Point3D p2 = points[r + 1][c];
                Point3D p3 = points[r + 1][c + 1];
                Point3D p4 = points[r][c + 1];

                double avgZ = (data[r][c] + data[r + 1][c] + data[r + 1][c + 1] + data[r][c + 1]) / 4.0;
                // A cell is a surface point shared by up to four facets; selecting it lights them all.
                boolean selected = isSelected(r, c) || isSelected(r + 1, c)
                    || isSelected(r + 1, c + 1) || isSelected(r, c + 1);
                Color facet = CalibrationColorPalette.background(avgZ, minValue, maxValue);
                // As in tables: the value colour under the selection tint, outlined in the selection colour.
                g2d.setColor(selected ? CalibrationColorPalette.selected(facet) : facet);

                int[] xPoints = {p1.x, p2.x, p3.x, p4.x};
                int[] yPoints = {p1.y, p2.y, p3.y, p4.y};
                g2d.fillPolygon(xPoints, yPoints, 4);

                if (selected) {
                    g2d.setColor(CalibrationColorPalette.selection());
                } else {
                    g2d.setColor(Color.DARK_GRAY); // [tag:fixed_color] facet edges between value colours
                }
                g2d.drawPolygon(xPoints, yPoints, 4);
            }
        }

        paintAxes(g2d, scale, centerX, centerY);
        paintSelectedPoints(g2d, points);
        paintLiveTrail(g2d, scale, centerX, centerY);
    }

    /**
     * Draws the trail on the surface, at the height the firmware interpolates between the four
     * surrounding cells, fading with age, and the current point last and largest.
     */
    private void paintLiveTrail(Graphics2D g2d, double scale, int centerX, int centerY) {
        if (liveTrail == null) {
            return;
        }
        Stroke stroke = g2d.getStroke();
        Point3D previous = null;
        for (int i = 0; i < liveTrail.length; i++) {
            double[] point = liveTrail[i];
            Point3D projected = liveProjection(point[0], point[1], scale, centerX, centerY);
            if (projected == null) {
                previous = null;
                continue;
            }
            int alpha = (int) Math.round(255 * Math.max(0, Math.min(1, point[2])));
            boolean current = i == liveTrail.length - 1;
            if (previous != null && alpha > 0) {
                g2d.setStroke(new BasicStroke(3f));
                g2d.setColor(new Color(0, 0, 0, alpha)); // [tag:fixed_color] trail edge, like the live point ring
                g2d.drawLine(previous.x, previous.y, projected.x, projected.y);
                g2d.setStroke(new BasicStroke(1f));
                g2d.setColor(new Color(255, 255, 255, alpha)); // [tag:fixed_color] trail core, like the live point
                g2d.drawLine(previous.x, previous.y, projected.x, projected.y);
            }
            int radius = current ? LIVE_POINT_RADIUS : 3;
            if (current || alpha > 0) {
                g2d.setStroke(new BasicStroke(current ? 2f : 1f));
                g2d.setColor(current ? LIVE_POINT_COLOR
                    : new Color(255, 255, 255, alpha)); // [tag:fixed_color] faded trail dot
                g2d.fillOval(projected.x - radius, projected.y - radius, 2 * radius, 2 * radius);
                g2d.setColor(current ? Color.BLACK // [tag:fixed_color] ring of the live point
                    : new Color(0, 0, 0, alpha)); // [tag:fixed_color] ring of a faded trail dot
                g2d.drawOval(projected.x - radius, projected.y - radius, 2 * radius, 2 * radius);
            }
            previous = projected;
        }
        g2d.setStroke(stroke);
    }

    /** Screen position of fractional data indexes, on the bilinearly interpolated surface; null if unknown. */
    private Point3D liveProjection(double xIndex, double yIndex, double scale, int centerX, int centerY) {
        int rows = data.length;
        int cols = data[0].length;
        if (!(xIndex >= 0 && xIndex <= cols - 1 && yIndex >= 0 && yIndex <= rows - 1)) {
            return null;
        }
        int c0 = (int) Math.floor(xIndex);
        int r0 = (int) Math.floor(yIndex);
        int c1 = Math.min(c0 + 1, cols - 1);
        int r1 = Math.min(r0 + 1, rows - 1);
        double fx = xIndex - c0;
        double fy = yIndex - r0;
        if (data[r0][c0] == null || data[r0][c1] == null || data[r1][c0] == null || data[r1][c1] == null) {
            return null;
        }
        double value = (1 - fy) * ((1 - fx) * data[r0][c0] + fx * data[r0][c1])
            + fy * ((1 - fx) * data[r1][c0] + fx * data[r1][c1]);
        double x = xPositions[c0] + fx * (xPositions[c1] - xPositions[c0]);
        double y = yPositions[r0] + fy * (yPositions[r1] - yPositions[r0]);
        double z = maxValue > minValue ? (value - minValue) / (maxValue - minValue) - 0.5 : 0;
        return project(x, y, z, yaw, pitch, scale, centerX, centerY);
    }

    /** A selected cell's own point: the selection colour, ringed in white. */
    static Color selectedPointColor() {
        return CalibrationColorPalette.selection();
    }

    /** Marks each selected cell's own point in the middle of its lit facets. */
    private void paintSelectedPoints(Graphics2D g2d, Point3D[][] points) {
        int radius = 3;
        for (int r = 0; r < points.length; r++) {
            for (int c = 0; c < points[r].length; c++) {
                if (isSelected(r, c)) {
                    Point3D point = points[r][c];
                    g2d.setColor(selectedPointColor());
                    g2d.fillOval(point.x - radius, point.y - radius, 2 * radius, 2 * radius);
                    g2d.setColor(Color.WHITE); // [tag:fixed_color] ring of the selected point
                    g2d.drawOval(point.x - radius - 1, point.y - radius - 1, 2 * radius + 2, 2 * radius + 2);
                }
            }
        }
    }

    /**
     * Axis lines along the box edges through the lowest x, y and value corner, labelled with the
     * first, middle and last bins, and the axis title after the last one.
     */
    private void paintAxes(Graphics2D g2d, double scale, int centerX, int centerY) {
        Point3D origin = project(-0.5, -0.5, -0.5, yaw, pitch, scale, centerX, centerY);
        Point3D xEnd = project(0.5, -0.5, -0.5, yaw, pitch, scale, centerX, centerY);
        Point3D yEnd = project(-0.5, 0.5, -0.5, yaw, pitch, scale, centerX, centerY);
        Point3D zEnd = project(-0.5, -0.5, 0.5, yaw, pitch, scale, centerX, centerY);
        g2d.setColor(TuningStyle.color("Console.Tuning.surface.axis", Color.DARK_GRAY));
        g2d.setStroke(new BasicStroke(1.5f));
        g2d.drawLine(origin.x, origin.y, xEnd.x, xEnd.y);
        g2d.drawLine(origin.x, origin.y, yEnd.x, yEnd.y);
        g2d.drawLine(origin.x, origin.y, zEnd.x, zEnd.y);
        g2d.setStroke(new BasicStroke(1));
        g2d.setFont(getFont().deriveFont(10f));

        int columns = xPositions.length;
        for (int index : labelIndexes(columns)) {
            String text = bin(xBins, index, columns) + title(xTitle, index == columns - 1);
            drawLabel(g2d, text, project(xPositions[index], -0.5 - LABEL_OFFSET, -0.5,
                yaw, pitch, scale, centerX, centerY));
        }
        int rows = yPositions.length;
        for (int index : labelIndexes(rows)) {
            String text = bin(yBins, index, rows) + title(yTitle, index == rows - 1);
            drawLabel(g2d, text, project(-0.5 - LABEL_OFFSET, yPositions[index], -0.5,
                yaw, pitch, scale, centerX, centerY));
        }
        double[] values = maxValue > minValue
            ? new double[]{minValue, (minValue + maxValue) / 2, maxValue} : new double[]{minValue};
        for (int i = 0; i < values.length; i++) {
            double z = values.length == 1 ? -0.5 : (double) i / (values.length - 1) - 0.5;
            String text = formatValue(values[i]) + title(zTitle, i == values.length - 1);
            drawLabel(g2d, text, project(-0.5 - LABEL_OFFSET, -0.5 - LABEL_OFFSET, z,
                yaw, pitch, scale, centerX, centerY));
        }
    }

    private static int[] labelIndexes(int count) {
        if (count <= 1) {
            return new int[]{0};
        }
        return count == 2 ? new int[]{0, 1} : new int[]{0, (count - 1) / 2, count - 1};
    }

    private static String bin(Double[] bins, int index, int count) {
        return hasUsableAxis(bins, count) ? formatValue(bins[index]) : Integer.toString(index + 1);
    }

    private static String title(String title, boolean last) {
        return last && title != null && !title.isEmpty() ? " " + title : "";
    }

    static String formatValue(double value) {
        if (Math.abs(value) >= 100) {
            return Long.toString(Math.round(value));
        }
        return String.format(java.util.Locale.ROOT, "%.1f", value).replaceFirst("\\.0$", "");
    }

    private static void drawLabel(Graphics2D g2d, String text, Point3D anchor) {
        FontMetrics metrics = g2d.getFontMetrics();
        g2d.drawString(text, anchor.x - metrics.stringWidth(text) / 2,
            anchor.y + (metrics.getAscent() - metrics.getDescent()) / 2);
    }

    /**
     * The largest scale at which the unit cube holding the surface, and the axis labels around it,
     * stay inside the panel at any rotation, so nothing is cut and the surface does not change
     * size while it is dragged.
     */
    static double fitScale(int width, int height) {
        // Label anchors lie within the cube grown by LABEL_OFFSET on each side.
        double extent = 1 + 2 * LABEL_OFFSET;
        return ZOOM * Math.max(0, Math.min((width - 2 * LABEL_HALF_WIDTH) / (2 * MAX_HALF_WIDTH * extent),
            (height - 2 * LABEL_HALF_HEIGHT) / (2 * MAX_HALF_HEIGHT * extent)));
    }

    static Point3D project(double x, double y, double z, double yaw, double pitch,
                           double scale, int centerX, int centerY) {
        // Rotation
        double x1 = x * Math.cos(yaw) - y * Math.sin(yaw);
        double y1 = x * Math.sin(yaw) + y * Math.cos(yaw);

        double y2 = y1 * Math.cos(pitch) - z * Math.sin(pitch);
        double z2 = y1 * Math.sin(pitch) + z * Math.cos(pitch);

        return new Point3D(
            (int) (centerX + x1 * scale),
            (int) (centerY - y2 * scale)
        );
    }

    static class Point3D {
        int x, y;
        Point3D(int x, int y) {
            this.x = x;
            this.y = y;
        }
    }
}
