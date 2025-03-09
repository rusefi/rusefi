package com.rusefi;

import java.util.HashSet;
import java.util.Set;

// [tag:java8] java 11 API in java 8 world
public class CompatibilitySet {
    public static <E> Set<E> of(final E... elements) {
        final Set<E> result = new HashSet<>();
        for (final E element: elements) {
            result.add(element);
        }
        return result;
    }
}
