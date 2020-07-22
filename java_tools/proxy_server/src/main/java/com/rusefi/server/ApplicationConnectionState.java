package com.rusefi.server;

import com.rusefi.io.IoStream;
import org.jetbrains.annotations.NotNull;

import java.util.Objects;

public class ApplicationConnectionState {
    private final UserDetails userDetails;
    @NotNull
    private final IoStream clientStream;

    public ApplicationConnectionState(UserDetails userDetails, ApplicationRequest applicationRequest, IoStream clientStream) {
        this.userDetails = Objects.requireNonNull(userDetails, "userDetails");
        this.clientStream = Objects.requireNonNull(clientStream, "clientStream");
    }

    @NotNull
    public IoStream getClientStream() {
        return clientStream;
    }

    public UserDetails getUserDetails() {
        return userDetails;
    }

    public void close() {
        clientStream.close();
    }

    @Override
    public String toString() {
        return "ApplicationConnectionState{" +
                "userDetails=" + userDetails +
                '}';
    }
}
