package com.rusefi.io;

/**
 * 3/8/14
 * (c) Andrey Belomutskiy
 */
public interface InvocationConfirmationListener {
    InvocationConfirmationListener VOID = new InvocationConfirmationListener() {
        @Override
        public void onCommandConfirmation() {
        }
    };

    void onCommandConfirmation();
}
