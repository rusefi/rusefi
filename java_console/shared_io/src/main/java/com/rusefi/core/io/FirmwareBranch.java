package com.rusefi.core.io;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

public final class FirmwareBranch {
    private static final Set<String> AUTOMATIC = new HashSet<>(Arrays.asList(
        "development", "lts-25kansas", "lts-26lima", "lts-26lima-dev"));

    private FirmwareBranch() { }

    public static String normalize(String branch) {
        return "master".equals(branch) || "development".equals(branch) ? "development" : branch;
    }

    public static boolean isSameBranch(String first, String second) {
        String a = normalize(first);
        String b = normalize(second);
        return AUTOMATIC.contains(a) && a.equals(b);
    }

    public static boolean canSwitch(String from, String to) {
        String source = normalize(from);
        String destination = normalize(to);
        return ("lts-26lima".equals(source)
            && ("lts-26lima-dev".equals(destination) || "development".equals(destination)))
            || ("lts-26lima-dev".equals(source) && "development".equals(destination));
    }
}
