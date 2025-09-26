package com.rusefi.core.io;

import com.devexperts.logging.Logging;
import com.rusefi.core.net.ConnectionAndMeta;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;

import java.util.function.Function;

import static com.devexperts.logging.Logging.getLogging;

public class BundleInfoStrategy {
    private static final Logging log = getLogging(BundleInfoStrategy.class);

    public static String selectBranchName(BundleInfo bundleInfo) {
        final String branchName = bundleInfo.getBranchName();
        final String nextBranchName = bundleInfo.getNextBranchName();
        if (nextBranchName != null && !nextBranchName.trim().isEmpty()) {
            if (JOptionPane.showConfirmDialog(
                null,
                String.format("A new version `%s` is available!\nWould you like to update from `%s` to `%s` now?",
                    nextBranchName,
                    branchName,
                    nextBranchName
                ),
                "Release selection",
                JOptionPane.YES_NO_OPTION
            ) == JOptionPane.YES_OPTION) {
                return nextBranchName;
            }
        }
        return branchName;
    }

    @NotNull
    public static String getDownloadUrl(BundleInfo bundleInfo, String baseUrl, Function<BundleInfo, String> branchSelector) {
        String branchUrl;
        if (bundleInfo.isMaster()) {
            log.info("Snapshot requested");
            branchUrl = baseUrl + ConnectionAndMeta.AUTOUPDATE;
        } else {
            final String branchName = branchSelector.apply(bundleInfo);
            branchUrl = baseUrl + "/lts/" + branchName + ConnectionAndMeta.AUTOUPDATE;
        }
        return branchUrl;
    }
}
