package com.rusefi.pinout;

import javax.imageio.ImageIO;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.geom.Arc2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Collection;
import java.util.Map;

/**
 * Renders one connector photo with pins of the current assembly step highlighted in their
 * wire colors, and pins of previous steps dimmed out.
 */
public class StepImageRenderer {
    private static final int SCALE = 3;
    private static final int RADIUS = 22;

    /**
     * @param highlight  pin name to wire color string, pins to wire in this step
     * @param done       pins already wired in previous steps
     */
    public static void render(ConnectorYaml connector, Map<String, String> highlight,
                              Collection<String> done, File outputFile) throws IOException {
        if (connector.imageFile == null || !connector.imageFile.isFile()) {
            throw new IOException("No image for connector " + connector.name);
        }
        BufferedImage source = ImageIO.read(connector.imageFile);
        int width = source.getWidth() * SCALE;
        int height = source.getHeight() * SCALE;
        BufferedImage output = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
        Graphics2D g = output.createGraphics();
        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        g.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_BICUBIC);
        g.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
        g.drawImage(source, 0, 0, width, height, null);

        for (String pin : done) {
            ConnectorYaml.Coord coord = connector.coords.get(pin);
            if (coord == null || highlight.containsKey(pin)) {
                continue;
            }
            g.setColor(new Color(90, 90, 90, 180));
            g.setStroke(new BasicStroke(4));
            g.drawOval(coord.x * SCALE - RADIUS, coord.y * SCALE - RADIUS, 2 * RADIUS, 2 * RADIUS);
        }

        g.setFont(new Font(Font.SANS_SERIF, Font.BOLD, 20));
        for (Map.Entry<String, String> entry : highlight.entrySet()) {
            String pin = entry.getKey();
            ConnectorYaml.Coord coord = connector.coords.get(pin);
            if (coord == null) {
                continue;
            }
            int cx = coord.x * SCALE;
            int cy = coord.y * SCALE;
            Color[] stripes = WireColors.resolve(entry.getValue());
            for (int i = 0; i < stripes.length; i++) {
                g.setColor(stripes[i]);
                g.fill(new Arc2D.Double(cx - RADIUS, cy - RADIUS, 2.0 * RADIUS, 2.0 * RADIUS,
                        90 + 360.0 * i / stripes.length, 360.0 / stripes.length, Arc2D.PIE));
            }
            g.setColor(Color.ORANGE);
            g.setStroke(new BasicStroke(5));
            g.drawOval(cx - RADIUS, cy - RADIUS, 2 * RADIUS, 2 * RADIUS);

            String label = pin;
            int textWidth = g.getFontMetrics().stringWidth(label);
            int tx = cx - textWidth / 2;
            int ty = cy + g.getFontMetrics().getAscent() / 2 - 2;
            g.setColor(WireColors.isLight(stripes[0]) ? Color.BLACK : Color.WHITE);
            g.drawString(label, tx, ty);
        }
        g.dispose();
        ImageIO.write(output, "png", outputFile);
    }
}
