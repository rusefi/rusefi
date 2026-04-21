package com.rusefi.tune_manifest;

import com.devexperts.logging.Logging;
import com.rusefi.core.FileUtil;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.List;

import static com.devexperts.logging.Logging.getLogging;

public class TuneManifestHelper {
    private static final Logging log = getLogging(TuneManifestHelper.class);

    private static final String HTTPS = "https";

    // Regex to match any character that is NOT a letter (a-z, A-Z) or digit etc
    private static final String LESS_SAFE_SYMBOLS = "[^a-zA-Z0-9_-]";
    static final String LOCAL_CACHE = FileUtil.RUSEFI_SETTINGS_FOLDER + "tune_manifest" + File.separator;
    static final String MANIFEST_FILE_NAME = "manifest.json";

    static String trimUrlToLocalName(String url) {
        if (url.startsWith(HTTPS))
            url = url.substring(HTTPS.length());
        return url.replaceAll(LESS_SAFE_SYMBOLS, "");
    }

    static @Nullable String downloadFile(String baseUrl, String fullFileUrl, String localFileName) throws IOException {
        return downloadFile(baseUrl, fullFileUrl, localFileName, null);
    }

    static @Nullable String downloadFile(String baseUrl, String fullFileUrl, String localFileName, @Nullable String acceptHeader) throws IOException {
        String localFolderForSpecificUrl = getLocalFolder(baseUrl);

        String fullLocalFileName = localFolderForSpecificUrl + localFileName;

        URL url = new URL(fullFileUrl);
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        if (acceptHeader != null) {
            connection.setRequestProperty("Accept", acceptHeader);
        }
        try {
            int status = connection.getResponseCode();
            if (status == HttpURLConnection.HTTP_NOT_FOUND) {
                log.error("File not found: " + fullFileUrl);
                return null;
            }
            if (status / 100 != 2) {
                String body = readErrorBody(connection);
                throw new IOException("HTTP " + status + " " + connection.getResponseMessage()
                    + " for " + fullFileUrl + (body.isEmpty() ? "" : ": " + body));
            }
            try (InputStream in = connection.getInputStream()) {
                // Copy the stream to a file, replacing the file if it already exists
                Files.copy(in, Paths.get(fullLocalFileName), StandardCopyOption.REPLACE_EXISTING);
                log.info("File downloaded successfully to " + fullLocalFileName);
            }
        } finally {
            connection.disconnect();
        }
        return fullLocalFileName;
    }

    private static String readErrorBody(HttpURLConnection connection) {
        InputStream err = connection.getErrorStream();
        if (err == null)
            return "";
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(err))) {
            StringBuilder sb = new StringBuilder();
            String line;
            int maxChars = 500;
            while ((line = reader.readLine()) != null && sb.length() < maxChars) {
                sb.append(line).append(' ');
            }
            String s = sb.toString().trim();
            return s.length() > maxChars ? s.substring(0, maxChars) + "..." : s;
        } catch (IOException ignored) {
            return "";
        }
    }

    public static @NotNull String getLocalFolder(String baseUrl) {
        String localCacheForSpecificUrl = LOCAL_CACHE + trimUrlToLocalName(baseUrl) + File.separator;
        new File(localCacheForSpecificUrl).mkdirs();
        return localCacheForSpecificUrl;
    }

    static List<TuneModel> parseManifest(String localManifest) throws IOException, ParseException {
        try {
            List<TuneModel> tunes = new ArrayList<>();
            JSONParser parser = new JSONParser();
            try (FileReader reader = new FileReader(localManifest)) {
                Object obj = parser.parse(reader);
                JSONArray topLevelArray = (JSONArray) obj;
                for (int i = 0; i < topLevelArray.size(); i++) {
                    JSONObject object = (JSONObject) topLevelArray.get(i);
                    tunes.add(TuneModel.parse(object));
                }
            }
            return tunes;
        } catch (ClassCastException | ParseException e) {
            throw new ManifestParseException(readFileBodySafely(localManifest), e);
        }
    }

    private static String readFileBodySafely(String path) {
        try {
            return new String(Files.readAllBytes(Paths.get(path)));
        } catch (IOException e) {
            return "(could not read local manifest file: " + e.getMessage() + ")";
        }
    }

    public static void downloadAllTunes(String baseUrl, Callback callback) throws IOException, ParseException {
        String localManifest = downloadFile(baseUrl, baseUrl + MANIFEST_FILE_NAME, MANIFEST_FILE_NAME, "application/json");
        if (localManifest == null) {
            throw new IOException("Manifest not found at " + baseUrl + MANIFEST_FILE_NAME);
        }
        List<TuneModel> tunes = parseManifest(localManifest);

        for (TuneModel t : tunes) {
            String localTuneName = t.getSaferLocalFileName();
            // at the moment we always download latest version of all tunes
            String localFile = downloadFile(baseUrl, t.getUrl(), localTuneName);
            t.setError(localFile == null);
        }
        callback.onDownloaded(tunes);
    }

    public interface Callback {
        void onDownloaded(List<TuneModel> tunes);
    }
}
