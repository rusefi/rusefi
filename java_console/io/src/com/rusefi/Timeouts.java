package com.rusefi;

/**
 * All i/o timeouts in one place
 */
public interface Timeouts {
    int SECOND = 1000;

    int COMMAND_TIMEOUT_SEC = 10; // seconds

    int RESTART_DELAY = 20 * SECOND;

    int BINARY_IO_TIMEOUT = 10 * SECOND;
    int CMD_TIMEOUT = 20;
}

