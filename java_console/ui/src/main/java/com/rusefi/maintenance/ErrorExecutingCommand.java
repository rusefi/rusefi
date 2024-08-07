package com.rusefi.maintenance;

import java.io.IOException;

public class ErrorExecutingCommand extends Throwable {
    private final IOException e;

    public ErrorExecutingCommand(IOException e) {
        this.e = e;
    }
}
