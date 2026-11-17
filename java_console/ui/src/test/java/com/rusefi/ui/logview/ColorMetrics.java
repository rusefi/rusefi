package com.rusefi.ui.logview;

import java.awt.Color;

/** Colour measures for checking that chart colours stay readable and distinguishable. */
final class ColorMetrics {
    /** Machado et al. 2009 full-severity simulation matrices, applied to linear RGB. */
    static final double[][] DEUTERANOPIA = {
            {0.367322, 0.860646, -0.227968},
            {0.280085, 0.672501, 0.047413},
            {-0.011820, 0.042940, 0.968881}};
    static final double[][] PROTANOPIA = {
            {0.152286, 1.052583, -0.204868},
            {0.114503, 0.786281, 0.099216},
            {-0.003882, -0.048116, 1.051998}};

    private ColorMetrics() {
    }

    private static double linear(int channel) {
        double c = channel / 255.0;
        return c <= 0.04045 ? c / 12.92 : Math.pow((c + 0.055) / 1.055, 2.4);
    }

    private static int encoded(double linear) {
        double v = Math.min(1, Math.max(0, linear));
        return (int) Math.round(255 * (v <= 0.0031308 ? 12.92 * v : 1.055 * Math.pow(v, 1 / 2.4) - 0.055));
    }

    private static double luminance(Color color) {
        return 0.2126 * linear(color.getRed()) + 0.7152 * linear(color.getGreen()) + 0.0722 * linear(color.getBlue());
    }

    /** WCAG 2 contrast ratio, from 1 (same) to 21 (black on white). */
    static double contrast(Color a, Color b) {
        double la = luminance(a);
        double lb = luminance(b);
        return (Math.max(la, lb) + 0.05) / (Math.min(la, lb) + 0.05);
    }

    static Color simulate(Color color, double[][] matrix) {
        double[] rgb = {linear(color.getRed()), linear(color.getGreen()), linear(color.getBlue())};
        int[] out = new int[3];
        for (int i = 0; i < 3; i++) {
            out[i] = encoded(matrix[i][0] * rgb[0] + matrix[i][1] * rgb[1] + matrix[i][2] * rgb[2]);
        }
        return new Color(out[0], out[1], out[2]);
    }

    private static double[] lab(Color color) {
        double r = linear(color.getRed());
        double g = linear(color.getGreen());
        double b = linear(color.getBlue());
        double x = (0.4124 * r + 0.3576 * g + 0.1805 * b) / 0.95047;
        double y = 0.2126 * r + 0.7152 * g + 0.0722 * b;
        double z = (0.0193 * r + 0.1192 * g + 0.9505 * b) / 1.08883;
        double fx = labF(x);
        double fy = labF(y);
        double fz = labF(z);
        return new double[]{116 * fy - 16, 500 * (fx - fy), 200 * (fy - fz)};
    }

    private static double labF(double t) {
        return t > 216.0 / 24389 ? Math.cbrt(t) : (24389.0 / 27 * t + 16) / 116;
    }

    /** CIEDE2000 colour difference: about 2 is barely noticeable, above 10 clearly different. */
    static double deltaE2000(Color first, Color second) {
        double[] p = lab(first);
        double[] q = lab(second);
        double c1 = Math.hypot(p[1], p[2]);
        double c2 = Math.hypot(q[1], q[2]);
        double cMean7 = Math.pow((c1 + c2) / 2, 7);
        double gFactor = 0.5 * (1 - Math.sqrt(cMean7 / (cMean7 + Math.pow(25, 7))));
        double a1 = (1 + gFactor) * p[1];
        double a2 = (1 + gFactor) * q[1];
        double c1p = Math.hypot(a1, p[2]);
        double c2p = Math.hypot(a2, q[2]);
        double h1 = (Math.toDegrees(Math.atan2(p[2], a1)) + 360) % 360;
        double h2 = (Math.toDegrees(Math.atan2(q[2], a2)) + 360) % 360;
        double dL = q[0] - p[0];
        double dC = c2p - c1p;
        double dh = 0;
        if (c1p * c2p != 0) {
            dh = h2 - h1;
            if (dh > 180) {
                dh -= 360;
            } else if (dh < -180) {
                dh += 360;
            }
        }
        double dH = 2 * Math.sqrt(c1p * c2p) * Math.sin(Math.toRadians(dh / 2));
        double lMean = (p[0] + q[0]) / 2;
        double cMeanP = (c1p + c2p) / 2;
        double hMean;
        if (c1p * c2p == 0) {
            hMean = h1 + h2;
        } else if (Math.abs(h1 - h2) <= 180) {
            hMean = (h1 + h2) / 2;
        } else if (h1 + h2 < 360) {
            hMean = (h1 + h2 + 360) / 2;
        } else {
            hMean = (h1 + h2 - 360) / 2;
        }
        double t = 1 - 0.17 * Math.cos(Math.toRadians(hMean - 30)) + 0.24 * Math.cos(Math.toRadians(2 * hMean))
                + 0.32 * Math.cos(Math.toRadians(3 * hMean + 6)) - 0.20 * Math.cos(Math.toRadians(4 * hMean - 63));
        double dTheta = 30 * Math.exp(-Math.pow((hMean - 275) / 25, 2));
        double cMeanP7 = Math.pow(cMeanP, 7);
        double rc = 2 * Math.sqrt(cMeanP7 / (cMeanP7 + Math.pow(25, 7)));
        double sl = 1 + 0.015 * Math.pow(lMean - 50, 2) / Math.sqrt(20 + Math.pow(lMean - 50, 2));
        double sc = 1 + 0.045 * cMeanP;
        double sh = 1 + 0.015 * cMeanP * t;
        double rt = -Math.sin(Math.toRadians(2 * dTheta)) * rc;
        return Math.sqrt(Math.pow(dL / sl, 2) + Math.pow(dC / sc, 2) + Math.pow(dH / sh, 2)
                + rt * (dC / sc) * (dH / sh));
    }
}
