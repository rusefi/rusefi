package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.ui.util.URLLabel;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;

import static com.devexperts.logging.Logging.getLogging;

public class LogoHelper {
    private static final Logging log = getLogging(LogoHelper.class);
    public static final String LINK_TEXT = "rusEFI (c) 2012-2026";
    public static final String URI = "http://rusefi.com/?java_console";

    public static JLabel createLogoLabel() {
        ImageIcon logoIcon = getBundleSpecificIcon();
        if (logoIcon == null)
            return null;
        if (isDark(UIManager.getColor("Panel.background"))) {
            logoIcon = new ImageIcon(forDarkBackground(logoIcon.getImage()));
        }
        JLabel logo = new JLabel(logoIcon);
        logo.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 10));
        URLLabel.addUrlAction(logo, URLLabel.createUri(URI));
        logo.setCursor(new Cursor(Cursor.HAND_CURSOR));
        return logo;
    }

    @Nullable
    public static ImageIcon getBundleSpecificIcon() {
        String bundle = BundleUtil.readBundleFullNameNotNull().getTarget();
        String logoName;
        // these should be about 213px wide
        if (bundle.contains("proteus")) {
            logoName = BasicLogoHelper.LOGO_PATH + "logo_proteus.png";
//        } else if (bundle.contains("honda")) {
//            logoName = LOGO_PATH + "logo_tutomo.png";
        } else if (bundle.contains("alphax")) {
            logoName = BasicLogoHelper.LOGO_PATH + "logo_alphax.png";
        } else if (bundle.contains(".mre")) {
            logoName = BasicLogoHelper.LOGO_PATH + "logo_mre.png";
        } else {
            logoName = BasicLogoHelper.GENERIC_LOGO;
        }
        ImageIcon imageIcon = AutoupdateUtil.loadIcon(logoName);
        log.info(imageIcon + " for " + logoName);
        return imageIcon;
    }

    /** Whether {@code background} is a dark theme's, e.g. the dark console theme's panels. */
    static boolean isDark(@Nullable Color background) {
        return background != null
            && 0.2126 * background.getRed() + 0.7152 * background.getGreen() + 0.0722 * background.getBlue() < 128;
    }

    /**
     * The logo for a dark background: on a logo with a transparent background, black and dark grey
     * (the "EFI" of the rusEFI logo) become white, keeping their transparency; coloured parts stay.
     * A logo without transparency brings its own background and is returned as drawn. Not used for
     * the window icon, which the operating system shows on its own background.
     */
    static Image forDarkBackground(Image image) {
        int width = Math.max(1, image.getWidth(null));
        int height = Math.max(1, image.getHeight(null));
        BufferedImage copy = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = copy.createGraphics();
        try {
            g.drawImage(image, 0, 0, null);
        } finally {
            g.dispose();
        }
        boolean transparent = false;
        for (int y = 0; y < height && !transparent; y++) {
            for (int x = 0; x < width && !transparent; x++) {
                transparent = (copy.getRGB(x, y) >>> 24) < 0xff;
            }
        }
        if (!transparent) {
            return image;
        }
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int argb = copy.getRGB(x, y);
                int r = (argb >> 16) & 0xff;
                int gr = (argb >> 8) & 0xff;
                int b = argb & 0xff;
                int max = Math.max(r, Math.max(gr, b));
                int min = Math.min(r, Math.min(gr, b));
                boolean grey = max == 0 || (max - min) / (double) max < 0.25;
                if (grey && 0.2126 * r + 0.7152 * gr + 0.0722 * b < 128) {
                    copy.setRGB(x, y, (argb & 0xff000000) | 0xffffff);
                }
            }
        }
        return copy;
    }

    @NotNull
    public static URLLabel createUrlLabel() {
        return new URLLabel(LINK_TEXT, URI);
    }
}
