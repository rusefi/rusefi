package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.server.ApplicationRequest;

import java.io.IOException;

public class LocalApplicationProxy {
    private final Logger logger;
    private final ApplicationRequest applicationRequest;

    public LocalApplicationProxy(Logger logger, ApplicationRequest applicationRequest) {
        this.logger = logger;
        this.applicationRequest = applicationRequest;
    }

    public void run(IoStream authenticatorToProxyStream) throws IOException {
        // right from connection push session authentication data
        new HelloCommand(logger, applicationRequest.toJson()).handle(authenticatorToProxyStream);
    }
}
