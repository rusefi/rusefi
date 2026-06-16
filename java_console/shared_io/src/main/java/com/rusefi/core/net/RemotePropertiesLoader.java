package com.rusefi.core.net;

import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Properties;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Fetches remote property overrides from the universal bundle backend.
 * Returns an empty map on any failure — callers must tolerate that.
 */
public class RemotePropertiesLoader {

    private static final int TIMEOUT_MS = 4_000;

    /**
     * Fetch remote properties for the given board target.
     * Falls back to /console/properties if boardTarget is null or "universal".
     *
     * @param apiBaseUrl  e.g. "http://localhost:8080/api/v1"
     * @param apiToken    Bearer token (read scope)
     * @param boardTarget e.g. "proteus_f7", or null/empty/universal
     * @return properties map, never null, empty on failure
     */
    public static Properties load(String apiBaseUrl, String apiToken, String boardTarget) {
        if (apiBaseUrl == null || apiBaseUrl.isBlank()) {
            return new Properties();
        }

        String path = (boardTarget == null || boardTarget.isBlank() || "universal".equals(boardTarget))
            ? "/console/properties"
            : "/boards/" + boardTarget + "/properties";

        try {
            URL url = new URL(apiBaseUrl.replaceAll("/+$", "") + path);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setConnectTimeout(TIMEOUT_MS);
            conn.setReadTimeout(TIMEOUT_MS);
            conn.setRequestProperty("Authorization", "Bearer " + apiToken);
            conn.setRequestProperty("Accept", "application/json");

            if (conn.getResponseCode() != 200) {
                return new Properties();
            }

            try (InputStream in = conn.getInputStream()) {
                return parseJson(new String(in.readAllBytes()));
            }
        } catch (Exception e) {
            // network down, bad token, API unavailable — silent fallback
            return new Properties();
        }
    }

    /**
     * Minimal JSON parser for {"properties": {"k": "v", ...}}.
     * No external dependency — avoids pulling in a JSON library for this one call.
     * ponytail: replace with org.json / gson if the console already bundles one.
     */
    static Properties parseJson(String json) {
        Properties result = new Properties();
        int start = json.indexOf("\"properties\"");
        if (start < 0) return result;
        int braceOpen  = json.indexOf('{', start + 12);
        int braceClose = json.lastIndexOf('}');
        if (braceOpen < 0 || braceClose <= braceOpen) return result;

        String inner = json.substring(braceOpen + 1, braceClose);
        Matcher m = Pattern.compile("\"([^\"]+)\"\\s*:\\s*\"([^\"]*)\"").matcher(inner);
        while (m.find()) {
            result.setProperty(m.group(1), m.group(2));
        }
        return result;
    }
}
