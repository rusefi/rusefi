package com.rusefi.models.test;

import org.junit.Test;

import java.util.TreeMap;

import static com.rusefi.models.Utils.findClosest;
import static junit.framework.Assert.assertEquals;

/**
 * @author Andrey Belomutskiy
 *         1/29/13
 */
public class UtilTest {
    @Test
    public void testClosest() {
        TreeMap<Integer, Object> map = new TreeMap<>();
        map.put(0, "0");
        map.put(1, "1");
        map.put(10, "10");
        map.put(11, "11");

        assertEquals(0, findClosest(map, -1));
        assertEquals(0, findClosest(map, 0));
        assertEquals(1, findClosest(map, 1));
        assertEquals(1, findClosest(map, 3));
        assertEquals(10, findClosest(map, 10));
    }
}
