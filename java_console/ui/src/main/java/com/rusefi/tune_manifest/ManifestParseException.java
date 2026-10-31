package com.rusefi.tune_manifest;

import java.io.IOException;

public class ManifestParseException extends IOException {
    private final String body;

    public ManifestParseException(String body, Throwable cause) {
        super("Failed to parse tune manifest: " + cause, cause);
        this.body = body;
    }

    public String getBody() {
        return body;
    }
}
