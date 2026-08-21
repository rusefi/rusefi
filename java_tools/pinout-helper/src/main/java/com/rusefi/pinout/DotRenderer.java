package com.rusefi.pinout;

import javax.imageio.ImageIO;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.geom.Arc2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

/**
 * Tiny color swatch PNGs shown inline in the markdown wire tables - markdown itself cannot color
 * text, so the color has to arrive as an image.
 */
public class DotRenderer {
    static final int SIZE = 14;

    /**
     * @param stripes one color per stripe, as returned by {@link WireColors#resolve(String)}
     */
    public static void render(Color[] stripes, File outputFile) throws IOException {
        // transparent background: the dot reads the same on light and dark markdown themes
        BufferedImage image = new BufferedImage(SIZE, SIZE, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g = image.createGraphics();
        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        for (int i = 0; i < stripes.length; i++) {
            g.setColor(stripes[i]);
            g.fill(new Arc2D.Double(0.5, 0.5, SIZE - 1.0, SIZE - 1.0,
                    90 + 360.0 * i / stripes.length, 360.0 / stripes.length, Arc2D.PIE));
        }
        // outline keeps white and yellow dots visible on a white page
        g.setColor(new Color(60, 60, 60));
        g.setStroke(new BasicStroke(1));
        g.drawOval(0, 0, SIZE - 1, SIZE - 1);
        g.dispose();
        ImageIO.write(image, "png", outputFile);
    }
}
