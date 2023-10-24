package com.rusefi.io;

public interface UpdateOperationCallbacks {
    void log(String message);
    void done();
    void error();

    class UpdateOperationDummy implements UpdateOperationCallbacks {
        @Override
        public void log(String message) {
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
