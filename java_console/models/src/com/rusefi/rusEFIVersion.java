package com.rusefi;

import java.util.concurrent.atomic.AtomicReference;

public class rusEFIVersion {
    public static final int CONSOLE_VERSION = 20200622;
    public static AtomicReference<String> firmwareVersion = new AtomicReference<>("N/A");
}
