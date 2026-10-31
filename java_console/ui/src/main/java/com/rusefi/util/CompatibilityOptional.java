package com.rusefi.util;

import java.util.Optional;
import java.util.function.Consumer;

// [tag:java8] java 11 API in java 8 world
public class CompatibilityOptional {
    public static <T> void ifPresentOrElse(Optional<T> value, Consumer<? super T> action, Runnable emptyAction) {
        if (value.isPresent()) {
            action.accept(value.get());
        } else {
            emptyAction.run();
        }
    }
}
