package com.rusefi;

import com.opensr5.ConfigurationImage;
import com.rusefi.core.Pair;

/**
 * This class has the logic of finding all ranges of changes between who {@link ConfigurationImage}
 * It's used to send just the diff to the ECU.
 *
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/6/2015
 */
public class ConfigurationImageDiff {
    private ConfigurationImageDiff() {
    }

    /**
     * @return NULL if images are the same from OFFSET and to the end
     */
    public static Pair<Integer, Integer> findDifferences(ConfigurationImage image1, ConfigurationImage image2, int position) {
        byte[] c1 = image1.getContent();
        byte[] c2 = image2.getContent();
        int length = c1.length;
        if (length != c2.length)
            throw new IllegalArgumentException("Length mismatch");

        while (position < length && c1[position] == c2[position])
            position++;
        if (position == length)
            return null; // no difference

        int startOfDiff = position;
        int prevDiff = position;

        while (position - prevDiff < 4 && position < length) {
            if (c1[position] != c2[position]) {
                prevDiff = position;
            }
            position++;
        }

        return new Pair<>(startOfDiff, position);
    }
}
