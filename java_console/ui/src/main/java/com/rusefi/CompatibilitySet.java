package com.rusefi;

import java.util.HashSet;
import java.util.Set;

// [tag:java8] java 11 API in java 8 world
public class CompatibilitySet {
    public static <E> Set<E> of(E e1, E e2) {
        Set<E> result = new HashSet<>();
        result.add(e1);
        result.add(e2);
        return result;
    }

    public static <E> Set<E> of(E e1, E e2, E e3) {
        Set<E> result = new HashSet<>();
        result.add(e1);
        result.add(e2);
        result.add(e3);
        return result;
    }
}
