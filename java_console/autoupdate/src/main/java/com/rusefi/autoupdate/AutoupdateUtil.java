package com.rusefi.autoupdate;

import com.rusefi.shared.ConnectionAndMeta;
import com.rusefi.ui.util.FrameHelper;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.Date;
import java.util.concurrent.atomic.AtomicReference;

public class AutoupdateUtil {
    public static final boolean runHeadless = Boolean.getBoolean("run_headless") || GraphicsEnvironment.isHeadless();

    // todo: figure out a better way to work with absolute path
    private static final String APPICON = "/appicon.png";

    public static JComponent wrap(JComponent component) {
        JPanel result = new JPanel();
        result.add(component);
        return result;
    }

    public static void downloadAutoupdateFile(String localZipFileName, ConnectionAndMeta connectionAndMeta, String title) throws IOException {
        FrameHelper frameHelper = null;
        final AtomicReference<JProgressBar> jProgressBarAtomicReference = new AtomicReference<>();
        if (!runHeadless) {
            frameHelper = new FrameHelper();
            JProgressBar jProgressBar = new JProgressBar();

            frameHelper.getFrame().setTitle(title);
            jProgressBar.setMaximum(ConnectionAndMeta.STEPS);
            jProgressBarAtomicReference.set(jProgressBar);
            frameHelper.showFrame(jProgressBar, true);
        }

        ConnectionAndMeta.DownloadProgressListener listener = currentProgress -> {
            if (!runHeadless) {
                SwingUtilities.invokeLater(() -> jProgressBarAtomicReference.get().setValue(currentProgress));
            }
        };

        ConnectionAndMeta.downloadFile(localZipFileName, connectionAndMeta, listener);

        if (!runHeadless) {
            frameHelper.getFrame().dispose();
        }
    }

    @NotNull
    public static URLClassLoader getClassLoaderByJar(String jar) throws MalformedURLException {
        return new URLClassLoader(
                new URL[]{new File(jar).toURI().toURL()},
                AutoupdateUtil.class.getClassLoader()
        );
    }

    public static void trueLayout(Component component) {
        if (component == null)
            return;
        component.invalidate();
        component.validate();
        component.repaint();
    }

    public static boolean hasExistingFile(String zipFileName, long completeFileSize, long lastModified) {
        File file = new File(zipFileName);
        System.out.println("We have " + file.length() + " " + new Date(file.lastModified()));
        return file.length() == completeFileSize && file.lastModified() == lastModified;
    }

    public static ImageIcon loadIcon(String strPath) {
        URL imgURL = AutoupdateUtil.class.getResource(strPath);
        if (imgURL != null) {
            return new ImageIcon(imgURL);
        } else {
            imgURL = AutoupdateUtil.class.getResource("/com/rusefi/" + strPath);
            if (imgURL != null) {
                return new ImageIcon(imgURL);
            }
            return null;
        }
    }

    public static void setAppIcon(JFrame frame) {
        ImageIcon icon = loadIcon(APPICON);
        if (icon != null)
            frame.setIconImage(icon.getImage());
    }

    public static void pack(Window window) {
        trueLayout(window);
        if (window != null)
            window.pack();
        trueLayout(window);
    }
}
