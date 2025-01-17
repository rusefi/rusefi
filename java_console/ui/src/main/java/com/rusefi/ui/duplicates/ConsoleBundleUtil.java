package com.rusefi.ui.duplicates;

import com.devexperts.logging.Logging;
import org.jetbrains.annotations.NotNull;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.InvalidPathException;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.TreeMap;

import static com.devexperts.logging.Logging.getLogging;

// `com.rusefi.ui.duplicates.ConsoleBundleUtil` class duplicates `com.rusefi.core.io.BundleUtil` class to avoid crash
// on console update (caused by missed `BundleInfo.getNextBranchName` method) - see `readme.md` for details.
// TODO: in June 2025 we need to get rid of this class and to replace its usage with `com.rusefi.core.io.BundleUtil`.
public class ConsoleBundleUtil {
    private static final Logging log = getLogging(ConsoleBundleUtil.class);

    private static final String DEVELOPMENT = "development";
    private static final String BRANCH_REF_FILE = "release.txt";

    /**
     * @return null in case of error
     */
    public static List<@NotNull String> readBundleFullName() {
        File f = new File(BRANCH_REF_FILE);
        if (!f.exists()) {
            log.error(BRANCH_REF_FILE + " not found");
            return null;
        }
        try {
            return Files.readAllLines(f.toPath());
        } catch (InvalidPathException | IOException e) {
            log.error("Error reading bundle name", e);
            return null;
        }
    }

    @NotNull
    public static BundleInfo readBundleFullNameNotNull() {
        List<@NotNull String> info = readBundleFullName();
        if (info == null)
            return BundleInfo.UNKNOWN;
        return parse(info);
    }

    public static String getBundleTarget() {
        return readBundleFullNameNotNull().getTarget();
    }

    public static BundleInfo parse(List<@NotNull String> info) {
        Map<String, String> keyValues = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        for (String line : info) {
            String[] pair = line.split("=", 2);
            keyValues.put(pair[0], pair[1]);
        }
        String target = keyValues.get("platform");
        String branchName = keyValues.get("release");
        String nextBranchName = keyValues.get("nextRelease");
        if (target == null || branchName == null) {
            log.info(BRANCH_REF_FILE + " says " + keyValues);
            return BundleInfo.UNKNOWN;
        }
        return new ConsoleBundleUtil.BundleInfo(branchName, nextBranchName, target);
    }

    public static class BundleInfo {
        static final BundleInfo UNKNOWN = new BundleInfo("unknown", null, "unknown");

        private final String branchName;
        private final String nextBranchName;
        private final String target;

        public BundleInfo(String branchName, String nextBranchName, String target) {
            this.branchName = Objects.requireNonNull(branchName, "branchName");
            this.nextBranchName = nextBranchName;
            this.target = Objects.requireNonNull(target, "target");
        }

        public static boolean isUndefined(BundleInfo bundleInfo) {
            return bundleInfo == UNKNOWN;
        }

        public String getBranchName() {
            return branchName;
        }

        public String getNextBranchName() {
            return nextBranchName;
        }

        public boolean isMaster() {
            return DEVELOPMENT.equals(branchName);
        }

        public String getTarget() {
            return target;
        }

        @Override
        public String toString() {
            return "BundleInfo{" +
                "branchName='" + branchName + '\'' +
                "nextBranchName='" + nextBranchName + '\'' +
                ", target='" + target + '\'' +
                '}';
        }

        public String getUiLabel() {
            return target + "." + branchName;
        }
    }
}
