package com.rusefi.test;

import com.rusefi.BinarySearch;
import junit.framework.TestCase;

import java.util.Arrays;

public class BinarySearchTest extends TestCase {
    public static void main(String[] args) {
        new BinarySearchTest().testBinary();
    }

    public void testBinary() {

        double[] angles = new double[] {0, 56, 126, 180, 236, 279, 306, 416, 486, 540, 596, 666};

        Arrays.sort(angles);

        assertEquals(0, BinarySearch.binarySearch(0, angles));

        assertEquals(0, BinarySearch.binarySearch(40, angles));
        assertEquals(1, BinarySearch.binarySearch(56, angles));

        assertEquals(1, BinarySearch.binarySearch(60, angles));

        assertEquals(10, BinarySearch.binarySearch(660, angles));

        assertEquals(11, BinarySearch.binarySearch(666, angles));

        assertEquals(11, BinarySearch.binarySearch(700, angles));
    }
}