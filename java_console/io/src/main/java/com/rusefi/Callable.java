package com.rusefi;

/**
 * Similar to {@link java.util.concurrent.Callable} but without exception in the signature
 */
@FunctionalInterface
public interface Callable<V> {
    V call();
}
