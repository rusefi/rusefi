package com.rusefi.core.io;

import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.FileSystemNotFoundException;
import java.util.Date;

public class BundleUtil {
    /**
     * @return null in case of error
     */
    @Nullable
    public static String readBundleFullName() {
        try {
            Path path = Paths.get("..");
            String fullName = path.getFileName().toString();
            if (fullName.length() < 3)
                return null; // just paranoia check
            return fullName;
        } catch (FileSystemNotFoundException e) {
            System.err.println(new Date() + ": BundleUtil: Error reading bundle name");
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
