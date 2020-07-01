package com.rusefi.autoupdate;

import com.rusefi.ui.util.FrameHelper;
import org.jetbrains.annotations.NotNull;

import javax.net.ssl.*;
import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.util.Date;
import java.util.Objects;
import java.util.concurrent.atomic.AtomicReference;

public class AutoupdateUtil {
    public static final boolean runHeadless = Boolean.getBoolean("run_headless") || GraphicsEnvironment.isHeadless();

    private static final int BUFFER_SIZE = 32 * 1024;
    private static final int STEPS = 1000;
    // todo: figure out a better way to work with absolute path
    private static final String APPICON = "/appicon.png";

    public static void downloadAutoupdateFile(String localZipFileName, ConnectionAndMeta connectionAndMeta, String title) throws IOException {
        HttpURLConnection httpConnection = connectionAndMeta.httpConnection;
        long completeFileSize = connectionAndMeta.completeFileSize;
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
        new File(localZipFileName).setLastModified(connectionAndMeta.lastModified);

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
        window.pack();
        trueLayout(window);
    }

    public static class ConnectionAndMeta {
        private String zipFileName;
        private HttpsURLConnection httpConnection;
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

        public ConnectionAndMeta invoke() throws IOException, NoSuchAlgorithmException, KeyManagementException {
            // user can have java with expired certificates or funny proxy, we shall accept any certificate :(
            SSLContext ctx = SSLContext.getInstance("TLS");
            ctx.init(new KeyManager[0], new TrustManager[] {new AcceptAnyCertificateTrustManager()}, new SecureRandom());

            URL url = new URL("https://rusefi.com/build_server/autoupdate/" + zipFileName);
            httpConnection = (HttpsURLConnection) url.openConnection();
            httpConnection.setSSLSocketFactory(ctx.getSocketFactory());
            completeFileSize = httpConnection.getContentLength();
            lastModified = httpConnection.getLastModified();
            return this;
        }
    }

    private static class AcceptAnyCertificateTrustManager implements X509TrustManager {

        @Override
        public void checkClientTrusted(X509Certificate[] arg0, String arg1) throws CertificateException {}

        @Override
        public void checkServerTrusted(X509Certificate[] arg0, String arg1) throws CertificateException {}

        @Override
        public X509Certificate[] getAcceptedIssuers() {
            return null;
        }
    }
}
