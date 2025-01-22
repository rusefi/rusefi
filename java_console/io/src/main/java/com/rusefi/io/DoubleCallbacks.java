package com.rusefi.io;

public class DoubleCallbacks implements UpdateOperationCallbacks {
    private final UpdateOperationCallbacks one;
    private final UpdateOperationCallbacks two;

    public DoubleCallbacks(UpdateOperationCallbacks one, UpdateOperationCallbacks two) {
        this.one = one;
        this.two = two;
    }

    @Override
    public void log(String message, boolean breakLineOnTextArea, boolean sendToLogger) {
        one.log(message, breakLineOnTextArea, sendToLogger);
        two.log(message, breakLineOnTextArea, sendToLogger);
    }

    @Override
    public void done() {
        one.done();
        two.done();
    }

    @Override
    public void error() {
        one.error();
        two.error();
    }

    @Override
    public void clear() {
        one.clear();
        two.clear();
    }
}
