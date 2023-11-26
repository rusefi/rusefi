package com.rusefi.models;

import java.util.SortedMap;
import java.util.TreeMap;

/**
 * @author Andrey Belomutskiy
 *         1/29/13
 */
public class Utils {
    // I do not want to deal with overflow so no MAX_VALUE for me
    private static final int LARGE_VALUE = 10000000;

    /**
     * finds a key in the map which is closest to the specified element
     */
    public static int findClosest(TreeMap<Integer, ?> map, int value) {
        SortedMap<Integer, ?> head = map.headMap(value, true);
        SortedMap<Integer, ?> tail = map.tailMap(value, true);
        if (head.isEmpty() && tail.isEmpty())
            throw new IllegalStateException("Empty map? " + value);

        int fromHead = head.isEmpty() ? LARGE_VALUE : head.lastKey();
        int fromTail = tail.isEmpty() ? LARGE_VALUE : tail.firstKey();

        int headDiff = Math.abs(value - fromHead);
        int tailDiff = Math.abs(value - fromTail);
        return headDiff < tailDiff ? fromHead : fromTail;
    }

    public static int parseIntWithReason(String number, String reason) {
        try {
            return Integer.parseInt(number);
        } catch (NumberFormatException e) {
            throw new IllegalArgumentException("Unexpected [" + number + "] for " + reason, e);
        }
    }
}
