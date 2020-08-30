package com.rusefi.proxy.client;

import java.io.IOException;

public class IncompatibleBackendException extends IOException {
    public IncompatibleBackendException(String message) {
        super(message);
    }
}
