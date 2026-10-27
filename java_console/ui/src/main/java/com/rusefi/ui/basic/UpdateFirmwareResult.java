package com.rusefi.ui.basic;

import java.awt.Color;

/**
 * Terminal outcome of the most recent firmware-update job, so a persistent success/failure indication
 * can survive the post-flash reconnect handoff (issue #9832 — the transient green report used to vanish
 * the moment the console reconnected). The text/color mapping is pure and Swing-free so it is
 * unit-testable without a UI (see docs/java-connectivity-ui-unit-testing.md).
 */
public enum UpdateFirmwareResult {
    /** No update completed this session (or a new job just started). */
    NONE,
    SUCCESS,
    WARNING,
    FAILURE;

    private static final Color GREEN = new Color(0, 128, 0);

    /**
     * Persistent banner text shown after the post-flash reconnect for a board on {@code portName}.
     */
    public String bannerText(final String portName) {
        switch (this) {
            case SUCCESS:
                return "Firmware update and settings steps completed — reconnect requested on " + portName
                    + " — check firmware version and settings before use";
            case WARNING:
                return "Firmware update steps completed, but settings warnings remain — see the report — reconnect requested on "
                    + portName + " — check firmware version and settings before use";
            case FAILURE:
                return "Firmware update did not complete all steps — programming may have succeeded — see the report — ECU detected on "
                    + portName + " — check firmware and settings before use";
            default:
                return "Connected to " + portName + " — click Connect to open console";
        }
    }

    /** Foreground color for {@link #bannerText}. */
    public Color bannerColor() {
        switch (this) {
            case SUCCESS:
                return GREEN;
            case WARNING:
                return new Color(160, 100, 0);
            case FAILURE:
                return Color.RED;
            default:
                return Color.DARK_GRAY;
        }
    }
}
