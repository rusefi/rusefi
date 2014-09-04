package com.rusefi.io.serial;

/**
 * (c) Andrey Belomutskiy
 * 9/4/14
 */
public interface PortHolderListener {
    PortHolderListener VOID = new PortHolderListener() {
        @Override
        public void onPortHolderMessage(Class clazz, String message) {
        }
    };

    void onPortHolderMessage(final Class clazz, final String message);
}
