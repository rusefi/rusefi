package com.rusefi.server;

import java.util.function.Function;

public interface UserDetailsResolver extends Function<String, UserDetails> {
}
