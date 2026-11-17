package com.rusefi.core.net;

import com.devexperts.logging.Logging;
import org.jetbrains.annotations.NotNull;

import javax.net.ssl.*;
import java.io.*;
import java.net.HttpURLConnection;
import java.net.ProtocolException;
import java.net.URL;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.cert.X509Certificate;
import java.util.*;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;

public class ConnectionAndMeta {
    private static final Logging log = getLogging(ConnectionAndMeta.class);

    public static final String BASE_URL_RELEASE = "https://github.com/rusefi/rusefi/releases/latest/download/";
    public static final String DEFAULT_WHITE_LABEL = "rusefi";
    public static final String AUTOUPDATE = "/autoupdate/";
    public static final String RUSEFI_WIKI_DOWNLOAD_PAGE = "https://wiki.rusefi.com/Download/";

    private static final int BUFFER_SIZE = 32 * 1024;
    private static final int TIMEOUT_MILLIS = 10_000;
    public static final int CENTUM = 100;
    private final String zipFileName;
    private final ConnectionFactory connectionFactory;
    private HttpsURLConnection httpConnection;
    private long completeFileSize;
    private long lastModified;
    private int responseCode;
    private String contentType;

    @FunctionalInterface
    interface ConnectionFactory {
        HttpsURLConnection open(URL url) throws IOException;
    }

    public ConnectionAndMeta(String zipFileName) {
        this(zipFileName, url -> (HttpsURLConnection) url.openConnection());
    }

    ConnectionAndMeta(String zipFileName, ConnectionFactory connectionFactory) {
        this.zipFileName = zipFileName;
        this.connectionFactory = connectionFactory;
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

    public static boolean getBoolean(String propertyName) {
        return getBoolean(propertyName, getProperties());
    }

    public static boolean getBoolean(String propertyName, Properties properties) {
        return PropertiesHolder.INSTANCE.getBoolean(propertyName, properties);
    }

    public static boolean getBoolean(String propertyName, Properties properties, boolean defaultValue) {
        return PropertiesHolder.getBoolean(propertyName, properties, defaultValue);
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
        if (completeFileSize == 0) {
            log.error("Server did not provide valid metadata, zero size?!");
            completeFileSize = 1;
        }
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
                log.info("Downloaded " + currentPercentage + "%");
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

    public int getResponseCode() {
        return responseCode;
    }

    public String getContentType() {
        return contentType;
    }

    public ConnectionAndMeta invoke(String baseUrl) throws IOException {
        return invoke(baseUrl, "GET");
    }

    /** Request response headers without downloading the firmware ZIP body. */
    public ConnectionAndMeta invokeMetadata(String baseUrl) throws IOException {
        return invoke(baseUrl, "HEAD");
    }

    private ConnectionAndMeta invoke(String baseUrl, String method) throws IOException {
        SSLContext ctx = acceptAnyCertificate();

        String randomSuffix = "?u=" + UUID.randomUUID();
        URL url = new URL(baseUrl + zipFileName + randomSuffix);
        log.info("Connecting to " + url);
        httpConnection = connectionFactory.open(url);
        httpConnection.setConnectTimeout(TIMEOUT_MILLIS);
        httpConnection.setReadTimeout(TIMEOUT_MILLIS);
        try {
            configureRequest(httpConnection, method);
            httpConnection.setSSLSocketFactory(ctx.getSocketFactory());
            log.info("Request Headers: " + httpConnection.getRequestProperties());
            responseCode = httpConnection.getResponseCode();
            log.info("Response code " + responseCode);
            if ("GET".equals(method) && responseCode == HttpURLConnection.HTTP_FORBIDDEN) {
                InputStream errorStream = httpConnection.getErrorStream();
                echoErrorStream(errorStream);
            }
            completeFileSize = httpConnection.getContentLength();
            lastModified = httpConnection.getLastModified();
            contentType = httpConnection.getContentType();
            return this;
        } catch (IOException | RuntimeException e) {
            httpConnection.disconnect();
            httpConnection = null;
            throw e;
        }
    }

    private static void configureRequest(HttpsURLConnection connection, String method) throws ProtocolException {
        String mySecretUA = "RE-Internal-Sync";
        connection.setRequestMethod(method);
        connection.setUseCaches(false);
        connection.setDefaultUseCaches(false);
        connection.setRequestProperty("User-Agent", mySecretUA);
        connection.setRequestProperty("Cache-Control", "no-cache, no-store, must-revalidate, max-age=0");
        connection.setRequestProperty("Pragma", "no-cache");
        connection.setRequestProperty("Expires", "0");
    }

    private static void echoErrorStream(InputStream errorStream) {
        String line;
        while (true) {
            try {
                if (!((line = new BufferedReader(new InputStreamReader(errorStream)).readLine()) != null)) break;
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
            log.warn("responce " + line);
        }
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
