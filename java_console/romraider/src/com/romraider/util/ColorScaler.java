/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2012 RomRaider.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

package com.romraider.util;

import java.awt.Color;

import com.romraider.Settings;

public final class ColorScaler {

    private ColorScaler() {
    }

    public static Color getScaledColor(double scale) {

        Settings settings = SettingsManager.getSettings();
        if(0.0 == scale) {
            return settings.getMinColor();
        }

        if(Double.NaN == scale) {
            return settings.getMaxColor();
        }

        Color minColor = settings.getMinColor();
        Color maxColor = settings.getMaxColor();

        float[] minColorHSB = new float[3];
        float[] maxColorHSB = new float[3];

        rgbToHsb(minColor, minColorHSB);
        rgbToHsb(maxColor, maxColorHSB);

        float h = minColorHSB[0] + (maxColorHSB[0] - minColorHSB[0]) * (float) scale;
        float s = minColorHSB[1] + (maxColorHSB[1] - minColorHSB[1]) * (float) scale;
        float b = minColorHSB[2] + (maxColorHSB[2] - minColorHSB[2]) * (float) scale;

        return Color.getHSBColor(h, s, b);

    }

    private static void rgbToHsb(Color color, float[] colorHSB) {
        Color.RGBtoHSB(color.getRed(), color.getGreen(), color.getBlue(), colorHSB);
    }
}
