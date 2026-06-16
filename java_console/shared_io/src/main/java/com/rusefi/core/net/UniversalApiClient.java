package com.rusefi.core.net;

import com.devexperts.logging.Logging;
import com.rusefi.core.Pair;
import com.rusefi.core.SignatureHelper;

import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Optional;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Thin client for the universal bundle backend API.
 * Reads api_base_url and api_token from shared_io.properties (via PropertiesHolder).
 * All methods return empty / null and fall through gracefully on any failure.
 */
public class UniversalApiClient {
    private static final Logging log = getLogging(UniversalApiClient.class);

    private static final int TIMEOUT_MS = 5_000;

    /**
     * Resolve .ini for the given board + config hash via the API.
     * The API returns a 302 redirect to the actual .ini URL on the file server,
     * plus an X-Bundle-Url header pointing to the full bundle ZIP.
     * The .ini is downloaded and cached in ~/.rusEFI/ini_database/.
     *
     * @return Pair(localIniPath, bundleUrl), or null on failure.
     *         bundleUrl (second) may be null if the server didn't return X-Bundle-Url.
     */
    @Nullable
    public static Pair<String, String> resolveIni(String boardId, String hash) {
        String apiBaseUrl = apiBaseUrl();
        if (apiBaseUrl == null) return null;

        String apiUrl = apiBaseUrl + "/boards/" + boardId + "/ini/" + hash;
        log.info("Resolving ini via API: " + apiUrl);
        try {
            HttpURLConnection conn = openConn(apiUrl);
            int code = conn.getResponseCode();
            String bundleUrl = conn.getHeaderField("X-Bundle-Url");
            String location  = conn.getHeaderField("Location");
            conn.disconnect();

            if ((code == 301 || code == 302) && location != null) {
                String localPath = SignatureHelper.downloadIfNotAvailable(new Pair<>(location, hash + ".ini"));
                if (localPath != null) {
                    return new Pair<>(localPath, bundleUrl);
                }
            } else {
                log.info("API returned " + code + " for " + apiUrl);
            }
        } catch (IOException e) {
            log.info("API unreachable: " + e.getMessage());
        }
        return null;
    }

    /**
     * Ask the API for the direct autoupdate zip URL for this board+branch.
     *
     * Returns empty when:
     *   - api_base_url is not configured (feature off)
     *   - the API returns 404 (no URL registered — use convention)
     *   - any network / IO error
     *
     * Returns the full URL to the autoupdate zip when the API returns 302.
     */
    public static Optional<String> getAutoupdateUrl(String boardTarget, String branch) {
        String apiBaseUrl = apiBaseUrl();
        if (apiBaseUrl == null) return Optional.empty();

        String path = apiBaseUrl + "/boards/" + boardTarget + "/autoupdate?branch=" + branch;
        try {
            HttpURLConnection conn = openConn(path);
            int code = conn.getResponseCode();
            if (code == HttpURLConnection.HTTP_MOVED_TEMP) {
                return Optional.ofNullable(conn.getHeaderField("Location"));
            }
            // 404 = not registered → caller falls back to convention
        } catch (Exception e) {
            log.info("getAutoupdateUrl failed: " + e.getMessage());
        }
        return Optional.empty();
    }

    /** Infer isObfuscated from the autoupdate zip filename.
     * "_obfuscated_public_autoupdate.zip" is enforced by bundle.mk.
     */
    public static boolean isObfuscatedUrl(@NotNull String fullUrl) {
        return fullUrl.contains("obfuscated");
    }

    /** Split a full URL into [baseUrl, fileName] for ConnectionAndMeta.
     *  "https://host.com/path/file.zip" → ["https://host.com/path/", "file.zip"] */
    public static String[] splitUrl(@NotNull String fullUrl) {
        int q = fullUrl.indexOf('?');
        String clean = q >= 0 ? fullUrl.substring(0, q) : fullUrl;
        int slash = clean.lastIndexOf('/');
        return new String[]{ clean.substring(0, slash + 1), clean.substring(slash + 1) };
    }

    /** @return api_base_url from properties, stripped; null if empty (feature off) */
    @Nullable
    private static String apiBaseUrl() {
        String url = PropertiesHolder.getProperty("api_base_url", "").trim();
        return url.isEmpty() ? null : url.replaceAll("/+$", "");
    }

    private static HttpURLConnection openConn(String url) throws IOException {
        HttpURLConnection conn = (HttpURLConnection) new URL(url).openConnection();
        conn.setInstanceFollowRedirects(false);
        conn.setConnectTimeout(TIMEOUT_MS);
        conn.setReadTimeout(TIMEOUT_MS);
        conn.setRequestProperty("Authorization", "Bearer " + PropertiesHolder.getProperty("api_token", ""));
        conn.setRequestProperty("User-Agent", "RE-Internal-Sync");
        return conn;
    }
}
