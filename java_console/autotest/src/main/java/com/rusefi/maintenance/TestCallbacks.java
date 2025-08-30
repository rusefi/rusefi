package com.rusefi.maintenance;

import com.rusefi.io.UpdateOperationCallbacks;

public class TestCallbacks implements UpdateOperationCallbacks {

    @Override
    public void log(String message, boolean breakLineOnTextArea, boolean sendToLogger) {
        content.append(message.replaceAll(Character.toString((char) 219), ""));
        if (breakLineOnTextArea) {
            content.append("\r\n");
        }
    }

    @Override
    public void done() {}

    @Override
    public void warning() {

    }

    @Override
    public void error() {}

    @Override
    public void clear() {
        content.setLength(0);
    }

    public String getContent() {
        return content.toString();
    }

    private StringBuilder content = new StringBuilder();
}
