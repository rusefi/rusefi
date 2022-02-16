package com.rusefi;

/**
 * All i/o timeouts in one place
 */
public interface Timeouts {
    int SECOND = 1000;
    int MINUTE = 60 * SECOND;
    int COMMAND_TIMEOUT_SEC = 10; // seconds

    /**
     * The longest justified binary communication delay would be related to stm32f407 flash saving time
     */
    int BINARY_IO_TIMEOUT = 5 * SECOND;
    int READ_IMAGE_TIMEOUT = 60 * SECOND;

    int CONNECTION_RESTART_DELAY = 1 * SECOND;

    int CMD_TIMEOUT = 20 * SECOND;
    int SET_ENGINE_TIMEOUT = 60 * SECOND;
    int TEXT_PULL_PERIOD = 100;
}

