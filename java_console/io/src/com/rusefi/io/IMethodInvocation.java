package com.rusefi.io;

/**
 * (c) Andrey Belomutskiy 2013-2019
 */
public interface IMethodInvocation {
    String getCommand();

    int getTimeout();

    InvocationConfirmationListener getListener();

    boolean isFireEvent();
}
