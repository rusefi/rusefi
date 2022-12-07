package com.rusefi.core.io;

import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Date;

public class BundleUtil {
    private static final String BUNDLE_NAME_FILE = "../bundle_name.txt";

    /**
     * @return null in case of error
     */
    @Nullable
    public static String readBundleFullName() {
        try {
            BufferedReader r = new BufferedReader(new FileReader(BUNDLE_NAME_FILE));
            String fullName = r.readLine();
            fullName = fullName.trim();
            if (fullName.length() < 3)
                return null; // just paranoia check
            return fullName;
        } catch (IOException e) {
            System.err.println(new Date() + ": BundleUtil: Error reading " + BUNDLE_NAME_FILE);
            return null;
        }
    }

    @NotNull
    public static String readBundleFullNameNotNull() {
        String bundle = readBundleFullName();
        bundle = bundle == null ? "unknown bundle" : bundle;
        return bundle;
    }
}
