package com.rusefi.io;

public interface UpdateOperationCallbacks {
    void log(String message, boolean breakLineOnTextArea, boolean sendToLogger);

    default void logLine(final String message) {
        log(message, true, true);
    }

    default void append(final String message, final boolean breakLineOnTextArea, final boolean sendToLogger) {
      log(message, breakLineOnTextArea, sendToLogger);
    }

    default void appendLine(final String message) {
        append(message, true, true);
    }

    void done();
    void error();

    class UpdateOperationDummy implements UpdateOperationCallbacks {
        @Override
        public void log(final String message, final boolean breakLineOnTextArea, boolean sendToLogger) {
        }

        @Override
        public void done() {
        }

        @Override
        public void error() {
        }
    }

    public static UpdateOperationCallbacks DUMMY = new UpdateOperationDummy();
}
