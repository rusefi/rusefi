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
    private static final Color AXIS_COLOR = Color.DARK_GRAY;
    private static final Color SELECTION_COLOR = Color.ORANGE;
    static final Color SELECTED_POINT_COLOR = Color.BLACK;

    public Surface3DView() {
        setBackground(Color.WHITE);
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
                g2d.setColor(selected ? SELECTION_COLOR : CalibrationColorPalette.background(avgZ, minValue, maxValue));

                int[] xPoints = {p1.x, p2.x, p3.x, p4.x};
                int[] yPoints = {p1.y, p2.y, p3.y, p4.y};
                g2d.fillPolygon(xPoints, yPoints, 4);

                g2d.setColor(Color.DARK_GRAY);
                g2d.drawPolygon(xPoints, yPoints, 4);
            }
        }

        paintAxes(g2d, scale, centerX, centerY);
        paintSelectedPoints(g2d, points);
    }

    /** Marks each selected cell's own point in the middle of its lit facets. */
    private void paintSelectedPoints(Graphics2D g2d, Point3D[][] points) {
        int radius = 3;
        for (int r = 0; r < points.length; r++) {
            for (int c = 0; c < points[r].length; c++) {
                if (isSelected(r, c)) {
                    Point3D point = points[r][c];
                    g2d.setColor(SELECTED_POINT_COLOR);
                    g2d.fillOval(point.x - radius, point.y - radius, 2 * radius, 2 * radius);
                    g2d.setColor(Color.WHITE);
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
        g2d.setColor(AXIS_COLOR);
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
