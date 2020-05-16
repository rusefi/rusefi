package com.rusefi.io.serial;

public enum BaudRateHolder {
    INSTANCE;

    /**
     * Nasty code: this field is not final, we have UI which overrides this default!
     */
    public int baudRate = 115200;

}
