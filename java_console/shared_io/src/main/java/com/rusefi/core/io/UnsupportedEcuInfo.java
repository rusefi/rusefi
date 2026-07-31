package com.rusefi.core.io;

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
}
