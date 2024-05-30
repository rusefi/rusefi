package com.rusefi.core.io;

import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.InvalidPathException;
import java.util.Date;

public class BundleUtil {
    private static final char BUNDLE_TOKEN_SEPARATOR = '.';

    /**
     * @return null in case of error
     */
    @Nullable
    public static String readBundleFullName() {
        try {
            Path path = Paths.get("").toAbsolutePath();
            String fullName = path.getParent().getFileName().toString();
            if (fullName.length() < 3)
                return null; // just paranoia check
            return fullName;
        } catch (InvalidPathException e) {
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

    public static String getBundleTarget() {
        return getBundleTarget(readBundleFullName());
    }

    public static BundleInfo parse(String bundleFullName) {
        String[] bundleFullNameSplit = bundleFullName.split("\\" + BundleUtil.BUNDLE_TOKEN_SEPARATOR);
        if (bundleFullNameSplit.length != 3)
            throw new IllegalStateException("Unexpected parent folder name/bundleFullName [" + bundleFullName + "] exactly two dots expected");
        String branchName = bundleFullNameSplit[1];
        String target = bundleFullNameSplit[2];
        return new BundleInfo(branchName, target);
    }

    public static String getBundleTarget(String s) {
        return parse(s).getTarget();
    }

    public static class BundleInfo {
        private final String branchName;
        private final String target;

        public BundleInfo(String branchName, String target) {
            this.branchName = branchName;
            this.target = target;
        }

        public String getBranchName() {
            return branchName;
        }

        public String getTarget() {
            return target;
        }

        @Override
        public String toString() {
            return "BundleInfo{" +
                "branchName='" + branchName + '\'' +
                ", target='" + target + '\'' +
                '}';
        }
    }
}
