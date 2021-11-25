package com.rusefi.shared;

import javax.net.ssl.*;
import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.cert.X509Certificate;
import java.util.Objects;

public class ConnectionAndMeta {
    public static final String BASE_URL_RELEASE = "https://github.com/rusefi/rusefi/releases/latest/download/";
    public static final String BASE_URL_LATEST = "https://rusefi.com/build_server/autoupdate/";

    private static final int BUFFER_SIZE = 32 * 1024;
    public static final int STEPS = 1000;
    private final String zipFileName;
    private HttpsURLConnection httpConnection;
    private long completeFileSize;
    private long lastModified;

    public ConnectionAndMeta(String zipFileName) {
        this.zipFileName = zipFileName;
    }

    public static void downloadFile(String localTargetFileName, ConnectionAndMeta connectionAndMeta, DownloadProgressListener listener) throws IOException {
        HttpURLConnection httpConnection = connectionAndMeta.getHttpConnection();
        long completeFileSize = connectionAndMeta.getCompleteFileSize();
        Objects.requireNonNull(httpConnection, "httpConnection");
        BufferedInputStream in = new BufferedInputStream(httpConnection.getInputStream());
        FileOutputStream fos = new FileOutputStream(localTargetFileName);
        BufferedOutputStream bout = new BufferedOutputStream(fos, BUFFER_SIZE);
        byte[] data = new byte[BUFFER_SIZE];
        long downloadedFileSize = 0;
        int newDataSize;

        int printedPercentage = 0;

        while ((newDataSize = in.read(data, 0, BUFFER_SIZE)) >= 0) {
            downloadedFileSize += newDataSize;

            // calculate progress
            final int currentProgress = (int) ((((double) downloadedFileSize) / ((double) completeFileSize)) * STEPS);

            int currentPercentage = (int) (100L * downloadedFileSize / completeFileSize);
            if (currentPercentage > printedPercentage + 5) {
                System.out.println("Downloaded " + currentPercentage + "%");
                printedPercentage = currentPercentage;
            }

            listener.onPercentage(currentProgress);

            bout.write(data, 0, newDataSize);
        }
        bout.close();
        in.close();
        new File(localTargetFileName).setLastModified(connectionAndMeta.getLastModified());
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

    public ConnectionAndMeta invoke(String baseUrl) throws IOException, NoSuchAlgorithmException, KeyManagementException {
        // user can have java with expired certificates or funny proxy, we shall accept any certificate :(
        SSLContext ctx = SSLContext.getInstance("TLS");
        ctx.init(new KeyManager[0], new TrustManager[]{new AcceptAnyCertificateTrustManager()}, new SecureRandom());

        URL url = new URL(baseUrl + zipFileName);
        System.out.println("Connecting to " + url);
        httpConnection = (HttpsURLConnection) url.openConnection();
        httpConnection.setSSLSocketFactory(ctx.getSocketFactory());
        completeFileSize = httpConnection.getContentLength();
        lastModified = httpConnection.getLastModified();
        return this;
    }

    public interface DownloadProgressListener {
        void onPercentage(int currentProgress);
    }

    private static class AcceptAnyCertificateTrustManager implements X509TrustManager {
        @Override
        public void checkClientTrusted(X509Certificate[] arg0, String arg1) {
        }

        @Override
        public void checkServerTrusted(X509Certificate[] arg0, String arg1) {
        }

        @Override
        public X509Certificate[] getAcceptedIssuers() {
            return null;
        }
    }
}
