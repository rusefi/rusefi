package com.rusefi;

public interface Listener<T> {
    void onResult(T parameter);

    static <T> Listener<T> empty() {
        return parameter -> {
        };
    }
}
