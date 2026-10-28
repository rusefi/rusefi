package com.rusefi.ui.widgets;

import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.ui.basic.UiHelper;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;

/**
 * The coloured line icons in icons/tuning (tab row, Tuning menu, setting help, frame menus). Their deep blues
 * and purples are drawn for a light background; in the dark console theme every pixel is mixed part of the way
 * toward white, keeping hue and shape, so the icons stay visible on dark strips. The light theme gets the
 * images unchanged. [tag:console_dark_mode]
 */
public final class TuningIcons {
    /** Share of white mixed into each pixel in the dark theme. */
    static final float DARK_LIGHTEN = 0.3f;

    private TuningIcons() {
    }

    /** icons/tuning/{@code name}48.png for the installed console theme, or null when it is missing. */
    public static ImageIcon load(String name) {
        ImageIcon icon = AutoupdateUtil.loadIcon("icons/tuning/" + name + "48.png");
        if (icon == null || !UiHelper.isDarkTheme()) {
            return icon;
        }
        return new ImageIcon(lighten(icon.getImage(), DARK_LIGHTEN));
    }

    /** A copy of {@code image} with {@code share} of white mixed into every pixel; alpha is kept. */
    static BufferedImage lighten(Image image, float share) {
        int width = Math.max(1, image.getWidth(null));
        int height = Math.max(1, image.getHeight(null));
        BufferedImage copy = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = copy.createGraphics();
        try {
            g.drawImage(image, 0, 0, null);
        } finally {
            g.dispose();
        }
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int argb = copy.getRGB(x, y);
                int r = mix((argb >> 16) & 0xff, share);
                int gr = mix((argb >> 8) & 0xff, share);
                int b = mix(argb & 0xff, share);
                copy.setRGB(x, y, (argb & 0xff000000) | (r << 16) | (gr << 8) | b);
            }
        }
        return copy;
    }

    private static int mix(int channel, float share) {
        return Math.round(channel + (255 - channel) * share);
    }
}
