package com.rusefi.core.ui;

import org.junit.jupiter.api.Test;

import javax.net.ssl.SSLHandshakeException;
import javax.swing.*;
import java.awt.Dimension;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.ConnectException;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * The bundle updater's progress window used to report a failed download with the raw exception text,
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
    void missingFileOnServerSaysTheUpdateIsNotAvailable() {
        assertEquals(AutoupdateUtil.NOT_ON_SERVER, AutoupdateUtil.downloadErrorMessage(NOT_ON_SERVER));
    }

    @Test
    void localFileThatCannotBeWrittenSaysSo() {
        assertEquals(AutoupdateUtil.CANNOT_SAVE, AutoupdateUtil.downloadErrorMessage(
            new FileNotFoundException("C:\\Users\\me\\.rusEFI\\e78_development\\rusefi_bundle_e78_autoupdate.zip (Access is denied)")));
    }

    @Test
    void unreachableServer() {
        assertEquals(AutoupdateUtil.CANNOT_REACH_SERVER,
            AutoupdateUtil.downloadErrorMessage(new ConnectException("Connection refused: connect")));
        assertEquals(AutoupdateUtil.CANNOT_REACH_SERVER,
            AutoupdateUtil.downloadErrorMessage(new SocketTimeoutException("connect timed out")));
    }

    @Test
    void failedSecureConnection() {
        assertEquals(AutoupdateUtil.SECURE_CONNECTION_FAILED,
            AutoupdateUtil.downloadErrorMessage(new SSLHandshakeException("Remote host terminated the handshake")));
    }

    @Test
    void serverError() {
        assertEquals(AutoupdateUtil.SERVER_PROBLEM, AutoupdateUtil.downloadErrorMessage(
            new IOException("Server returned HTTP response code: 503 for URL: https://rusefi.com/x.zip")));
    }

    @Test
    void otherIoErrorsSayTheDownloadDidNotComplete() {
        assertEquals(AutoupdateUtil.DOWNLOAD_INCOMPLETE,
            AutoupdateUtil.downloadErrorMessage(new IOException("disk full")));
        assertEquals(AutoupdateUtil.DOWNLOAD_INCOMPLETE,
            AutoupdateUtil.downloadErrorMessage(new SocketException("Connection reset")));
        assertEquals(AutoupdateUtil.DOWNLOAD_INCOMPLETE,
            AutoupdateUtil.downloadErrorMessage(new IOException()));
    }

    @Test
    void messagesNeverShowExceptionText() {
        IOException[] errors = {NOT_ON_SERVER, new IOException("disk full"), new ConnectException("refused")};
        for (IOException e : errors) {
            String message = AutoupdateUtil.downloadErrorMessage(e);
            assertFalse(message.contains("Exception"), message);
            assertFalse(message.contains("http"), message);
        }
    }

    @Test
    void errorTextWrapsWithinTheWindow() throws Exception {
        ProgressView view = new ProgressView(null, new JProgressBar(), new JPanel());
        String message = AutoupdateUtil.downloadErrorMessage(NOT_ON_SERVER);
        SwingUtilities.invokeAndWait(() -> view.showError(message));

        JLabel label = view.getErrorLabelForUnitTest();
        Dimension size = label.getPreferredSize();
        assertTrue(size.width <= AVAILABLE_WIDTH, "label is " + size.width + " px wide");
        assertTrue(label.getText().contains(message), label.getText());
        int lineHeight = label.getFontMetrics(label.getFont()).getHeight();
        assertTrue(size.height > lineHeight, "the message wraps to more than one line: " + size);
    }
}
