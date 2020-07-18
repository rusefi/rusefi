package com.rusefi.server;

import java.util.function.Function;

public class JsonUserDetailsResolver implements Function<String, UserDetails> {
    @Override
    public UserDetails apply(String authToken) {
        // todo
        return new UserDetails("", 0);
    }
}
