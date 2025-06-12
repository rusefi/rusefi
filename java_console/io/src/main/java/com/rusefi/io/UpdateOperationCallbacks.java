package com.rusefi.io;

import com.devexperts.logging.Logging;
import com.rusefi.ui.StatusConsumer;

public interface UpdateOperationCallbacks extends StatusConsumer {
    Logging log = Logging.getLogging(UpdateOperationCallbacks.class);

    void log(String message, boolean breakLineOnTextArea, boolean sendToLogger);

    @Override
    default void logLine(final String message) {
        log(message, true, true);
    }

    void done();
    void error();

    void clear();

    UpdateOperationCallbacks DUMMY = new UpdateOperationCallbacks() {
        @Override
        public void log(final String message, final boolean breakLineOnTextArea, boolean sendToLogger) {
        }

        @Override
        public void done() {
        }

        @Override
        public void error() {
        }

        @Override
        public void clear() {
        }
    };

    UpdateOperationCallbacks LOGGER = new UpdateOperationCallbacks() {
        @Override
        public void log(final String message, final boolean breakLineOnTextArea, boolean sendToLogger) {
            log.info(message);
        }

        @Override
        public void done() {
            log.info("[DONE]");
        }

        @Override
        public void error() {
            log.info("[error]");
        }

        @Override
        public void clear() {
        }
    };

    UpdateOperationCallbacks CONSOLE = new UpdateOperationCallbacks() {
        @Override
        public void log(final String message, final boolean breakLineOnTextArea, boolean sendToLogger) {
            if (breakLineOnTextArea) {
                System.out.println(message);
            } else {
                System.out.print(message);
            }
            if (sendToLogger) {
                LOGGER.log(message, breakLineOnTextArea, true);
            }
        }

        @Override
        public void done() {
            log.info("[DONE]");
            LOGGER.done();
        }

        @Override
        public void error() {
            log.info("[error]");
            LOGGER.error();
        }

        @Override
        public void clear() {
        }
    };
}
