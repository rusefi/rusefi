package com.rusefi.tools.online;

import org.json.simple.JSONArray;

public class UploadResult {
    private final boolean isError;
    private final JSONArray message;

    public UploadResult(boolean isError, JSONArray message) {
        this.isError = isError;
        this.message = message;
    }

    public UploadResult(boolean isError, String value) {
        message = new JSONArray();
        message.add(value);
        this.isError = isError;
    }

    public boolean isError() {
        return isError;
    }

    public JSONArray getMessageArray() {
        return message;
    }

    public String getFirstMessage() {
        return message.get(0).toString();
    }
}
