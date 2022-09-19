package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.rusefi.binaryprotocol.IncomingDataBuffer;

import static com.devexperts.logging.Logging.getLogging;

/**
 * @see StatusWindow
 */
public interface StatusConsumer {
    Logging log = getLogging(StatusConsumer.class);

    StatusConsumer ANONYMOUS = log::info;
    StatusConsumer VOID = s -> {
    };

    void append(String s);
}
