package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.core.io.BundleUtil;
import org.jetbrains.annotations.Nullable;

import static com.devexperts.logging.Logging.getLogging;

public class UiBundleUtil {
    private static final Logging log = getLogging(BundleUtil.class);
    /**
     * @return null in case of error
     */
    @Nullable
    public static String readBranchNameToDisplay() {
        final String bundleFullName = BundleUtil.readBundleFullName();
        if (bundleFullName != null) {
            try {
                final BundleUtil.BundleInfo bundleInfo = BundleUtil.parse(bundleFullName);
                // TODO: get rid of the pornography below:
                //  we should use `development` instead of `snapshot` for master branch in bundle name.
                return (bundleInfo.isMaster() ? "development" : bundleInfo.getBranchName());
            } catch (Throwable e) {
                log.warn(String.format("We failed to parse bundle full name `%s`", bundleFullName), e);
            }
        }
        return null;
    }
}
