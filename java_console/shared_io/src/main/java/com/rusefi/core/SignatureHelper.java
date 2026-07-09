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
    private final static String LOCAL_INI_CACHE_FOLDER = RUSEFI_SETTINGS_FOLDER + "ini_database";

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

    /**
     * Copies a user-selected .ini into the local cache keyed by the signature hash,
     * so subsequent connects resolve it without any download or prompt.
     * @return cached file path, or null if the signature cannot be parsed
     */
    public static String importIntoCache(String signature, File source) throws IOException {
        return importIntoCache(signature, source, LOCAL_INI_CACHE_FOLDER);
    }

    // package-private for tests
    static String importIntoCache(String signature, File source, String cacheFolder) throws IOException {
        Pair<String, String> p = getUrl(signature);
        if (p == null)
            return null;
        new File(cacheFolder).mkdirs();
        String cachedIniFile = cacheFolder + File.separator + p.second;
        java.nio.file.Files.copy(source.toPath(), new File(cachedIniFile).toPath(),
                java.nio.file.StandardCopyOption.REPLACE_EXISTING);
        log.info("Imported " + source + " into cache as " + cachedIniFile);
        return cachedIniFile;
    }

    public static RusEfiSignature parseSrec(String srecName) {
        if (srecName == null) {
            return null;
        }
        if (srecName.endsWith(".srec")) {
            srecName = srecName.substring(0, srecName.length() - 5);
        }
        if (srecName.endsWith("_update")) {
            srecName = srecName.substring(0, srecName.length() - 7);
        }
        String[] parts = srecName.split("_");
        // format: rusefi_development_2026-05-09_uaefi_pro_4226383888_8849742d4267db6407b1400ae917a1ed39795d32
        // or: rusefi_development_2026-04-27_3659024206688255410edc1e751b6736281e0efd
        if (parts.length < 4) {
            return null;
        }
        String branch = parts[1];
        String date = parts[2];
        String[] dateParts = date.split("-");
        if (dateParts.length != 3) {
            return null;
        }
        String year = dateParts[0];
        String month = dateParts[1];
        String day = dateParts[2];

        if (parts.length >= 6) {
            // New format
            // format: rusefi_branch_date_target_number_hash
            // example: rusefi_development_2026-05-09_uaefi_pro_4226383888_8849742d4267db6407b1400ae917a1ed39795d32
            // parts[0] = rusefi
            // parts[1] = development
            // parts[2] = 2026-05-09
            // parts[parts.length - 1] = hash
            // parts[parts.length - 2] = number
            // target is everything between parts[2] and parts[parts.length - 2]
            StringBuilder targetBuilder = new StringBuilder();
            for (int i = 3; i < parts.length - 2; i++) {
                if (targetBuilder.length() > 0) {
                    targetBuilder.append("_");
                }
                targetBuilder.append(parts[i]);
            }
            String bundleTarget = targetBuilder.toString();
            String hash = parts[parts.length - 2];
            return new RusEfiSignature(branch, year, month, day, bundleTarget, hash, false);
        } else if (parts.length == 4) {
            // Legacy format
            // Legacy format only has the git hash, it does not have the numeric hash
            return new RusEfiSignature(branch, year, month, day, "all", null, true);
        }

        return null;
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

        return new RusEfiSignature(branch, year, month, day, bundleTarget, hash, false);
    }
}
