package com.rusefi.ui;

import com.devexperts.logging.Logging;

import static com.devexperts.logging.Logging.getLogging;

/**
 * @see StatusWindow
 */
public interface StatusConsumer {
    Logging log = getLogging(StatusConsumer.class);

    StatusConsumer ANONYMOUS = (status, breakLineOnTextArea, sendToLogger) -> {
        if (sendToLogger) {
            log.info(status);
        }
    };
    StatusConsumer VOID = (status, breakLineOnTextArea, sendToLogger) -> {
    };

    default void append(String status) {
        append(status, true, true);
    }

    void append(String status, boolean breakLineOnTextArea, boolean sendToLogger);
}
