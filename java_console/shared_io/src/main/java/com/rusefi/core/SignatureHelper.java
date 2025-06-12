package com.rusefi.core;

import com.devexperts.logging.Logging;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.core.FileUtil.RUSEFI_SETTINGS_FOLDER;

public class SignatureHelper {
    private static final Logging log = getLogging(SignatureHelper.class);
    private final static String LOCAL_INI_CACHE_FOLDER = RUSEFI_SETTINGS_FOLDER + File.separator + "ini_database";

    // todo: find a way to reference Fields.PROTOCOL_SIGNATURE_PREFIX
    private static final String PREFIX = "rusEFI ";
    private static final char SLASH = '/';
    /**
     * java -Dextra_local_ini_file_name=path_to_local_generated.ini ...
     */
    private static final String EXTRA_INI_SOURCE = System.getProperty("extra_local_ini_file_name");

    public static Pair<String, String> getUrl(String signature) {
        RusEfiSignature s = parse(signature);
        if (s == null)
            return null;

        String fileName = s.getHash() + ".ini";
        return new Pair<>("https://rusefi.com/online/ini/rusefi/" + s.getBranch() + SLASH + s.getYear() + SLASH +
                s.getMonth() + SLASH +
                s.getDay() + SLASH +
                s.getBundleTarget() + SLASH +
                fileName, fileName);
    }

    /**
     * @return local .ini file name
     */
    public static String downloadIfNotAvailable(Pair<String, String> p) {
        if (p == null)
            return null;
        new File(LOCAL_INI_CACHE_FOLDER).mkdirs();
        String localIniFile = LOCAL_INI_CACHE_FOLDER + File.separator + p.second;
        File file = new File(localIniFile);
        if (file.exists() && file.length() > 10000) {
            log.info("Found cached at " + LOCAL_INI_CACHE_FOLDER);
            return localIniFile;
        }
        if (EXTRA_INI_SOURCE != null) {
            return EXTRA_INI_SOURCE;
        }
        log.info(".ini not found in " + LOCAL_INI_CACHE_FOLDER + "(" + localIniFile + "), trying to download " + p.first);
        try {
            HttpURLConnection httpURLConnection = (HttpURLConnection) new URL(p.first).openConnection();
            int statusCode = httpURLConnection.getResponseCode();
            if (statusCode >= 300) {
                log.info("Unexpected code " + statusCode);
                return null;
            }
            try (BufferedInputStream in = new BufferedInputStream(httpURLConnection.getInputStream());
                 FileOutputStream fileOutputStream = new FileOutputStream(localIniFile)) {
                byte[] dataBuffer = new byte[32 * 1024];
                int bytesRead;
                while ((bytesRead = in.read(dataBuffer, 0, dataBuffer.length)) != -1) {
                    fileOutputStream.write(dataBuffer, 0, bytesRead);
                }
                return localIniFile;
            }
        } catch (IOException e) {
            System.err.println(e.getMessage());
            return null;
        }
    }

    public static RusEfiSignature parse(final String signature) {
        if (signature == null || !signature.startsWith(PREFIX))
            return null;
        final String signatureWithoutPrefix = signature.substring(PREFIX.length()).trim();
        final String[] elements = signatureWithoutPrefix.split("\\.");
        if (elements.length != 6)
            return null;

        String branch = elements[0];
        String year = elements[1];
        String month = elements[2];
        String day = elements[3];
        String bundleTarget = elements[4];
        String hash = elements[5];

        return new RusEfiSignature(branch, year, month, day, bundleTarget, hash);
    }
}
