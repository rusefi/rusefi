package com.rusefi.autoupdate;

import java.io.IOException;

/**
 * IO Exception which was already reported with a UI dialog
 */
public class ReportedIOException extends IOException {
    private IOException e;

    public ReportedIOException(IOException e) {
        this.e = e;
    }
}
