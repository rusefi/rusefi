package com.rusefi.ui.util;

import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.imageio.ImageIO;
import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.text.JTextComponent;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import java.net.URL;
import java.net.URLClassLoader;

import static com.rusefi.ui.util.LocalizedMessages.CLEAR;
import static com.rusefi.ui.util.LocalizedMessages.PAUSE;
import static com.rusefi.ui.util.LocalizedMessages.RESUME;

/**
 * 7/7/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class UiUtils {
    private static final String SAVE_IMAGE = "save image";

    // todo: figure out a better way to work with absolute path
    private static final String APPICON = "/appicon.png";


    public static void saveImageWithPrompt(String fileName, Component parentForDialog, Component content) {
        JFileChooser fc = getFileChooser(new FileNameExtensionFilter("PNG files", "png"));
        fc.setSelectedFile(new File(fileName));
        if (fc.showSaveDialog(parentForDialog) == JFileChooser.APPROVE_OPTION) {
            fileName = fc.getSelectedFile().getAbsolutePath();
            UiUtils.saveImage(fileName, content);
        }
    }

    @NotNull
    public static JFileChooser getFileChooser(FileNameExtensionFilter filter) {
        JFileChooser fc = new JFileChooser();
        fc.setFileFilter(filter);
        fc.addChoosableFileFilter(filter);
        return fc;
    }

    public static void saveImage(String fileName, Component component) {
        BufferedImage img = getScreenShot(component);
        System.out.println("Saving " + fileName);
        try {
            ImageIO.write(img, "png", new File(fileName));
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    private static BufferedImage getScreenShot(Component component) {
        assertAwtThread();
        // http://stackoverflow.com/questions/5853879/swing-obtain-image-of-jframe/5853992
        BufferedImage image = new BufferedImage(component.getWidth(), component.getHeight(), BufferedImage.TYPE_INT_RGB);
        // call the Component's paint method, using
        // the Graphics object of the image.
        component.paint(image.getGraphics());
        return image;
    }

    public static void setPauseButtonText(JButton pauseButton, boolean isPaused) {
        setPauseButtonText(pauseButton, isPaused, "");
    }

    public static void setPauseButtonText(JButton pauseButton, boolean isPaused, String suffix) {
        pauseButton.setText((isPaused ? RESUME.getMessage() : PAUSE.getMessage()) + suffix);
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
        trueLayout(control);
    }

    /**
     * todo: one 'trueXXX' method should be enough, which one?
     */
    public static void trueLayout(Component component) {
        assertAwtThread();
        if (component == null)
            return;
        component.invalidate();
        component.validate();
        component.repaint();
    }

    private static void assertAwtThread() {
        if (!SwingUtilities.isEventDispatchThread()) {
            Exception e = new IllegalStateException("Not on AWT thread but " + Thread.currentThread().getName());

            StringBuilder trace = new StringBuilder(e + "\n");
            for(StackTraceElement element : e.getStackTrace())
                trace.append(element.toString()).append("\n");
            SwingUtilities.invokeLater(() -> {
                Window w = getSelectedWindow(Window.getWindows());
                JOptionPane.showMessageDialog(w, trace, "Error", JOptionPane.ERROR_MESSAGE);
            });
        }
    }

    private static Window getSelectedWindow(Window[] windows) {
        for (Window window : windows) {
            if (window.isActive()) {
                return window;
            } else {
                Window[] ownedWindows = window.getOwnedWindows();
                if (ownedWindows != null) {
                    return getSelectedWindow(ownedWindows);
                }
            }
        }
        return null;
    }

    /**
     * Utility method for multi-line tooltips
     */
    public static void setToolTip(JComponent component, String... lines) {
        StringBuilder sb = new StringBuilder();
        for (String line : lines) {
            if (sb.length() != 0)
                sb.append("<br>");
            sb.append(line);
        }
        component.setToolTipText("<html>" + sb + "</html>");
    }

    public static JComponent wrap(JComponent component) {
        assertAwtThread();
        JPanel result = new JPanel();
        result.add(component);
        return result;
    }

    public static JButton createSaveImageButton() {
        JButton imageButton = new JButton(SAVE_IMAGE);
        imageButton.setMnemonic('s');
        return imageButton;
    }

    @NotNull
    public static JButton createPauseButton() {
        return createPauseButton("");
    }

    @NotNull
    public static JButton createPauseButton(String suffix) {
        final JButton pauseButton = new JButton(PAUSE.getMessage() + suffix);
        pauseButton.setMnemonic('p');
        return pauseButton;
    }

    @NotNull
    public static JButton createClearButton() {
        JButton clearButton = new JButton(CLEAR.getMessage());
        clearButton.setMnemonic('c');
        return clearButton;
    }

    public static void installPopupMenu(JPopupMenu menu, JComponent component) {
        component.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                pop(e);
            }

            @Override
            public void mouseReleased(MouseEvent e) {
                pop(e);
            }

            private void pop(MouseEvent e) {
                if (e.isPopupTrigger())
                    menu.show(e.getComponent(), e.getX(), e.getY());
            }
        });
    }

    @NotNull
    public static JMenuItem createCopyMenu(final JTextComponent control) {
        JMenuItem copy = new JMenuItem("Copy");
        copy.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                control.copy();
            }
        });
        return copy;
    }

    private static class DynamicForResourcesURLClassLoader extends URLClassLoader {
        public DynamicForResourcesURLClassLoader( URL[] urls, ClassLoader parent ) { super( urls, parent ); }

        @Override
        public void addURL( URL url ) {
            super.addURL( url );
        }

        /**
         * Let's here emulate Class.getResource() logic
         * @param name resource name
         * @return resource url
         */
        @Nullable
        @Override
        public URL getResource( String name ) {
            if ( name.startsWith( "/" ) )
                name = name.substring( 1 );
            return super.getResource( name );
        }
    }

    private static final DynamicForResourcesURLClassLoader dynamicResourcesLoader = new DynamicForResourcesURLClassLoader( new URL[ 0 ], UiUtils.class.getClassLoader() );

    public static void setAppIcon(JFrame frame) {
        ImageIcon icon = loadIcon(APPICON);
        if (icon != null)
            frame.setIconImage(icon.getImage());
    }

    public static ImageIcon loadIcon( String strPath ) {
        URL imgURL = dynamicResourcesLoader.getResource( strPath );
        if (imgURL != null) {
            return new ImageIcon(imgURL);
        } else {
            imgURL = dynamicResourcesLoader.getResource("/com/rusefi/" + strPath);
            if (imgURL != null) {
                return new ImageIcon(imgURL);
            }
            return null;
        }
    }
}
