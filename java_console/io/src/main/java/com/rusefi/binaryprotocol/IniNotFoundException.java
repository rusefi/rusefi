package com.rusefi.binaryprotocol;

public class IniNotFoundException extends Throwable {
    public IniNotFoundException(String s) {
        super(s);
    }

    public IniNotFoundException(String message, Throwable cause) {
        super(message, cause);
    }
}
