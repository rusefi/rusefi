package com.rusefi.core.net;

import org.jetbrains.annotations.NotNull;

import javax.net.ssl.*;
import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.security.CodeSource;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.cert.X509Certificate;
import java.util.Objects;
import java.util.Optional;
import java.util.Properties;
import com.devexperts.logging.Logging;

public class ConnectionAndMeta {
    public static final String BASE_URL_RELEASE = "https://github.com/rusefi/rusefi/releases/latest/download/";
    public static final String DEFAULT_WHITE_LABEL = "rusefi";
    public static final String AUTOUPDATE = "/autoupdate/";

    private static final int BUFFER_SIZE = 32 * 1024;
    private volatile static Properties properties; // sad: we do not completely understand #6777 but caching should not hurt
    public static final int CENTUM = 100;
    public static final String IO_PROPERTIES = "/shared_io.properties";
    private final String zipFileName;
    private HttpsURLConnection httpConnection;
    private long completeFileSize;
    private long lastModified;

    private static final Logging log = Logging.getLogging(ConnectionAndMeta.class);

    public ConnectionAndMeta(String zipFileName) {
        this.zipFileName = zipFileName;
    }

    public static String getBaseUrl() {
        String result = getProperties().getProperty("auto_update_root_url");
        System.out.println(ConnectionAndMeta.class + ": got [" + result + "]");
        return result;
    }

    public static String getJarFileNamePrefix() {
        try {
            CodeSource codeSource = ConnectionAndMeta.class.getProtectionDomain().getCodeSource();
            if (codeSource != null) {
                URL jarUrl = codeSource.getLocation();
                String jarPath = jarUrl.getPath();
                String jarFileName = jarPath.substring(jarPath.lastIndexOf('/') + 1);
                int startOfSuffix = jarFileName.lastIndexOf('_');
                if (startOfSuffix == -1)
                    throw new IllegalStateException("Unexpected jar file name: " + jarFileName);
                String jarFilePrefix = jarFileName.substring(0, startOfSuffix);
                log.info("jarFilePrefix = " + jarFilePrefix);
                return jarFilePrefix;
            }
        } catch (Exception e) {
            log.error("Unexpected " + e, e);
        }
        return "rusefi";
    }

    public static String getWhiteLabel(Properties properties) {
        return getStringProperty(properties, "white_label", DEFAULT_WHITE_LABEL);
    }

    public static String getRusEfiConsoleJarName() {
        return getStringProperty(getProperties(), "console_jar", getJarFileNamePrefix() + "_console.jar");
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

    // TS multiplier is technically different from autoscale, open question when we shall allow multiplier without autoscale
    public static boolean flexibleAutoscale() {
        return getBoolean("flexible_autoscale");
    }

    public static boolean showUpdateCalibrations() {
        return getBoolean("show_update_calibrations");
    }

    public static boolean getBoolean(String propertyName) {
        return getBoolean(propertyName, getProperties());
    }

    public static boolean getBoolean(String propertyName, Properties properties) {
        String flag = properties.getProperty(propertyName);
        return Boolean.TRUE.toString().equalsIgnoreCase(flag);
    }

    public synchronized static Properties getProperties() throws RuntimeException {
        if (properties == null) {
            properties = getPropertiesForReal();
        }
        return properties;
    }

    private static Properties getPropertiesForReal() throws RuntimeException {
        Properties props = new Properties();
        try {
            InputStream stream = ConnectionAndMeta.class.getResourceAsStream(IO_PROPERTIES);
            Objects.requireNonNull(stream, "Error reading " + IO_PROPERTIES);
            props.load(stream);
            return props;
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static String getDefaultAutoUpdateUrl() {
        return getBaseUrl() + AUTOUPDATE;
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
        // user can have java with expired certificates or funny proxy, we shall accept any certificate :(
        SSLContext ctx = null;
        try {
            ctx = SSLContext.getInstance("TLS");
            ctx.init(new KeyManager[0], new TrustManager[]{new AcceptAnyCertificateTrustManager()}, new SecureRandom());
        } catch (NoSuchAlgorithmException | KeyManagementException e) {
            throw new IOException("TLS exception", e);
        }

        URL url = new URL(baseUrl + zipFileName);
        System.out.println("Connecting to " + url);
        httpConnection = (HttpsURLConnection) url.openConnection();
        httpConnection.setSSLSocketFactory(ctx.getSocketFactory());
        completeFileSize = httpConnection.getContentLength();
        lastModified = httpConnection.getLastModified();
        return this;
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
