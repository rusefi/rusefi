package com.rusefi.ui.util;

public enum LocalizedMessages {
    CLEAR("clear"),
    PAUSE("pause"),
    RESUME("resume"),
    RESET("reset"),
    ;
    private final String message;

    LocalizedMessages(String message) {
        this.message = message;
    }

    public String getMessage() {
        return message;
    }
}