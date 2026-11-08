package com.rusefi.autoupdate;

import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.net.UnknownHostException;

import static org.junit.jupiter.api.Assertions.assertEquals;

/**
 * https://github.com/rusefi/rusefi/issues/10191
 */
class AutoupdateDownloadErrorMessageTest {
    @Test
    void unknownHostAsksToCheckInternetConnection() {
        assertEquals("Check your internet connection",
            Autoupdate.downloadErrorMessage(new UnknownHostException("rusefi.com")));
    }

    @Test
    void otherIoErrorsDoNotShowTheExceptionText() {
        assertEquals("The download did not complete. Please try again.",
            Autoupdate.downloadErrorMessage(new IOException("disk full")));
    }
}
