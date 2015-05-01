package com.rusefi.ui.util;

import javax.imageio.ImageIO;
import javax.swing.*;
import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import static com.rusefi.ui.util.LocalizedMessages.PAUSE;
import static com.rusefi.ui.util.LocalizedMessages.RESUME;

/**
 * 7/7/13
 * (c) Andrey Belomutskiy
 */
public class UiUtils {
    private static final String SAVE_IMAGE = "save image";

    public static void saveImageWithPrompt(String fileName, Component parentForDialog, Component content) {
        JFileChooser fc = new JFileChooser();
        FileFilter filter = new FileNameExtensionFilter("PNG files", "png");
        fc.setFileFilter(filter);
        fc.addChoosableFileFilter(filter);
        fc.setSelectedFile(new File(fileName));
        if (fc.showSaveDialog(parentForDialog) == JFileChooser.APPROVE_OPTION) {
            fileName = fc.getSelectedFile().getAbsolutePath();
            UiUtils.saveImage(fileName, content);
        }
    }

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
        pauseButton.setText(isPaused ? RESUME.getMessage() : PAUSE.getMessage());
    }

    public static void centerWindow(Window w) {
        w.pack();
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        Dimension frameSize = w.getSize();
        w.setLocation((screenSize.width - frameSize.width) / 2, (screenSize.height - frameSize.height) / 2);
    }

    /**
     * This does not make any sense :( That's an attempt
     * to hack http://rusefi.com/forum/viewtopic.php?f=2&t=631&p=10083#p10081
     */
    public static void trueRepaint(JComponent control) {
        control.invalidate();
        control.repaint();
    }

    public static void trueRepaint(Container control) {
        trueLayout(control);
//        if (control == null)
//            return;
//        control.invalidate();
//        control.repaint();
    }

    /**
     * todo: one 'trueXXX' method should be enough, which one?
     */
    public static void trueLayout(Component component) {
        if (component == null)
            return;
        component.invalidate();
        component.validate();
        component.repaint();
    }

    public static void setTwoLineToolTip(JComponent component, String line1, String line2) {
        component.setToolTipText("<html>" + line1 + "<br>" + line2 + "</html>");
    }

    public static JComponent wrap(JComponent component) {
        JPanel result = new JPanel();
        result.add(component);
        return result;
    }

    public static JButton createSaveImageButton() {
        JButton imageButton = new JButton(SAVE_IMAGE);
        imageButton.setMnemonic('s');
        return imageButton;
    }
}
