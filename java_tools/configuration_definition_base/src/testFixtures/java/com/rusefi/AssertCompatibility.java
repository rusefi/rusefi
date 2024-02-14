package com.rusefi;


import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.function.Executable;

public class AssertCompatibility {
    // FACEPALM: between junit4 and junit4 they have changed order of arguments
    public static void assertNotNull(String message, Object actual) {
        Assertions.assertNotNull(actual, message);
    }

    public static void assertNotNull(Object actual) {
        Assertions.assertNotNull(actual);
    }

    public static void assertEquals(Object expected, Object actual) {
        Assertions.assertEquals(expected, actual);
    }

    public static void assertEquals(String message, int expected, int actual) {
        Assertions.assertEquals(expected, actual, message);
    }

    public static void assertTrue(boolean condition) {
        Assertions.assertTrue(condition);
    }

    public static void assertFalse(boolean condition) {
        Assertions.assertFalse(condition);
    }

    public static <T extends Throwable> T assertThrows(Class<T> expectedThrowable, Executable executable) {
        return Assertions.assertThrows(expectedThrowable, executable);
    }

    public static void assertTrue(String message, boolean condition) {
        Assertions.assertTrue(condition, message);
    }
}
