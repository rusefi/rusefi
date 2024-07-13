package com.rusefi.io;

public interface UpdateOperationCallbacks {
    void log(String message, boolean breakLineOnTextArea);

    default void append(final String message, final boolean breakLineOnTextArea) {
      log(message, breakLineOnTextArea);
    }

    void done();
    void error();

    class UpdateOperationDummy implements UpdateOperationCallbacks {
        @Override
        public void log(final String message, final boolean breakLineOnTextArea) {
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
