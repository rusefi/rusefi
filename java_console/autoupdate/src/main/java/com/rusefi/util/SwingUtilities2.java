package com.rusefi.util;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;

public class SwingUtilities2 {
    public static void invokeAndWait(Runnable runnable) throws InterruptedException, InvocationTargetException {
        if (SwingUtilities.isEventDispatchThread()) {
            runnable.run();
        } else {
            SwingUtilities.invokeAndWait(runnable);
        }
    }
}
