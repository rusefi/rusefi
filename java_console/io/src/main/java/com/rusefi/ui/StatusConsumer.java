package com.rusefi.ui;

public interface StatusConsumer {
    StatusConsumer VOID = s -> {
    };

    void append(String s);
}
