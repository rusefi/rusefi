package com.rusefi.autoupdate;

import com.rusefi.ui.util.FrameHelper;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.Objects;
import java.util.concurrent.atomic.AtomicReference;

public class AutoupdateUtil {
    public static final boolean runHeadless = Boolean.getBoolean("run_headless") || GraphicsEnvironment.isHeadless();

    private static final int BUFFER_SIZE = 32 * 1024;
    private static final int STEPS = 1000;

    public static void downloadAutoupdateFile(String localZipFileName, HttpURLConnection httpConnection, long completeFileSize, String title) throws IOException {
        Objects.requireNonNull(httpConnection, "httpConnection");
        BufferedInputStream in = new BufferedInputStream(httpConnection.getInputStream());
        FileOutputStream fos = new FileOutputStream(localZipFileName);
        BufferedOutputStream bout = new BufferedOutputStream(fos, BUFFER_SIZE);
        byte[] data = new byte[BUFFER_SIZE];
        long downloadedFileSize = 0;
        int newDataSize;

        int printedPercentage = 0;

        FrameHelper frameHelper = null;
        final AtomicReference<JProgressBar> jProgressBarAtomicReference = new AtomicReference<>();
        if (!runHeadless) {
            frameHelper = new FrameHelper();
            JProgressBar jProgressBar = new JProgressBar();

            frameHelper.getFrame().setTitle(title);
            jProgressBar.setMaximum(STEPS);
            jProgressBarAtomicReference.set(jProgressBar);
            frameHelper.showFrame(jProgressBar, true);
        }

        while ((newDataSize = in.read(data, 0, BUFFER_SIZE)) >= 0) {
            downloadedFileSize += newDataSize;

            // calculate progress
            final int currentProgress = (int) ((((double) downloadedFileSize) / ((double) completeFileSize)) * STEPS);

            int currentPercentage = (int) (100L * downloadedFileSize / completeFileSize);
            if (currentPercentage > printedPercentage + 5) {
                System.out.println("Downloaded " + currentPercentage + "%");
                printedPercentage = currentPercentage;
            }

            if (!runHeadless) {
                SwingUtilities.invokeLater(() -> jProgressBarAtomicReference.get().setValue(currentProgress));
            }

            bout.write(data, 0, newDataSize);
        }
        bout.close();
        in.close();

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

    public static class ConnectionAndMeta {
        private String zipFileName;
        private HttpURLConnection httpConnection;
        private long completeFileSize;
        private long lastModified;

        public ConnectionAndMeta(String zipFileName) {
            this.zipFileName = zipFileName;
        }

        public HttpURLConnection getHttpConnection() {
            return httpConnection;
        }

        public long getCompleteFileSize() {
            return completeFileSize;
        }

        public long getLastModified() {
            return lastModified;
        }

        public ConnectionAndMeta invoke() throws IOException {
            URL url = new URL("https://rusefi.com/build_server/autoupdate/" + zipFileName);
            httpConnection = (HttpURLConnection) url.openConnection();
            completeFileSize = httpConnection.getContentLength();
            lastModified = httpConnection.getLastModified();
            return this;
        }
    }
}
