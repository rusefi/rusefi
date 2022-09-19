package com.rusefi;

import org.mockito.stubbing.Answer;

public class MockitoTestHelper {
    public static final Answer<?> NEGATIVE_ANSWER = invocation -> {
        throw new UnsupportedOperationException("Not mocked " + invocation);
    };
}
