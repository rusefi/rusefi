package com.rusefi.io;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 9/4/14
 */
public interface CommunicationLoggingListener {
    CommunicationLoggingListener VOID = new CommunicationLoggingListener() {
        @Override
        public void onPortHolderMessage(Class clazz, String message) {
        }
    };

    void onPortHolderMessage(final Class clazz, final String message);
}
