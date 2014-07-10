package com.rusefi.ui;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

/**
 * 7/7/13
 * (c) Andrey Belomutskiy
 */
public class UiUtils {
    public static void saveImage(String fileName, Component component) {
        BufferedImage img = getScreenShot(component);
        try {
            ImageIO.write(img, "png", new File(fileName));
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    private static BufferedImage getScreenShot(Component component) {
        // http://stackoverflow.com/questions/5853879/swing-obtain-image-of-jframe/5853992
        BufferedImage image = new BufferedImage(component.getWidth(), component.getHeight(), BufferedImage.TYPE_INT_RGB);
        // call the Component's paint method, using
        // the Graphics object of the image.
        component.paint(image.getGraphics());
        return image;
    }

    public static void setPauseButtonText(JButton pauseButton, boolean isPaused) {
        pauseButton.setText(isPaused ? "resume" : "pause");
    }
}
