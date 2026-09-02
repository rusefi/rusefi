package com.rusefi.core.ui;

import com.devexperts.logging.Logging;
import com.rusefi.autoupdate.ReportedIOException;
import com.rusefi.core.net.ConnectionAndMeta;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.lang.reflect.InvocationTargetException;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import java.net.UnknownHostException;
import java.util.Date;
import java.util.concurrent.atomic.AtomicReference;

import static com.devexperts.logging.Logging.getLogging;

public class AutoupdateUtil {
    private static final Logging log = getLogging(AutoupdateUtil.class);
    public static final boolean runHeadless = Boolean.getBoolean("run_headless") || GraphicsEnvironment.isHeadless();

    // todo: figure out a better way to work with absolute path
    private static final String APPICON = "/appicon.png";

    /**
     * Where icons and logos actually live on the classpath, see java_console/shared_ui/resources.
     */
    static final String RUSEFI_RESOURCE_PACKAGE = "/com/rusefi/";

    public static JComponent wrap(JComponent component) {
        AutoupdateUtil.assertAwtThread();
        JPanel result = new JPanel();
        result.add(component);
        return result;
    }

    public static ProgressView doCreateProgressView(String title) {
        if (runHeadless) {
            return new ProgressView(null, null, null);
        } else {
            return ProgressView.create(title);
        }
    }

    public static void downloadAutoupdateFile(String localZipFileName, ConnectionAndMeta connectionAndMeta, String title) throws IOException {
        ProgressView view = createProgressView(title);

        try {
            ConnectionAndMeta.DownloadProgressListener listener = currentProgress -> {
                if (!runHeadless) {
                    SwingUtilities.invokeLater(() -> view.getProgressBar().setValue(currentProgress));
                }
            };
            while (true) {
                try {
                    ConnectionAndMeta.downloadFile(localZipFileName, connectionAndMeta, listener);
                    return;
                } catch (IOException e) {
                    log.error("downloadAutoupdateFile: " + e, e);
                    if (view.getProgressBar() == null) {
                        throw e;
                    }
                    String message = (e instanceof UnknownHostException)
                        ? "Please fix your internet connection"
                        : "Error downloading: " + e;
                    boolean retry = view.showErrorAndWaitForRetry(message);
                    if (!retry) {
                        throw new ReportedIOException(e);
                    }
                    view.resetForRetry();
                }
            }
        } finally {
            view.dispose();
        }
    }

    private static ProgressView createProgressView(String title) {
        AtomicReference<ProgressView> result = new AtomicReference<>();
        try {
            SwingUtilities.invokeAndWait(() -> result.set(doCreateProgressView(title)));
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        } catch (InvocationTargetException e) {
            throw new RuntimeException(e);
        }
        return result.get();
    }

    private static class DynamicForResourcesURLClassLoader extends URLClassLoader {
        public DynamicForResourcesURLClassLoader(ClassLoader parent ) {
            super(new URL[ 0 ], parent );
        }

        // public morozov pattern: making protected public
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

    private static final DynamicForResourcesURLClassLoader dynamicResourcesLoader = new DynamicForResourcesURLClassLoader(AutoupdateUtil.class.getClassLoader() );

    @NotNull
    public static URLClassLoader getClassLoaderByJar(String jar) throws MalformedURLException {
        final URL jarURL = new File( jar ).toURI().toURL();
        dynamicResourcesLoader.addURL( jarURL );
        return new URLClassLoader(
                new URL[]{ new File( jar ).toURI().toURL() },
                dynamicResourcesLoader
        );
    }

    public static void trueLayoutAndRepaint(Component component) {
        assertAwtThread();
        if (component == null)
            return;
        component.invalidate();
        component.validate();
        component.repaint();
    }

    public static void assertNotAwtThread() {
        if (SwingUtilities.isEventDispatchThread()) {
            showError("Non AWT thread expected");
        }
    }

    public static void assertAwtThread() {
        if (!SwingUtilities.isEventDispatchThread()) {
            showError("Not on AWT thread but " + Thread.currentThread().getName());
        }
    }

    private static void showError(String error) {
        Exception e = new IllegalStateException(error);

        StringBuilder trace = new StringBuilder(e + "\n");
        for(StackTraceElement element : e.getStackTrace())
            trace.append(element.toString()).append("\n");
        SwingUtilities.invokeLater(() -> ErrorMessageHelper.showErrorDialog(trace.toString(), "Error"));
    }

    public static boolean hasExistingFile(String zipFileName, long completeFileSize, long lastModified) {
        File file = new File(zipFileName);
        System.out.println("We have size=" + file.length() + " modified=" + new Date(file.lastModified()) + " " + file.getAbsolutePath());
        return file.length() == completeFileSize && file.lastModified() == lastModified;
    }

    public static ImageIcon loadIcon( String strPath ) {
        URL imgURL = dynamicResourcesLoader.getResource( strPath );
        if (imgURL != null) {
            return new ImageIcon(imgURL);
        } else {
            log.info("Using secondary resource path for " + strPath);
            imgURL = dynamicResourcesLoader.getResource(secondaryResourcePath(strPath));
            if (imgURL != null) {
                return new ImageIcon(imgURL);
            }
            log.warn("icon resource not found " + strPath);
            return null;
        }
    }

    /**
     * Maps a bare resource name onto the package our resources actually live in, e.g.
     * {@code /appicon.png} to {@code /com/rusefi/appicon.png}.
     * <p>
     * Careful with the slash: plain concatenation produced {@code /com/rusefi//appicon.png}, and a
     * jar entry lookup is an exact string match, so the doubled slash silently resolved to nothing
     * whenever the console ran from its jar rather than from an IDE class directory. That is why
     * {@link #setAppIcon} used to leave the default Java coffee cup on the frame - see #6928.
     */
    static String secondaryResourcePath(String strPath) {
        String relative = strPath.startsWith("/") ? strPath.substring(1) : strPath;
        return RUSEFI_RESOURCE_PACKAGE + relative;
    }

    public static void setAppIcon(JFrame frame) {
        // huh? sometimes we are making icon from logo and sometimes we have dedicated icon file?!
        ImageIcon icon = loadIcon(APPICON);
        if (icon != null) {
            frame.setIconImage(icon.getImage());
        }
    }

    public static void pack(Window window) {
        trueLayoutAndRepaint(window);
        if (window != null)
            window.pack();
        trueLayoutAndRepaint(window);
    }
}
