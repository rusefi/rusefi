package com.rusefi.ui.widgets.tune;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class Surface3DView extends JPanel {
    private Double[][] data;
    private double minValue;
    private double maxValue;

    private double yaw = Math.toRadians(45);
    private double pitch = Math.toRadians(30);
    private Point lastMousePos;

    private static final double ZOOM = 0.8;

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
        this.minValue = minValue;
        this.maxValue = maxValue;
        repaint();
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

        double scale = Math.min(width, height) * ZOOM;

        Point3D[][] points = new Point3D[rows][cols];
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                double x = (double) c / (cols - 1) - 0.5;
                double y = (double) r / (rows - 1) - 0.5;
                double z = 0;
                if (maxValue > minValue) {
                    z = (data[r][c] - minValue) / (maxValue - minValue) - 0.5;
                }
                points[r][c] = project(x, y, z, scale, centerX, centerY);
            }
        }

        for (int r = 0; r < rows - 1; r++) {
            for (int c = 0; c < cols - 1; c++) {
                Point3D p1 = points[r][c];
                Point3D p2 = points[r + 1][c];
                Point3D p3 = points[r + 1][c + 1];
                Point3D p4 = points[r][c + 1];

                double avgZ = (data[r][c] + data[r + 1][c] + data[r + 1][c + 1] + data[r][c + 1]) / 4.0;
                g2d.setColor(getGradientColor(avgZ));

                int[] xPoints = {p1.x, p2.x, p3.x, p4.x};
                int[] yPoints = {p1.y, p2.y, p3.y, p4.y};
                g2d.fillPolygon(xPoints, yPoints, 4);

                g2d.setColor(Color.DARK_GRAY);
                g2d.drawPolygon(xPoints, yPoints, 4);
            }
        }
    }

    private Point3D project(double x, double y, double z, double scale, int centerX, int centerY) {
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

    private Color getGradientColor(double value) {
        if (maxValue <= minValue) return Color.WHITE;
        double ratio = (value - minValue) / (maxValue - minValue);
        ratio = Math.max(0, Math.min(1, ratio));

        int r1 = 173, g1 = 216, b1 = 230; // Pastel Blue
        int r2 = 255, g2 = 182, b2 = 193; // Pastel Red

        int red = (int) (r1 + ratio * (r2 - r1));
        int green = (int) (g1 + ratio * (g2 - g1));
        int blue = (int) (b1 + ratio * (b2 - b1));
        return new Color(red, green, blue);
    }

    private static class Point3D {
        int x, y;
        Point3D(int x, int y) {
            this.x = x;
            this.y = y;
        }
    }
}
