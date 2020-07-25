package com.rusefi.server;

import com.rusefi.io.IoStream;
import org.jetbrains.annotations.NotNull;

import java.util.Objects;

public class ApplicationConnectionState {
    private final UserDetails userDetails;
    @NotNull
    private final IoStream clientStream;
    private final ControllerConnectionState state;
    private final Birthday birthday = new Birthday();

    public ApplicationConnectionState(UserDetails userDetails, IoStream clientStream, ControllerConnectionState state) {
        this.userDetails = Objects.requireNonNull(userDetails, "userDetails");
        this.clientStream = Objects.requireNonNull(clientStream, "clientStream");
        this.state = Objects.requireNonNull(state, "state");

        if (clientStream.getStreamStats().getPreviousPacketArrivalTime() == 0)
            throw new IllegalStateException("Invalid state - no packets on " + this);
        if (!state.getTwoKindSemaphore().isUsed())
            throw new IllegalArgumentException("state is supposed to be used by us");
    }

    @NotNull
    public IoStream getClientStream() {
        return clientStream;
    }

    public UserDetails getUserDetails() {
        return userDetails;
    }

    public Birthday getBirthday() {
        return birthday;
    }

    public void close() {
        try {
            clientStream.close();
        } finally {
            state.getTwoKindSemaphore().releaseFromLongTermUsage();
        }
    }

    @Override
    public String toString() {
        return "ApplicationConnectionState{" +
                "userDetails=" + userDetails +
                '}';
    }
}
