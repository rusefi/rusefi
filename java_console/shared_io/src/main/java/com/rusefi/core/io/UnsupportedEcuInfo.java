package com.rusefi.core.io;

import com.rusefi.core.net.PropertiesHolder;

import static com.rusefi.core.net.ConnectionAndMeta.RUSEFI_WIKI_DOWNLOAD_PAGE;

/** A positively identified ECU target that the current bundle is not allowed to serve. */
public final class UnsupportedEcuInfo {
    private final String ecuTarget;
    private final String bundleTarget;

    public UnsupportedEcuInfo(String ecuTarget, String bundleTarget) {
        this.ecuTarget = ecuTarget;
        this.bundleTarget = bundleTarget;
    }

    public String getEcuTarget() {
        return ecuTarget;
    }

    public String getBundleTarget() {
        return bundleTarget;
    }

    public String getDownloadUrl() {
        return PropertiesHolder.getUpdateHelpUrl(RUSEFI_WIKI_DOWNLOAD_PAGE);
    }

    public String getMessage() {
        return String.format(
            "Unsupported ECU detected!\n\n" +
                "Connected ECU: %s\n" +
                "Bundle target: %s\n\n" +
                "This bundle cannot safely connect to, tune, or update this ECU.\n\n" +
                "Download a compatible bundle from:\n%s",
            ecuTarget, bundleTarget, getDownloadUrl()
        );
    }
}
