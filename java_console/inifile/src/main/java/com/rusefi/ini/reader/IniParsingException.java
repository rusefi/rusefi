package com.rusefi.ini.reader;

public class IniParsingException extends Throwable {
    public IniParsingException(String s, RuntimeException e) {
        super(s, e);
    }
}
