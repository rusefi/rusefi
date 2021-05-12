package com.rusefi.proxy.client;

import com.rusefi.proxy.NetworkConnector;

public enum UpdateType {
    CONTROLLER(NetworkConnector.UPDATE_CONNECTOR_SOFTWARE_LATEST),
    FIRMWARE(NetworkConnector.UPDATE_FIRMWARE_LATEST),
    CONTROLLER_RELEASE(NetworkConnector.UPDATE_CONNECTOR_SOFTWARE_RELEASE),
    FIRMWARE_RELEASE(NetworkConnector.UPDATE_FIRMWARE_RELEASE),
    ;

    private final byte code;

    UpdateType(int code) {
        this.code = (byte) code;
    }

    public byte getCode() {
        return code;
    }
}
