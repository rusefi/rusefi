package com.rusefi;

/**
 * All i/o timeouts in one place
 */
public interface Timeouts {
    int SECOND = 1000;
    int COMMAND_TIMEOUT_SEC = 10; // seconds
    int BINARY_IO_TIMEOUT = 5 * SECOND;
    int CMD_TIMEOUT = 20;
    int READ_IMAGE_TIMEOUT = 60 * SECOND;

    int CONNECTION_RESTART_DELAY = 20 * SECOND;
    int CS_TIMEOUT = 3000;
}

