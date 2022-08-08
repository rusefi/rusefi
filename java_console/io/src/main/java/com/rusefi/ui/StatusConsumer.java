package com.rusefi.ui;

/**
 * @see StatusWindow
 */
public interface StatusConsumer {
    StatusConsumer VOID = s -> {
    };

    void append(String s);
}
