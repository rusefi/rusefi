package com.rusefi.models;

/**
 * Date: 3/24/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class Point3D {
    private final double x;
    private final double y;
    private final float z;

    public Point3D(int rpm, double key, float value) {
        this((double) rpm, key, value);
    }

    public Point3D(double x, double y, float z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }

    public float getZ() {
        return z;
    }

    @Override
    public String toString() {
        return "Point3D{" +
                "x=" + x +
                ", y=" + y +
                ", z=" + z +
                '}';
    }

    public static Point3D parseLine(String line) {
        String[] tokens = line.split(",");
        double x = Double.parseDouble(tokens[1]);
        double y = Double.parseDouble(tokens[3]);
        float z = Float.parseFloat(tokens[5]);
        return new Point3D(x, y, z);
    }
}
