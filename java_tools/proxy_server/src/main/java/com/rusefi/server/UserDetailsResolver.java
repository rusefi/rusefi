package com.rusefi.server;

import org.jetbrains.annotations.Nullable;

public interface UserDetailsResolver {
    /***
     * @param authToken
     * @return null in case authentication issues
     */
    @Nullable
    UserDetails apply(String authToken);
}
