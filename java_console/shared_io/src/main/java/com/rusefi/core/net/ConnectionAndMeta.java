package com.rusefi.core.net;

import org.jetbrains.annotations.NotNull;

import javax.net.ssl.*;
import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.cert.X509Certificate;
import java.util.*;
import java.util.stream.Collectors;

public class ConnectionAndMeta {
    public static final String BASE_URL_RELEASE = "https://github.com/rusefi/rusefi/releases/latest/download/";
    public static final String DEFAULT_WHITE_LABEL = "rusefi";
    public static final String AUTOUPDATE = "/autoupdate/";

    private static final int BUFFER_SIZE = 32 * 1024;
    public static final int CENTUM = 100;
    private final String zipFileName;
    private HttpsURLConnection httpConnection;
    private long completeFileSize;
    private long lastModified;

    public ConnectionAndMeta(String zipFileName) {
        this.zipFileName = zipFileName;
    }

    public static String getWhiteLabel(Properties properties) {
        return getStringProperty(properties, "white_label", DEFAULT_WHITE_LABEL);
    }

    public static String getRusEfiConsoleJarName() {
        String defaultValue = JarFileUtil.getJarFileNamePrefix() + "_console.jar";
        // why would we need this configurable? if we need it for development under IDE it should probably be done differently?
        //return getStringProperty(getProperties(), "console_jar", defaultValue);
        return defaultValue;
    }

    private static @NotNull String getStringProperty(Properties properties, String key, String defaultValue) {
        return Optional.ofNullable(properties.getProperty(key)).map(String::trim)
            .orElse(defaultValue);
    }

    public static String getSignatureWhiteLabel() {
        String signatureWhiteLabel = getProperties().getProperty("signature_white_label");
        signatureWhiteLabel = signatureWhiteLabel == null ? null : signatureWhiteLabel.trim();
        return signatureWhiteLabel;
    }
/*
    public static boolean showUpdateCalibrations() {
        return getBoolean("show_update_calibrations");
    }
*/
    public static boolean getBoolean(String propertyName) {
        return getBoolean(propertyName, getProperties());
    }

    public static boolean getBoolean(String propertyName, Properties properties) {
        return PropertiesHolder.INSTANCE.getBoolean(propertyName, properties);
    }

    public synchronized static Properties getProperties() throws RuntimeException {
        return PropertiesHolder.INSTANCE.getProperties();
    }

    public static String getDefaultAutoUpdateUrl() {
        return PropertiesHolder.INSTANCE.getBaseUrl() + AUTOUPDATE;
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
            int currentPercentage = (int) (CENTUM * downloadedFileSize / completeFileSize);
            if (currentPercentage > printedPercentage + 5) {
                System.out.println("Downloaded " + currentPercentage + "%");
                printedPercentage = currentPercentage;
                listener.onPercentage(currentPercentage);
            }


            bout.write(data, 0, newDataSize);
        }
        bout.close();
        in.close();
        new File(localTargetFileName).setLastModified(connectionAndMeta.getLastModified());
    }

    public static boolean isDefaultWhitelabel(String whiteLabel) {
        return DEFAULT_WHITE_LABEL.equals(whiteLabel);
    }

    public static boolean saveSettingsToFile() {
        return getBoolean("binary_config_image");
    }

    public static boolean saveReadmeHtmlToFile() {
        return Boolean.TRUE.toString().equalsIgnoreCase(getStringProperty(getProperties(), "write_readme_html", "false"));
    }
/*
    public static boolean startConsoleInAutoupdateProcess() {
        return false;
    }
*/
    public static Set<String> getNonMigratableIniFields() {
        final String nonMergeableIniFields = getStringProperty(getProperties(), "non_migratable_ini_fields", "");
        return Arrays.stream(nonMergeableIniFields.split(","))
            .map(String::trim)
            .filter(s -> !s.isEmpty())
            .collect(Collectors.toSet());
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

    public ConnectionAndMeta invoke(String baseUrl) throws IOException {
        SSLContext ctx = acceptAnyCertificate();

        URL url = new URL(baseUrl + zipFileName);
        System.out.println("Connecting to " + url);
        httpConnection = (HttpsURLConnection) url.openConnection();
        httpConnection.setSSLSocketFactory(ctx.getSocketFactory());
        completeFileSize = httpConnection.getContentLength();
        lastModified = httpConnection.getLastModified();
        return this;
    }

    private static @NotNull SSLContext acceptAnyCertificate() throws IOException {
        // user can have java with expired certificates or funny proxy, we shall accept any certificate :(
        SSLContext ctx;
        try {
            ctx = SSLContext.getInstance("TLS");
            ctx.init(new KeyManager[0], new TrustManager[]{new AcceptAnyCertificateTrustManager()}, new SecureRandom());
        } catch (NoSuchAlgorithmException | KeyManagementException e) {
            throw new IOException("TLS exception", e);
        }
        return ctx;
    }

    public interface DownloadProgressListener {
        void onPercentage(int currentPercentage);
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
