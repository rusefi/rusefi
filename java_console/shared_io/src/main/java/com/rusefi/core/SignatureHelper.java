package com.rusefi.core;

import com.devexperts.logging.Logging;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.core.FileUtil.RUSEFI_SETTINGS_FOLDER;

public class SignatureHelper {
    private static final Logging log = getLogging(SignatureHelper.class);
    private final static String LOCAL_INI_CACHE_FOLDER = RUSEFI_SETTINGS_FOLDER + "ini_database";
    private static final int HTTP_TIMEOUT_MILLIS = 10_000;

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
     * Checks the local cache before optionally contacting the remote INI archive.
     */
    public static String downloadIfNotAvailable(Pair<String, String> p) {
        return downloadIfNotAvailable(p, null, LOCAL_INI_CACHE_FOLDER);
    }

    /**
     * Same as {@link #downloadIfNotAvailable(Pair)}, but also validates a cache hit against the
     * requested {@code signature}: a cached .ini whose embedded signature diverges from the
     * requested one (in practice: the firmware was rebuilt on a later day with the same config
     * hash) is stale, so the entry is dropped and the regular download/manual-picker path takes over.
     */
    public static String downloadIfNotAvailable(Pair<String, String> p, String signature) {
        return downloadIfNotAvailable(p, signature, LOCAL_INI_CACHE_FOLDER);
    }

    // package-private for tests
    static String downloadIfNotAvailable(Pair<String, String> p, String signature, String cacheFolder) {
        if (p == null)
            return null;
        new File(cacheFolder).mkdirs();
        String localIniFile = cacheFolder + File.separator + p.second;
        File file = new File(localIniFile);
        if (file.exists() && file.length() > 10000) {
            if (signature != null && isStaleCachedIni(file, signature)) {
                log.info("Dropping stale cached ini " + localIniFile
                        + ": embedded signature does not match requested " + signature);
                file.delete();
            } else {
                log.info("Found cached at " + cacheFolder);
                return localIniFile;
            }
        }
        if (EXTRA_INI_SOURCE != null) {
            return EXTRA_INI_SOURCE;
        }
        log.info(".ini not found in " + cacheFolder + "(" + localIniFile + "), trying to download " + p.first);

        // atomic download via .tmp + rename — prevents corrupted partial files from becoming the cache (#10030)
        File tempFile = new File(localIniFile + ".tmp");
        HttpURLConnection httpURLConnection = null;
        try {
            httpURLConnection = (HttpURLConnection) new URL(p.first).openConnection();
            httpURLConnection.setConnectTimeout(HTTP_TIMEOUT_MILLIS);
            httpURLConnection.setReadTimeout(HTTP_TIMEOUT_MILLIS);
            int statusCode = httpURLConnection.getResponseCode();
            if (statusCode >= 300) {
                log.info("Unexpected code " + statusCode);
                tempFile.delete();
                return null;
            }
            try (BufferedInputStream in = new BufferedInputStream(httpURLConnection.getInputStream());
                 FileOutputStream fileOutputStream = new FileOutputStream(tempFile)) {
                byte[] dataBuffer = new byte[32 * 1024];
                int bytesRead;
                while ((bytesRead = in.read(dataBuffer, 0, dataBuffer.length)) != -1) {
                    fileOutputStream.write(dataBuffer, 0, bytesRead);
                }
            }
            tempFile.renameTo(file);
            return localIniFile;
        } catch (IOException e) {
            System.err.println(e.getMessage());
            tempFile.delete();
            return null;
        } finally {
            if (httpURLConnection != null) {
                httpURLConnection.disconnect();
            }
        }
    }

    /**
     * A cached .ini is stale when it carries no {@code signature =} line, or when its embedded
     * signature diverges from the requested one while still sharing the requested config hash.
     * The cache is keyed by hash, so a hit with a different hash means the user explicitly
     * imported a non-matching ini via the manual picker - respect that choice and keep it. A hit
     * with the SAME hash but a different date is the "ini_database and firmware dates diverged"
     * situation (firmware rebuilt on a later day, config unchanged) and must be dropped.
     */
    static boolean isStaleCachedIni(File cachedIni, String requestedSignature) {
        String embeddedSignature = readIniSignature(cachedIni);
        if (embeddedSignature == null) {
            // no signature line - cannot be trusted
            return true;
        }
        if (requestedSignature.equals(embeddedSignature)) {
            return false;
        }
        RusEfiSignature requested = parse(requestedSignature);
        RusEfiSignature embedded = parse(embeddedSignature);
        if (requested == null || embedded == null) {
            // unparseable signatures - not enough information to declare staleness
            return false;
        }
        return java.util.Objects.equals(requested.getHash(), embedded.getHash());
    }

    /**
     * Extract the {@code signature = "rusEFI ..."} value declared near the top of a TunerStudio
     * .ini without a full parse (the first 200 lines are enough). Returns null when the file is
     * unreadable or has no signature line.
     */
    static String readIniSignature(File iniFile) {
        try (BufferedReader reader = new BufferedReader(new FileReader(iniFile))) {
            String line;
            int scanned = 0;
            while ((line = reader.readLine()) != null && scanned++ < 200) {
                final String trimmed = line.trim();
                if (trimmed.startsWith("signature")) {
                    final int eq = trimmed.indexOf('=');
                    if (eq < 0 || !trimmed.substring(0, eq).trim().equals("signature")) {
                        // some other "signature..." token, not the signature declaration
                        continue;
                    }
                    final int first = trimmed.indexOf('"', eq);
                    final int last = trimmed.lastIndexOf('"');
                    if (first >= 0 && last > first) {
                        return trimmed.substring(first + 1, last);
                    }
                }
            }
        } catch (IOException e) {
            // unreadable candidate - treat as "no signature"
        }
        return null;
    }

    /**
     * Path under which the .ini matching {@code signature} would be cached, or null if the
     * signature cannot be parsed. Lets callers remember a locally found ini in the cache
     * without duplicating the cache-folder logic.
     */
    public static String getLocalIniCacheFile(String signature) {
        Pair<String, String> p = getUrl(signature);
        if (p == null) {
            return null;
        }
        return LOCAL_INI_CACHE_FOLDER + File.separator + p.second;
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
