package com.rusefi.autoupdate;

import com.rusefi.shared.ConnectionAndMeta;
import com.rusefi.ui.util.FrameHelper;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import java.net.URLStreamHandlerFactory;
import java.util.Date;
import java.util.concurrent.atomic.AtomicReference;

public class AutoupdateUtil {
    public static final boolean runHeadless = Boolean.getBoolean("run_headless") || GraphicsEnvironment.isHeadless();

    // todo: figure out a better way to work with absolute path
    private static final String APPICON = "/appicon.png";

    public static JComponent wrap(JComponent component) {
        AutoupdateUtil.assertAwtThread();
        JPanel result = new JPanel();
        result.add(component);
        return result;
    }

    static class ProgressView {
        private final FrameHelper frameHelper;
        private JProgressBar progressBar;

        ProgressView(FrameHelper frameHelper, JProgressBar progressBar) {
            this.frameHelper = frameHelper;
            this.progressBar = progressBar;
        }

        public void dispose() {
            if (frameHelper != null) {
                frameHelper.getFrame().dispose();
            }
        }
    }

    private static ProgressView createProgressView(String title) {
        if (runHeadless) {
            return new ProgressView(null, null);
        } else {
            FrameHelper frameHelper = new FrameHelper();
            JProgressBar jProgressBar = new JProgressBar();

            frameHelper.getFrame().setTitle(title);
            jProgressBar.setMaximum(ConnectionAndMeta.CENTUM);
            frameHelper.showFrame(jProgressBar, true);
            return new ProgressView(frameHelper, jProgressBar);
        }
    }

    public static void downloadAutoupdateFile(String localZipFileName, ConnectionAndMeta connectionAndMeta, String title) throws IOException {
        ProgressView view = createProgressView(title);

        try {
            ConnectionAndMeta.DownloadProgressListener listener = currentProgress -> {
                if (!runHeadless) {
                    SwingUtilities.invokeLater(() -> view.progressBar.setValue(currentProgress));
                }
            };

            ConnectionAndMeta.downloadFile(localZipFileName, connectionAndMeta, listener);
        } catch (IOException e) {
            if (view.progressBar!=null) {
                JOptionPane.showMessageDialog(view.progressBar, "Error downloading: " + e, "Error", JOptionPane.ERROR_MESSAGE);
                throw new ReportedIOException(e);
            } else
                throw e;
        } finally {
            view.dispose();
        }
    }

    private static class DynamicForResourcesURLClassLoader extends URLClassLoader {

        public DynamicForResourcesURLClassLoader( URL[] urls, ClassLoader parent ) { super( urls, parent ); }

        public DynamicForResourcesURLClassLoader( URL[] urls ) { super( urls ); }

        public DynamicForResourcesURLClassLoader( URL[] urls, ClassLoader parent, URLStreamHandlerFactory factory ) { super( urls, parent, factory ); }

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

    private static final DynamicForResourcesURLClassLoader dynamicResourcesLoader = new DynamicForResourcesURLClassLoader( new URL[ 0 ], AutoupdateUtil.class.getClassLoader() );

    @NotNull
    public static URLClassLoader getClassLoaderByJar(String jar) throws MalformedURLException {
        final URL jarURL = new File( jar ).toURI().toURL();
        dynamicResourcesLoader.addURL( jarURL );
        return new URLClassLoader(
                new URL[]{ new File( jar ).toURI().toURL() },
                dynamicResourcesLoader
        );
    }

    public static void trueLayout(Component component) {
        assertAwtThread();
        if (component == null)
            return;
        component.invalidate();
        component.validate();
        component.repaint();
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

    public static void assertAwtThread() {
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

    public static boolean hasExistingFile(String zipFileName, long completeFileSize, long lastModified) {
        File file = new File(zipFileName);
        System.out.println("We have " + file.length() + " " + new Date(file.lastModified()) + " " + file.getAbsolutePath());
        return file.length() == completeFileSize && file.lastModified() == lastModified;
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
