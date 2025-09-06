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
    static final String LOCAL_CACHE = FileUtil.RUSEFI_SETTINGS_FOLDER + File.separator + "tune_manifest" + File.separator;
    static final String MANIFEST_FILE_NAME = "manifest.json";

    static String trimUrlToLocalName(String url) {
        if (url.startsWith(HTTPS))
            url = url.substring(HTTPS.length());
        return url.replaceAll(LESS_SAFE_SYMBOLS, "");
    }

    static @Nullable String downloadFile(String baseUrl, String fullFileUrl, String localFileName) throws IOException {
        String localCacheForSpecificUrl = getLocalFolder(baseUrl);

        String fullLocalFileName = localCacheForSpecificUrl + localFileName;

        URL url = new URL(fullFileUrl);
        try {
            try (InputStream in = url.openStream()) {
                // Copy the stream to a file, replacing the file if it already exists
                Files.copy(in, Paths.get(fullLocalFileName), StandardCopyOption.REPLACE_EXISTING);
                log.info("File downloaded successfully to " + fullLocalFileName);
            }
        } catch (FileNotFoundException e) {
            log.error("File not found: " + fullFileUrl);
            return null;
        }
        return fullLocalFileName;
    }

    private static @NotNull String getLocalFolder(String baseUrl) {
        String localCacheForSpecificUrl = LOCAL_CACHE + trimUrlToLocalName(baseUrl) + File.separator;
        new File(localCacheForSpecificUrl).mkdirs();
        return localCacheForSpecificUrl;
    }

    static List<TuneModel> parseManifest(String localManifest) throws IOException, ParseException {
        List<TuneModel> tunes = new ArrayList<>();
        try {
            JSONParser parser = new JSONParser();
            try (FileReader reader = new FileReader(localManifest)) {
                // Parse the JSON file
                Object obj = parser.parse(reader);

                // Cast the parsed object to a JSONObject
                JSONArray topLevelArray = (JSONArray) obj;

                for (int i = 0; i < topLevelArray.size(); i++) {
                    JSONObject object = (JSONObject) topLevelArray.get(i);
                    tunes.add(TuneModel.parse(object));
                }
            }
            return tunes;
        } catch (ClassCastException e) {
            throw new IOException("JSON error " + e, e);
        }
    }
}
