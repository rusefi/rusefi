package com.rusefi;

import com.rusefi.core.Pair;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URL;

import static com.rusefi.shared.FileUtil.RUSEFI_SETTINGS_FOLDER;

public class SignatureHelper {
    private final static String LOCAL_INI = RUSEFI_SETTINGS_FOLDER + File.separator + "ini_database";

    // todo: find a way to reference Fields.PROTOCOL_SIGNATURE_PREFIX
    private static final String PREFIX = "rusEFI ";
    private static final char SLASH = '/';

    public static Pair<String, String> getUrl(String signature) {
        RusEfiSignature s = parse(signature);
        if (s == null)
            return null;

        String fileName = s.getHash() + ".ini";
        return new Pair("https://rusefi.com/online/ini/rusefi/" + s.getYear() + SLASH +
                s.getMonth() + SLASH +
                s.getDay() + SLASH +
                s.getBundle() + SLASH +
                fileName, fileName);
    }

    public static String downloadIfNotAvailable(Pair<String, String> p) {
        if (p == null)
            return null;
        new File(LOCAL_INI).mkdirs();
        String localIniFile = LOCAL_INI + File.separator + p.second;
        File file = new File(localIniFile);
        if (file.exists() && file.length() > 10000)
            return localIniFile;
        try (BufferedInputStream in = new BufferedInputStream(new URL(p.first).openStream());
             FileOutputStream fileOutputStream = new FileOutputStream(localIniFile)) {
            byte[] dataBuffer = new byte[32 * 1024];
            int bytesRead;
            while ((bytesRead = in.read(dataBuffer, 0, dataBuffer.length)) != -1) {
                fileOutputStream.write(dataBuffer, 0, bytesRead);
            }
            return localIniFile;
        } catch (IOException e) {
            System.err.println(e.getMessage());
            return null;
        }
    }

    public static RusEfiSignature parse(String signature) {
        if (signature == null || !signature.startsWith(PREFIX))
            return null;
        signature = signature.substring(PREFIX.length()).trim();
        String[] elements = signature.split("\\.");
        if (elements.length != 5)
            return null;

        String year = elements[0];
        String month = elements[1];
        String day = elements[2];
        String bundle = elements[3];
        String hash = elements[4];

        return new RusEfiSignature(year, month, day, bundle, hash);
    }
}
