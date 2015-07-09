package com.rusefi.models;

/**
 * @author Andrey Belomutskiy
 *         1/29/13
 */
public interface Factory<K, V> {
    V create(K key);
}
