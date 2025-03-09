package com.rusefi.server;

import org.jetbrains.annotations.NotNull;

import java.time.Duration;

public class Birthday {
    private final long createAt = System.currentTimeMillis();

    public String getDuration() {
        return humanReadableFormat(System.currentTimeMillis() - createAt);
    }

    @NotNull
    static String humanReadableFormat(long millis) {
        return humanReadableFormat(Duration.ofMillis(millis));
    }

    @NotNull
    static String humanReadableFormat(Duration duration) {
        return duration.toString()
                .substring(2)
                .replaceAll("(\\d[HMS])(?!$)", "$1 ")
                .toLowerCase();
    }
}
