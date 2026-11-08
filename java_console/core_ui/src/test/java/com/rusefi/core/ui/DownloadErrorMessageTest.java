package com.rusefi.core.ui;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.UnknownHostException;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * The bundle updater's progress window reported a failed download with the raw exception text,
 * cut off at the window edge, e.g. "Error downloading: java.io.FileNotFoundException: https://rusefi.com/...".
 */
class DownloadErrorMessageTest {
    /**
     * How HttpURLConnection reports an HTTP 404.
     */
    private static final FileNotFoundException NOT_ON_SERVER =
        new FileNotFoundException("https://rusefi.com/build_server/autoupdate/rusefi_bundle_e78_autoupdate.zip?u=0b6c2f5e-3d1a-4f7e-9c2b-8a4d5e6f7a8b");

    /**
     * The progress window is 480 px wide with a 40 px border on both sides, minus the frame's own insets.
     */
    private static final int AVAILABLE_WIDTH = 380;

    @Test
    void unknownHostAsksToCheckInternetConnection() {
        assertEquals(AutoupdateUtil.CHECK_INTERNET_CONNECTION,
            AutoupdateUtil.downloadErrorMessage(new UnknownHostException("rusefi.com")));
    }

    @Test
    void missingFileOnServerShowsTheExceptionText() {
        assertEquals("Error downloading: " + NOT_ON_SERVER,
            AutoupdateUtil.downloadErrorMessage(NOT_ON_SERVER));
    }

    @Test
    void otherIoErrorsShowTheExceptionText() {
        assertEquals("Error downloading: java.io.IOException: disk full",
            AutoupdateUtil.downloadErrorMessage(new IOException("disk full")));
    }

    @Test
    void errorTextIsWiderThanTheWindow() throws Exception {
        ProgressView view = new ProgressView(null, new JProgressBar(), new JPanel());
        SwingUtilities.invokeAndWait(() -> view.showError(AutoupdateUtil.downloadErrorMessage(NOT_ON_SERVER)));

        int width = view.getErrorLabelForUnitTest().getPreferredSize().width;
        assertTrue(width > AVAILABLE_WIDTH, "label is " + width + " px wide");
    }
}
