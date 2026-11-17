package com.rusefi.ui.widgets;

import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.ui.basic.UiHelper;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.util.*;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

/**
 * [tag:console_dark_mode] Every icons/tuning icon stays visible on the dark tab row once TuningIcons lightens
 * it, and the light theme gets the images unchanged.
 */
public class TuningIconsTest {
    private static final File ICONS = new File("src/main/resources/com/rusefi/icons/tuning");
    /** WCAG 2 minimum contrast for graphics */
    private static final double GRAPHIC = 3.0;

    @Test
    public void iconsAreVisibleOnTheDarkTabRowAndUnchangedInLight() throws Exception {
        List<String> names = new ArrayList<>();
        for (String file : Objects.requireNonNull(ICONS.list())) {
            if (file.endsWith("48.png")) {
                names.add(file.substring(0, file.length() - "48.png".length()));
            }
        }
        assertFalse(names.isEmpty(), "icons found in " + ICONS.getAbsolutePath());
        List<String> problems = new ArrayList<>();
        LookAndFeel previous = UIManager.getLookAndFeel();
        boolean previousFrameDecorated = JFrame.isDefaultLookAndFeelDecorated();
        boolean previousDialogDecorated = JDialog.isDefaultLookAndFeelDecorated();
        try {
            SwingUtilities.invokeAndWait(() -> {
                UiHelper.configureLaf(true);
                Color tabRow = UIManager.getColor("Console.TabArea.background");
                for (String name : names) {
                    double contrast = contrast(mainColor(pixels(TuningIcons.load(name).getImage())), tabRow);
                    if (contrast < GRAPHIC) {
                        problems.add(String.format("%s: %.2f on the dark tab row", name, contrast));
                    }
                }
                UiHelper.unregisterThemeForUnitTest();
                UiHelper.configureLaf(false);
                for (String name : names) {
                    BufferedImage light = pixels(TuningIcons.load(name).getImage());
                    BufferedImage original = pixels(AutoupdateUtil.loadIcon("icons/tuning/" + name + "48.png").getImage());
                    if (!samePixels(light, original)) {
                        problems.add(name + ": changed in the light theme");
                    }
                }
            });
        } finally {
            SwingUtilities.invokeAndWait(() -> {
                UiHelper.unregisterThemeForUnitTest();
                JFrame.setDefaultLookAndFeelDecorated(previousFrameDecorated);
                JDialog.setDefaultLookAndFeelDecorated(previousDialogDecorated);
                try {
                    UIManager.setLookAndFeel(previous);
                } catch (UnsupportedLookAndFeelException e) {
                    throw new AssertionError(e);
                }
            });
        }
        assertTrue(problems.isEmpty(), String.join("\n", problems));
    }

    private static BufferedImage pixels(Image image) {
        BufferedImage copy = new BufferedImage(image.getWidth(null), image.getHeight(null), BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = copy.createGraphics();
        g.drawImage(image, 0, 0, null);
        g.dispose();
        return copy;
    }

    private static boolean samePixels(BufferedImage a, BufferedImage b) {
        if (a.getWidth() != b.getWidth() || a.getHeight() != b.getHeight()) {
            return false;
        }
        for (int y = 0; y < a.getHeight(); y++) {
            for (int x = 0; x < a.getWidth(); x++) {
                if (a.getRGB(x, y) != b.getRGB(x, y)) {
                    return false;
                }
            }
        }
        return true;
    }

    /** The most common opaque colour, in steps of 16 per channel: the icon's stroke colour. */
    private static Color mainColor(BufferedImage image) {
        Map<Integer, Integer> counts = new HashMap<>();
        for (int y = 0; y < image.getHeight(); y++) {
            for (int x = 0; x < image.getWidth(); x++) {
                int argb = image.getRGB(x, y);
                if ((argb >>> 24) > 200) {
                    counts.merge(argb & 0xf0f0f0, 1, Integer::sum);
                }
            }
        }
        int best = Collections.max(counts.entrySet(), Map.Entry.comparingByValue()).getKey();
        return new Color(best);
    }

    private static double contrast(Color a, Color b) {
        double la = luminance(a);
        double lb = luminance(b);
        return (Math.max(la, lb) + 0.05) / (Math.min(la, lb) + 0.05);
    }

    private static double luminance(Color c) {
        return 0.2126 * channel(c.getRed()) + 0.7152 * channel(c.getGreen()) + 0.0722 * channel(c.getBlue());
    }

    private static double channel(int value) {
        double v = value / 255.0;
        return v <= 0.03928 ? v / 12.92 : Math.pow((v + 0.055) / 1.055, 2.4);
    }
}
