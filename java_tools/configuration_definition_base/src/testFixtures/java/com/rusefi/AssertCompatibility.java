package com.rusefi;


import org.junit.jupiter.api.Assertions;

public class AssertCompatibility {
    // FACEPALM: between junit4 and junit4 they have changed order of arguments
    public static void assertNotNull(String message, Object actual) {
        Assertions.assertNotNull(actual, message);
    }

    public static void assertEquals(Object expected, Object actual) {
        Assertions.assertEquals(expected, actual);
    }
}
